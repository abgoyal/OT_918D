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
 * Support code for the SCOOP interface found on various Sharp PDAs
 *
 * Copyright (c) 2004 Richard Purdie
 *
 *	Based on code written by Sharp/Lineo for 2.4 kernels
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 */

#include <linux/device.h>
#include <linux/string.h>
#include <linux/slab.h>
#include <linux/platform_device.h>
#include <linux/io.h>
#include <asm/gpio.h>
#include <asm/hardware/scoop.h>

/* PCMCIA to Scoop linkage

   There is no easy way to link multiple scoop devices into one
   single entity for the pxa2xx_pcmcia device so this structure
   is used which is setup by the platform code.

   This file is never modular so this symbol is always
   accessile to the board support files.
*/
struct scoop_pcmcia_config *platform_scoop_config;
EXPORT_SYMBOL(platform_scoop_config);

struct  scoop_dev {
	void __iomem *base;
	struct gpio_chip gpio;
	spinlock_t scoop_lock;
	unsigned short suspend_clr;
	unsigned short suspend_set;
	u32 scoop_gpwr;
};

void reset_scoop(struct device *dev)
{
	struct scoop_dev *sdev = dev_get_drvdata(dev);

	iowrite16(0x0100, sdev->base + SCOOP_MCR);  // 00
	iowrite16(0x0000, sdev->base + SCOOP_CDR);  // 04
	iowrite16(0x0000, sdev->base + SCOOP_CCR);  // 10
	iowrite16(0x0000, sdev->base + SCOOP_IMR);  // 18
	iowrite16(0x00FF, sdev->base + SCOOP_IRM);  // 14
	iowrite16(0x0000, sdev->base + SCOOP_ISR);  // 1C
	iowrite16(0x0000, sdev->base + SCOOP_IRM);
}

static void __scoop_gpio_set(struct scoop_dev *sdev,
			unsigned offset, int value)
{
	unsigned short gpwr;

	gpwr = ioread16(sdev->base + SCOOP_GPWR);
	if (value)
		gpwr |= 1 << (offset + 1);
	else
		gpwr &= ~(1 << (offset + 1));
	iowrite16(gpwr, sdev->base + SCOOP_GPWR);
}

static void scoop_gpio_set(struct gpio_chip *chip, unsigned offset, int value)
{
	struct scoop_dev *sdev = container_of(chip, struct scoop_dev, gpio);
	unsigned long flags;

	spin_lock_irqsave(&sdev->scoop_lock, flags);

	__scoop_gpio_set(sdev, offset, value);

	spin_unlock_irqrestore(&sdev->scoop_lock, flags);
}

static int scoop_gpio_get(struct gpio_chip *chip, unsigned offset)
{
	struct scoop_dev *sdev = container_of(chip, struct scoop_dev, gpio);

	/* XXX: I'm unsure, but it seems so */
	return ioread16(sdev->base + SCOOP_GPRR) & (1 << (offset + 1));
}

static int scoop_gpio_direction_input(struct gpio_chip *chip,
			unsigned offset)
{
	struct scoop_dev *sdev = container_of(chip, struct scoop_dev, gpio);
	unsigned long flags;
	unsigned short gpcr;

	spin_lock_irqsave(&sdev->scoop_lock, flags);

	gpcr = ioread16(sdev->base + SCOOP_GPCR);
	gpcr &= ~(1 << (offset + 1));
	iowrite16(gpcr, sdev->base + SCOOP_GPCR);

	spin_unlock_irqrestore(&sdev->scoop_lock, flags);

	return 0;
}

static int scoop_gpio_direction_output(struct gpio_chip *chip,
			unsigned offset, int value)
{
	struct scoop_dev *sdev = container_of(chip, struct scoop_dev, gpio);
	unsigned long flags;
	unsigned short gpcr;

	spin_lock_irqsave(&sdev->scoop_lock, flags);

	__scoop_gpio_set(sdev, offset, value);

	gpcr = ioread16(sdev->base + SCOOP_GPCR);
	gpcr |= 1 << (offset + 1);
	iowrite16(gpcr, sdev->base + SCOOP_GPCR);

	spin_unlock_irqrestore(&sdev->scoop_lock, flags);

	return 0;
}

unsigned short read_scoop_reg(struct device *dev, unsigned short reg)
{
	struct scoop_dev *sdev = dev_get_drvdata(dev);
	return ioread16(sdev->base + reg);
}

void write_scoop_reg(struct device *dev, unsigned short reg, unsigned short data)
{
	struct scoop_dev *sdev = dev_get_drvdata(dev);
	iowrite16(data, sdev->base + reg);
}

