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

/* linux/arch/arm/mach-s5pv210/gpiolib.c
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * S5PV210 - GPIOlib support
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/kernel.h>
#include <linux/irq.h>
#include <linux/io.h>
#include <linux/gpio.h>
#include <plat/gpio-core.h>
#include <plat/gpio-cfg.h>
#include <plat/gpio-cfg-helpers.h>
#include <mach/map.h>

static struct s3c_gpio_cfg gpio_cfg = {
	.set_config	= s3c_gpio_setcfg_s3c64xx_4bit,
	.set_pull	= s3c_gpio_setpull_updown,
	.get_pull	= s3c_gpio_getpull_updown,
};

static struct s3c_gpio_cfg gpio_cfg_noint = {
	.set_config	= s3c_gpio_setcfg_s3c64xx_4bit,
	.set_pull	= s3c_gpio_setpull_updown,
	.get_pull	= s3c_gpio_getpull_updown,
};

/* GPIO bank's base address given the index of the bank in the
 * list of all gpio banks.
 */
#define S5PV210_BANK_BASE(bank_nr)	(S5P_VA_GPIO + ((bank_nr) * 0x20))

/*
 * Following are the gpio banks in v210.
 *
 * The 'config' member when left to NULL, is initialized to the default
 * structure gpio_cfg in the init function below.
 *
 * The 'base' member is also initialized in the init function below.
 * Note: The initialization of 'base' member of s3c_gpio_chip structure
 * uses the above macro and depends on the banks being listed in order here.
 */
static struct s3c_gpio_chip s5pv210_gpio_4bit[] = {
	{
		.chip	= {
			.base	= S5PV210_GPA0(0),
			.ngpio	= S5PV210_GPIO_A0_NR,
			.label	= "GPA0",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPA1(0),
			.ngpio	= S5PV210_GPIO_A1_NR,
			.label	= "GPA1",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPB(0),
			.ngpio	= S5PV210_GPIO_B_NR,
			.label	= "GPB",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPC0(0),
			.ngpio	= S5PV210_GPIO_C0_NR,
			.label	= "GPC0",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPC1(0),
			.ngpio	= S5PV210_GPIO_C1_NR,
			.label	= "GPC1",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPD0(0),
			.ngpio	= S5PV210_GPIO_D0_NR,
			.label	= "GPD0",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPD1(0),
			.ngpio	= S5PV210_GPIO_D1_NR,
			.label	= "GPD1",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPE0(0),
			.ngpio	= S5PV210_GPIO_E0_NR,
			.label	= "GPE0",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPE1(0),
			.ngpio	= S5PV210_GPIO_E1_NR,
			.label	= "GPE1",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPF0(0),
			.ngpio	= S5PV210_GPIO_F0_NR,
			.label	= "GPF0",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPF1(0),
			.ngpio	= S5PV210_GPIO_F1_NR,
			.label	= "GPF1",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPF2(0),
			.ngpio	= S5PV210_GPIO_F2_NR,
			.label	= "GPF2",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPF3(0),
			.ngpio	= S5PV210_GPIO_F3_NR,
			.label	= "GPF3",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPG0(0),
			.ngpio	= S5PV210_GPIO_G0_NR,
			.label	= "GPG0",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPG1(0),
			.ngpio	= S5PV210_GPIO_G1_NR,
			.label	= "GPG1",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPG2(0),
			.ngpio	= S5PV210_GPIO_G2_NR,
			.label	= "GPG2",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPG3(0),
			.ngpio	= S5PV210_GPIO_G3_NR,
			.label	= "GPG3",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPI(0),
			.ngpio	= S5PV210_GPIO_I_NR,
			.label	= "GPI",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPJ0(0),
			.ngpio	= S5PV210_GPIO_J0_NR,
			.label	= "GPJ0",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPJ1(0),
			.ngpio	= S5PV210_GPIO_J1_NR,
			.label	= "GPJ1",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPJ2(0),
			.ngpio	= S5PV210_GPIO_J2_NR,
			.label	= "GPJ2",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPJ3(0),
			.ngpio	= S5PV210_GPIO_J3_NR,
			.label	= "GPJ3",
		},
	}, {
		.chip	= {
			.base	= S5PV210_GPJ4(0),
			.ngpio	= S5PV210_GPIO_J4_NR,
			.label	= "GPJ4",
		},
	}, {
		.config	= &gpio_cfg_noint,
		.chip	= {
			.base	= S5PV210_MP01(0),
			.ngpio	= S5PV210_GPIO_MP01_NR,
			.label	= "MP01",
		},
	}, {
		.config	= &gpio_cfg_noint,
		.chip	= {
			.base	= S5PV210_MP02(0),
			.ngpio	= S5PV210_GPIO_MP02_NR,
			.label	= "MP02",
		},
	}, {
		.config	= &gpio_cfg_noint,
		.chip	= {
			.base	= S5PV210_MP03(0),
			.ngpio	= S5PV210_GPIO_MP03_NR,
			.label	= "MP03",
		},
	}, {
		.base	= (S5P_VA_GPIO + 0xC00),
		.config	= &gpio_cfg_noint,
		.chip	= {
			.base	= S5PV210_GPH0(0),
			.ngpio	= S5PV210_GPIO_H0_NR,
			.label	= "GPH0",
		},
	}, {
		.base	= (S5P_VA_GPIO + 0xC20),
		.config	= &gpio_cfg_noint,
		.chip	= {
			.base	= S5PV210_GPH1(0),
			.ngpio	= S5PV210_GPIO_H1_NR,
			.label	= "GPH1",
		},
	}, {
		.base	= (S5P_VA_GPIO + 0xC40),
		.config	= &gpio_cfg_noint,
		.chip	= {
			.base	= S5PV210_GPH2(0),
			.ngpio	= S5PV210_GPIO_H2_NR,
			.label	= "GPH2",
		},
	}, {
		.base	= (S5P_VA_GPIO + 0xC60),
		.config	= &gpio_cfg_noint,
		.chip	= {
			.base	= S5PV210_GPH3(0),
			.ngpio	= S5PV210_GPIO_H3_NR,
			.label	= "GPH3",
		},
	},
};

static __init int s5pv210_gpiolib_init(void)
{
	struct s3c_gpio_chip *chip = s5pv210_gpio_4bit;
	int nr_chips = ARRAY_SIZE(s5pv210_gpio_4bit);
	int i = 0;

	for (i = 0; i < nr_chips; i++, chip++) {
		if (chip->config == NULL)
			chip->config = &gpio_cfg;
		if (chip->base == NULL)
			chip->base = S5PV210_BANK_BASE(i);
	}

	samsung_gpiolib_add_4bit_chips(s5pv210_gpio_4bit, nr_chips);

	return 0;
}
core_initcall(s5pv210_gpiolib_init);