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
 * include/linux/mfd/wm8994/core.h -- Core interface for WM8994
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

#ifndef __MFD_WM8994_CORE_H__
#define __MFD_WM8994_CORE_H__

#include <linux/interrupt.h>

struct regulator_dev;
struct regulator_bulk_data;

#define WM8994_NUM_GPIO_REGS 11
#define WM8994_NUM_LDO_REGS   2
#define WM8994_NUM_IRQ_REGS   2

#define WM8994_IRQ_TEMP_SHUT		0
#define WM8994_IRQ_MIC1_DET		1
#define WM8994_IRQ_MIC1_SHRT		2
#define WM8994_IRQ_MIC2_DET		3
#define WM8994_IRQ_MIC2_SHRT		4
#define WM8994_IRQ_FLL1_LOCK		5
#define WM8994_IRQ_FLL2_LOCK		6
#define WM8994_IRQ_SRC1_LOCK		7
#define WM8994_IRQ_SRC2_LOCK		8
#define WM8994_IRQ_AIF1DRC1_SIG_DET	9
#define WM8994_IRQ_AIF1DRC2_SIG_DET	10
#define WM8994_IRQ_AIF2DRC_SIG_DET	11
#define WM8994_IRQ_FIFOS_ERR		12
#define WM8994_IRQ_WSEQ_DONE		13
#define WM8994_IRQ_DCS_DONE		14
#define WM8994_IRQ_TEMP_WARN		15

/* GPIOs in the chip are numbered from 1-11 */
#define WM8994_IRQ_GPIO(x) (x + WM8994_IRQ_TEMP_WARN)

struct wm8994 {
	struct mutex io_lock;
	struct mutex irq_lock;

	struct device *dev;
	int (*read_dev)(struct wm8994 *wm8994, unsigned short reg,
			int bytes, void *dest);
	int (*write_dev)(struct wm8994 *wm8994, unsigned short reg,
			 int bytes, void *src);

	void *control_data;

	int gpio_base;
	int irq_base;

	int irq;
	u16 irq_masks_cur[WM8994_NUM_IRQ_REGS];
	u16 irq_masks_cache[WM8994_NUM_IRQ_REGS];

	/* Used over suspend/resume */
	u16 ldo_regs[WM8994_NUM_LDO_REGS];
	u16 gpio_regs[WM8994_NUM_GPIO_REGS];

	struct regulator_dev *dbvdd;
	struct regulator_bulk_data *supplies;
};

/* Device I/O API */
int wm8994_reg_read(struct wm8994 *wm8994, unsigned short reg);
int wm8994_reg_write(struct wm8994 *wm8994, unsigned short reg,
		 unsigned short val);
int wm8994_set_bits(struct wm8994 *wm8994, unsigned short reg,
		    unsigned short mask, unsigned short val);
int wm8994_bulk_read(struct wm8994 *wm8994, unsigned short reg,
		     int count, u16 *buf);


/* Helper to save on boilerplate */
static inline int wm8994_request_irq(struct wm8994 *wm8994, int irq,
				     irq_handler_t handler, const char *name,
				     void *data)
{
	if (!wm8994->irq_base)
		return -EINVAL;
	return request_threaded_irq(wm8994->irq_base + irq, NULL, handler,
				    IRQF_TRIGGER_RISING, name,
				    data);
}
static inline void wm8994_free_irq(struct wm8994 *wm8994, int irq, void *data)
{
	if (!wm8994->irq_base)
		return;
	free_irq(wm8994->irq_base + irq, data);
}

int wm8994_irq_init(struct wm8994 *wm8994);
void wm8994_irq_exit(struct wm8994 *wm8994);

#endif
