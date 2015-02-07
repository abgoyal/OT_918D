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
 *  Amstrad E3 FIQ handling
 *
 *  Copyright (C) 2009 Janusz Krzysztofik
 *  Copyright (c) 2006 Matt Callow
 *  Copyright (c) 2004 Amstrad Plc
 *  Copyright (C) 2001 RidgeRun, Inc.
 *
 * Parts of this code are taken from linux/arch/arm/mach-omap/irq.c
 * in the MontaVista 2.4 kernel (and the Amstrad changes therein)
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published by
 * the Free Software Foundation.
 */
#include <linux/gpio.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/module.h>
#include <linux/io.h>

#include <plat/board-ams-delta.h>

#include <asm/fiq.h>
#include <mach/ams-delta-fiq.h>

static struct fiq_handler fh = {
	.name	= "ams-delta-fiq"
};

/*
 * This buffer is shared between FIQ and IRQ contexts.
 * The FIQ and IRQ isrs can both read and write it.
 * It is structured as a header section several 32bit slots,
 * followed by the circular buffer where the FIQ isr stores
 * keystrokes received from the qwerty keyboard.
 * See ams-delta-fiq.h for details of offsets.
 */
unsigned int fiq_buffer[1024];
EXPORT_SYMBOL(fiq_buffer);

static unsigned int irq_counter[16];

static irqreturn_t deferred_fiq(int irq, void *dev_id)
{
	struct irq_desc *irq_desc;
	struct irq_chip *irq_chip = NULL;
	int gpio, irq_num, fiq_count;

	irq_desc = irq_to_desc(IH_GPIO_BASE);
	if (irq_desc)
		irq_chip = irq_desc->chip;

	/*
	 * For each handled GPIO interrupt, keep calling its interrupt handler
	 * until the IRQ counter catches the FIQ incremented interrupt counter.
	 */
	for (gpio = AMS_DELTA_GPIO_PIN_KEYBRD_CLK;
			gpio <= AMS_DELTA_GPIO_PIN_HOOK_SWITCH; gpio++) {
		irq_num = gpio_to_irq(gpio);
		fiq_count = fiq_buffer[FIQ_CNT_INT_00 + gpio];

		while (irq_counter[gpio] < fiq_count) {
			if (gpio != AMS_DELTA_GPIO_PIN_KEYBRD_CLK) {
				/*
				 * It looks like handle_edge_irq() that
				 * OMAP GPIO edge interrupts default to,
				 * expects interrupt already unmasked.
				 */
				if (irq_chip && irq_chip->unmask)
					irq_chip->unmask(irq_num);
			}
			generic_handle_irq(irq_num);

			irq_counter[gpio]++;
		}
	}
	return IRQ_HANDLED;
}

void __init ams_delta_init_fiq(void)
{
	void *fiqhandler_start;
	unsigned int fiqhandler_length;
	struct pt_regs FIQ_regs;
	unsigned long val, offset;
	int i, retval;

	fiqhandler_start = &qwerty_fiqin_start;
	fiqhandler_length = &qwerty_fiqin_end - &qwerty_fiqin_start;
	pr_info("Installing fiq handler from %p, length 0x%x\n",
			fiqhandler_start, fiqhandler_length);

	retval = claim_fiq(&fh);
	if (retval) {
		pr_err("ams_delta_init_fiq(): couldn't claim FIQ, ret=%d\n",
				retval);
		return;
	}

	retval = request_irq(INT_DEFERRED_FIQ, deferred_fiq,
			IRQ_TYPE_EDGE_RISING, "deferred_fiq", 0);
	if (retval < 0) {
		pr_err("Failed to get deferred_fiq IRQ, ret=%d\n", retval);
		release_fiq(&fh);
		return;
	}
	/*
	 * Since no set_type() method is provided by OMAP irq chip,
	 * switch to edge triggered interrupt type manually.
	 */
	offset = IRQ_ILR0_REG_OFFSET + INT_DEFERRED_FIQ * 0x4;
	val = omap_readl(DEFERRED_FIQ_IH_BASE + offset) & ~(1 << 1);
	omap_writel(val, DEFERRED_FIQ_IH_BASE + offset);

	set_fiq_handler(fiqhandler_start, fiqhandler_length);

	/*
	 * Initialise the buffer which is shared
	 * between FIQ mode and IRQ mode
	 */
	fiq_buffer[FIQ_GPIO_INT_MASK]	= 0;
	fiq_buffer[FIQ_MASK]		= 0;
	fiq_buffer[FIQ_STATE]		= 0;
	fiq_buffer[FIQ_KEY]		= 0;
	fiq_buffer[FIQ_KEYS_CNT]	= 0;
	fiq_buffer[FIQ_KEYS_HICNT]	= 0;
	fiq_buffer[FIQ_TAIL_OFFSET]	= 0;
	fiq_buffer[FIQ_HEAD_OFFSET]	= 0;
	fiq_buffer[FIQ_BUF_LEN]		= 256;
	fiq_buffer[FIQ_MISSED_KEYS]	= 0;
	fiq_buffer[FIQ_BUFFER_START]	=
			(unsigned int) &fiq_buffer[FIQ_CIRC_BUFF];

	for (i = FIQ_CNT_INT_00; i <= FIQ_CNT_INT_15; i++)
		fiq_buffer[i] = 0;

	/*
	 * FIQ mode r9 always points to the fiq_buffer, becauses the FIQ isr
	 * will run in an unpredictable context. The fiq_buffer is the FIQ isr's
	 * only means of communication with the IRQ level and other kernel
	 * context code.
	 */
	FIQ_regs.ARM_r9 = (unsigned int)fiq_buffer;
	set_fiq_regs(&FIQ_regs);

	pr_info("request_fiq(): fiq_buffer = %p\n", fiq_buffer);

	/*
	 * Redirect GPIO interrupts to FIQ
	 */
	offset = IRQ_ILR0_REG_OFFSET + INT_GPIO_BANK1 * 0x4;
	val = omap_readl(OMAP_IH1_BASE + offset) | 1;
	omap_writel(val, OMAP_IH1_BASE + offset);
}
