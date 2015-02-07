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
 * stmp37xx: TIMROT register definitions
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
#ifndef _MACH_REGS_TIMROT
#define _MACH_REGS_TIMROT

#define REGS_TIMROT_BASE	(STMP3XXX_REGS_BASE + 0x68000)

#define HW_TIMROT_ROTCTRL	0x0
#define BM_TIMROT_ROTCTRL_CLKGATE	0x40000000
#define BM_TIMROT_ROTCTRL_SFTRST	0x80000000

#define HW_TIMROT_TIMCTRL0	(0x20 + 0 * 0x20)
#define HW_TIMROT_TIMCTRL1	(0x20 + 1 * 0x20)
#define HW_TIMROT_TIMCTRL2	(0x20 + 2 * 0x20)

#define HW_TIMROT_TIMCTRLn	0x20
#define BM_TIMROT_TIMCTRLn_SELECT	0x0000000F
#define BP_TIMROT_TIMCTRLn_SELECT	0
#define BM_TIMROT_TIMCTRLn_PRESCALE	0x00000030
#define BP_TIMROT_TIMCTRLn_PRESCALE	4
#define BM_TIMROT_TIMCTRLn_RELOAD	0x00000040
#define BM_TIMROT_TIMCTRLn_UPDATE	0x00000080
#define BM_TIMROT_TIMCTRLn_IRQ_EN	0x00004000
#define BM_TIMROT_TIMCTRLn_IRQ	0x00008000

#define HW_TIMROT_TIMCOUNT0	(0x30 + 0 * 0x20)
#define HW_TIMROT_TIMCOUNT1	(0x30 + 1 * 0x20)
#define HW_TIMROT_TIMCOUNT2	(0x30 + 2 * 0x20)

#define HW_TIMROT_TIMCOUNTn	0x30
#endif
