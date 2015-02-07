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

/* tevii-nec.h - Keytable for tevii_nec Remote Controller
 *
 * keymap imported from ir-keymaps.c
 *
 * Copyright (c) 2010 by Mauro Carvalho Chehab <mchehab@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <media/rc-map.h>

static struct ir_scancode tevii_nec[] = {
	{ 0x0a, KEY_POWER2},
	{ 0x0c, KEY_MUTE},
	{ 0x11, KEY_1},
	{ 0x12, KEY_2},
	{ 0x13, KEY_3},
	{ 0x14, KEY_4},
	{ 0x15, KEY_5},
	{ 0x16, KEY_6},
	{ 0x17, KEY_7},
	{ 0x18, KEY_8},
	{ 0x19, KEY_9},
	{ 0x10, KEY_0},
	{ 0x1c, KEY_MENU},
	{ 0x0f, KEY_VOLUMEDOWN},
	{ 0x1a, KEY_LAST},
	{ 0x0e, KEY_OPEN},
	{ 0x04, KEY_RECORD},
	{ 0x09, KEY_VOLUMEUP},
	{ 0x08, KEY_CHANNELUP},
	{ 0x07, KEY_PVR},
	{ 0x0b, KEY_TIME},
	{ 0x02, KEY_RIGHT},
	{ 0x03, KEY_LEFT},
	{ 0x00, KEY_UP},
	{ 0x1f, KEY_OK},
	{ 0x01, KEY_DOWN},
	{ 0x05, KEY_TUNER},
	{ 0x06, KEY_CHANNELDOWN},
	{ 0x40, KEY_PLAYPAUSE},
	{ 0x1e, KEY_REWIND},
	{ 0x1b, KEY_FAVORITES},
	{ 0x1d, KEY_BACK},
	{ 0x4d, KEY_FASTFORWARD},
	{ 0x44, KEY_EPG},
	{ 0x4c, KEY_INFO},
	{ 0x41, KEY_AB},
	{ 0x43, KEY_AUDIO},
	{ 0x45, KEY_SUBTITLE},
	{ 0x4a, KEY_LIST},
	{ 0x46, KEY_F1},
	{ 0x47, KEY_F2},
	{ 0x5e, KEY_F3},
	{ 0x5c, KEY_F4},
	{ 0x52, KEY_F5},
	{ 0x5a, KEY_F6},
	{ 0x56, KEY_MODE},
	{ 0x58, KEY_SWITCHVIDEOMODE},
};

static struct rc_keymap tevii_nec_map = {
	.map = {
		.scan    = tevii_nec,
		.size    = ARRAY_SIZE(tevii_nec),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_TEVII_NEC,
	}
};

static int __init init_rc_map_tevii_nec(void)
{
	return ir_register_map(&tevii_nec_map);
}

static void __exit exit_rc_map_tevii_nec(void)
{
	ir_unregister_map(&tevii_nec_map);
}

module_init(init_rc_map_tevii_nec)
module_exit(exit_rc_map_tevii_nec)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
