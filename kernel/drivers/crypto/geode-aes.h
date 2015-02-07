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

/* Copyright (C) 2003-2006, Advanced Micro Devices, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef _GEODE_AES_H_
#define _GEODE_AES_H_

/* driver logic flags */
#define AES_IV_LENGTH  16
#define AES_KEY_LENGTH 16
#define AES_MIN_BLOCK_SIZE 16

#define AES_MODE_ECB 0
#define AES_MODE_CBC 1

#define AES_DIR_DECRYPT 0
#define AES_DIR_ENCRYPT 1

#define AES_FLAGS_HIDDENKEY (1 << 0)

/* Register definitions */

#define AES_CTRLA_REG  0x0000

#define AES_CTRL_START     0x01
#define AES_CTRL_DECRYPT   0x00
#define AES_CTRL_ENCRYPT   0x02
#define AES_CTRL_WRKEY     0x04
#define AES_CTRL_DCA       0x08
#define AES_CTRL_SCA       0x10
#define AES_CTRL_CBC       0x20

#define AES_INTR_REG  0x0008

#define AES_INTRA_PENDING (1 << 16)
#define AES_INTRB_PENDING (1 << 17)

#define AES_INTR_PENDING  (AES_INTRA_PENDING | AES_INTRB_PENDING)
#define AES_INTR_MASK     0x07

#define AES_SOURCEA_REG   0x0010
#define AES_DSTA_REG      0x0014
#define AES_LENA_REG      0x0018
#define AES_WRITEKEY0_REG 0x0030
#define AES_WRITEIV0_REG  0x0040

/*  A very large counter that is used to gracefully bail out of an
 *  operation in case of trouble
 */

#define AES_OP_TIMEOUT    0x50000

struct geode_aes_op {

	void *src;
	void *dst;

	u32 mode;
	u32 dir;
	u32 flags;
	int len;

	u8 key[AES_KEY_LENGTH];
	u8 *iv;

	union {
		struct crypto_blkcipher *blk;
		struct crypto_cipher *cip;
	} fallback;
	u32 keylen;
};

#endif
