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
 * SH5-101/SH5-103 CPU Setup
 *
 *  Copyright (C) 2009  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 */
#include <linux/platform_device.h>
#include <linux/init.h>
#include <linux/serial.h>
#include <linux/serial_sci.h>
#include <linux/io.h>
#include <linux/mm.h>
#include <linux/sh_timer.h>
#include <asm/addrspace.h>

static struct plat_sci_port scif0_platform_data = {
	.mapbase	= PHYS_PERIPHERAL_BLOCK + 0x01030000,
	.flags		= UPF_BOOT_AUTOCONF | UPF_IOREMAP,
	.type		= PORT_SCIF,
	.irqs		= { 39, 40, 42, 0 },
};

static struct platform_device scif0_device = {
	.name		= "sh-sci",
	.id		= 0,
	.dev		= {
		.platform_data	= &scif0_platform_data,
	},
};

static struct resource rtc_resources[] = {
	[0] = {
		.start	= PHYS_PERIPHERAL_BLOCK + 0x01040000,
		.end	= PHYS_PERIPHERAL_BLOCK + 0x01040000 + 0x58 - 1,
		.flags	= IORESOURCE_IO,
	},
	[1] = {
		/* Period IRQ */
		.start	= IRQ_PRI,
		.flags	= IORESOURCE_IRQ,
	},
	[2] = {
		/* Carry IRQ */
		.start	= IRQ_CUI,
		.flags	= IORESOURCE_IRQ,
	},
	[3] = {
		/* Alarm IRQ */
		.start	= IRQ_ATI,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device rtc_device = {
	.name		= "sh-rtc",
	.id		= -1,
	.num_resources	= ARRAY_SIZE(rtc_resources),
	.resource	= rtc_resources,
};

#define	TMU_BLOCK_OFF	0x01020000
#define TMU_BASE	PHYS_PERIPHERAL_BLOCK + TMU_BLOCK_OFF
#define TMU0_BASE	(TMU_BASE + 0x8 + (0xc * 0x0))
#define TMU1_BASE	(TMU_BASE + 0x8 + (0xc * 0x1))
#define TMU2_BASE	(TMU_BASE + 0x8 + (0xc * 0x2))

static struct sh_timer_config tmu0_platform_data = {
	.channel_offset = 0x04,
	.timer_bit = 0,
	.clockevent_rating = 200,
};

static struct resource tmu0_resources[] = {
	[0] = {
		.start	= TMU0_BASE,
		.end	= TMU0_BASE + 0xc - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TUNI0,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device tmu0_device = {
	.name		= "sh_tmu",
	.id		= 0,
	.dev = {
		.platform_data	= &tmu0_platform_data,
	},
	.resource	= tmu0_resources,
	.num_resources	= ARRAY_SIZE(tmu0_resources),
};

static struct sh_timer_config tmu1_platform_data = {
	.channel_offset = 0x10,
	.timer_bit = 1,
	.clocksource_rating = 200,
};

static struct resource tmu1_resources[] = {
	[0] = {
		.start	= TMU1_BASE,
		.end	= TMU1_BASE + 0xc - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TUNI1,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device tmu1_device = {
	.name		= "sh_tmu",
	.id		= 1,
	.dev = {
		.platform_data	= &tmu1_platform_data,
	},
	.resource	= tmu1_resources,
	.num_resources	= ARRAY_SIZE(tmu1_resources),
};

static struct sh_timer_config tmu2_platform_data = {
	.channel_offset = 0x1c,
	.timer_bit = 2,
};

static struct resource tmu2_resources[] = {
	[0] = {
		.start	= TMU2_BASE,
		.end	= TMU2_BASE + 0xc - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= IRQ_TUNI2,
		.flags	= IORESOURCE_IRQ,
	},
};

static struct platform_device tmu2_device = {
	.name		= "sh_tmu",
	.id		= 2,
	.dev = {
		.platform_data	= &tmu2_platform_data,
	},
	.resource	= tmu2_resources,
	.num_resources	= ARRAY_SIZE(tmu2_resources),
};

static struct platform_device *sh5_early_devices[] __initdata = {
	&scif0_device,
	&tmu0_device,
	&tmu1_device,
	&tmu2_device,
};

static struct platform_device *sh5_devices[] __initdata = {
	&rtc_device,
};

static int __init sh5_devices_setup(void)
{
	int ret;

	ret = platform_add_devices(sh5_early_devices,
				   ARRAY_SIZE(sh5_early_devices));
	if (unlikely(ret != 0))
		return ret;

	return platform_add_devices(sh5_devices,
				    ARRAY_SIZE(sh5_devices));
}
arch_initcall(sh5_devices_setup);

void __init plat_early_device_setup(void)
{
	early_platform_add_devices(sh5_early_devices,
				   ARRAY_SIZE(sh5_early_devices));
}