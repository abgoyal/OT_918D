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
 * stmp37xx: CLKCTRL register definitions
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
#ifndef _MACH_REGS_CLKCTRL
#define _MACH_REGS_CLKCTRL

#define REGS_CLKCTRL_BASE	(STMP3XXX_REGS_BASE + 0x40000)

#define HW_CLKCTRL_PLLCTRL0	0x0
#define BM_CLKCTRL_PLLCTRL0_EN_USB_CLKS	0x00040000

#define HW_CLKCTRL_CPU		0x20
#define BM_CLKCTRL_CPU_DIV_CPU	0x0000003F
#define BP_CLKCTRL_CPU_DIV_CPU	0

#define HW_CLKCTRL_HBUS		0x30
#define BM_CLKCTRL_HBUS_DIV	0x0000001F
#define BP_CLKCTRL_HBUS_DIV	0

#define HW_CLKCTRL_XBUS		0x40

#define HW_CLKCTRL_XTAL		0x50

#define HW_CLKCTRL_PIX		0x60
#define BM_CLKCTRL_PIX_DIV	0x00007FFF
#define BP_CLKCTRL_PIX_DIV	0
#define BM_CLKCTRL_PIX_CLKGATE	0x80000000

#define HW_CLKCTRL_SSP		0x70

#define HW_CLKCTRL_GPMI		0x80

#define HW_CLKCTRL_SPDIF	0x90

#define HW_CLKCTRL_EMI		0xA0

#define HW_CLKCTRL_IR		0xB0

#define HW_CLKCTRL_SAIF		0xC0

#define HW_CLKCTRL_FRAC		0xD0
#define BM_CLKCTRL_FRAC_EMIFRAC	0x00003F00
#define BP_CLKCTRL_FRAC_EMIFRAC	8
#define BM_CLKCTRL_FRAC_PIXFRAC	0x003F0000
#define BP_CLKCTRL_FRAC_PIXFRAC	16
#define BM_CLKCTRL_FRAC_CLKGATEPIX	0x00800000

#define HW_CLKCTRL_CLKSEQ	0xE0
#define BM_CLKCTRL_CLKSEQ_BYPASS_PIX	0x00000002

#define HW_CLKCTRL_RESET	0xF0
#define BM_CLKCTRL_RESET_DIG	0x00000001
#define BP_CLKCTRL_RESET_DIG	0

#endif
