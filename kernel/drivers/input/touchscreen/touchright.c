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
 * Touchright serial touchscreen driver
 *
 * Copyright (c) 2006 Rick Koch <n1gp@hotmail.com>
 *
 * Based on MicroTouch driver (drivers/input/touchscreen/mtouch.c)
 * Copyright (c) 2004 Vojtech Pavlik
 * and Dan Streetman <ddstreet@ieee.org>
 */

/*
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#include <linux/errno.h>
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/input.h>
#include <linux/serio.h>
#include <linux/init.h>

#define DRIVER_DESC	"Touchright serial touchscreen driver"

MODULE_AUTHOR("Rick Koch <n1gp@hotmail.com>");
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");

/*
 * Definitions & global arrays.
 */

#define TR_FORMAT_TOUCH_BIT	0x01
#define TR_FORMAT_STATUS_BYTE	0x40
#define TR_FORMAT_STATUS_MASK	~TR_FORMAT_TOUCH_BIT

#define TR_LENGTH 5

#define TR_MIN_XC 0
#define TR_MAX_XC 0x1ff
#define TR_MIN_YC 0
#define TR_MAX_YC 0x1ff

/*
 * Per-touchscreen data.
 */

struct tr {
	struct input_dev *dev;
	struct serio *serio;
	int idx;
	unsigned char data[TR_LENGTH];
	char phys[32];
};

static irqreturn_t tr_interrupt(struct serio *serio,
		unsigned char data, unsigned int flags)
{
	struct tr *tr = serio_get_drvdata(serio);
	struct input_dev *dev = tr->dev;

	tr->data[tr->idx] = data;

	if ((tr->data[0] & TR_FORMAT_STATUS_MASK) == TR_FORMAT_STATUS_BYTE) {
		if (++tr->idx == TR_LENGTH) {
			input_report_abs(dev, ABS_X,
				(tr->data[1] << 5) | (tr->data[2] >> 1));
			input_report_abs(dev, ABS_Y,
				(tr->data[3] << 5) | (tr->data[4] >> 1));
			input_report_key(dev, BTN_TOUCH,
				tr->data[0] & TR_FORMAT_TOUCH_BIT);
			input_sync(dev);
			tr->idx = 0;
		}
	}

	return IRQ_HANDLED;
}

/*
 * tr_disconnect() is the opposite of tr_connect()
 */

static void tr_disconnect(struct serio *serio)
{
	struct tr *tr = serio_get_drvdata(serio);

	input_get_device(tr->dev);
	input_unregister_device(tr->dev);
	serio_close(serio);
	serio_set_drvdata(serio, NULL);
	input_put_device(tr->dev);
	kfree(tr);
}

/*
 * tr_connect() is the routine that is called when someone adds a
 * new serio device that supports the Touchright protocol and registers it as
 * an input device.
 */

static int tr_connect(struct serio *serio, struct serio_driver *drv)
{
	struct tr *tr;
	struct input_dev *input_dev;
	int err;

	tr = kzalloc(sizeof(struct tr), GFP_KERNEL);
	input_dev = input_allocate_device();
	if (!tr || !input_dev) {
		err = -ENOMEM;
		goto fail1;
	}

	tr->serio = serio;
	tr->dev = input_dev;
	snprintf(tr->phys, sizeof(tr->phys), "%s/input0", serio->phys);

	input_dev->name = "Touchright Serial TouchScreen";
	input_dev->phys = tr->phys;
	input_dev->id.bustype = BUS_RS232;
	input_dev->id.vendor = SERIO_TOUCHRIGHT;
	input_dev->id.product = 0;
	input_dev->id.version = 0x0100;
	input_dev->dev.parent = &serio->dev;
	input_dev->evbit[0] = BIT_MASK(EV_KEY) | BIT_MASK(EV_ABS);
	input_dev->keybit[BIT_WORD(BTN_TOUCH)] = BIT_MASK(BTN_TOUCH);
	input_set_abs_params(tr->dev, ABS_X, TR_MIN_XC, TR_MAX_XC, 0, 0);
	input_set_abs_params(tr->dev, ABS_Y, TR_MIN_YC, TR_MAX_YC, 0, 0);

	serio_set_drvdata(serio, tr);

	err = serio_open(serio, drv);
	if (err)
		goto fail2;

	err = input_register_device(tr->dev);
	if (err)
		goto fail3;

	return 0;

 fail3:	serio_close(serio);
 fail2:	serio_set_drvdata(serio, NULL);
 fail1:	input_free_device(input_dev);
	kfree(tr);
	return err;
}

/*
 * The serio driver structure.
 */

static struct serio_device_id tr_serio_ids[] = {
	{
		.type	= SERIO_RS232,
		.proto	= SERIO_TOUCHRIGHT,
		.id	= SERIO_ANY,
		.extra	= SERIO_ANY,
	},
	{ 0 }
};

MODULE_DEVICE_TABLE(serio, tr_serio_ids);

static struct serio_driver tr_drv = {
	.driver		= {
		.name	= "touchright",
	},
	.description	= DRIVER_DESC,
	.id_table	= tr_serio_ids,
	.interrupt	= tr_interrupt,
	.connect	= tr_connect,
	.disconnect	= tr_disconnect,
};

/*
 * The functions for inserting/removing us as a module.
 */

static int __init tr_init(void)
{
	return serio_register_driver(&tr_drv);
}

static void __exit tr_exit(void)
{
	serio_unregister_driver(&tr_drv);
}

module_init(tr_init);
module_exit(tr_exit);