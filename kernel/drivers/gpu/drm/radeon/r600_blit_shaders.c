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
 * Copyright 2009 Advanced Micro Devices, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice (including the next
 * paragraph) shall be included in all copies or substantial portions of the
 * Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 *
 * Authors:
 *     Alex Deucher <alexander.deucher@amd.com>
 */

#include <linux/types.h>
#include <linux/kernel.h>

/*
 * R6xx+ cards need to use the 3D engine to blit data which requires
 * quite a bit of hw state setup.  Rather than pull the whole 3D driver
 * (which normally generates the 3D state) into the DRM, we opt to use
 * statically generated state tables.  The regsiter state and shaders
 * were hand generated to support blitting functionality.  See the 3D
 * driver or documentation for descriptions of the registers and
 * shader instructions.
 */

const u32 r6xx_default_state[] =
{
	0xc0002400,
	0x00000000,
	0xc0012800,
	0x80000000,
	0x80000000,
	0xc0016800,
	0x00000010,
	0x00008000,
	0xc0016800,
	0x00000542,
	0x07000003,
	0xc0016800,
	0x000005c5,
	0x00000000,
	0xc0016800,
	0x00000363,
	0x00000000,
	0xc0016800,
	0x0000060c,
	0x82000000,
	0xc0016800,
	0x0000060e,
	0x01020204,
	0xc0016f00,
	0x00000000,
	0x00000000,
	0xc0016f00,
	0x00000001,
	0x00000000,
	0xc0096900,
	0x0000022a,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0xc0016900,
	0x00000004,
	0x00000000,
	0xc0016900,
	0x0000000a,
	0x00000000,
	0xc0016900,
	0x0000000b,
	0x00000000,
	0xc0016900,
	0x0000010c,
	0x00000000,
	0xc0016900,
	0x0000010d,
	0x00000000,
	0xc0016900,
	0x00000200,
	0x00000000,
	0xc0016900,
	0x00000343,
	0x00000060,
	0xc0016900,
	0x00000344,
	0x00000040,
	0xc0016900,
	0x00000351,
	0x0000aa00,
	0xc0016900,
	0x00000104,
	0x00000000,
	0xc0016900,
	0x0000010e,
	0x00000000,
	0xc0046900,
	0x00000105,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0xc0036900,
	0x00000109,
	0x00000000,
	0x00000000,
	0x00000000,
	0xc0046900,
	0x0000030c,
	0x01000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0xc0046900,
	0x00000048,
	0x3f800000,
	0x00000000,
	0x3f800000,
	0x3f800000,
	0xc0016900,
	0x0000008e,
	0x0000000f,
	0xc0016900,
	0x00000080,
	0x00000000,
	0xc0016900,
	0x00000083,
	0x0000ffff,
	0xc0016900,
	0x00000084,
	0x00000000,
	0xc0016900,
	0x00000085,
	0x20002000,
	0xc0016900,
	0x00000086,
	0x00000000,
	0xc0016900,
	0x00000087,
	0x20002000,
	0xc0016900,
	0x00000088,
	0x00000000,
	0xc0016900,
	0x00000089,
	0x20002000,
	0xc0016900,
	0x0000008a,
	0x00000000,
	0xc0016900,
	0x0000008b,
	0x20002000,
	0xc0016900,
	0x0000008c,
	0x00000000,
	0xc0016900,
	0x00000094,
	0x80000000,
	0xc0016900,
	0x00000095,
	0x20002000,
	0xc0026900,
	0x000000b4,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x00000096,
	0x80000000,
	0xc0016900,
	0x00000097,
	0x20002000,
	0xc0026900,
	0x000000b6,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x00000098,
	0x80000000,
	0xc0016900,
	0x00000099,
	0x20002000,
	0xc0026900,
	0x000000b8,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x0000009a,
	0x80000000,
	0xc0016900,
	0x0000009b,
	0x20002000,
	0xc0026900,
	0x000000ba,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x0000009c,
	0x80000000,
	0xc0016900,
	0x0000009d,
	0x20002000,
	0xc0026900,
	0x000000bc,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x0000009e,
	0x80000000,
	0xc0016900,
	0x0000009f,
	0x20002000,
	0xc0026900,
	0x000000be,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000a0,
	0x80000000,
	0xc0016900,
	0x000000a1,
	0x20002000,
	0xc0026900,
	0x000000c0,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000a2,
	0x80000000,
	0xc0016900,
	0x000000a3,
	0x20002000,
	0xc0026900,
	0x000000c2,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000a4,
	0x80000000,
	0xc0016900,
	0x000000a5,
	0x20002000,
	0xc0026900,
	0x000000c4,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000a6,
	0x80000000,
	0xc0016900,
	0x000000a7,
	0x20002000,
	0xc0026900,
	0x000000c6,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000a8,
	0x80000000,
	0xc0016900,
	0x000000a9,
	0x20002000,
	0xc0026900,
	0x000000c8,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000aa,
	0x80000000,
	0xc0016900,
	0x000000ab,
	0x20002000,
	0xc0026900,
	0x000000ca,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000ac,
	0x80000000,
	0xc0016900,
	0x000000ad,
	0x20002000,
	0xc0026900,
	0x000000cc,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000ae,
	0x80000000,
	0xc0016900,
	0x000000af,
	0x20002000,
	0xc0026900,
	0x000000ce,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000b0,
	0x80000000,
	0xc0016900,
	0x000000b1,
	0x20002000,
	0xc0026900,
	0x000000d0,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000b2,
	0x80000000,
	0xc0016900,
	0x000000b3,
	0x20002000,
	0xc0026900,
	0x000000d2,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x00000293,
	0x00004010,
	0xc0016900,
	0x00000300,
	0x00000000,
	0xc0016900,
	0x00000301,
	0x00000000,
	0xc0016900,
	0x00000312,
	0xffffffff,
	0xc0016900,
	0x00000307,
	0x00000000,
	0xc0016900,
	0x00000308,
	0x00000000,
	0xc0016900,
	0x00000283,
	0x00000000,
	0xc0016900,
	0x00000292,
	0x00000000,
	0xc0066900,
	0x0000010f,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0xc0016900,
	0x00000206,
	0x00000000,
	0xc0016900,
	0x00000207,
	0x00000000,
	0xc0016900,
	0x00000208,
	0x00000000,
	0xc0046900,
	0x00000303,
	0x3f800000,
	0x3f800000,
	0x3f800000,
	0x3f800000,
	0xc0016900,
	0x00000205,
	0x00000004,
	0xc0016900,
	0x00000280,
	0x00000000,
	0xc0016900,
	0x00000281,
	0x00000000,
	0xc0016900,
	0x0000037e,
	0x00000000,
	0xc0016900,
	0x00000382,
	0x00000000,
	0xc0016900,
	0x00000380,
	0x00000000,
	0xc0016900,
	0x00000383,
	0x00000000,
	0xc0016900,
	0x00000381,
	0x00000000,
	0xc0016900,
	0x00000282,
	0x00000008,
	0xc0016900,
	0x00000302,
	0x0000002d,
	0xc0016900,
	0x0000037f,
	0x00000000,
	0xc0016900,
	0x000001b2,
	0x00000000,
	0xc0016900,
	0x000001b6,
	0x00000000,
	0xc0016900,
	0x000001b7,
	0x00000000,
	0xc0016900,
	0x000001b8,
	0x00000000,
	0xc0016900,
	0x000001b9,
	0x00000000,
	0xc0016900,
	0x00000225,
	0x00000000,
	0xc0016900,
	0x00000229,
	0x00000000,
	0xc0016900,
	0x00000237,
	0x00000000,
	0xc0016900,
	0x00000100,
	0x00000800,
	0xc0016900,
	0x00000101,
	0x00000000,
	0xc0016900,
	0x00000102,
	0x00000000,
	0xc0016900,
	0x000002a8,
	0x00000000,
	0xc0016900,
	0x000002a9,
	0x00000000,
	0xc0016900,
	0x00000103,
	0x00000000,
	0xc0016900,
	0x00000284,
	0x00000000,
	0xc0016900,
	0x00000290,
	0x00000000,
	0xc0016900,
	0x00000285,
	0x00000000,
	0xc0016900,
	0x00000286,
	0x00000000,
	0xc0016900,
	0x00000287,
	0x00000000,
	0xc0016900,
	0x00000288,
	0x00000000,
	0xc0016900,
	0x00000289,
	0x00000000,
	0xc0016900,
	0x0000028a,
	0x00000000,
	0xc0016900,
	0x0000028b,
	0x00000000,
	0xc0016900,
	0x0000028c,
	0x00000000,
	0xc0016900,
	0x0000028d,
	0x00000000,
	0xc0016900,
	0x0000028e,
	0x00000000,
	0xc0016900,
	0x0000028f,
	0x00000000,
	0xc0016900,
	0x000002a1,
	0x00000000,
	0xc0016900,
	0x000002a5,
	0x00000000,
	0xc0016900,
	0x000002ac,
	0x00000000,
	0xc0016900,
	0x000002ad,
	0x00000000,
	0xc0016900,
	0x000002ae,
	0x00000000,
	0xc0016900,
	0x000002c8,
	0x00000000,
	0xc0016900,
	0x00000206,
	0x00000100,
	0xc0016900,
	0x00000204,
	0x00010000,
	0xc0036e00,
	0x00000000,
	0x00000012,
	0x00000000,
	0x00000000,
	0xc0016900,
	0x0000008f,
	0x0000000f,
	0xc0016900,
	0x000001e8,
	0x00000001,
	0xc0016900,
	0x00000202,
	0x00cc0000,
	0xc0016900,
	0x00000205,
	0x00000244,
	0xc0016900,
	0x00000203,
	0x00000210,
	0xc0016900,
	0x000001b1,
	0x00000000,
	0xc0016900,
	0x00000185,
	0x00000000,
	0xc0016900,
	0x000001b3,
	0x00000001,
	0xc0016900,
	0x000001b4,
	0x00000000,
	0xc0016900,
	0x00000191,
	0x00000b00,
	0xc0016900,
	0x000001b5,
	0x00000000,
};

