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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*
 * SMBus driver for ACPI SMBus CMI
 *
 * Copyright (C) 2009 Crane Cai <crane.cai@amd.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation version 2.
 */

#include <linux/module.h>
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/stddef.h>
#include <linux/init.h>
#include <linux/i2c.h>
#include <linux/acpi.h>

#define ACPI_SMBUS_HC_CLASS		"smbus"
#define ACPI_SMBUS_HC_DEVICE_NAME	"cmi"

ACPI_MODULE_NAME("smbus_cmi");

struct smbus_methods_t {
	char *mt_info;
	char *mt_sbr;
	char *mt_sbw;
};

struct acpi_smbus_cmi {
	acpi_handle handle;
	struct i2c_adapter adapter;
	u8 cap_info:1;
	u8 cap_read:1;
	u8 cap_write:1;
};

static const struct smbus_methods_t smbus_methods = {
	.mt_info = "_SBI",
	.mt_sbr  = "_SBR",
	.mt_sbw  = "_SBW",
};

static const struct acpi_device_id acpi_smbus_cmi_ids[] = {
	{"SMBUS01", 0},
	{"", 0}
};

#define ACPI_SMBUS_STATUS_OK			0x00
#define ACPI_SMBUS_STATUS_FAIL			0x07
#define ACPI_SMBUS_STATUS_DNAK			0x10
#define ACPI_SMBUS_STATUS_DERR			0x11
#define ACPI_SMBUS_STATUS_CMD_DENY		0x12
#define ACPI_SMBUS_STATUS_UNKNOWN		0x13
#define ACPI_SMBUS_STATUS_ACC_DENY		0x17
#define ACPI_SMBUS_STATUS_TIMEOUT		0x18
#define ACPI_SMBUS_STATUS_NOTSUP		0x19
#define ACPI_SMBUS_STATUS_BUSY			0x1a
#define ACPI_SMBUS_STATUS_PEC			0x1f

#define ACPI_SMBUS_PRTCL_WRITE			0x00
#define ACPI_SMBUS_PRTCL_READ			0x01
#define ACPI_SMBUS_PRTCL_QUICK			0x02
#define ACPI_SMBUS_PRTCL_BYTE			0x04
#define ACPI_SMBUS_PRTCL_BYTE_DATA		0x06
#define ACPI_SMBUS_PRTCL_WORD_DATA		0x08
#define ACPI_SMBUS_PRTCL_BLOCK_DATA		0x0a


static int
acpi_smbus_cmi_access(struct i2c_adapter *adap, u16 addr, unsigned short flags,
		   char read_write, u8 command, int size,
		   union i2c_smbus_data *data)
{
	int result = 0;
	struct acpi_smbus_cmi *smbus_cmi = adap->algo_data;
	unsigned char protocol;
	acpi_status status = 0;
	struct acpi_object_list input;
	union acpi_object mt_params[5];
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	union acpi_object *obj;
	union acpi_object *pkg;
	char *method;
	int len = 0;

