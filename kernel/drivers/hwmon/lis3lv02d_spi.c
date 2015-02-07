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
 * lis3lv02d_spi - SPI glue layer for lis3lv02d
 *
 * Copyright (c) 2009 Daniel Mack <daniel@caiaq.de>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2 as
 *  publishhed by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/err.h>
#include <linux/input.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <linux/spi/spi.h>

#include "lis3lv02d.h"

#define DRV_NAME 	"lis3lv02d_spi"
#define LIS3_SPI_READ	0x80

static int lis3_spi_read(struct lis3lv02d *lis3, int reg, u8 *v)
{
	struct spi_device *spi = lis3->bus_priv;
	int ret = spi_w8r8(spi, reg | LIS3_SPI_READ);
	if (ret < 0)
		return -EINVAL;

	*v = (u8) ret;
	return 0;
}

static int lis3_spi_write(struct lis3lv02d *lis3, int reg, u8 val)
{
	u8 tmp[2] = { reg, val };
	struct spi_device *spi = lis3->bus_priv;
	return spi_write(spi, tmp, sizeof(tmp));
}

static int lis3_spi_init(struct lis3lv02d *lis3)
{
	u8 reg;
	int ret;

	/* power up the device */
	ret = lis3->read(lis3, CTRL_REG1, &reg);
	if (ret < 0)
		return ret;

	reg |= CTRL1_PD0;
	return lis3->write(lis3, CTRL_REG1, reg);
}

static struct axis_conversion lis3lv02d_axis_normal = { 1, 2, 3 };

static int __devinit lis302dl_spi_probe(struct spi_device *spi)
{
	int ret;

	spi->bits_per_word = 8;
	spi->mode = SPI_MODE_0;
	ret = spi_setup(spi);
	if (ret < 0)
		return ret;

	lis3_dev.bus_priv	= spi;
	lis3_dev.init		= lis3_spi_init;
	lis3_dev.read		= lis3_spi_read;
	lis3_dev.write		= lis3_spi_write;
	lis3_dev.irq		= spi->irq;
	lis3_dev.ac		= lis3lv02d_axis_normal;
	lis3_dev.pdata		= spi->dev.platform_data;
	spi_set_drvdata(spi, &lis3_dev);

	return lis3lv02d_init_device(&lis3_dev);
}

static int __devexit lis302dl_spi_remove(struct spi_device *spi)
{
	struct lis3lv02d *lis3 = spi_get_drvdata(spi);
	lis3lv02d_joystick_disable();
	lis3lv02d_poweroff(lis3);

	return lis3lv02d_remove_fs(&lis3_dev);
}

#ifdef CONFIG_PM
static int lis3lv02d_spi_suspend(struct spi_device *spi, pm_message_t mesg)
{
	struct lis3lv02d *lis3 = spi_get_drvdata(spi);

	if (!lis3->pdata->wakeup_flags)
		lis3lv02d_poweroff(&lis3_dev);

	return 0;
}

static int lis3lv02d_spi_resume(struct spi_device *spi)
{
	struct lis3lv02d *lis3 = spi_get_drvdata(spi);

	if (!lis3->pdata->wakeup_flags)
		lis3lv02d_poweron(lis3);

	return 0;
}

#else
#define lis3lv02d_spi_suspend	NULL
#define lis3lv02d_spi_resume	NULL
#endif

static struct spi_driver lis302dl_spi_driver = {
	.driver	 = {
		.name   = DRV_NAME,
		.owner  = THIS_MODULE,
	},
	.probe	= lis302dl_spi_probe,
	.remove	= __devexit_p(lis302dl_spi_remove),
	.suspend = lis3lv02d_spi_suspend,
	.resume  = lis3lv02d_spi_resume,
};

static int __init lis302dl_init(void)
{
	return spi_register_driver(&lis302dl_spi_driver);
}

static void __exit lis302dl_exit(void)
{
	spi_unregister_driver(&lis302dl_spi_driver);
}

module_init(lis302dl_init);
module_exit(lis302dl_exit);

MODULE_AUTHOR("Daniel Mack <daniel@caiaq.de>");
MODULE_DESCRIPTION("lis3lv02d SPI glue layer");
MODULE_LICENSE("GPL");
MODULE_ALIAS("spi:" DRV_NAME);