EXPORT_SYMBOL(reset_scoop);
EXPORT_SYMBOL(read_scoop_reg);
EXPORT_SYMBOL(write_scoop_reg);

#ifdef CONFIG_PM
static void check_scoop_reg(struct scoop_dev *sdev)
{
	unsigned short mcr;

	mcr = ioread16(sdev->base + SCOOP_MCR);
	if ((mcr & 0x100) == 0)
		iowrite16(0x0101, sdev->base + SCOOP_MCR);
}

static int scoop_suspend(struct platform_device *dev, pm_message_t state)
{
	struct scoop_dev *sdev = platform_get_drvdata(dev);

	check_scoop_reg(sdev);
	sdev->scoop_gpwr = ioread16(sdev->base + SCOOP_GPWR);
	iowrite16((sdev->scoop_gpwr & ~sdev->suspend_clr) | sdev->suspend_set, sdev->base + SCOOP_GPWR);

	return 0;
}

static int scoop_resume(struct platform_device *dev)
{
	struct scoop_dev *sdev = platform_get_drvdata(dev);

	check_scoop_reg(sdev);
	iowrite16(sdev->scoop_gpwr, sdev->base + SCOOP_GPWR);

	return 0;
}
#else
#define scoop_suspend	NULL
#define scoop_resume	NULL
#endif

static int __devinit scoop_probe(struct platform_device *pdev)
{
	struct scoop_dev *devptr;
	struct scoop_config *inf;
	struct resource *mem = platform_get_resource(pdev, IORESOURCE_MEM, 0);
	int ret;
	int temp;

	if (!mem)
		return -EINVAL;

	devptr = kzalloc(sizeof(struct scoop_dev), GFP_KERNEL);
	if (!devptr)
		return -ENOMEM;

	spin_lock_init(&devptr->scoop_lock);

	inf = pdev->dev.platform_data;
	devptr->base = ioremap(mem->start, mem->end - mem->start + 1);

	if (!devptr->base) {
		ret = -ENOMEM;
		goto err_ioremap;
	}

	platform_set_drvdata(pdev, devptr);

	printk("Sharp Scoop Device found at 0x%08x -> 0x%8p\n",(unsigned int)mem->start, devptr->base);

	iowrite16(0x0140, devptr->base + SCOOP_MCR);
	reset_scoop(&pdev->dev);
	iowrite16(0x0000, devptr->base + SCOOP_CPR);
	iowrite16(inf->io_dir & 0xffff, devptr->base + SCOOP_GPCR);
	iowrite16(inf->io_out & 0xffff, devptr->base + SCOOP_GPWR);

	devptr->suspend_clr = inf->suspend_clr;
	devptr->suspend_set = inf->suspend_set;

	devptr->gpio.base = -1;

	if (inf->gpio_base != 0) {
		devptr->gpio.label = dev_name(&pdev->dev);
		devptr->gpio.base = inf->gpio_base;
		devptr->gpio.ngpio = 12; /* PA11 = 0, PA12 = 1, etc. up to PA22 = 11 */
		devptr->gpio.set = scoop_gpio_set;
		devptr->gpio.get = scoop_gpio_get;
		devptr->gpio.direction_input = scoop_gpio_direction_input;
		devptr->gpio.direction_output = scoop_gpio_direction_output;

		ret = gpiochip_add(&devptr->gpio);
		if (ret)
			goto err_gpio;
	}

	return 0;

	if (devptr->gpio.base != -1)
		temp = gpiochip_remove(&devptr->gpio);
err_gpio:
	platform_set_drvdata(pdev, NULL);
err_ioremap:
	iounmap(devptr->base);
	kfree(devptr);

	return ret;
}

static int __devexit scoop_remove(struct platform_device *pdev)
{
	struct scoop_dev *sdev = platform_get_drvdata(pdev);
	int ret;

	if (!sdev)
		return -EINVAL;

	if (sdev->gpio.base != -1) {
		ret = gpiochip_remove(&sdev->gpio);
		if (ret) {
			dev_err(&pdev->dev, "Can't remove gpio chip: %d\n", ret);
			return ret;
		}
	}

	platform_set_drvdata(pdev, NULL);
	iounmap(sdev->base);
	kfree(sdev);

	return 0;
}

static struct platform_driver scoop_driver = {
	.probe		= scoop_probe,
	.remove		= __devexit_p(scoop_remove),
	.suspend	= scoop_suspend,
	.resume		= scoop_resume,
	.driver		= {
		.name	= "sharp-scoop",
	},
};

static int __init scoop_init(void)
{
	return platform_driver_register(&scoop_driver);
}

subsys_initcall(scoop_init);
