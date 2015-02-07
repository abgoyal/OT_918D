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
 * arch/sh/drivers/superhyway/ops-sh4-202.c
 *
 * SuperHyway bus support for SH4-202
 *
 * Copyright (C) 2005  Paul Mundt
 *
 * This file is subject to the terms and conditions of the GNU
 * General Public License.  See the file "COPYING" in the main
 * directory of this archive for more details.
 */
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/superhyway.h>
#include <linux/string.h>
#include <asm/addrspace.h>
#include <asm/io.h>

#define PHYS_EMI_CBLOCK		P4SEGADDR(0x1ec00000)
#define PHYS_EMI_DBLOCK		P4SEGADDR(0x08000000)
#define PHYS_FEMI_CBLOCK	P4SEGADDR(0x1f800000)
#define PHYS_FEMI_DBLOCK	P4SEGADDR(0x00000000)

#define PHYS_EPBR_BLOCK		P4SEGADDR(0x1de00000)
#define PHYS_DMAC_BLOCK		P4SEGADDR(0x1fa00000)
#define PHYS_PBR_BLOCK		P4SEGADDR(0x1fc00000)

static struct resource emi_resources[] = {
	[0] = {
		.start	= PHYS_EMI_CBLOCK,
		.end	= PHYS_EMI_CBLOCK + 0x00300000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= PHYS_EMI_DBLOCK,
		.end	= PHYS_EMI_DBLOCK + 0x08000000 - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct superhyway_device emi_device = {
	.name		= "emi",
	.num_resources	= ARRAY_SIZE(emi_resources),
	.resource	= emi_resources,
};

static struct resource femi_resources[] = {
	[0] = {
		.start	= PHYS_FEMI_CBLOCK,
		.end	= PHYS_FEMI_CBLOCK + 0x00100000 - 1,
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= PHYS_FEMI_DBLOCK,
		.end	= PHYS_FEMI_DBLOCK + 0x08000000 - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct superhyway_device femi_device = {
	.name		= "femi",
	.num_resources	= ARRAY_SIZE(femi_resources),
	.resource	= femi_resources,
};

static struct resource epbr_resources[] = {
	[0] = {
		.start	= P4SEGADDR(0x1e7ffff8),
		.end	= P4SEGADDR(0x1e7ffff8 + (sizeof(u32) * 2) - 1),
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= PHYS_EPBR_BLOCK,
		.end	= PHYS_EPBR_BLOCK + 0x00a00000 - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct superhyway_device epbr_device = {
	.name		= "epbr",
	.num_resources	= ARRAY_SIZE(epbr_resources),
	.resource	= epbr_resources,
};

static struct resource dmac_resource = {
	.start	= PHYS_DMAC_BLOCK,
	.end	= PHYS_DMAC_BLOCK + 0x00100000 - 1,
	.flags	= IORESOURCE_MEM,
};

static struct superhyway_device dmac_device = {
	.name		= "dmac",
	.num_resources	= 1,
	.resource	= &dmac_resource,
};

static struct resource pbr_resources[] = {
	[0] = {
		.start	= P4SEGADDR(0x1ffffff8),
		.end	= P4SEGADDR(0x1ffffff8 + (sizeof(u32) * 2) - 1),
		.flags	= IORESOURCE_MEM,
	},
	[1] = {
		.start	= PHYS_PBR_BLOCK,
		.end	= PHYS_PBR_BLOCK + 0x00400000 - (sizeof(u32) * 2) - 1,
		.flags	= IORESOURCE_MEM,
	},
};

static struct superhyway_device pbr_device = {
	.name		= "pbr",
	.num_resources	= ARRAY_SIZE(pbr_resources),
	.resource	= pbr_resources,
};

static struct superhyway_device *sh4202_devices[] __initdata = {
	&emi_device, &femi_device, &epbr_device, &dmac_device, &pbr_device,
};

static int sh4202_read_vcr(unsigned long base, struct superhyway_vcr_info *vcr)
{
	u32 vcrh, vcrl;
	u64 tmp;

	/*
	 * XXX: Even though the SH4-202 Evaluation Device documentation
	 * indicates that VCRL is mapped first with VCRH at a + 0x04
	 * offset, the opposite seems to be true.
	 *
	 * Some modules (PBR and ePBR for instance) also appear to have
	 * VCRL/VCRH flipped in the documentation, but on the SH4-202
	 * itself it appears that these are all consistently mapped with
	 * VCRH preceding VCRL.
	 *
	 * Do not trust the documentation, for it is evil.
	 */
	vcrh = __raw_readl(base);
	vcrl = __raw_readl(base + sizeof(u32));

	tmp = ((u64)vcrh << 32) | vcrl;
	memcpy(vcr, &tmp, sizeof(u64));

	return 0;
}

static int sh4202_write_vcr(unsigned long base, struct superhyway_vcr_info vcr)
{
	u64 tmp = *(u64 *)&vcr;

	__raw_writel((tmp >> 32) & 0xffffffff, base);
	__raw_writel(tmp & 0xffffffff, base + sizeof(u32));

	return 0;
}

static struct superhyway_ops sh4202_superhyway_ops = {
	.read_vcr	= sh4202_read_vcr,
	.write_vcr	= sh4202_write_vcr,
};

struct superhyway_bus superhyway_channels[] = {
	{ &sh4202_superhyway_ops, },
	{ 0, },
};

int __init superhyway_scan_bus(struct superhyway_bus *bus)
{
	return superhyway_add_devices(bus, sh4202_devices,
				      ARRAY_SIZE(sh4202_devices));
}

