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
 * YAFFS: Yet another Flash File System . A NAND-flash specific file system.
 *
 * Copyright (C) 2002-2010 Aleph One Ltd.
 *   for Toby Churchill Ltd and Brightstar Engineering
 *
 * Created by Charles Manning <charles@aleph1.co.uk>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License version 2.1 as
 * published by the Free Software Foundation.
 *
 * Note: Only YAFFS headers are LGPL, YAFFS C code is covered by GPL.
 */


#ifndef __YTRACE_H__
#define __YTRACE_H__

extern unsigned int yaffs_traceMask;
extern unsigned int yaffs_wr_attempts;

/*
 * Tracing flags.
 * The flags masked in YAFFS_TRACE_ALWAYS are always traced.
 */

#define YAFFS_TRACE_OS			0x00000002
#define YAFFS_TRACE_ALLOCATE		0x00000004
#define YAFFS_TRACE_SCAN		0x00000008
#define YAFFS_TRACE_BAD_BLOCKS		0x00000010
#define YAFFS_TRACE_ERASE		0x00000020
#define YAFFS_TRACE_GC			0x00000040
#define YAFFS_TRACE_WRITE		0x00000080
#define YAFFS_TRACE_TRACING		0x00000100
#define YAFFS_TRACE_DELETION		0x00000200
#define YAFFS_TRACE_BUFFERS		0x00000400
#define YAFFS_TRACE_NANDACCESS		0x00000800
#define YAFFS_TRACE_GC_DETAIL		0x00001000
#define YAFFS_TRACE_SCAN_DEBUG		0x00002000
#define YAFFS_TRACE_MTD			0x00004000
#define YAFFS_TRACE_CHECKPOINT		0x00008000

#define YAFFS_TRACE_VERIFY		0x00010000
#define YAFFS_TRACE_VERIFY_NAND		0x00020000
#define YAFFS_TRACE_VERIFY_FULL		0x00040000
#define YAFFS_TRACE_VERIFY_ALL		0x000F0000

#define YAFFS_TRACE_SYNC		0x00100000
#define YAFFS_TRACE_BACKGROUND		0x00200000
#define YAFFS_TRACE_LOCK		0x00400000

#define YAFFS_TRACE_ERROR		0x40000000
#define YAFFS_TRACE_BUG			0x80000000
#define YAFFS_TRACE_ALWAYS		0xF0000000


#define T(mask, p) do { if ((mask) & (yaffs_traceMask | YAFFS_TRACE_ALWAYS)) TOUT(p); } while (0)

#endif
