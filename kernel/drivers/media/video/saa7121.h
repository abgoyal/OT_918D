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

/* saa7121.h - saa7121 initializations
   Copyright (C) 1999 Nathan Laredo (laredo@gnu.org)

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

 */
#ifndef __SAA7121_H__
#define __SAA7121_H__

#define NTSC_BURST_START	0x19	/* 28 */
#define NTSC_BURST_END		0x1d	/* 29 */
#define NTSC_CHROMA_PHASE	0x67	/* 5a */
#define NTSC_GAINU		0x76	/* 5b */
#define NTSC_GAINV		0xa5	/* 5c */
#define NTSC_BLACK_LEVEL	0x2a	/* 5d */
#define NTSC_BLANKING_LEVEL	0x2e	/* 5e */
#define NTSC_VBI_BLANKING	0x2e	/* 5f */
#define NTSC_DAC_CONTROL	0x11	/* 61 */
#define NTSC_BURST_AMP		0x3f	/* 62 */
#define NTSC_SUBC3		0x1f	/* 63 */
#define NTSC_SUBC2		0x7c	/* 64 */
#define NTSC_SUBC1		0xf0	/* 65 */
#define NTSC_SUBC0		0x21	/* 66 */
#define NTSC_HTRIG		0x72	/* 6c */
#define NTSC_VTRIG		0x00	/* 6c */
#define NTSC_MULTI		0x30	/* 6e */
#define NTSC_CCTTX		0x11	/* 6f */
#define NTSC_FIRST_ACTIVE	0x12	/* 7a */
#define NTSC_LAST_ACTIVE	0x02	/* 7b */
#define NTSC_MSB_VERTICAL	0x40	/* 7c */

#define PAL_BURST_START		0x21	/* 28 */
#define PAL_BURST_END		0x1d	/* 29 */
#define PAL_CHROMA_PHASE	0x3f	/* 5a */
#define PAL_GAINU		0x7d	/* 5b */
#define PAL_GAINV		0xaf	/* 5c */
#define PAL_BLACK_LEVEL		0x23	/* 5d */
#define PAL_BLANKING_LEVEL	0x35	/* 5e */
#define PAL_VBI_BLANKING	0x35	/* 5f */
#define PAL_DAC_CONTROL		0x02	/* 61 */
#define PAL_BURST_AMP		0x2f	/* 62 */
#define PAL_SUBC3		0xcb	/* 63 */
#define PAL_SUBC2		0x8a	/* 64 */
#define PAL_SUBC1		0x09	/* 65 */
#define PAL_SUBC0		0x2a	/* 66 */
#define PAL_HTRIG		0x86	/* 6c */
#define PAL_VTRIG		0x04	/* 6d */
#define PAL_MULTI		0x20	/* 6e */
#define PAL_CCTTX		0x15	/* 6f */
#define PAL_FIRST_ACTIVE	0x16	/* 7a */
#define PAL_LAST_ACTIVE		0x36	/* 7b */
#define PAL_MSB_VERTICAL	0x40	/* 7c */

/* Initialization Sequence */

