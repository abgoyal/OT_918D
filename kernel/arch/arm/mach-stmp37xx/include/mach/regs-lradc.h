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
 * stmp37xx: LRADC register definitions
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
#define REGS_LRADC_BASE	(STMP3XXX_REGS_BASE + 0x50000)

#define HW_LRADC_CTRL0		0x0
#define BM_LRADC_CTRL0_SCHEDULE	0x000000FF
#define BP_LRADC_CTRL0_SCHEDULE	0
#define BM_LRADC_CTRL0_XPLUS_ENABLE	0x00010000
#define BM_LRADC_CTRL0_YPLUS_ENABLE	0x00020000
#define BM_LRADC_CTRL0_XMINUS_ENABLE	0x00040000
#define BM_LRADC_CTRL0_YMINUS_ENABLE	0x00080000
#define BM_LRADC_CTRL0_TOUCH_DETECT_ENABLE	0x00100000
#define BM_LRADC_CTRL0_ONCHIP_GROUNDREF	0x00200000
#define BM_LRADC_CTRL0_CLKGATE	0x40000000
#define BM_LRADC_CTRL0_SFTRST	0x80000000

#define HW_LRADC_CTRL1		0x10
#define BM_LRADC_CTRL1_LRADC0_IRQ	0x00000001
#define BP_LRADC_CTRL1_LRADC0_IRQ	0
#define BM_LRADC_CTRL1_LRADC5_IRQ	0x00000020
#define BM_LRADC_CTRL1_LRADC6_IRQ	0x00000040
#define BM_LRADC_CTRL1_TOUCH_DETECT_IRQ	0x00000100
#define BM_LRADC_CTRL1_LRADC0_IRQ_EN	0x00010000
#define BM_LRADC_CTRL1_LRADC5_IRQ_EN	0x00200000
#define BM_LRADC_CTRL1_TOUCH_DETECT_IRQ_EN	0x01000000

#define HW_LRADC_CTRL2		0x20
#define BM_LRADC_CTRL2_BL_BRIGHTNESS	0x001F0000
#define BP_LRADC_CTRL2_BL_BRIGHTNESS	16
#define BM_LRADC_CTRL2_BL_MUX_SELECT	0x00200000
#define BM_LRADC_CTRL2_BL_ENABLE	0x00400000
#define BM_LRADC_CTRL2_DIVIDE_BY_TWO	0xFF000000
#define BP_LRADC_CTRL2_DIVIDE_BY_TWO	24

#define HW_LRADC_CTRL3		0x30
#define BM_LRADC_CTRL3_CYCLE_TIME	0x00000300
#define BP_LRADC_CTRL3_CYCLE_TIME	8

#define HW_LRADC_STATUS		0x40
#define BM_LRADC_STATUS_TOUCH_DETECT_RAW	0x00000001
#define BP_LRADC_STATUS_TOUCH_DETECT_RAW	0

#define HW_LRADC_CH0		(0x50 + 0 * 0x10)
#define HW_LRADC_CH1		(0x50 + 1 * 0x10)
#define HW_LRADC_CH2		(0x50 + 2 * 0x10)
#define HW_LRADC_CH3		(0x50 + 3 * 0x10)
#define HW_LRADC_CH4		(0x50 + 4 * 0x10)
#define HW_LRADC_CH5		(0x50 + 5 * 0x10)
#define HW_LRADC_CH6		(0x50 + 6 * 0x10)
#define HW_LRADC_CH7		(0x50 + 7 * 0x10)

#define HW_LRADC_CHn		0x50
#define BM_LRADC_CHn_VALUE	0x0003FFFF
#define BP_LRADC_CHn_VALUE	0
#define BM_LRADC_CHn_NUM_SAMPLES	0x1F000000
#define BP_LRADC_CHn_NUM_SAMPLES	24
#define BM_LRADC_CHn_ACCUMULATE	0x20000000

#define HW_LRADC_DELAY0		(0xD0 + 0 * 0x10)
#define HW_LRADC_DELAY1		(0xD0 + 1 * 0x10)
#define HW_LRADC_DELAY2		(0xD0 + 2 * 0x10)
#define HW_LRADC_DELAY3		(0xD0 + 3 * 0x10)

#define HW_LRADC_DELAYn		0xD0
#define BM_LRADC_DELAYn_DELAY	0x000007FF
#define BP_LRADC_DELAYn_DELAY	0
#define BM_LRADC_DELAYn_LOOP_COUNT	0x0000F800
#define BP_LRADC_DELAYn_LOOP_COUNT	11
#define BM_LRADC_DELAYn_TRIGGER_DELAYS	0x000F0000
#define BP_LRADC_DELAYn_TRIGGER_DELAYS	16
#define BM_LRADC_DELAYn_KICK	0x00100000
#define BM_LRADC_DELAYn_TRIGGER_LRADCS	0xFF000000
#define BP_LRADC_DELAYn_TRIGGER_LRADCS	24

#define HW_LRADC_CTRL4		0x140
#define BM_LRADC_CTRL4_LRADC6SELECT	0x0F000000
#define BP_LRADC_CTRL4_LRADC6SELECT	24
#define BM_LRADC_CTRL4_LRADC7SELECT	0xF0000000
#define BP_LRADC_CTRL4_LRADC7SELECT	28