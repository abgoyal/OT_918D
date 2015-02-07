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
 * fsi-da7210.c
 *
 * Copyright (C) 2009 Renesas Solutions Corp.
 * Kuninori Morimoto <morimoto.kuninori@renesas.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 */

#include <linux/interrupt.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <linux/i2c.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/soc-dapm.h>

#include <sound/sh_fsi.h>
#include "../codecs/da7210.h"

static int fsi_da7210_init(struct snd_soc_codec *codec)
{
	return snd_soc_dai_set_fmt(&da7210_dai,
				   SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF |
				   SND_SOC_DAIFMT_CBM_CFM);
}

static struct snd_soc_dai_link fsi_da7210_dai = {
	.name		= "DA7210",
	.stream_name	= "DA7210",
	.cpu_dai	= &fsi_soc_dai[1], /* FSI B */
	.codec_dai	= &da7210_dai,
	.init		= fsi_da7210_init,
};

static struct snd_soc_card fsi_soc_card = {
	.name		= "FSI",
	.platform	= &fsi_soc_platform,
	.dai_link	= &fsi_da7210_dai,
	.num_links	= 1,
};

static struct snd_soc_device fsi_da7210_snd_devdata = {
	.card		= &fsi_soc_card,
	.codec_dev	= &soc_codec_dev_da7210,
};

static struct platform_device *fsi_da7210_snd_device;

static int __init fsi_da7210_sound_init(void)
{
	int ret;

	fsi_da7210_snd_device = platform_device_alloc("soc-audio", -1);
	if (!fsi_da7210_snd_device)
		return -ENOMEM;

	platform_set_drvdata(fsi_da7210_snd_device, &fsi_da7210_snd_devdata);
	fsi_da7210_snd_devdata.dev = &fsi_da7210_snd_device->dev;
	ret = platform_device_add(fsi_da7210_snd_device);
	if (ret)
		platform_device_put(fsi_da7210_snd_device);

	return ret;
}

static void __exit fsi_da7210_sound_exit(void)
{
	platform_device_unregister(fsi_da7210_snd_device);
}

module_init(fsi_da7210_sound_init);
module_exit(fsi_da7210_sound_exit);

/* Module information */
MODULE_DESCRIPTION("ALSA SoC FSI DA2710");
MODULE_AUTHOR("Kuninori Morimoto <morimoto.kuninori@renesas.com>");
MODULE_LICENSE("GPL");
