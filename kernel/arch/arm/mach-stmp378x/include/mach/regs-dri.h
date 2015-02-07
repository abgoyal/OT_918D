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
 * stmp378x: DRI register definitions
 *
 * Copyright (c) 2008 Freescale Semiconductor
 * Copyright 2008 Embedded Alley Solutions, Inc All Rights Reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */
#define REGS_DRI_BASE	(STMP3XXX_REGS_BASE + 0x74000)
#define REGS_DRI_PHYS	0x80074000
#define REGS_DRI_SIZE	0x2000

#define HW_DRI_CTRL		0x0
#define BM_DRI_CTRL_RUN		0x00000001
#define BP_DRI_CTRL_RUN		0
#define BM_DRI_CTRL_ATTENTION_IRQ	0x00000002
#define BM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ	0x00000004
#define BM_DRI_CTRL_OVERFLOW_IRQ	0x00000008
#define BM_DRI_CTRL_ATTENTION_IRQ_EN	0x00000200
#define BM_DRI_CTRL_PILOT_SYNC_LOSS_IRQ_EN	0x00000400
#define BM_DRI_CTRL_OVERFLOW_IRQ_EN	0x00000800
#define BM_DRI_CTRL_REACQUIRE_PHASE	0x00008000
#define BM_DRI_CTRL_STOP_ON_PILOT_ERROR	0x02000000
#define BM_DRI_CTRL_STOP_ON_OFLOW_ERROR	0x04000000
#define BM_DRI_CTRL_ENABLE_INPUTS	0x20000000
#define BM_DRI_CTRL_CLKGATE	0x40000000
#define BM_DRI_CTRL_SFTRST	0x80000000

#define HW_DRI_TIMING		0x10
#define BM_DRI_TIMING_GAP_DETECTION_INTERVAL	0x000000FF
#define BP_DRI_TIMING_GAP_DETECTION_INTERVAL	0
#define BM_DRI_TIMING_PILOT_REP_RATE	0x000F0000
#define BP_DRI_TIMING_PILOT_REP_RATE	16
