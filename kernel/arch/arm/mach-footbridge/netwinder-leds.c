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
 *  linux/arch/arm/mach-footbridge/netwinder-leds.c
 *
 *  Copyright (C) 1998-1999 Russell King
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * NetWinder LED control routines.
 *
 * The Netwinder uses the leds as follows:
 *  - Green - toggles state every 50 timer interrupts
 *  - Red   - On if the system is not idle
 *
 * Changelog:
 *   02-05-1999	RMK	Various cleanups
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/spinlock.h>

#include <mach/hardware.h>
#include <asm/leds.h>
#include <asm/mach-types.h>
#include <asm/system.h>

#define LED_STATE_ENABLED	1
#define LED_STATE_CLAIMED	2
static char led_state;
static char hw_led_state;

static DEFINE_SPINLOCK(leds_lock);

static void netwinder_leds_event(led_event_t evt)
{
	unsigned long flags;

	spin_lock_irqsave(&leds_lock, flags);

	switch (evt) {
	case led_start:
		led_state |= LED_STATE_ENABLED;
		hw_led_state = GPIO_GREEN_LED;
		break;

	case led_stop:
		led_state &= ~LED_STATE_ENABLED;
		break;

	case led_claim:
		led_state |= LED_STATE_CLAIMED;
		hw_led_state = 0;
		break;

	case led_release:
		led_state &= ~LED_STATE_CLAIMED;
		hw_led_state = 0;
		break;

#ifdef CONFIG_LEDS_TIMER
	case led_timer:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state ^= GPIO_GREEN_LED;
		break;
#endif

#ifdef CONFIG_LEDS_CPU
	case led_idle_start:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state &= ~GPIO_RED_LED;
		break;

	case led_idle_end:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state |= GPIO_RED_LED;
		break;
#endif

	case led_halted:
		if (!(led_state & LED_STATE_CLAIMED))
			hw_led_state |= GPIO_RED_LED;
		break;

	case led_green_on:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state |= GPIO_GREEN_LED;
		break;

	case led_green_off:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state &= ~GPIO_GREEN_LED;
		break;

	case led_amber_on:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state |= GPIO_GREEN_LED | GPIO_RED_LED;
		break;

	case led_amber_off:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state &= ~(GPIO_GREEN_LED | GPIO_RED_LED);
		break;

	case led_red_on:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state |= GPIO_RED_LED;
		break;

	case led_red_off:
		if (led_state & LED_STATE_CLAIMED)
			hw_led_state &= ~GPIO_RED_LED;
		break;

	default:
		break;
	}

	spin_unlock_irqrestore(&leds_lock, flags);

	if  (led_state & LED_STATE_ENABLED) {
		spin_lock_irqsave(&nw_gpio_lock, flags);
		nw_gpio_modify_op(GPIO_RED_LED | GPIO_GREEN_LED, hw_led_state);
		spin_unlock_irqrestore(&nw_gpio_lock, flags);
	}
}

static int __init leds_init(void)
{
	if (machine_is_netwinder())
		leds_event = netwinder_leds_event;

	leds_event(led_start);

	return 0;
}

__initcall(leds_init);