static __u8 init7121ntsc[] = {
	0x26, 0x0,	0x27, 0x0,
	0x28, NTSC_BURST_START,		0x29, NTSC_BURST_END,
	0x2a, 0x0,	0x2b, 0x0,	0x2c, 0x0,	0x2d, 0x0,
	0x2e, 0x0,	0x2f, 0x0,	0x30, 0x0,	0x31, 0x0,
	0x32, 0x0,	0x33, 0x0,	0x34, 0x0,	0x35, 0x0,
	0x36, 0x0,	0x37, 0x0,	0x38, 0x0,	0x39, 0x0,
	0x3a, 0x03,	0x3b, 0x0,	0x3c, 0x0,	0x3d, 0x0,
	0x3e, 0x0,	0x3f, 0x0,	0x40, 0x0,	0x41, 0x0,
	0x42, 0x0,	0x43, 0x0,	0x44, 0x0,	0x45, 0x0,
	0x46, 0x0,	0x47, 0x0,	0x48, 0x0,	0x49, 0x0,
	0x4a, 0x0,	0x4b, 0x0,	0x4c, 0x0,	0x4d, 0x0,
	0x4e, 0x0,	0x4f, 0x0,	0x50, 0x0,	0x51, 0x0,
	0x52, 0x0,	0x53, 0x0,	0x54, 0x0,	0x55, 0x0,
	0x56, 0x0,	0x57, 0x0,	0x58, 0x0,	0x59, 0x0,
	0x5a, NTSC_CHROMA_PHASE,	0x5b, NTSC_GAINU,
	0x5c, NTSC_GAINV,		0x5d, NTSC_BLACK_LEVEL,
	0x5e, NTSC_BLANKING_LEVEL,	0x5f, NTSC_VBI_BLANKING,
	0x60, 0x0,			0x61, NTSC_DAC_CONTROL,
	0x62, NTSC_BURST_AMP,		0x63, NTSC_SUBC3,
	0x64, NTSC_SUBC2,		0x65, NTSC_SUBC1,
	0x66, NTSC_SUBC0,		0x67, 0x80,	0x68, 0x80,
	0x69, 0x80,	0x6a, 0x80,	0x6b, 0x29,
	0x6c, NTSC_HTRIG,		0x6d, NTSC_VTRIG,
	0x6e, NTSC_MULTI,		0x6f, NTSC_CCTTX,
	0x70, 0xc9,	0x71, 0x68,	0x72, 0x60,	0x73, 0x0,
	0x74, 0x0,	0x75, 0x0,	0x76, 0x0,	0x77, 0x0,
	0x78, 0x0,	0x79, 0x0,	0x7a, NTSC_FIRST_ACTIVE,
	0x7b, NTSC_LAST_ACTIVE,		0x7c, NTSC_MSB_VERTICAL,
	0x7d, 0x0,	0x7e, 0x0,	0x7f, 0x0
};
#define INIT7121LEN	(sizeof(init7121ntsc)/2)

static __u8 init7121pal[] = {
	0x26, 0x0,	0x27, 0x0,
	0x28, PAL_BURST_START,		0x29, PAL_BURST_END,
	0x2a, 0x0,	0x2b, 0x0,	0x2c, 0x0,	0x2d, 0x0,
	0x2e, 0x0,	0x2f, 0x0,	0x30, 0x0,	0x31, 0x0,
	0x32, 0x0,	0x33, 0x0,	0x34, 0x0,	0x35, 0x0,
	0x36, 0x0,	0x37, 0x0,	0x38, 0x0,	0x39, 0x0,
	0x3a, 0x03,	0x3b, 0x0,	0x3c, 0x0,	0x3d, 0x0,
	0x3e, 0x0,	0x3f, 0x0,	0x40, 0x0,	0x41, 0x0,
	0x42, 0x0,	0x43, 0x0,	0x44, 0x0,	0x45, 0x0,
	0x46, 0x0,	0x47, 0x0,	0x48, 0x0,	0x49, 0x0,
	0x4a, 0x0,	0x4b, 0x0,	0x4c, 0x0,	0x4d, 0x0,
	0x4e, 0x0,	0x4f, 0x0,	0x50, 0x0,	0x51, 0x0,
	0x52, 0x0,	0x53, 0x0,	0x54, 0x0,	0x55, 0x0,
	0x56, 0x0,	0x57, 0x0,	0x58, 0x0,	0x59, 0x0,
	0x5a, PAL_CHROMA_PHASE,		0x5b, PAL_GAINU,
	0x5c, PAL_GAINV,		0x5d, PAL_BLACK_LEVEL,
	0x5e, PAL_BLANKING_LEVEL,	0x5f, PAL_VBI_BLANKING,
	0x60, 0x0,			0x61, PAL_DAC_CONTROL,
	0x62, PAL_BURST_AMP,		0x63, PAL_SUBC3,
	0x64, PAL_SUBC2,		0x65, PAL_SUBC1,
	0x66, PAL_SUBC0,		0x67, 0x80,	0x68, 0x80,
	0x69, 0x80,	0x6a, 0x80,	0x6b, 0x29,
	0x6c, PAL_HTRIG,		0x6d, PAL_VTRIG,
	0x6e, PAL_MULTI,		0x6f, PAL_CCTTX,
	0x70, 0xc9,	0x71, 0x68,	0x72, 0x60,	0x73, 0x0,
	0x74, 0x0,	0x75, 0x0,	0x76, 0x0,	0x77, 0x0,
	0x78, 0x0,	0x79, 0x0,	0x7a, PAL_FIRST_ACTIVE,
	0x7b, PAL_LAST_ACTIVE,		0x7c, PAL_MSB_VERTICAL,
	0x7d, 0x0,	0x7e, 0x0,	0x7f, 0x0
};
#endif
