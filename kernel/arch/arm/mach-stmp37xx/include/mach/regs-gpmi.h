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
 * stmp37xx: GPMI register definitions
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
#define REGS_GPMI_BASE	(STMP3XXX_REGS_BASE + 0xC000)
#define REGS_GPMI_PHYS	0x8000C000
#define REGS_GPMI_SIZE	0x2000

#define HW_GPMI_CTRL0		0x0
#define BM_GPMI_CTRL0_XFER_COUNT	0x0000FFFF
#define BP_GPMI_CTRL0_XFER_COUNT	0
#define BM_GPMI_CTRL0_CS	0x00300000
#define BP_GPMI_CTRL0_CS	20
#define BM_GPMI_CTRL0_LOCK_CS	0x00400000
#define BM_GPMI_CTRL0_WORD_LENGTH	0x00800000
#define BM_GPMI_CTRL0_COMMAND_MODE	0x03000000
#define BP_GPMI_CTRL0_COMMAND_MODE	24
#define BV_GPMI_CTRL0_COMMAND_MODE__WRITE	    0x0
#define BV_GPMI_CTRL0_COMMAND_MODE__READ	     0x1
#define BV_GPMI_CTRL0_COMMAND_MODE__READ_AND_COMPARE 0x2
#define BV_GPMI_CTRL0_COMMAND_MODE__WAIT_FOR_READY   0x3
#define BM_GPMI_CTRL0_RUN	0x20000000
#define BM_GPMI_CTRL0_CLKGATE	0x40000000
#define BM_GPMI_CTRL0_SFTRST	0x80000000
#define BM_GPMI_ECCCTRL_ENABLE_ECC	0x00001000
#define BM_GPMI_ECCCTRL_ECC_CMD	0x00006000
#define BP_GPMI_ECCCTRL_ECC_CMD	13

#define HW_GPMI_CTRL1		0x60
#define BM_GPMI_CTRL1_GPMI_MODE	0x00000003
#define BP_GPMI_CTRL1_GPMI_MODE	0
#define BM_GPMI_CTRL1_ATA_IRQRDY_POLARITY	0x00000004
#define BM_GPMI_CTRL1_DEV_RESET	0x00000008
#define BM_GPMI_CTRL1_TIMEOUT_IRQ	0x00000200
#define BM_GPMI_CTRL1_DEV_IRQ	0x00000400
#define BM_GPMI_CTRL1_DSAMPLE_TIME	0x00007000
#define BP_GPMI_CTRL1_DSAMPLE_TIME	12

#define HW_GPMI_TIMING0		0x70
#define BM_GPMI_TIMING0_DATA_SETUP	0x000000FF
#define BP_GPMI_TIMING0_DATA_SETUP	0
#define BM_GPMI_TIMING0_DATA_HOLD	0x0000FF00
#define BP_GPMI_TIMING0_DATA_HOLD	8

#define HW_GPMI_TIMING1		0x80
#define BM_GPMI_TIMING1_DEVICE_BUSY_TIMEOUT	0xFFFF0000
#define BP_GPMI_TIMING1_DEVICE_BUSY_TIMEOUT	16