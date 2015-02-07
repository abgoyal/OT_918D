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

#include <linux/interrupt.h>
#include <linux/irq.h>
#include <linux/mutex.h>
#include <linux/device.h>
#include <linux/kernel.h>
#include <linux/sysfs.h>
#include <linux/list.h>
#include <linux/spi/spi.h>

#include "../iio.h"
#include "../sysfs.h"
#include "../trigger.h"
#include "adis16240.h"

/**
 * adis16240_data_rdy_trig_poll() the event handler for the data rdy trig
 **/
static int adis16240_data_rdy_trig_poll(struct iio_dev *dev_info,
				       int index,
				       s64 timestamp,
				       int no_test)
{
	struct adis16240_state *st = iio_dev_get_devdata(dev_info);
	struct iio_trigger *trig = st->trig;

	trig->timestamp = timestamp;
	iio_trigger_poll(trig);

	return IRQ_HANDLED;
}

IIO_EVENT_SH(data_rdy_trig, &adis16240_data_rdy_trig_poll);

static DEVICE_ATTR(name, S_IRUGO, iio_trigger_read_name, NULL);

static struct attribute *adis16240_trigger_attrs[] = {
	&dev_attr_name.attr,
	NULL,
};

static const struct attribute_group adis16240_trigger_attr_group = {
	.attrs = adis16240_trigger_attrs,
};

/**
 * adis16240_data_rdy_trigger_set_state() set datardy interrupt state
 **/
static int adis16240_data_rdy_trigger_set_state(struct iio_trigger *trig,
						bool state)
{
	struct adis16240_state *st = trig->private_data;
	struct iio_dev *indio_dev = st->indio_dev;
	int ret = 0;

	dev_dbg(&indio_dev->dev, "%s (%d)\n", __func__, state);
	ret = adis16240_set_irq(&st->indio_dev->dev, state);
	if (state == false) {
		iio_remove_event_from_list(&iio_event_data_rdy_trig,
					   &indio_dev->interrupts[0]
					   ->ev_list);
		flush_scheduled_work();
	} else {
		iio_add_event_to_list(&iio_event_data_rdy_trig,
				      &indio_dev->interrupts[0]->ev_list);
	}
	return ret;
}

/**
 * adis16240_trig_try_reen() try renabling irq for data rdy trigger
 * @trig:	the datardy trigger
 **/
static int adis16240_trig_try_reen(struct iio_trigger *trig)
{
	struct adis16240_state *st = trig->private_data;
	enable_irq(st->us->irq);
	return 0;
}

int adis16240_probe_trigger(struct iio_dev *indio_dev)
{
	int ret;
	struct adis16240_state *st = indio_dev->dev_data;

	st->trig = iio_allocate_trigger();
	st->trig->name = kmalloc(IIO_TRIGGER_NAME_LENGTH, GFP_KERNEL);
	if (!st->trig->name) {
		ret = -ENOMEM;
		goto error_free_trig;
	}
	snprintf((char *)st->trig->name,
		 IIO_TRIGGER_NAME_LENGTH,
		 "adis16240-dev%d", indio_dev->id);
	st->trig->dev.parent = &st->us->dev;
	st->trig->owner = THIS_MODULE;
	st->trig->private_data = st;
	st->trig->set_trigger_state = &adis16240_data_rdy_trigger_set_state;
	st->trig->try_reenable = &adis16240_trig_try_reen;
	st->trig->control_attrs = &adis16240_trigger_attr_group;
	ret = iio_trigger_register(st->trig);

	/* select default trigger */
	indio_dev->trig = st->trig;
	if (ret)
		goto error_free_trig_name;

	return 0;

error_free_trig_name:
	kfree(st->trig->name);
error_free_trig:
	iio_free_trigger(st->trig);

	return ret;
}

void adis16240_remove_trigger(struct iio_dev *indio_dev)
{
	struct adis16240_state *state = indio_dev->dev_data;

	iio_trigger_unregister(state->trig);
	kfree(state->trig->name);
	iio_free_trigger(state->trig);
}