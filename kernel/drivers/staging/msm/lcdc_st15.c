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

/* Copyright (c) 2010, Code Aurora Forum. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 and
 * only version 2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include <linux/i2c.h>
#include <linux/delay.h>
#include "msm_fb.h"

#define DEVICE_NAME "sii9022"
#define SII9022_DEVICE_ID   0xB0

struct sii9022_i2c_addr_data{
	u8 addr;
	u8 data;
};

/* video mode data */
static u8 video_mode_data[] = {
	0x00,
	0xF9, 0x1C, 0x70, 0x17, 0x72, 0x06, 0xEE, 0x02,
};

static u8 avi_io_format[] = {
	0x09,
	0x00, 0x00,
};

/* power state */
static struct sii9022_i2c_addr_data regset0[] = {
	{ 0x60, 0x04 },
	{ 0x63, 0x00 },
	{ 0x1E, 0x00 },
};

static u8 video_infoframe[] = {
	0x0C,
	0xF0, 0x00, 0x68, 0x00, 0x04, 0x00, 0x19, 0x00,
	0xE9, 0x02, 0x04, 0x01, 0x04, 0x06,
};

/* configure audio */
static struct sii9022_i2c_addr_data regset1[] = {
	{ 0x26, 0x90 },
	{ 0x20, 0x90 },
	{ 0x1F, 0x80 },
	{ 0x26, 0x80 },
	{ 0x24, 0x02 },
	{ 0x25, 0x0B },
	{ 0xBC, 0x02 },
	{ 0xBD, 0x24 },
	{ 0xBE, 0x02 },
};

/* enable audio */
static u8 misc_infoframe[] = {
	0xBF,
	0xC2, 0x84, 0x01, 0x0A, 0x6F, 0x02, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

/* set HDMI, active */
static struct sii9022_i2c_addr_data regset2[] = {
	{ 0x1A, 0x01 },
	{ 0x3D, 0x00 },
};

static int send_i2c_data(struct i2c_client *client,
			 struct sii9022_i2c_addr_data *regset,
			 int size)
{
	int i;
	int rc = 0;

	for (i = 0; i < size; i++) {
		rc = i2c_smbus_write_byte_data(
			client,
			regset[i].addr, regset[i].data);
		if (rc)
			break;
	}
	return rc;
}

static int hdmi_sii_enable(struct i2c_client *client)
{
	int rc;
	int retries = 10;
	int count;

	rc = i2c_smbus_write_byte_data(client, 0xC7, 0x00);
	if (rc)
		goto enable_exit;

	do {
		msleep(1);
		rc = i2c_smbus_read_byte_data(client, 0x1B);
	} while ((rc != SII9022_DEVICE_ID) && retries--);

	if (rc != SII9022_DEVICE_ID)
		return -ENODEV;

	rc = i2c_smbus_write_byte_data(client, 0x1A, 0x11);
	if (rc)
		goto enable_exit;

	count = ARRAY_SIZE(video_mode_data);
	rc = i2c_master_send(client, video_mode_data, count);
	if (rc != count) {
		rc = -EIO;
		goto enable_exit;
	}

	rc = i2c_smbus_write_byte_data(client, 0x08, 0x20);
	if (rc)
		goto enable_exit;
	count = ARRAY_SIZE(avi_io_format);
	rc = i2c_master_send(client, avi_io_format, count);
	if (rc != count) {
		rc = -EIO;
		goto enable_exit;
	}

	rc = send_i2c_data(client, regset0, ARRAY_SIZE(regset0));
	if (rc)
		goto enable_exit;

	count = ARRAY_SIZE(video_infoframe);
	rc = i2c_master_send(client, video_infoframe, count);
	if (rc != count) {
		rc = -EIO;
		goto enable_exit;
	}

	rc = send_i2c_data(client, regset1, ARRAY_SIZE(regset1));
	if (rc)
		goto enable_exit;

	count = ARRAY_SIZE(misc_infoframe);
	rc = i2c_master_send(client, misc_infoframe, count);
	if (rc != count) {
		rc = -EIO;
		goto enable_exit;
	}

	rc = send_i2c_data(client, regset2, ARRAY_SIZE(regset2));
	if (rc)
		goto enable_exit;

	return 0;
enable_exit:
	printk(KERN_ERR "%s: exited rc=%d\n", __func__, rc);
	return rc;
}

static const struct i2c_device_id hmdi_sii_id[] = {
	{ DEVICE_NAME, 0 },
	{ }
};

static int hdmi_sii_probe(struct i2c_client *client,
			const struct i2c_device_id *id)
{
	int rc;

	if (!i2c_check_functionality(client->adapter,
				     I2C_FUNC_SMBUS_BYTE | I2C_FUNC_I2C))
		return -ENODEV;
	rc = hdmi_sii_enable(client);
	return rc;
}


static struct i2c_driver hdmi_sii_i2c_driver = {
	.driver = {
		.name = DEVICE_NAME,
		.owner = THIS_MODULE,
	},
	.probe = hdmi_sii_probe,
	.remove =  __exit_p(hdmi_sii_remove),
	.id_table = hmdi_sii_id,
};

static int __init lcdc_st15_init(void)
{
	int ret;
	struct msm_panel_info pinfo;

	if (msm_fb_detect_client("lcdc_st15"))
		return 0;

	pinfo.xres = 1366;
	pinfo.yres = 768;
	pinfo.type = LCDC_PANEL;
	pinfo.pdest = DISPLAY_1;
	pinfo.wait_cycle = 0;
	pinfo.bpp = 24;
	pinfo.fb_num = 2;
	pinfo.clk_rate = 74250000;

	pinfo.lcdc.h_back_porch = 120;
	pinfo.lcdc.h_front_porch = 20;
	pinfo.lcdc.h_pulse_width = 40;
	pinfo.lcdc.v_back_porch = 25;
	pinfo.lcdc.v_front_porch = 1;
	pinfo.lcdc.v_pulse_width = 7;
	pinfo.lcdc.border_clr = 0;      /* blk */
	pinfo.lcdc.underflow_clr = 0xff;        /* blue */
	pinfo.lcdc.hsync_skew = 0;

	ret = lcdc_device_register(&pinfo);
	if (ret) {
		printk(KERN_ERR "%s: failed to register device!\n", __func__);
		goto init_exit;
	}

	ret = i2c_add_driver(&hdmi_sii_i2c_driver);
	if (ret)
		printk(KERN_ERR "%s: failed to add i2c driver\n", __func__);

init_exit:
	return ret;
}

module_init(lcdc_st15_init);