	dev_dbg(&adap->dev, "access size: %d %s\n", size,
		(read_write) ? "READ" : "WRITE");
	switch (size) {
	case I2C_SMBUS_QUICK:
		protocol = ACPI_SMBUS_PRTCL_QUICK;
		command = 0;
		if (read_write == I2C_SMBUS_WRITE) {
			mt_params[3].type = ACPI_TYPE_INTEGER;
			mt_params[3].integer.value = 0;
			mt_params[4].type = ACPI_TYPE_INTEGER;
			mt_params[4].integer.value = 0;
		}
		break;

	case I2C_SMBUS_BYTE:
		protocol = ACPI_SMBUS_PRTCL_BYTE;
		if (read_write == I2C_SMBUS_WRITE) {
			mt_params[3].type = ACPI_TYPE_INTEGER;
			mt_params[3].integer.value = 0;
			mt_params[4].type = ACPI_TYPE_INTEGER;
			mt_params[4].integer.value = 0;
		} else {
			command = 0;
		}
		break;

	case I2C_SMBUS_BYTE_DATA:
		protocol = ACPI_SMBUS_PRTCL_BYTE_DATA;
		if (read_write == I2C_SMBUS_WRITE) {
			mt_params[3].type = ACPI_TYPE_INTEGER;
			mt_params[3].integer.value = 1;
			mt_params[4].type = ACPI_TYPE_INTEGER;
			mt_params[4].integer.value = data->byte;
		}
		break;

	case I2C_SMBUS_WORD_DATA:
		protocol = ACPI_SMBUS_PRTCL_WORD_DATA;
		if (read_write == I2C_SMBUS_WRITE) {
			mt_params[3].type = ACPI_TYPE_INTEGER;
			mt_params[3].integer.value = 2;
			mt_params[4].type = ACPI_TYPE_INTEGER;
			mt_params[4].integer.value = data->word;
		}
		break;

	case I2C_SMBUS_BLOCK_DATA:
		protocol = ACPI_SMBUS_PRTCL_BLOCK_DATA;
		if (read_write == I2C_SMBUS_WRITE) {
			len = data->block[0];
			if (len == 0 || len > I2C_SMBUS_BLOCK_MAX)
				return -EINVAL;
			mt_params[3].type = ACPI_TYPE_INTEGER;
			mt_params[3].integer.value = len;
			mt_params[4].type = ACPI_TYPE_BUFFER;
			mt_params[4].buffer.pointer = data->block + 1;
		}
		break;

	default:
		dev_warn(&adap->dev, "Unsupported transaction %d\n", size);
		return -EOPNOTSUPP;
	}

	if (read_write == I2C_SMBUS_READ) {
		protocol |= ACPI_SMBUS_PRTCL_READ;
		method = smbus_methods.mt_sbr;
		input.count = 3;
	} else {
		protocol |= ACPI_SMBUS_PRTCL_WRITE;
		method = smbus_methods.mt_sbw;
		input.count = 5;
	}

	input.pointer = mt_params;
	mt_params[0].type = ACPI_TYPE_INTEGER;
	mt_params[0].integer.value = protocol;
	mt_params[1].type = ACPI_TYPE_INTEGER;
	mt_params[1].integer.value = addr;
	mt_params[2].type = ACPI_TYPE_INTEGER;
	mt_params[2].integer.value = command;

	status = acpi_evaluate_object(smbus_cmi->handle, method, &input,
				      &buffer);
	if (ACPI_FAILURE(status)) {
		ACPI_ERROR((AE_INFO, "Evaluating %s: %i", method, status));
		return -EIO;
	}

	pkg = buffer.pointer;
	if (pkg && pkg->type == ACPI_TYPE_PACKAGE)
		obj = pkg->package.elements;
	else {
		ACPI_ERROR((AE_INFO, "Invalid argument type"));
		result = -EIO;
		goto out;
	}
	if (obj == NULL || obj->type != ACPI_TYPE_INTEGER) {
		ACPI_ERROR((AE_INFO, "Invalid argument type"));
		result = -EIO;
		goto out;
	}

	result = obj->integer.value;
	ACPI_DEBUG_PRINT((ACPI_DB_INFO, "%s return status: %i\n",
			  method, result));

	switch (result) {
	case ACPI_SMBUS_STATUS_OK:
		result = 0;
		break;
	case ACPI_SMBUS_STATUS_BUSY:
		result = -EBUSY;
		goto out;
	case ACPI_SMBUS_STATUS_TIMEOUT:
		result = -ETIMEDOUT;
		goto out;
	case ACPI_SMBUS_STATUS_DNAK:
		result = -ENXIO;
		goto out;
	default:
		result = -EIO;
		goto out;
	}

	if (read_write == I2C_SMBUS_WRITE || size == I2C_SMBUS_QUICK)
		goto out;

	obj = pkg->package.elements + 1;
	if (obj == NULL || obj->type != ACPI_TYPE_INTEGER) {
		ACPI_ERROR((AE_INFO, "Invalid argument type"));
		result = -EIO;
		goto out;
	}

