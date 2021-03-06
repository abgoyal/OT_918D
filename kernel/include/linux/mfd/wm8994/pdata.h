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
 * include/linux/mfd/wm8994/pdata.h -- Platform data for WM8994
 *
 * Copyright 2009 Wolfson Microelectronics PLC.
 *
 * Author: Mark Brown <broonie@opensource.wolfsonmicro.com>
 *
 *  This program is free software; you can redistribute  it and/or modify it
 *  under  the terms of  the GNU General  Public License as published by the
 *  Free Software Foundation;  either version 2 of the  License, or (at your
 *  option) any later version.
 *
 */

#ifndef __MFD_WM8994_PDATA_H__
#define __MFD_WM8994_PDATA_H__

#define WM8994_NUM_LDO   2
#define WM8994_NUM_GPIO 11

struct wm8994_ldo_pdata {
	/** GPIOs to enable regulator, 0 or less if not available */
	int enable;

	const char *supply;
	struct regulator_init_data *init_data;
};

#define WM8994_CONFIGURE_GPIO 0x8000

#define WM8994_DRC_REGS 5
#define WM8994_EQ_REGS  19

/**
 * DRC configurations are specified with a label and a set of register
 * values to write (the enable bits will be ignored).  At runtime an
 * enumerated control will be presented for each DRC block allowing
 * the user to choose the configration to use.
 *
 * Configurations may be generated by hand or by using the DRC control
 * panel provided by the WISCE - see  http://www.wolfsonmicro.com/wisce/
 * for details.
 */
struct wm8994_drc_cfg {
        const char *name;
        u16 regs[WM8994_DRC_REGS];
};

/**
 * ReTune Mobile configurations are specified with a label, sample
 * rate and set of values to write (the enable bits will be ignored).
 *
 * Configurations are expected to be generated using the ReTune Mobile
 * control panel in WISCE - see http://www.wolfsonmicro.com/wisce/
 */
struct wm8994_retune_mobile_cfg {
        const char *name;
        unsigned int rate;
        u16 regs[WM8994_EQ_REGS];
};

struct wm8994_pdata {
	int gpio_base;

	/**
	 * Default values for GPIOs if non-zero, WM8994_CONFIGURE_GPIO
	 * can be used for all zero values.
	 */
	int gpio_defaults[WM8994_NUM_GPIO];

	struct wm8994_ldo_pdata ldo[WM8994_NUM_LDO];

	int irq_base;  /** Base IRQ number for WM8994, required for IRQs */

        int num_drc_cfgs;
        struct wm8994_drc_cfg *drc_cfgs;

        int num_retune_mobile_cfgs;
        struct wm8994_retune_mobile_cfg *retune_mobile_cfgs;

        /* LINEOUT can be differential or single ended */
        unsigned int lineout1_diff:1;
        unsigned int lineout2_diff:1;

        /* Common mode feedback */
        unsigned int lineout1fb:1;
        unsigned int lineout2fb:1;

        /* Microphone biases: 0=0.9*AVDD1 1=0.65*AVVD1 */
        unsigned int micbias1_lvl:1;
        unsigned int micbias2_lvl:1;

        /* Jack detect threashold levels, see datasheet for values */
        unsigned int jd_scthr:2;
        unsigned int jd_thr:2;
};

#endif
