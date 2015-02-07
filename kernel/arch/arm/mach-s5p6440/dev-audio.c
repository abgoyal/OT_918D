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

/* linux/arch/arm/mach-s5p6440/dev-audio.c
 *
 * Copyright (c) 2010 Samsung Electronics Co. Ltd
 *	Jaswinder Singh <jassi.brar@samsung.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/platform_device.h>
#include <linux/dma-mapping.h>

#include <plat/gpio-cfg.h>
#include <plat/audio.h>

#include <mach/gpio.h>
#include <mach/map.h>
#include <mach/dma.h>
#include <mach/irqs.h>

static int s5p6440_cfg_i2s(struct platform_device *pdev)
{
	/* configure GPIO for i2s port */
	switch (pdev->id) {
	case -1:
		s3c_gpio_cfgpin(S5P6440_GPR(4), S3C_GPIO_SFN(5));
		s3c_gpio_cfgpin(S5P6440_GPR(5), S3C_GPIO_SFN(5));
		s3c_gpio_cfgpin(S5P6440_GPR(6), S3C_GPIO_SFN(5));
		s3c_gpio_cfgpin(S5P6440_GPR(7), S3C_GPIO_SFN(5));
		s3c_gpio_cfgpin(S5P6440_GPR(8), S3C_GPIO_SFN(5));
		s3c_gpio_cfgpin(S5P6440_GPR(13), S3C_GPIO_SFN(5));
		s3c_gpio_cfgpin(S5P6440_GPR(14), S3C_GPIO_SFN(5));
		break;

	default:
		printk(KERN_ERR "Invalid Device %d\n", pdev->id);
		return -EINVAL;
	}

	return 0;
}

static struct s3c_audio_pdata s3c_i2s_pdata = {
	.cfg_gpio = s5p6440_cfg_i2s,
};

static struct resource s5p6440_iis0_resource[] = {
	[0] = {
		.start = S5P6440_PA_I2S,
		.end   = S5P6440_PA_I2S + 0x100 - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = DMACH_I2S0_TX,
		.end   = DMACH_I2S0_TX,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.start = DMACH_I2S0_RX,
		.end   = DMACH_I2S0_RX,
		.flags = IORESOURCE_DMA,
	},
};

struct platform_device s5p6440_device_iis = {
	.name		  = "s3c64xx-iis-v4",
	.id		  = -1,
	.num_resources	  = ARRAY_SIZE(s5p6440_iis0_resource),
	.resource	  = s5p6440_iis0_resource,
	.dev = {
		.platform_data = &s3c_i2s_pdata,
	},
};

/* PCM Controller platform_devices */

static int s5p6440_pcm_cfg_gpio(struct platform_device *pdev)
{
	switch (pdev->id) {
	case 0:
		s3c_gpio_cfgpin(S5P6440_GPR(7), S3C_GPIO_SFN(2));
		s3c_gpio_cfgpin(S5P6440_GPR(13), S3C_GPIO_SFN(2));
		s3c_gpio_cfgpin(S5P6440_GPR(14), S3C_GPIO_SFN(2));
		s3c_gpio_cfgpin(S5P6440_GPR(8), S3C_GPIO_SFN(2));
		s3c_gpio_cfgpin(S5P6440_GPR(6), S3C_GPIO_SFN(2));
		break;

	default:
		printk(KERN_DEBUG "Invalid PCM Controller number!");
		return -EINVAL;
	}

	return 0;
}

static struct s3c_audio_pdata s3c_pcm_pdata = {
	.cfg_gpio = s5p6440_pcm_cfg_gpio,
};

static struct resource s5p6440_pcm0_resource[] = {
	[0] = {
		.start = S5P6440_PA_PCM,
		.end   = S5P6440_PA_PCM + 0x100 - 1,
		.flags = IORESOURCE_MEM,
	},
	[1] = {
		.start = DMACH_PCM0_TX,
		.end   = DMACH_PCM0_TX,
		.flags = IORESOURCE_DMA,
	},
	[2] = {
		.start = DMACH_PCM0_RX,
		.end   = DMACH_PCM0_RX,
		.flags = IORESOURCE_DMA,
	},
};

struct platform_device s5p6440_device_pcm = {
	.name		  = "samsung-pcm",
	.id		  = 0,
	.num_resources	  = ARRAY_SIZE(s5p6440_pcm0_resource),
	.resource	  = s5p6440_pcm0_resource,
	.dev = {
		.platform_data = &s3c_pcm_pdata,
	},
};