	len = obj->integer.value;
	obj = pkg->package.elements + 2;
	switch (size) {
	case I2C_SMBUS_BYTE:
	case I2C_SMBUS_BYTE_DATA:
	case I2C_SMBUS_WORD_DATA:
		if (obj == NULL || obj->type != ACPI_TYPE_INTEGER) {
			ACPI_ERROR((AE_INFO, "Invalid argument type"));
			result = -EIO;
			goto out;
		}
		if (len == 2)
			data->word = obj->integer.value;
		else
			data->byte = obj->integer.value;
		break;
	case I2C_SMBUS_BLOCK_DATA:
		if (obj == NULL || obj->type != ACPI_TYPE_BUFFER) {
			ACPI_ERROR((AE_INFO, "Invalid argument type"));
			result = -EIO;
			goto out;
		}
		if (len == 0 || len > I2C_SMBUS_BLOCK_MAX)
			return -EPROTO;
		data->block[0] = len;
		memcpy(data->block + 1, obj->buffer.pointer, len);
		break;
	}

out:
	kfree(buffer.pointer);
	dev_dbg(&adap->dev, "Transaction status: %i\n", result);
	return result;
}

static u32 acpi_smbus_cmi_func(struct i2c_adapter *adapter)
{
	struct acpi_smbus_cmi *smbus_cmi = adapter->algo_data;
	u32 ret;

	ret = smbus_cmi->cap_read | smbus_cmi->cap_write ?
		I2C_FUNC_SMBUS_QUICK : 0;

	ret |= smbus_cmi->cap_read ?
		(I2C_FUNC_SMBUS_READ_BYTE |
		I2C_FUNC_SMBUS_READ_BYTE_DATA |
		I2C_FUNC_SMBUS_READ_WORD_DATA |
		I2C_FUNC_SMBUS_READ_BLOCK_DATA) : 0;

	ret |= smbus_cmi->cap_write ?
		(I2C_FUNC_SMBUS_WRITE_BYTE |
		I2C_FUNC_SMBUS_WRITE_BYTE_DATA |
		I2C_FUNC_SMBUS_WRITE_WORD_DATA |
		I2C_FUNC_SMBUS_WRITE_BLOCK_DATA) : 0;

	return ret;
}

static const struct i2c_algorithm acpi_smbus_cmi_algorithm = {
	.smbus_xfer = acpi_smbus_cmi_access,
	.functionality = acpi_smbus_cmi_func,
};


static int acpi_smbus_cmi_add_cap(struct acpi_smbus_cmi *smbus_cmi,
				  const char *name)
{
	struct acpi_buffer buffer = { ACPI_ALLOCATE_BUFFER, NULL };
	union acpi_object *obj;
	acpi_status status;

	if (!strcmp(name, smbus_methods.mt_info)) {
		status = acpi_evaluate_object(smbus_cmi->handle,
					smbus_methods.mt_info,
					NULL, &buffer);
		if (ACPI_FAILURE(status)) {
			ACPI_ERROR((AE_INFO, "Evaluating %s: %i",
				   smbus_methods.mt_info, status));
			return -EIO;
		}

		obj = buffer.pointer;
		if (obj && obj->type == ACPI_TYPE_PACKAGE)
			obj = obj->package.elements;
		else {
			ACPI_ERROR((AE_INFO, "Invalid argument type"));
			kfree(buffer.pointer);
			return -EIO;
		}

		if (obj->type != ACPI_TYPE_INTEGER) {
			ACPI_ERROR((AE_INFO, "Invalid argument type"));
			kfree(buffer.pointer);
			return -EIO;
		} else
			ACPI_DEBUG_PRINT((ACPI_DB_INFO, "SMBus CMI Version %x"
					  "\n", (int)obj->integer.value));

		kfree(buffer.pointer);
		smbus_cmi->cap_info = 1;
	} else if (!strcmp(name, smbus_methods.mt_sbr))
		smbus_cmi->cap_read = 1;
	else if (!strcmp(name, smbus_methods.mt_sbw))
		smbus_cmi->cap_write = 1;
	else
		ACPI_DEBUG_PRINT((ACPI_DB_INFO, "Unsupported CMI method: %s\n",
				 name));

	return 0;
}

