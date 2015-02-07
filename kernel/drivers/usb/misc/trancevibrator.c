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
 * PlayStation 2 Trance Vibrator driver
 *
 * Copyright (C) 2006 Sam Hocevar <sam@zoy.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* Standard include files */
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/module.h>
#include <linux/usb.h>

/* Version Information */
#define DRIVER_VERSION "v1.1"
#define DRIVER_AUTHOR "Sam Hocevar, sam@zoy.org"
#define DRIVER_DESC "PlayStation 2 Trance Vibrator driver"

#define TRANCEVIBRATOR_VENDOR_ID	0x0b49	/* ASCII Corporation */
#define TRANCEVIBRATOR_PRODUCT_ID	0x064f	/* Trance Vibrator */

static const struct usb_device_id id_table[] = {
	{ USB_DEVICE(TRANCEVIBRATOR_VENDOR_ID, TRANCEVIBRATOR_PRODUCT_ID) },
	{ },
};
MODULE_DEVICE_TABLE (usb, id_table);

/* Driver-local specific stuff */
struct trancevibrator {
	struct usb_device *udev;
	unsigned int speed;
};

static ssize_t show_speed(struct device *dev, struct device_attribute *attr,
			  char *buf)
{
	struct usb_interface *intf = to_usb_interface(dev);
	struct trancevibrator *tv = usb_get_intfdata(intf);

	return sprintf(buf, "%d\n", tv->speed);
}

static ssize_t set_speed(struct device *dev, struct device_attribute *attr,
			 const char *buf, size_t count)
{
	struct usb_interface *intf = to_usb_interface(dev);
	struct trancevibrator *tv = usb_get_intfdata(intf);
	int temp, retval, old;

	temp = simple_strtoul(buf, NULL, 10);
	if (temp > 255)
		temp = 255;
	else if (temp < 0)
		temp = 0;
	old = tv->speed;
	tv->speed = temp;

	dev_dbg(&tv->udev->dev, "speed = %d\n", tv->speed);

	/* Set speed */
	retval = usb_control_msg(tv->udev, usb_sndctrlpipe(tv->udev, 0),
				 0x01, /* vendor request: set speed */
				 USB_DIR_IN | USB_TYPE_VENDOR | USB_RECIP_OTHER,
				 tv->speed, /* speed value */
				 0, NULL, 0, USB_CTRL_GET_TIMEOUT);
	if (retval) {
		tv->speed = old;
		dev_dbg(&tv->udev->dev, "retval = %d\n", retval);
		return retval;
	}
	return count;
}

static DEVICE_ATTR(speed, S_IWUGO | S_IRUGO, show_speed, set_speed);

static int tv_probe(struct usb_interface *interface,
		    const struct usb_device_id *id)
{
	struct usb_device *udev = interface_to_usbdev(interface);
	struct trancevibrator *dev;
	int retval;

	dev = kzalloc(sizeof(struct trancevibrator), GFP_KERNEL);
	if (dev == NULL) {
		dev_err(&interface->dev, "Out of memory\n");
		retval = -ENOMEM;
		goto error;
	}

	dev->udev = usb_get_dev(udev);
	usb_set_intfdata(interface, dev);
	retval = device_create_file(&interface->dev, &dev_attr_speed);
	if (retval)
		goto error_create_file;

	return 0;

error_create_file:
	usb_put_dev(udev);
	usb_set_intfdata(interface, NULL);
error:
	kfree(dev);
	return retval;
}

static void tv_disconnect(struct usb_interface *interface)
{
	struct trancevibrator *dev;

	dev = usb_get_intfdata (interface);
	device_remove_file(&interface->dev, &dev_attr_speed);
	usb_set_intfdata(interface, NULL);
	usb_put_dev(dev->udev);
	kfree(dev);
}

/* USB subsystem object */
static struct usb_driver tv_driver = {
	.name =		"trancevibrator",
	.probe =	tv_probe,
	.disconnect =	tv_disconnect,
	.id_table =	id_table,
};

static int __init tv_init(void)
{
	int retval = usb_register(&tv_driver);
	if (retval) {
		err("usb_register failed. Error number %d", retval);
		return retval;
	}

	printk(KERN_INFO KBUILD_MODNAME ": " DRIVER_VERSION ":"
	       DRIVER_DESC "\n");
	return 0;
}

static void __exit tv_exit(void)
{
	usb_deregister(&tv_driver);
}

module_init (tv_init);
module_exit (tv_exit);

MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
MODULE_LICENSE("GPL");