const u32 r7xx_default_state[] =
{
	0xc0012800,
	0x80000000,
	0x80000000,
	0xc0016800,
	0x00000010,
	0x00008000,
	0xc0016800,
	0x00000542,
	0x07000002,
	0xc0016800,
	0x000005c5,
	0x00000000,
	0xc0016800,
	0x00000363,
	0x00004000,
	0xc0016800,
	0x0000060c,
	0x00000000,
	0xc0016800,
	0x0000060e,
	0x00420204,
	0xc0016f00,
	0x00000000,
	0x00000000,
	0xc0016f00,
	0x00000001,
	0x00000000,
	0xc0096900,
	0x0000022a,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0xc0016900,
	0x00000004,
	0x00000000,
	0xc0016900,
	0x0000000a,
	0x00000000,
	0xc0016900,
	0x0000000b,
	0x00000000,
	0xc0016900,
	0x0000010c,
	0x00000000,
	0xc0016900,
	0x0000010d,
	0x00000000,
	0xc0016900,
	0x00000200,
	0x00000000,
	0xc0016900,
	0x00000343,
	0x00000060,
	0xc0016900,
	0x00000344,
	0x00000000,
	0xc0016900,
	0x00000351,
	0x0000aa00,
	0xc0016900,
	0x00000104,
	0x00000000,
	0xc0016900,
	0x0000010e,
	0x00000000,
	0xc0046900,
	0x00000105,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0xc0046900,
	0x0000030c,
	0x01000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0xc0016900,
	0x0000008e,
	0x0000000f,
	0xc0016900,
	0x00000080,
	0x00000000,
	0xc0016900,
	0x00000083,
	0x0000ffff,
	0xc0016900,
	0x00000084,
	0x00000000,
	0xc0016900,
	0x00000085,
	0x20002000,
	0xc0016900,
	0x00000086,
	0x00000000,
	0xc0016900,
	0x00000087,
	0x20002000,
	0xc0016900,
	0x00000088,
	0x00000000,
	0xc0016900,
	0x00000089,
	0x20002000,
	0xc0016900,
	0x0000008a,
	0x00000000,
	0xc0016900,
	0x0000008b,
	0x20002000,
	0xc0016900,
	0x0000008c,
	0xaaaaaaaa,
	0xc0016900,
	0x00000094,
	0x80000000,
	0xc0016900,
	0x00000095,
	0x20002000,
	0xc0026900,
	0x000000b4,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x00000096,
	0x80000000,
	0xc0016900,
	0x00000097,
	0x20002000,
	0xc0026900,
	0x000000b6,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x00000098,
	0x80000000,
	0xc0016900,
	0x00000099,
	0x20002000,
	0xc0026900,
	0x000000b8,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x0000009a,
	0x80000000,
	0xc0016900,
	0x0000009b,
	0x20002000,
	0xc0026900,
	0x000000ba,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x0000009c,
	0x80000000,
	0xc0016900,
	0x0000009d,
	0x20002000,
	0xc0026900,
	0x000000bc,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x0000009e,
	0x80000000,
	0xc0016900,
	0x0000009f,
	0x20002000,
	0xc0026900,
	0x000000be,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000a0,
	0x80000000,
	0xc0016900,
	0x000000a1,
	0x20002000,
	0xc0026900,
	0x000000c0,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000a2,
	0x80000000,
	0xc0016900,
	0x000000a3,
	0x20002000,
	0xc0026900,
	0x000000c2,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000a4,
	0x80000000,
	0xc0016900,
	0x000000a5,
	0x20002000,
	0xc0026900,
	0x000000c4,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000a6,
	0x80000000,
	0xc0016900,
	0x000000a7,
	0x20002000,
	0xc0026900,
	0x000000c6,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000a8,
	0x80000000,
	0xc0016900,
	0x000000a9,
	0x20002000,
	0xc0026900,
	0x000000c8,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000aa,
	0x80000000,
	0xc0016900,
	0x000000ab,
	0x20002000,
	0xc0026900,
	0x000000ca,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000ac,
	0x80000000,
	0xc0016900,
	0x000000ad,
	0x20002000,
	0xc0026900,
	0x000000cc,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000ae,
	0x80000000,
	0xc0016900,
	0x000000af,
	0x20002000,
	0xc0026900,
	0x000000ce,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000b0,
	0x80000000,
	0xc0016900,
	0x000000b1,
	0x20002000,
	0xc0026900,
	0x000000d0,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x000000b2,
	0x80000000,
	0xc0016900,
	0x000000b3,
	0x20002000,
	0xc0026900,
	0x000000d2,
	0x00000000,
	0x3f800000,
	0xc0016900,
	0x00000293,
	0x00514000,
	0xc0016900,
	0x00000300,
	0x00000000,
	0xc0016900,
	0x00000301,
	0x00000000,
	0xc0016900,
	0x00000312,
	0xffffffff,
	0xc0016900,
	0x00000307,
	0x00000000,
	0xc0016900,
	0x00000308,
	0x00000000,
	0xc0016900,
	0x00000283,
	0x00000000,
	0xc0016900,
	0x00000292,
	0x00000000,
	0xc0066900,
	0x0000010f,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0x00000000,
	0xc0016900,
	0x00000206,
	0x00000000,
	0xc0016900,
	0x00000207,
	0x00000000,
	0xc0016900,
	0x00000208,
	0x00000000,
	0xc0046900,
	0x00000303,
	0x3f800000,
	0x3f800000,
	0x3f800000,
	0x3f800000,
	0xc0016900,
	0x00000205,
	0x00000004,
	0xc0016900,
	0x00000280,
	0x00000000,
	0xc0016900,
	0x00000281,
	0x00000000,
	0xc0016900,
	0x0000037e,
	0x00000000,
	0xc0016900,
	0x00000382,
	0x00000000,
	0xc0016900,
	0x00000380,
	0x00000000,
	0xc0016900,
	0x00000383,
	0x00000000,
	0xc0016900,
	0x00000381,
	0x00000000,
	0xc0016900,
	0x00000282,
	0x00000008,
	0xc0016900,
	0x00000302,
	0x0000002d,
	0xc0016900,
	0x0000037f,
	0x00000000,
	0xc0016900,
	0x000001b2,
	0x00000001,
	0xc0016900,
	0x000001b6,
	0x00000000,
	0xc0016900,
	0x000001b7,
	0x00000000,
	0xc0016900,
	0x000001b8,
	0x00000000,
	0xc0016900,
	0x000001b9,
	0x00000000,
	0xc0016900,
	0x00000225,
	0x00000000,
	0xc0016900,
	0x00000229,
	0x00000000,
	0xc0016900,
	0x00000237,
	0x00000000,
	0xc0016900,
	0x00000100,
	0x00000800,
	0xc0016900,
	0x00000101,
	0x00000000,
	0xc0016900,
	0x00000102,
	0x00000000,
	0xc0016900,
	0x000002a8,
	0x00000000,
	0xc0016900,
	0x000002a9,
	0x00000000,
	0xc0016900,
	0x00000103,
	0x00000000,
	0xc0016900,
	0x00000284,
	0x00000000,
	0xc0016900,
	0x00000290,
	0x00000000,
	0xc0016900,
	0x00000285,
	0x00000000,
	0xc0016900,
	0x00000286,
	0x00000000,
	0xc0016900,
	0x00000287,
	0x00000000,
	0xc0016900,
	0x00000288,
	0x00000000,
	0xc0016900,
	0x00000289,
	0x00000000,
	0xc0016900,
	0x0000028a,
	0x00000000,
	0xc0016900,
	0x0000028b,
	0x00000000,
	0xc0016900,
	0x0000028c,
	0x00000000,
	0xc0016900,
	0x0000028d,
	0x00000000,
	0xc0016900,
	0x0000028e,
	0x00000000,
	0xc0016900,
	0x0000028f,
	0x00000000,
	0xc0016900,
	0x000002a1,
	0x00000000,
	0xc0016900,
	0x000002a5,
	0x00000000,
	0xc0016900,
	0x000002ac,
	0x00000000,
	0xc0016900,
	0x000002ad,
	0x00000000,
	0xc0016900,
	0x000002ae,
	0x00000000,
	0xc0016900,
	0x000002c8,
	0x00000000,
	0xc0016900,
	0x00000206,
	0x00000100,
	0xc0016900,
	0x00000204,
	0x00010000,
	0xc0036e00,
	0x00000000,
	0x00000012,
	0x00000000,
	0x00000000,
	0xc0016900,
	0x0000008f,
	0x0000000f,
	0xc0016900,
	0x000001e8,
	0x00000001,
	0xc0016900,
	0x00000202,
	0x00cc0000,
	0xc0016900,
	0x00000205,
	0x00000244,
	0xc0016900,
	0x00000203,
	0x00000210,
	0xc0016900,
	0x000001b1,
	0x00000000,
	0xc0016900,
	0x00000185,
	0x00000000,
	0xc0016900,
	0x000001b3,
	0x00000001,
	0xc0016900,
	0x000001b4,
	0x00000000,
	0xc0016900,
	0x00000191,
	0x00000b00,
	0xc0016900,
	0x000001b5,
	0x00000000,
};

/* same for r6xx/r7xx */
const u32 r6xx_vs[] =
{
	0x00000004,
	0x81000000,
	0x0000203c,
	0x94000b08,
	0x00004000,
	0x14200b1a,
	0x00000000,
	0x00000000,
	0x3c000000,
	0x68cd1000,
	0x00080000,
	0x00000000,
};

const u32 r6xx_ps[] =
{
	0x00000002,
	0x80800000,
	0x00000000,
	0x94200688,
	0x00000010,
	0x000d1000,
	0xb0800000,
	0x00000000,
};

const u32 r6xx_ps_size = ARRAY_SIZE(r6xx_ps);
const u32 r6xx_vs_size = ARRAY_SIZE(r6xx_vs);
const u32 r6xx_default_size = ARRAY_SIZE(r6xx_default_state);
const u32 r7xx_default_size = ARRAY_SIZE(r7xx_default_state);