static acpi_status acpi_smbus_cmi_query_methods(acpi_handle handle, u32 level,
			void *context, void **return_value)
{
	char node_name[5];
	struct acpi_buffer buffer = { sizeof(node_name), node_name };
	struct acpi_smbus_cmi *smbus_cmi = context;
	acpi_status status;

	status = acpi_get_name(handle, ACPI_SINGLE_NAME, &buffer);

	if (ACPI_SUCCESS(status))
		acpi_smbus_cmi_add_cap(smbus_cmi, node_name);

	return AE_OK;
}

static int acpi_smbus_cmi_add(struct acpi_device *device)
{
	struct acpi_smbus_cmi *smbus_cmi;

	smbus_cmi = kzalloc(sizeof(struct acpi_smbus_cmi), GFP_KERNEL);
	if (!smbus_cmi)
		return -ENOMEM;

	smbus_cmi->handle = device->handle;
	strcpy(acpi_device_name(device), ACPI_SMBUS_HC_DEVICE_NAME);
	strcpy(acpi_device_class(device), ACPI_SMBUS_HC_CLASS);
	device->driver_data = smbus_cmi;
	smbus_cmi->cap_info = 0;
	smbus_cmi->cap_read = 0;
	smbus_cmi->cap_write = 0;

	acpi_walk_namespace(ACPI_TYPE_METHOD, smbus_cmi->handle, 1,
			    acpi_smbus_cmi_query_methods, smbus_cmi, NULL);

	if (smbus_cmi->cap_info == 0)
		goto err;

	snprintf(smbus_cmi->adapter.name, sizeof(smbus_cmi->adapter.name),
		"SMBus CMI adapter %s",
		acpi_device_name(device));
	smbus_cmi->adapter.owner = THIS_MODULE;
	smbus_cmi->adapter.algo = &acpi_smbus_cmi_algorithm;
	smbus_cmi->adapter.algo_data = smbus_cmi;
	smbus_cmi->adapter.class = I2C_CLASS_HWMON | I2C_CLASS_SPD;
	smbus_cmi->adapter.dev.parent = &device->dev;

	if (i2c_add_adapter(&smbus_cmi->adapter)) {
		dev_err(&device->dev, "Couldn't register adapter!\n");
		goto err;
	}

	return 0;

err:
	kfree(smbus_cmi);
	device->driver_data = NULL;
	return -EIO;
}

static int acpi_smbus_cmi_remove(struct acpi_device *device, int type)
{
	struct acpi_smbus_cmi *smbus_cmi = acpi_driver_data(device);

	i2c_del_adapter(&smbus_cmi->adapter);
	kfree(smbus_cmi);
	device->driver_data = NULL;

	return 0;
}

static struct acpi_driver acpi_smbus_cmi_driver = {
	.name = ACPI_SMBUS_HC_DEVICE_NAME,
	.class = ACPI_SMBUS_HC_CLASS,
	.ids = acpi_smbus_cmi_ids,
	.ops = {
		.add = acpi_smbus_cmi_add,
		.remove = acpi_smbus_cmi_remove,
	},
};

static int __init acpi_smbus_cmi_init(void)
{
	return acpi_bus_register_driver(&acpi_smbus_cmi_driver);
}

static void __exit acpi_smbus_cmi_exit(void)
{
	acpi_bus_unregister_driver(&acpi_smbus_cmi_driver);
}

module_init(acpi_smbus_cmi_init);
module_exit(acpi_smbus_cmi_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Crane Cai <crane.cai@amd.com>");
MODULE_DESCRIPTION("ACPI SMBus CMI driver");