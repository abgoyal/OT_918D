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
 * TX4939 irq routines
 * Based on linux/arch/mips/kernel/irq_txx9.c,
 *	    and RBTX49xx patch from CELF patch archive.
 *
 * Copyright 2001, 2003-2005 MontaVista Software Inc.
 * Author: MontaVista Software, Inc.
 *         ahennessy@mvista.com
 *         source@mvista.com
 * Copyright (C) 2000-2001,2005-2007 Toshiba Corporation
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
/*
 * TX4939 defines 64 IRQs.
 * Similer to irq_txx9.c but different register layouts.
 */
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/types.h>
#include <asm/irq_cpu.h>
#include <asm/txx9irq.h>
#include <asm/txx9/tx4939.h>

/* IRCER : Int. Control Enable */
#define TXx9_IRCER_ICE	0x00000001

/* IRCR : Int. Control */
#define TXx9_IRCR_LOW	0x00000000
#define TXx9_IRCR_HIGH	0x00000001
#define TXx9_IRCR_DOWN	0x00000002
#define TXx9_IRCR_UP	0x00000003
#define TXx9_IRCR_EDGE(cr)	((cr) & 0x00000002)

/* IRSCR : Int. Status Control */
#define TXx9_IRSCR_EIClrE	0x00000100
#define TXx9_IRSCR_EIClr_MASK	0x0000000f

/* IRCSR : Int. Current Status */
#define TXx9_IRCSR_IF	0x00010000

#define irc_dlevel	0
#define irc_elevel	1

static struct {
	unsigned char level;
	unsigned char mode;
} tx4939irq[TX4939_NUM_IR] __read_mostly;

static void tx4939_irq_unmask(unsigned int irq)
{
	unsigned int irq_nr = irq - TXX9_IRQ_BASE;
	u32 __iomem *lvlp;
	int ofs;
	if (irq_nr < 32) {
		irq_nr--;
		lvlp = &tx4939_ircptr->lvl[(irq_nr % 16) / 2].r;
	} else {
		irq_nr -= 32;
		lvlp = &tx4939_ircptr->lvl[8 + (irq_nr % 16) / 2].r;
	}
	ofs = (irq_nr & 16) + (irq_nr & 1) * 8;
	__raw_writel((__raw_readl(lvlp) & ~(0xff << ofs))
		     | (tx4939irq[irq_nr].level << ofs),
		     lvlp);
}

static inline void tx4939_irq_mask(unsigned int irq)
{
	unsigned int irq_nr = irq - TXX9_IRQ_BASE;
	u32 __iomem *lvlp;
	int ofs;
	if (irq_nr < 32) {
		irq_nr--;
		lvlp = &tx4939_ircptr->lvl[(irq_nr % 16) / 2].r;
	} else {
		irq_nr -= 32;
		lvlp = &tx4939_ircptr->lvl[8 + (irq_nr % 16) / 2].r;
	}
	ofs = (irq_nr & 16) + (irq_nr & 1) * 8;
	__raw_writel((__raw_readl(lvlp) & ~(0xff << ofs))
		     | (irc_dlevel << ofs),
		     lvlp);
	mmiowb();
}

static void tx4939_irq_mask_ack(unsigned int irq)
{
	unsigned int irq_nr = irq - TXX9_IRQ_BASE;

	tx4939_irq_mask(irq);
	if (TXx9_IRCR_EDGE(tx4939irq[irq_nr].mode)) {
		irq_nr--;
		/* clear edge detection */
		__raw_writel((TXx9_IRSCR_EIClrE | (irq_nr & 0xf))
			     << (irq_nr & 0x10),
			     &tx4939_ircptr->edc.r);
	}
}

