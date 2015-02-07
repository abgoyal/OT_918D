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
 * GE watchdog userspace interface
 *
 * Author:  Martyn Welch <martyn.welch@ge.com>
 *
 * Copyright 2008 GE Intelligent Platforms Embedded Systems, Inc.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * Based on: mv64x60_wdt.c (MV64X60 watchdog userspace interface)
 *   Author: James Chapman <jchapman@katalix.com>
 */

/* TODO:
 * This driver does not provide support for the hardwares capability of sending
 * an interrupt at a programmable threshold.
 *
 * This driver currently can only support 1 watchdog - there are 2 in the
 * hardware that this driver supports. Thus one could be configured as a
 * process-based watchdog (via /dev/watchdog), the second (using the interrupt
 * capabilities) a kernel-based watchdog.
 */

#include <linux/kernel.h>
#include <linux/compiler.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/miscdevice.h>
#include <linux/watchdog.h>
#include <linux/of.h>
#include <linux/of_platform.h>
#include <linux/io.h>
#include <linux/uaccess.h>

#include <sysdev/fsl_soc.h>

/*
 * The watchdog configuration register contains a pair of 2-bit fields,
 *   1.  a reload field, bits 27-26, which triggers a reload of
 *       the countdown register, and
 *   2.  an enable field, bits 25-24, which toggles between
 *       enabling and disabling the watchdog timer.
 * Bit 31 is a read-only field which indicates whether the
 * watchdog timer is currently enabled.
 *
 * The low 24 bits contain the timer reload value.
 */
#define GEF_WDC_ENABLE_SHIFT	24
#define GEF_WDC_SERVICE_SHIFT	26
#define GEF_WDC_ENABLED_SHIFT	31

#define GEF_WDC_ENABLED_TRUE	1
#define GEF_WDC_ENABLED_FALSE	0

/* Flags bits */
#define GEF_WDOG_FLAG_OPENED	0

static unsigned long wdt_flags;
static int wdt_status;
static void __iomem *gef_wdt_regs;
static int gef_wdt_timeout;
static int gef_wdt_count;
static unsigned int bus_clk;
static char expect_close;
static DEFINE_SPINLOCK(gef_wdt_spinlock);

static int nowayout = WATCHDOG_NOWAYOUT;
module_param(nowayout, int, 0);
MODULE_PARM_DESC(nowayout, "Watchdog cannot be stopped once started (default="
	__MODULE_STRING(WATCHDOG_NOWAYOUT) ")");


static int gef_wdt_toggle_wdc(int enabled_predicate, int field_shift)
{
	u32 data;
	u32 enabled;
	int ret = 0;

	spin_lock(&gef_wdt_spinlock);
	data = ioread32be(gef_wdt_regs);
	enabled = (data >> GEF_WDC_ENABLED_SHIFT) & 1;

	/* only toggle the requested field if enabled state matches predicate */
	if ((enabled ^ enabled_predicate) == 0) {
		/* We write a 1, then a 2 -- to the appropriate field */
		data = (1 << field_shift) | gef_wdt_count;
		iowrite32be(data, gef_wdt_regs);

		data = (2 << field_shift) | gef_wdt_count;
		iowrite32be(data, gef_wdt_regs);
		ret = 1;
	}
	spin_unlock(&gef_wdt_spinlock);

	return ret;
}

static void gef_wdt_service(void)
{
	gef_wdt_toggle_wdc(GEF_WDC_ENABLED_TRUE,
		GEF_WDC_SERVICE_SHIFT);
}

static void gef_wdt_handler_enable(void)
{
	if (gef_wdt_toggle_wdc(GEF_WDC_ENABLED_FALSE,
				   GEF_WDC_ENABLE_SHIFT)) {
		gef_wdt_service();
		printk(KERN_NOTICE "gef_wdt: watchdog activated\n");
	}
}

static void gef_wdt_handler_disable(void)
{
	if (gef_wdt_toggle_wdc(GEF_WDC_ENABLED_TRUE,
				   GEF_WDC_ENABLE_SHIFT))
		printk(KERN_NOTICE "gef_wdt: watchdog deactivated\n");
}

static void gef_wdt_set_timeout(unsigned int timeout)
{
	/* maximum bus cycle count is 0xFFFFFFFF */
	if (timeout > 0xFFFFFFFF / bus_clk)
		timeout = 0xFFFFFFFF / bus_clk;

	/* Register only holds upper 24 bits, bit shifted into lower 24 */
	gef_wdt_count = (timeout * bus_clk) >> 8;
	gef_wdt_timeout = timeout;
}


static ssize_t gef_wdt_write(struct file *file, const char __user *data,
				 size_t len, loff_t *ppos)
{
	if (len) {
		if (!nowayout) {
			size_t i;

			expect_close = 0;

			for (i = 0; i != len; i++) {
				char c;
				if (get_user(c, data + i))
					return -EFAULT;
				if (c == 'V')
					expect_close = 42;
			}
		}
		gef_wdt_service();
	}

	return len;
}

