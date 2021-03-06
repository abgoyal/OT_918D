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

/* proteus-2309.h - Keytable for proteus_2309 Remote Controller
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

/* Michal Majchrowicz <mmajchrowicz@gmail.com> */

static struct ir_scancode proteus_2309[] = {
	/* numeric */
	{ 0x00, KEY_0 },
	{ 0x01, KEY_1 },
	{ 0x02, KEY_2 },
	{ 0x03, KEY_3 },
	{ 0x04, KEY_4 },
	{ 0x05, KEY_5 },
	{ 0x06, KEY_6 },
	{ 0x07, KEY_7 },
	{ 0x08, KEY_8 },
	{ 0x09, KEY_9 },

	{ 0x5c, KEY_POWER },		/* power       */
	{ 0x20, KEY_ZOOM },		/* full screen */
	{ 0x0f, KEY_BACKSPACE },	/* recall      */
	{ 0x1b, KEY_ENTER },		/* mute        */
	{ 0x41, KEY_RECORD },		/* record      */
	{ 0x43, KEY_STOP },		/* stop        */
	{ 0x16, KEY_S },
	{ 0x1a, KEY_POWER2 },		/* off         */
	{ 0x2e, KEY_RED },
	{ 0x1f, KEY_CHANNELDOWN },	/* channel -   */
	{ 0x1c, KEY_CHANNELUP },	/* channel +   */
	{ 0x10, KEY_VOLUMEDOWN },	/* volume -    */
	{ 0x1e, KEY_VOLUMEUP },		/* volume +    */
	{ 0x14, KEY_F1 },
};

static struct rc_keymap proteus_2309_map = {
	.map = {
		.scan    = proteus_2309,
		.size    = ARRAY_SIZE(proteus_2309),
		.ir_type = IR_TYPE_UNKNOWN,	/* Legacy IR type */
		.name    = RC_MAP_PROTEUS_2309,
	}
};

static int __init init_rc_map_proteus_2309(void)
{
	return ir_register_map(&proteus_2309_map);
}

static void __exit exit_rc_map_proteus_2309(void)
{
	ir_unregister_map(&proteus_2309_map);
}

module_init(init_rc_map_proteus_2309)
module_exit(exit_rc_map_proteus_2309)

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mauro Carvalho Chehab <mchehab@redhat.com>");