static int tx4939_irq_set_type(unsigned int irq, unsigned int flow_type)
{
	unsigned int irq_nr = irq - TXX9_IRQ_BASE;
	u32 cr;
	u32 __iomem *crp;
	int ofs;
	int mode;

	if (flow_type & IRQF_TRIGGER_PROBE)
		return 0;
	switch (flow_type & IRQF_TRIGGER_MASK) {
	case IRQF_TRIGGER_RISING:
		mode = TXx9_IRCR_UP;
		break;
	case IRQF_TRIGGER_FALLING:
		mode = TXx9_IRCR_DOWN;
		break;
	case IRQF_TRIGGER_HIGH:
		mode = TXx9_IRCR_HIGH;
		break;
	case IRQF_TRIGGER_LOW:
		mode = TXx9_IRCR_LOW;
		break;
	default:
		return -EINVAL;
	}
	if (irq_nr < 32) {
		irq_nr--;
		crp = &tx4939_ircptr->dm[(irq_nr & 8) >> 3].r;
	} else {
		irq_nr -= 32;
		crp = &tx4939_ircptr->dm2[((irq_nr & 8) >> 3)].r;
	}
	ofs = (((irq_nr & 16) >> 1) | (irq_nr & (8 - 1))) * 2;
	cr = __raw_readl(crp);
	cr &= ~(0x3 << ofs);
	cr |= (mode & 0x3) << ofs;
	__raw_writel(cr, crp);
	tx4939irq[irq_nr].mode = mode;
	return 0;
}

static struct irq_chip tx4939_irq_chip = {
	.name		= "TX4939",
	.ack		= tx4939_irq_mask_ack,
	.mask		= tx4939_irq_mask,
	.mask_ack	= tx4939_irq_mask_ack,
	.unmask		= tx4939_irq_unmask,
	.set_type	= tx4939_irq_set_type,
};

static int tx4939_irq_set_pri(int irc_irq, int new_pri)
{
	int old_pri;

	if ((unsigned int)irc_irq >= TX4939_NUM_IR)
		return 0;
	old_pri = tx4939irq[irc_irq].level;
	tx4939irq[irc_irq].level = new_pri;
	return old_pri;
}

void __init tx4939_irq_init(void)
{
	int i;

	mips_cpu_irq_init();
	/* disable interrupt control */
	__raw_writel(0, &tx4939_ircptr->den.r);
	__raw_writel(0, &tx4939_ircptr->maskint.r);
	__raw_writel(0, &tx4939_ircptr->maskext.r);
	/* irq_base + 0 is not used */
	for (i = 1; i < TX4939_NUM_IR; i++) {
		tx4939irq[i].level = 4; /* middle level */
		tx4939irq[i].mode = TXx9_IRCR_LOW;
		set_irq_chip_and_handler(TXX9_IRQ_BASE + i,
					 &tx4939_irq_chip, handle_level_irq);
	}

	/* mask all IRC interrupts */
	__raw_writel(0, &tx4939_ircptr->msk.r);
	for (i = 0; i < 16; i++)
		__raw_writel(0, &tx4939_ircptr->lvl[i].r);
	/* setup IRC interrupt mode (Low Active) */
	for (i = 0; i < 2; i++)
		__raw_writel(0, &tx4939_ircptr->dm[i].r);
	for (i = 0; i < 2; i++)
		__raw_writel(0, &tx4939_ircptr->dm2[i].r);
	/* enable interrupt control */
	__raw_writel(TXx9_IRCER_ICE, &tx4939_ircptr->den.r);
	__raw_writel(irc_elevel, &tx4939_ircptr->msk.r);

	set_irq_chained_handler(MIPS_CPU_IRQ_BASE + TX4939_IRC_INT,
				handle_simple_irq);

	/* raise priority for errors, timers, sio */
	tx4939_irq_set_pri(TX4939_IR_WTOERR, 7);
	tx4939_irq_set_pri(TX4939_IR_PCIERR, 7);
	tx4939_irq_set_pri(TX4939_IR_PCIPME, 7);
	for (i = 0; i < TX4939_NUM_IR_TMR; i++)
		tx4939_irq_set_pri(TX4939_IR_TMR(i), 6);
	for (i = 0; i < TX4939_NUM_IR_SIO; i++)
		tx4939_irq_set_pri(TX4939_IR_SIO(i), 5);
}

int tx4939_irq(void)
{
	u32 csr = __raw_readl(&tx4939_ircptr->cs.r);

	if (likely(!(csr & TXx9_IRCSR_IF)))
		return TXX9_IRQ_BASE + (csr & (TX4939_NUM_IR - 1));
	return -1;
}
