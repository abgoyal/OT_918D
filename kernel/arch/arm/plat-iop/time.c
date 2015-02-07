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
 * arch/arm/plat-iop/time.c
 *
 * Timer code for IOP32x and IOP33x based systems
 *
 * Author: Deepak Saxena <dsaxena@mvista.com>
 *
 * Copyright 2002-2003 MontaVista Software Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 */

#include <linux/kernel.h>
#include <linux/interrupt.h>
#include <linux/time.h>
#include <linux/init.h>
#include <linux/timex.h>
#include <linux/io.h>
#include <linux/clocksource.h>
#include <linux/clockchips.h>
#include <mach/hardware.h>
#include <asm/irq.h>
#include <asm/uaccess.h>
#include <asm/mach/irq.h>
#include <asm/mach/time.h>
#include <mach/time.h>

/*
 * IOP clocksource (free-running timer 1).
 */
static cycle_t iop_clocksource_read(struct clocksource *unused)
{
	return 0xffffffffu - read_tcr1();
}

static struct clocksource iop_clocksource = {
	.name 		= "iop_timer1",
	.rating		= 300,
	.read		= iop_clocksource_read,
	.mask		= CLOCKSOURCE_MASK(32),
	.flags		= CLOCK_SOURCE_IS_CONTINUOUS,
};

static void __init iop_clocksource_set_hz(struct clocksource *cs, unsigned int hz)
{
	u64 temp;
	u32 shift;

	/* Find shift and mult values for hz. */
	shift = 32;
	do {
		temp = (u64) NSEC_PER_SEC << shift;
		do_div(temp, hz);
		if ((temp >> 32) == 0)
			break;
	} while (--shift != 0);

	cs->shift = shift;
	cs->mult = (u32) temp;

	printk(KERN_INFO "clocksource: %s uses shift %u mult %#x\n",
	       cs->name, cs->shift, cs->mult);
}

/*
 * IOP sched_clock() implementation via its clocksource.
 */
unsigned long long sched_clock(void)
{
	cycle_t cyc = iop_clocksource_read(NULL);
	struct clocksource *cs = &iop_clocksource;

	return clocksource_cyc2ns(cyc, cs->mult, cs->shift);
}

/*
 * IOP clockevents (interrupting timer 0).
 */
static int iop_set_next_event(unsigned long delta,
			      struct clock_event_device *unused)
{
	u32 tmr = IOP_TMR_PRIVILEGED | IOP_TMR_RATIO_1_1;

	BUG_ON(delta == 0);
	write_tmr0(tmr & ~(IOP_TMR_EN | IOP_TMR_RELOAD));
	write_tcr0(delta);
	write_tmr0((tmr & ~IOP_TMR_RELOAD) | IOP_TMR_EN);

	return 0;
}

static unsigned long ticks_per_jiffy;

static void iop_set_mode(enum clock_event_mode mode,
			 struct clock_event_device *unused)
{
	u32 tmr = read_tmr0();

	switch (mode) {
	case CLOCK_EVT_MODE_PERIODIC:
		write_tmr0(tmr & ~IOP_TMR_EN);
		write_tcr0(ticks_per_jiffy - 1);
		tmr |= (IOP_TMR_RELOAD | IOP_TMR_EN);
		break;
	case CLOCK_EVT_MODE_ONESHOT:
		/* ->set_next_event sets period and enables timer */
		tmr &= ~(IOP_TMR_RELOAD | IOP_TMR_EN);
		break;
	case CLOCK_EVT_MODE_RESUME:
		tmr |= IOP_TMR_EN;
		break;
	case CLOCK_EVT_MODE_SHUTDOWN:
	case CLOCK_EVT_MODE_UNUSED:
	default:
		tmr &= ~IOP_TMR_EN;
		break;
	}

	write_tmr0(tmr);
}

static struct clock_event_device iop_clockevent = {
	.name		= "iop_timer0",
	.features       = CLOCK_EVT_FEAT_PERIODIC | CLOCK_EVT_FEAT_ONESHOT,
	.rating         = 300,
	.set_next_event	= iop_set_next_event,
	.set_mode	= iop_set_mode,
};

static void __init iop_clockevent_set_hz(struct clock_event_device *ce, unsigned int hz)
{
	u64 temp;
	u32 shift;

	/* Find shift and mult values for hz. */
	shift = 32;
	do {
		temp = (u64) hz << shift;
		do_div(temp, NSEC_PER_SEC);
		if ((temp >> 32) == 0)
			break;
	} while (--shift != 0);

	ce->shift = shift;
	ce->mult = (u32) temp;

	printk(KERN_INFO "clockevent: %s uses shift %u mult %#lx\n",
	       ce->name, ce->shift, ce->mult);
}

static irqreturn_t
iop_timer_interrupt(int irq, void *dev_id)
{
	struct clock_event_device *evt = dev_id;

	write_tisr(1);
	evt->event_handler(evt);
	return IRQ_HANDLED;
}

static struct irqaction iop_timer_irq = {
	.name		= "IOP Timer Tick",
	.handler	= iop_timer_interrupt,
	.flags		= IRQF_DISABLED | IRQF_TIMER | IRQF_IRQPOLL,
	.dev_id		= &iop_clockevent,
};

static unsigned long iop_tick_rate;
unsigned long get_iop_tick_rate(void)
{
	return iop_tick_rate;
}
EXPORT_SYMBOL(get_iop_tick_rate);

void __init iop_init_time(unsigned long tick_rate)
{
	u32 timer_ctl;

	ticks_per_jiffy = DIV_ROUND_CLOSEST(tick_rate, HZ);
	iop_tick_rate = tick_rate;

	timer_ctl = IOP_TMR_EN | IOP_TMR_PRIVILEGED |
			IOP_TMR_RELOAD | IOP_TMR_RATIO_1_1;

	/*
	 * Set up interrupting clockevent timer 0.
	 */
	write_tmr0(timer_ctl & ~IOP_TMR_EN);
	setup_irq(IRQ_IOP_TIMER0, &iop_timer_irq);
	iop_clockevent_set_hz(&iop_clockevent, tick_rate);
	iop_clockevent.max_delta_ns =
		clockevent_delta2ns(0xfffffffe, &iop_clockevent);
	iop_clockevent.min_delta_ns =
		clockevent_delta2ns(0xf, &iop_clockevent);
	iop_clockevent.cpumask = cpumask_of(0);
	clockevents_register_device(&iop_clockevent);
	write_trr0(ticks_per_jiffy - 1);
	write_tcr0(ticks_per_jiffy - 1);
	write_tmr0(timer_ctl);

	/*
	 * Set up free-running clocksource timer 1.
	 */
	write_trr1(0xffffffff);
	write_tcr1(0xffffffff);
	write_tmr1(timer_ctl);
	iop_clocksource_set_hz(&iop_clocksource, tick_rate);
	clocksource_register(&iop_clocksource);
}
