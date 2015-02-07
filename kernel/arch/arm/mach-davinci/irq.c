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
 * Interrupt handler for DaVinci boards.
 *
 * Copyright (C) 2006 Texas Instruments.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <mach/cputype.h>
#include <mach/common.h>
#include <asm/mach/irq.h>

#define IRQ_BIT(irq)		((irq) & 0x1f)

#define FIQ_REG0_OFFSET		0x0000
#define FIQ_REG1_OFFSET		0x0004
#define IRQ_REG0_OFFSET		0x0008
#define IRQ_REG1_OFFSET		0x000C
#define IRQ_ENT_REG0_OFFSET	0x0018
#define IRQ_ENT_REG1_OFFSET	0x001C
#define IRQ_INCTL_REG_OFFSET	0x0020
#define IRQ_EABASE_REG_OFFSET	0x0024
#define IRQ_INTPRI0_REG_OFFSET	0x0030
#define IRQ_INTPRI7_REG_OFFSET	0x004C

static inline unsigned int davinci_irq_readl(int offset)
{
	return __raw_readl(davinci_intc_base + offset);
}

static inline void davinci_irq_writel(unsigned long value, int offset)
{
	__raw_writel(value, davinci_intc_base + offset);
}

/* Disable interrupt */
static void davinci_mask_irq(unsigned int irq)
{
	unsigned int mask;
	u32 l;

	mask = 1 << IRQ_BIT(irq);

	if (irq > 31) {
		l = davinci_irq_readl(IRQ_ENT_REG1_OFFSET);
		l &= ~mask;
		davinci_irq_writel(l, IRQ_ENT_REG1_OFFSET);
	} else {
		l = davinci_irq_readl(IRQ_ENT_REG0_OFFSET);
		l &= ~mask;
		davinci_irq_writel(l, IRQ_ENT_REG0_OFFSET);
	}
}

/* Enable interrupt */
static void davinci_unmask_irq(unsigned int irq)
{
	unsigned int mask;
	u32 l;

	mask = 1 << IRQ_BIT(irq);

	if (irq > 31) {
		l = davinci_irq_readl(IRQ_ENT_REG1_OFFSET);
		l |= mask;
		davinci_irq_writel(l, IRQ_ENT_REG1_OFFSET);
	} else {
		l = davinci_irq_readl(IRQ_ENT_REG0_OFFSET);
		l |= mask;
		davinci_irq_writel(l, IRQ_ENT_REG0_OFFSET);
	}
}

/* EOI interrupt */
static void davinci_ack_irq(unsigned int irq)
{
	unsigned int mask;

	mask = 1 << IRQ_BIT(irq);

	if (irq > 31)
		davinci_irq_writel(mask, IRQ_REG1_OFFSET);
	else
		davinci_irq_writel(mask, IRQ_REG0_OFFSET);
}

static struct irq_chip davinci_irq_chip_0 = {
	.name	= "AINTC",
	.ack	= davinci_ack_irq,
	.mask	= davinci_mask_irq,
	.unmask = davinci_unmask_irq,
};

/* ARM Interrupt Controller Initialization */
void __init davinci_irq_init(void)
{
	unsigned i;
	const u8 *davinci_def_priorities = davinci_soc_info.intc_irq_prios;

	davinci_intc_type = DAVINCI_INTC_TYPE_AINTC;
	davinci_intc_base = ioremap(davinci_soc_info.intc_base, SZ_4K);
	if (WARN_ON(!davinci_intc_base))
		return;

	/* Clear all interrupt requests */
	davinci_irq_writel(~0x0, FIQ_REG0_OFFSET);
	davinci_irq_writel(~0x0, FIQ_REG1_OFFSET);
	davinci_irq_writel(~0x0, IRQ_REG0_OFFSET);
	davinci_irq_writel(~0x0, IRQ_REG1_OFFSET);

	/* Disable all interrupts */
	davinci_irq_writel(0x0, IRQ_ENT_REG0_OFFSET);
	davinci_irq_writel(0x0, IRQ_ENT_REG1_OFFSET);

	/* Interrupts disabled immediately, IRQ entry reflects all */
	davinci_irq_writel(0x0, IRQ_INCTL_REG_OFFSET);

	/* we don't use the hardware vector table, just its entry addresses */
	davinci_irq_writel(0, IRQ_EABASE_REG_OFFSET);

	/* Clear all interrupt requests */
	davinci_irq_writel(~0x0, FIQ_REG0_OFFSET);
	davinci_irq_writel(~0x0, FIQ_REG1_OFFSET);
	davinci_irq_writel(~0x0, IRQ_REG0_OFFSET);
	davinci_irq_writel(~0x0, IRQ_REG1_OFFSET);

	for (i = IRQ_INTPRI0_REG_OFFSET; i <= IRQ_INTPRI7_REG_OFFSET; i += 4) {
		unsigned	j;
		u32		pri;

		for (j = 0, pri = 0; j < 32; j += 4, davinci_def_priorities++)
			pri |= (*davinci_def_priorities & 0x07) << j;
		davinci_irq_writel(pri, i);
	}

	/* set up genirq dispatch for ARM INTC */
	for (i = 0; i < davinci_soc_info.intc_irq_num; i++) {
		set_irq_chip(i, &davinci_irq_chip_0);
		set_irq_flags(i, IRQF_VALID | IRQF_PROBE);
		if (i != IRQ_TINT1_TINT34)
			set_irq_handler(i, handle_edge_irq);
		else
			set_irq_handler(i, handle_level_irq);
	}
}
