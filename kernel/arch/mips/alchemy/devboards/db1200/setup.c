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
 * Alchemy/AMD/RMI DB1200 board setup.
 *
 * Licensed under the terms outlined in the file COPYING in the root of
 * this source archive.
 */

#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/io.h>
#include <linux/kernel.h>
#include <asm/mach-au1x00/au1000.h>
#include <asm/mach-db1x00/bcsr.h>
#include <asm/mach-db1x00/db1200.h>

const char *get_system_type(void)
{
	return "Alchemy Db1200";
}

void __init board_setup(void)
{
	unsigned long freq0, clksrc, div, pfc;
	unsigned short whoami;

	bcsr_init(DB1200_BCSR_PHYS_ADDR,
		  DB1200_BCSR_PHYS_ADDR + DB1200_BCSR_HEXLED_OFS);

	whoami = bcsr_read(BCSR_WHOAMI);
	printk(KERN_INFO "Alchemy/AMD/RMI DB1200 Board, CPLD Rev %d"
		"  Board-ID %d  Daughtercard ID %d\n",
		(whoami >> 4) & 0xf, (whoami >> 8) & 0xf, whoami & 0xf);

	/* SMBus/SPI on PSC0, Audio on PSC1 */
	pfc = __raw_readl((void __iomem *)SYS_PINFUNC);
	pfc &= ~(SYS_PINFUNC_P0A | SYS_PINFUNC_P0B);
	pfc &= ~(SYS_PINFUNC_P1A | SYS_PINFUNC_P1B | SYS_PINFUNC_FS3);
	pfc |= SYS_PINFUNC_P1C;	/* SPI is configured later */
	__raw_writel(pfc, (void __iomem *)SYS_PINFUNC);
	wmb();

	/* Clock configurations: PSC0: ~50MHz via Clkgen0, derived from
	 * CPU clock; all other clock generators off/unused.
	 */
	div = (get_au1x00_speed() + 25000000) / 50000000;
	if (div & 1)
		div++;
	div = ((div >> 1) - 1) & 0xff;

	freq0 = div << SYS_FC_FRDIV0_BIT;
	__raw_writel(freq0, (void __iomem *)SYS_FREQCTRL0);
	wmb();
	freq0 |= SYS_FC_FE0;	/* enable F0 */
	__raw_writel(freq0, (void __iomem *)SYS_FREQCTRL0);
	wmb();

	/* psc0_intclk comes 1:1 from F0 */
	clksrc = SYS_CS_MUX_FQ0 << SYS_CS_ME0_BIT;
	__raw_writel(clksrc, (void __iomem *)SYS_CLKSRC);
	wmb();
}

static int __init db1200_arch_init(void)
{
	/* GPIO7 is low-level triggered CPLD cascade */
	set_irq_type(AU1200_GPIO7_INT, IRQF_TRIGGER_LOW);
	bcsr_init_irq(DB1200_INT_BEGIN, DB1200_INT_END, AU1200_GPIO7_INT);

	/* insert/eject pairs: one of both is always screaming.  To avoid
	 * issues they must not be automatically enabled when initially
	 * requested.
	 */
	irq_to_desc(DB1200_SD0_INSERT_INT)->status |= IRQ_NOAUTOEN;
	irq_to_desc(DB1200_SD0_EJECT_INT)->status |= IRQ_NOAUTOEN;
	irq_to_desc(DB1200_PC0_INSERT_INT)->status |= IRQ_NOAUTOEN;
	irq_to_desc(DB1200_PC0_EJECT_INT)->status |= IRQ_NOAUTOEN;
	irq_to_desc(DB1200_PC1_INSERT_INT)->status |= IRQ_NOAUTOEN;
	irq_to_desc(DB1200_PC1_EJECT_INT)->status |= IRQ_NOAUTOEN;

	return 0;
}
arch_initcall(db1200_arch_init);
