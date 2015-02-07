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
 *
 *  Copyright (C) 1999 ARM Limited
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
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */
#include <linux/init.h>
#include <linux/stddef.h>
#include <linux/list.h>
#include <linux/timer.h>
#include <linux/version.h>
#include <linux/io.h>

#include <mach/hardware.h>
#include <asm/irq.h>

#include <asm/mach/irq.h>
#include <mach/csp/intcHw_reg.h>
#include <mach/csp/mm_io.h>

static void bcmring_mask_irq0(unsigned int irq)
{
	writel(1 << (irq - IRQ_INTC0_START),
	       MM_IO_BASE_INTC0 + INTCHW_INTENCLEAR);
}

static void bcmring_unmask_irq0(unsigned int irq)
{
	writel(1 << (irq - IRQ_INTC0_START),
	       MM_IO_BASE_INTC0 + INTCHW_INTENABLE);
}

static void bcmring_mask_irq1(unsigned int irq)
{
	writel(1 << (irq - IRQ_INTC1_START),
	       MM_IO_BASE_INTC1 + INTCHW_INTENCLEAR);
}

static void bcmring_unmask_irq1(unsigned int irq)
{
	writel(1 << (irq - IRQ_INTC1_START),
	       MM_IO_BASE_INTC1 + INTCHW_INTENABLE);
}

static void bcmring_mask_irq2(unsigned int irq)
{
	writel(1 << (irq - IRQ_SINTC_START),
	       MM_IO_BASE_SINTC + INTCHW_INTENCLEAR);
}

static void bcmring_unmask_irq2(unsigned int irq)
{
	writel(1 << (irq - IRQ_SINTC_START),
	       MM_IO_BASE_SINTC + INTCHW_INTENABLE);
}

static struct irq_chip bcmring_irq0_chip = {
	.typename = "ARM-INTC0",
	.ack = bcmring_mask_irq0,
	.mask = bcmring_mask_irq0,	/* mask a specific interrupt, blocking its delivery. */
	.unmask = bcmring_unmask_irq0,	/* unmaks an interrupt */
};

static struct irq_chip bcmring_irq1_chip = {
	.typename = "ARM-INTC1",
	.ack = bcmring_mask_irq1,
	.mask = bcmring_mask_irq1,
	.unmask = bcmring_unmask_irq1,
};

static struct irq_chip bcmring_irq2_chip = {
	.typename = "ARM-SINTC",
	.ack = bcmring_mask_irq2,
	.mask = bcmring_mask_irq2,
	.unmask = bcmring_unmask_irq2,
};

static void vic_init(void __iomem *base, struct irq_chip *chip,
		     unsigned int irq_start, unsigned int vic_sources)
{
	unsigned int i;
	for (i = 0; i < 32; i++) {
		unsigned int irq = irq_start + i;
		set_irq_chip(irq, chip);
		set_irq_chip_data(irq, base);

		if (vic_sources & (1 << i)) {
			set_irq_handler(irq, handle_level_irq);
			set_irq_flags(irq, IRQF_VALID | IRQF_PROBE);
		}
	}
	writel(0, base + INTCHW_INTSELECT);
	writel(0, base + INTCHW_INTENABLE);
	writel(~0, base + INTCHW_INTENCLEAR);
	writel(0, base + INTCHW_IRQSTATUS);
	writel(~0, base + INTCHW_SOFTINTCLEAR);
}

void __init bcmring_init_irq(void)
{
	vic_init((void __iomem *)MM_IO_BASE_INTC0, &bcmring_irq0_chip,
		 IRQ_INTC0_START, IRQ_INTC0_VALID_MASK);
	vic_init((void __iomem *)MM_IO_BASE_INTC1, &bcmring_irq1_chip,
		 IRQ_INTC1_START, IRQ_INTC1_VALID_MASK);
	vic_init((void __iomem *)MM_IO_BASE_SINTC, &bcmring_irq2_chip,
		 IRQ_SINTC_START, IRQ_SINTC_VALID_MASK);

	/* special cases */
	if (INTCHW_INTC1_GPIO0 & IRQ_INTC1_VALID_MASK) {
		set_irq_handler(IRQ_GPIO0, handle_simple_irq);
	}
	if (INTCHW_INTC1_GPIO1 & IRQ_INTC1_VALID_MASK) {
		set_irq_handler(IRQ_GPIO1, handle_simple_irq);
	}
}
