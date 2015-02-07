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
 * l4f00242t03.c -- support for Epson L4F00242T03 LCD
 *
 * Copyright 2007-2009 Freescale Semiconductor, Inc. All Rights Reserved.
 *
 * Copyright (c) 2009 Alberto Panizzo <maramaopercheseimorto@gmail.com>
 * 	Inspired by Marek Vasut work in l4f00242t03.c
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/lcd.h>
#include <linux/slab.h>
#include <linux/regulator/consumer.h>

#include <linux/spi/spi.h>
#include <linux/spi/l4f00242t03.h>

struct l4f00242t03_priv {
	struct spi_device	*spi;
	struct lcd_device	*ld;
	int lcd_on:1;
	struct regulator *io_reg;
	struct regulator *core_reg;
};


static void l4f00242t03_reset(unsigned int gpio)
{
	pr_debug("l4f00242t03_reset.\n");
	gpio_set_value(gpio, 1);
	mdelay(100);
	gpio_set_value(gpio, 0);
	mdelay(10);	/* tRES >= 100us */
	gpio_set_value(gpio, 1);
	mdelay(20);
}

#define param(x) ((x) | 0x100)

static void l4f00242t03_lcd_init(struct spi_device *spi)
{
	struct l4f00242t03_pdata *pdata = spi->dev.platform_data;
	struct l4f00242t03_priv *priv = dev_get_drvdata(&spi->dev);
	const u16 cmd[] = { 0x36, param(0), 0x3A, param(0x60) };

	dev_dbg(&spi->dev, "initializing LCD\n");

	if (priv->io_reg) {
		regulator_set_voltage(priv->io_reg, 1800000, 1800000);
		regulator_enable(priv->io_reg);
	}

	if (priv->core_reg) {
		regulator_set_voltage(priv->core_reg, 2800000, 2800000);
		regulator_enable(priv->core_reg);
	}

	gpio_set_value(pdata->data_enable_gpio, 1);
	msleep(60);
	spi_write(spi, (const u8 *)cmd, ARRAY_SIZE(cmd) * sizeof(u16));
}

static int l4f00242t03_lcd_power_set(struct lcd_device *ld, int power)
{
	struct l4f00242t03_priv *priv = lcd_get_data(ld);
	struct spi_device *spi = priv->spi;

	const u16 slpout = 0x11;
	const u16 dison = 0x29;

	const u16 slpin = 0x10;
	const u16 disoff = 0x28;

	if (power) {
		if (priv->lcd_on)
			return 0;

		dev_dbg(&spi->dev, "turning on LCD\n");

		spi_write(spi, (const u8 *)&slpout, sizeof(u16));
		msleep(60);
		spi_write(spi, (const u8 *)&dison, sizeof(u16));

		priv->lcd_on = 1;
	} else {
		if (!priv->lcd_on)
			return 0;

		dev_dbg(&spi->dev, "turning off LCD\n");

		spi_write(spi, (const u8 *)&disoff, sizeof(u16));
		msleep(60);
		spi_write(spi, (const u8 *)&slpin, sizeof(u16));

		priv->lcd_on = 0;
	}

	return 0;
}

static struct lcd_ops l4f_ops = {
	.set_power	= l4f00242t03_lcd_power_set,
	.get_power	= NULL,
};

static int __devinit l4f00242t03_probe(struct spi_device *spi)
{
	struct l4f00242t03_priv *priv;
	struct l4f00242t03_pdata *pdata = spi->dev.platform_data;
	int ret;

	if (pdata == NULL) {
		dev_err(&spi->dev, "Uninitialized platform data.\n");
		return -EINVAL;
	}

	priv = kzalloc(sizeof(struct l4f00242t03_priv), GFP_KERNEL);

	if (priv == NULL) {
		dev_err(&spi->dev, "No memory for this device.\n");
		return -ENOMEM;
	}

	dev_set_drvdata(&spi->dev, priv);
	spi->bits_per_word = 9;
	spi_setup(spi);

	priv->spi = spi;

	ret = gpio_request(pdata->reset_gpio, "lcd l4f00242t03 reset");
	if (ret) {
		dev_err(&spi->dev,
			"Unable to get the lcd l4f00242t03 reset gpio.\n");
		goto err;
	}

	ret = gpio_direction_output(pdata->reset_gpio, 1);
	if (ret)
		goto err2;

	ret = gpio_request(pdata->data_enable_gpio,
				"lcd l4f00242t03 data enable");
	if (ret) {
		dev_err(&spi->dev,
			"Unable to get the lcd l4f00242t03 data en gpio.\n");
		goto err2;
	}

	ret = gpio_direction_output(pdata->data_enable_gpio, 0);
	if (ret)
		goto err3;

	if (pdata->io_supply) {
		priv->io_reg = regulator_get(NULL, pdata->io_supply);

		if (IS_ERR(priv->io_reg)) {
			pr_err("%s: Unable to get the IO regulator\n",
								__func__);
			goto err3;
		}
	}

	if (pdata->core_supply) {
		priv->core_reg = regulator_get(NULL, pdata->core_supply);

		if (IS_ERR(priv->core_reg)) {
			pr_err("%s: Unable to get the core regulator\n",
								__func__);
			goto err4;
		}
	}

	priv->ld = lcd_device_register("l4f00242t03",
					&spi->dev, priv, &l4f_ops);
	if (IS_ERR(priv->ld)) {
		ret = PTR_ERR(priv->ld);
		goto err5;
	}

	/* Init the LCD */
	l4f00242t03_reset(pdata->reset_gpio);
	l4f00242t03_lcd_init(spi);
	l4f00242t03_lcd_power_set(priv->ld, 1);

	dev_info(&spi->dev, "Epson l4f00242t03 lcd probed.\n");

	return 0;

err5:
	if (priv->core_reg)
		regulator_put(priv->core_reg);
err4:
	if (priv->io_reg)
		regulator_put(priv->io_reg);
err3:
	gpio_free(pdata->data_enable_gpio);
err2:
	gpio_free(pdata->reset_gpio);
err:
	kfree(priv);

	return ret;
}

static int __devexit l4f00242t03_remove(struct spi_device *spi)
{
	struct l4f00242t03_priv *priv = dev_get_drvdata(&spi->dev);
	struct l4f00242t03_pdata *pdata = priv->spi->dev.platform_data;

	l4f00242t03_lcd_power_set(priv->ld, 0);
	lcd_device_unregister(priv->ld);

	gpio_free(pdata->data_enable_gpio);
	gpio_free(pdata->reset_gpio);

	if (priv->io_reg)
		regulator_put(priv->io_reg);
	if (priv->core_reg)
		regulator_put(priv->core_reg);

	kfree(priv);

	return 0;
}

static struct spi_driver l4f00242t03_driver = {
	.driver = {
		.name	= "l4f00242t03",
		.owner	= THIS_MODULE,
	},
	.probe		= l4f00242t03_probe,
	.remove		= __devexit_p(l4f00242t03_remove),
};

static __init int l4f00242t03_init(void)
{
	return spi_register_driver(&l4f00242t03_driver);
}

static __exit void l4f00242t03_exit(void)
{
	spi_unregister_driver(&l4f00242t03_driver);
}

module_init(l4f00242t03_init);
module_exit(l4f00242t03_exit);

MODULE_AUTHOR("Alberto Panizzo <maramaopercheseimorto@gmail.com>");
MODULE_DESCRIPTION("EPSON L4F00242T03 LCD");
MODULE_LICENSE("GPL v2");