static long gef_wdt_ioctl(struct file *file, unsigned int cmd,
							unsigned long arg)
{
	int timeout;
	int options;
	void __user *argp = (void __user *)arg;
	static const struct watchdog_info info = {
		.options =	WDIOF_SETTIMEOUT | WDIOF_MAGICCLOSE |
				WDIOF_KEEPALIVEPING,
		.firmware_version = 0,
		.identity = "GE watchdog",
	};

	switch (cmd) {
	case WDIOC_GETSUPPORT:
		if (copy_to_user(argp, &info, sizeof(info)))
			return -EFAULT;
		break;

	case WDIOC_GETSTATUS:
	case WDIOC_GETBOOTSTATUS:
		if (put_user(wdt_status, (int __user *)argp))
			return -EFAULT;
		wdt_status &= ~WDIOF_KEEPALIVEPING;
		break;

	case WDIOC_SETOPTIONS:
		if (get_user(options, (int __user *)argp))
			return -EFAULT;

		if (options & WDIOS_DISABLECARD)
			gef_wdt_handler_disable();

		if (options & WDIOS_ENABLECARD)
			gef_wdt_handler_enable();
		break;

	case WDIOC_KEEPALIVE:
		gef_wdt_service();
		wdt_status |= WDIOF_KEEPALIVEPING;
		break;

	case WDIOC_SETTIMEOUT:
		if (get_user(timeout, (int __user *)argp))
			return -EFAULT;
		gef_wdt_set_timeout(timeout);
		/* Fall through */

	case WDIOC_GETTIMEOUT:
		if (put_user(gef_wdt_timeout, (int __user *)argp))
			return -EFAULT;
		break;

	default:
		return -ENOTTY;
	}

	return 0;
}

static int gef_wdt_open(struct inode *inode, struct file *file)
{
	if (test_and_set_bit(GEF_WDOG_FLAG_OPENED, &wdt_flags))
		return -EBUSY;

	if (nowayout)
		__module_get(THIS_MODULE);

	gef_wdt_handler_enable();

	return nonseekable_open(inode, file);
}

static int gef_wdt_release(struct inode *inode, struct file *file)
{
	if (expect_close == 42)
		gef_wdt_handler_disable();
	else {
		printk(KERN_CRIT
		       "gef_wdt: unexpected close, not stopping timer!\n");
		gef_wdt_service();
	}
	expect_close = 0;

	clear_bit(GEF_WDOG_FLAG_OPENED, &wdt_flags);

	return 0;
}

static const struct file_operations gef_wdt_fops = {
	.owner = THIS_MODULE,
	.llseek = no_llseek,
	.write = gef_wdt_write,
	.unlocked_ioctl = gef_wdt_ioctl,
	.open = gef_wdt_open,
	.release = gef_wdt_release,
};

static struct miscdevice gef_wdt_miscdev = {
	.minor = WATCHDOG_MINOR,
	.name = "watchdog",
	.fops = &gef_wdt_fops,
};


static int __devinit gef_wdt_probe(struct of_device *dev,
	const struct of_device_id *match)
{
	int timeout = 10;
	u32 freq;

	bus_clk = 133; /* in MHz */

	freq = fsl_get_sys_freq();
	if (freq != -1)
		bus_clk = freq;

	/* Map devices registers into memory */
	gef_wdt_regs = of_iomap(dev->dev.of_node, 0);
	if (gef_wdt_regs == NULL)
		return -ENOMEM;

	gef_wdt_set_timeout(timeout);

	gef_wdt_handler_disable();	/* in case timer was already running */

	return misc_register(&gef_wdt_miscdev);
}

static int __devexit gef_wdt_remove(struct platform_device *dev)
{
	misc_deregister(&gef_wdt_miscdev);

	gef_wdt_handler_disable();

	iounmap(gef_wdt_regs);

	return 0;
}

static const struct of_device_id gef_wdt_ids[] = {
	{
		.compatible = "gef,fpga-wdt",
	},
	{},
};

static struct of_platform_driver gef_wdt_driver = {
	.driver = {
		.name = "gef_wdt",
		.owner = THIS_MODULE,
		.of_match_table = gef_wdt_ids,
	},
	.probe		= gef_wdt_probe,
};

static int __init gef_wdt_init(void)
{
	printk(KERN_INFO "GE watchdog driver\n");
	return of_register_platform_driver(&gef_wdt_driver);
}

static void __exit gef_wdt_exit(void)
{
	of_unregister_platform_driver(&gef_wdt_driver);
}

module_init(gef_wdt_init);
module_exit(gef_wdt_exit);

MODULE_AUTHOR("Martyn Welch <martyn.welch@ge.com>");
MODULE_DESCRIPTION("GE watchdog driver");
MODULE_LICENSE("GPL");
MODULE_ALIAS_MISCDEV(WATCHDOG_MINOR);
MODULE_ALIAS("platform: gef_wdt");
