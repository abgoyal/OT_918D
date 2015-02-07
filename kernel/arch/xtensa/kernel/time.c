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
 */

/*
 * arch/xtensa/kernel/time.c
 *
 * Timer and clock support.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 2005 Tensilica Inc.
 *
 * Chris Zankel <chris@zankel.net>
 */

#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/time.h>
#include <linux/clocksource.h>
#include <linux/interrupt.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/profile.h>
#include <linux/delay.h>

#include <asm/timex.h>
#include <asm/platform.h>

#ifdef CONFIG_XTENSA_CALIBRATE_CCOUNT
unsigned long ccount_per_jiffy;		/* per 1/HZ */
unsigned long nsec_per_ccount;		/* nsec per ccount increment */
#endif

static cycle_t ccount_read(void)
{
	return (cycle_t)get_ccount();
}

static struct clocksource ccount_clocksource = {
	.name = "ccount",
	.rating = 200,
	.read = ccount_read,
	.mask = CLOCKSOURCE_MASK(32),
	/*
	 * With a shift of 22 the lower limit of the cpu clock is
	 * 1MHz, where NSEC_PER_CCOUNT is 1000 or a bit less than
	 * 2^10: Since we have 32 bits and the multiplicator can
	 * already take up as much as 10 bits, this leaves us with
	 * remaining upper 22 bits.
	 */
	.shift = 22,
};

static irqreturn_t timer_interrupt(int irq, void *dev_id);
static struct irqaction timer_irqaction = {
	.handler =	timer_interrupt,
	.flags =	IRQF_DISABLED,
	.name =		"timer",
};

void __init time_init(void)
{
#ifdef CONFIG_XTENSA_CALIBRATE_CCOUNT
	printk("Calibrating CPU frequency ");
	platform_calibrate_ccount();
	printk("%d.%02d MHz\n", (int)ccount_per_jiffy/(1000000/HZ),
			(int)(ccount_per_jiffy/(10000/HZ))%100);
#endif
	ccount_clocksource.mult =
		clocksource_hz2mult(CCOUNT_PER_JIFFY * HZ,
				ccount_clocksource.shift);
	clocksource_register(&ccount_clocksource);

	/* Initialize the linux timer interrupt. */

	setup_irq(LINUX_TIMER_INT, &timer_irqaction);
	set_linux_timer(get_ccount() + CCOUNT_PER_JIFFY);
}

/*
 * The timer interrupt is called HZ times per second.
 */

irqreturn_t timer_interrupt (int irq, void *dev_id)
{

	unsigned long next;

	next = get_linux_timer();

again:
	while ((signed long)(get_ccount() - next) > 0) {

		profile_tick(CPU_PROFILING);
#ifndef CONFIG_SMP
		update_process_times(user_mode(get_irq_regs()));
#endif

		write_seqlock(&xtime_lock);

		do_timer(1); /* Linux handler in kernel/timer.c */

		/* Note that writing CCOMPARE clears the interrupt. */

		next += CCOUNT_PER_JIFFY;
		set_linux_timer(next);

		write_sequnlock(&xtime_lock);
	}

	/* Allow platform to do something useful (Wdog). */

	platform_heartbeat();

	/* Make sure we didn't miss any tick... */

	if ((signed long)(get_ccount() - next) > 0)
		goto again;

	return IRQ_HANDLED;
}

#ifndef CONFIG_GENERIC_CALIBRATE_DELAY
void __cpuinit calibrate_delay(void)
{
	loops_per_jiffy = CCOUNT_PER_JIFFY;
	printk("Calibrating delay loop (skipped)... "
	       "%lu.%02lu BogoMIPS preset\n",
	       loops_per_jiffy/(1000000/HZ),
	       (loops_per_jiffy/(10000/HZ)) % 100);
}
#endif
