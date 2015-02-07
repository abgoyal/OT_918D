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
 * Backlight code for nVidia based graphic cards
 *
 * Copyright 2004 Antonino Daplas <adaplas@pol.net>
 * Copyright (c) 2006 Michael Hanselmann <linux-kernel@hansmi.ch>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/backlight.h>
#include <linux/fb.h>
#include <linux/pci.h>

#ifdef CONFIG_PMAC_BACKLIGHT
#include <asm/backlight.h>
#endif

#include "nv_local.h"
#include "nv_type.h"
#include "nv_proto.h"

/* We do not have any information about which values are allowed, thus
 * we used safe values.
 */
#define MIN_LEVEL 0x158
#define MAX_LEVEL 0x534
#define LEVEL_STEP ((MAX_LEVEL - MIN_LEVEL) / FB_BACKLIGHT_MAX)

static int nvidia_bl_get_level_brightness(struct nvidia_par *par,
		int level)
{
	struct fb_info *info = pci_get_drvdata(par->pci_dev);
	int nlevel;

	/* Get and convert the value */
	/* No locking of bl_curve since we read a single value */
	nlevel = MIN_LEVEL + info->bl_curve[level] * LEVEL_STEP;

	if (nlevel < 0)
		nlevel = 0;
	else if (nlevel < MIN_LEVEL)
		nlevel = MIN_LEVEL;
	else if (nlevel > MAX_LEVEL)
		nlevel = MAX_LEVEL;

	return nlevel;
}

static int nvidia_bl_update_status(struct backlight_device *bd)
{
	struct nvidia_par *par = bl_get_data(bd);
	u32 tmp_pcrt, tmp_pmc, fpcontrol;
	int level;

	if (!par->FlatPanel)
		return 0;

	if (bd->props.power != FB_BLANK_UNBLANK ||
	    bd->props.fb_blank != FB_BLANK_UNBLANK)
		level = 0;
	else
		level = bd->props.brightness;

	tmp_pmc = NV_RD32(par->PMC, 0x10F0) & 0x0000FFFF;
	tmp_pcrt = NV_RD32(par->PCRTC0, 0x081C) & 0xFFFFFFFC;
	fpcontrol = NV_RD32(par->PRAMDAC, 0x0848) & 0xCFFFFFCC;

	if (level > 0) {
		tmp_pcrt |= 0x1;
		tmp_pmc |= (1 << 31); /* backlight bit */
		tmp_pmc |= nvidia_bl_get_level_brightness(par, level) << 16;
		fpcontrol |= par->fpSyncs;
	} else
		fpcontrol |= 0x20000022;

	NV_WR32(par->PCRTC0, 0x081C, tmp_pcrt);
	NV_WR32(par->PMC, 0x10F0, tmp_pmc);
	NV_WR32(par->PRAMDAC, 0x848, fpcontrol);

	return 0;
}

static int nvidia_bl_get_brightness(struct backlight_device *bd)
{
	return bd->props.brightness;
}

static struct backlight_ops nvidia_bl_ops = {
	.get_brightness = nvidia_bl_get_brightness,
	.update_status	= nvidia_bl_update_status,
};

void nvidia_bl_init(struct nvidia_par *par)
{
	struct backlight_properties props;
	struct fb_info *info = pci_get_drvdata(par->pci_dev);
	struct backlight_device *bd;
	char name[12];

	if (!par->FlatPanel)
		return;

#ifdef CONFIG_PMAC_BACKLIGHT
	if (!machine_is(powermac) ||
	    !pmac_has_backlight_type("mnca"))
		return;
#endif

	snprintf(name, sizeof(name), "nvidiabl%d", info->node);

	memset(&props, 0, sizeof(struct backlight_properties));
	props.max_brightness = FB_BACKLIGHT_LEVELS - 1;
	bd = backlight_device_register(name, info->dev, par, &nvidia_bl_ops,
				       &props);
	if (IS_ERR(bd)) {
		info->bl_dev = NULL;
		printk(KERN_WARNING "nvidia: Backlight registration failed\n");
		goto error;
	}

	info->bl_dev = bd;
	fb_bl_default_curve(info, 0,
		0x158 * FB_BACKLIGHT_MAX / MAX_LEVEL,
		0x534 * FB_BACKLIGHT_MAX / MAX_LEVEL);

	bd->props.brightness = bd->props.max_brightness;
	bd->props.power = FB_BLANK_UNBLANK;
	backlight_update_status(bd);

	printk("nvidia: Backlight initialized (%s)\n", name);

	return;

error:
	return;
}

void nvidia_bl_exit(struct nvidia_par *par)
{
	struct fb_info *info = pci_get_drvdata(par->pci_dev);
	struct backlight_device *bd = info->bl_dev;

	backlight_device_unregister(bd);
	printk("nvidia: Backlight unloaded\n");
}
