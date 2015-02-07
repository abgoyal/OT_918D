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

/* arch/arm/mach-s3c2410/include/mach/regs-power.h
 *
 * Copyright (c) 2003-2006 Simtec Electronics <linux@simtec.co.uk>
 *	http://armlinux.simtec.co.uk/
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * S3C24XX power control register definitions
*/

#ifndef __ASM_ARM_REGS_PWR
#define __ASM_ARM_REGS_PWR __FILE__

#define S3C24XX_PWRREG(x) ((x) + S3C24XX_VA_CLKPWR)

#define S3C2412_PWRMODECON	S3C24XX_PWRREG(0x20)
#define S3C2412_PWRCFG		S3C24XX_PWRREG(0x24)

#define S3C2412_INFORM0		S3C24XX_PWRREG(0x70)
#define S3C2412_INFORM1		S3C24XX_PWRREG(0x74)
#define S3C2412_INFORM2		S3C24XX_PWRREG(0x78)
#define S3C2412_INFORM3		S3C24XX_PWRREG(0x7C)

#define S3C2412_PWRCFG_BATF_IRQ			(1<<0)
#define S3C2412_PWRCFG_BATF_IGNORE		(2<<0)
#define S3C2412_PWRCFG_BATF_SLEEP		(3<<0)
#define S3C2412_PWRCFG_BATF_MASK		(3<<0)

#define S3C2412_PWRCFG_STANDBYWFI_IGNORE	(0<<6)
#define S3C2412_PWRCFG_STANDBYWFI_IDLE		(1<<6)
#define S3C2412_PWRCFG_STANDBYWFI_STOP		(2<<6)
#define S3C2412_PWRCFG_STANDBYWFI_SLEEP		(3<<6)
#define S3C2412_PWRCFG_STANDBYWFI_MASK		(3<<6)

#define S3C2412_PWRCFG_RTC_MASKIRQ		(1<<8)
#define S3C2412_PWRCFG_NAND_NORST		(1<<9)

#endif /* __ASM_ARM_REGS_PWR */
