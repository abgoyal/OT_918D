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
 * PCI support for the Sega Dreamcast
 *
 * Copyright (C) 2001, 2002  M. R. Brown
 * Copyright (C) 2002, 2003  Paul Mundt
 *
 * This file originally bore the message (with enclosed-$):
 *	Id: pci.c,v 1.3 2003/05/04 19:29:46 lethal Exp
 *	Dreamcast PCI: Supports SEGA Broadband Adaptor only.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/param.h>
#include <linux/interrupt.h>
#include <linux/init.h>
#include <linux/irq.h>
#include <linux/pci.h>
#include <linux/module.h>
#include <asm/io.h>
#include <asm/irq.h>
#include <mach/pci.h>

static struct resource gapspci_resources[] = {
	{
		.name	= "GAPSPCI IO",
		.start	= GAPSPCI_BBA_CONFIG,
		.end	= GAPSPCI_BBA_CONFIG + GAPSPCI_BBA_CONFIG_SIZE - 1,
		.flags	= IORESOURCE_IO,
	},  {
		.name	= "GAPSPCI mem",
		.start	= GAPSPCI_DMA_BASE,
		.end	= GAPSPCI_DMA_BASE + GAPSPCI_DMA_SIZE - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct pci_channel dreamcast_pci_controller = {
	.pci_ops	= &gapspci_pci_ops,
	.resources	= gapspci_resources,
	.nr_resources	= ARRAY_SIZE(gapspci_resources),
	.io_offset	= 0x00000000,
	.mem_offset	= 0x00000000,
};

/*
 * gapspci init
 */

static int __init gapspci_init(void)
{
	char idbuf[16];
	int i;

	/*
	 * FIXME: All of this wants documenting to some degree,
	 * even some basic register definitions would be nice.
	 *
	 * I haven't seen anything this ugly since.. maple.
	 */

	for (i=0; i<16; i++)
		idbuf[i] = inb(GAPSPCI_REGS+i);

	if (strncmp(idbuf, "GAPSPCI_BRIDGE_2", 16))
		return -ENODEV;

	outl(0x5a14a501, GAPSPCI_REGS+0x18);

	for (i=0; i<1000000; i++)
		cpu_relax();

	if (inl(GAPSPCI_REGS+0x18) != 1)
		return -EINVAL;

	outl(0x01000000, GAPSPCI_REGS+0x20);
	outl(0x01000000, GAPSPCI_REGS+0x24);

	outl(GAPSPCI_DMA_BASE, GAPSPCI_REGS+0x28);
	outl(GAPSPCI_DMA_BASE+GAPSPCI_DMA_SIZE, GAPSPCI_REGS+0x2c);

	outl(1, GAPSPCI_REGS+0x14);
	outl(1, GAPSPCI_REGS+0x34);

	/* Setting Broadband Adapter */
	outw(0xf900, GAPSPCI_BBA_CONFIG+0x06);
	outl(0x00000000, GAPSPCI_BBA_CONFIG+0x30);
	outb(0x00, GAPSPCI_BBA_CONFIG+0x3c);
	outb(0xf0, GAPSPCI_BBA_CONFIG+0x0d);
	outw(0x0006, GAPSPCI_BBA_CONFIG+0x04);
	outl(0x00002001, GAPSPCI_BBA_CONFIG+0x10);
	outl(0x01000000, GAPSPCI_BBA_CONFIG+0x14);

	return register_pci_controller(&dreamcast_pci_controller);
}
arch_initcall(gapspci_init);
