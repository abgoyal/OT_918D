/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*
 * kernel/trace/trace-clock.c
 *
 * (C) Copyright	2008 -
 * 		Mathieu Desnoyers (mathieu.desnoyers@polymtl.ca)
 *
 * Generic kernel tracing clock for architectures without TSC.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <linux/workqueue.h>
#include <linux/cpu.h>
#include <linux/timex.h>
#include <linux/bitops.h>
#include <linux/trace-clock.h>
#include <linux/jiffies.h>

static int trace_clock_refcount;
static DEFINE_MUTEX(trace_clock_mutex);
static struct timer_list trace_clock_timer;
/*
 * bits 0..12 : counter, atomically incremented
 * bits 13..{32,64} : time counter, incremented each jiffy.
 */
atomic_long_t trace_clock_var;
EXPORT_SYMBOL(trace_clock_var);

static void trace_clock_update(void)
{
	long old_clock, new_clock;
	unsigned long ticks;

	/*
	 * Make sure we keep track of delayed timer.
	 */
	ticks = jiffies - trace_clock_timer.expires + 1;
	/* Don't update if ticks is zero, time would go backward. */
	if (unlikely(!ticks))
		return;
	do {
		old_clock = atomic_long_read(&trace_clock_var);
		new_clock = (old_clock + (ticks << TRACE_CLOCK_SHIFT))
			& (~((1 << TRACE_CLOCK_SHIFT) - 1));
	} while (atomic_long_cmpxchg(&trace_clock_var, old_clock, new_clock)
			!= old_clock);
}

static void trace_clock_timer_fct(unsigned long data)
{
	trace_clock_update();
	trace_clock_timer.expires = jiffies + 1;
	add_timer(&trace_clock_timer);
}

static void enable_trace_clock(void)
{
	init_timer(&trace_clock_timer);
	/* trace_clock_update() reads expires */
	trace_clock_timer.function = trace_clock_timer_fct;
	trace_clock_timer.expires = jiffies + 1;
	trace_clock_update();
	add_timer(&trace_clock_timer);
}

static void disable_trace_clock(void)
{
	del_timer_sync(&trace_clock_timer);
}

void get_trace_clock(void)
{
	get_synthetic_tsc();
	mutex_lock(&trace_clock_mutex);
	if (trace_clock_refcount++)
		goto end;
	enable_trace_clock();
end:
	mutex_unlock(&trace_clock_mutex);
}
EXPORT_SYMBOL_GPL(get_trace_clock);

void put_trace_clock(void)
{
	mutex_lock(&trace_clock_mutex);
	WARN_ON(trace_clock_refcount <= 0);
	if (trace_clock_refcount != 1)
		goto end;
	disable_trace_clock();
end:
	trace_clock_refcount--;
	mutex_unlock(&trace_clock_mutex);
	put_synthetic_tsc();
}
EXPORT_SYMBOL_GPL(put_trace_clock);
