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
 * linux/arch/arm/mach-mmp/include/mach/regs-smc.h
 *
 *  Static Memory Controller Registers
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#ifndef __ASM_MACH_REGS_SMC_H
#define __ASM_MACH_REGS_SMC_H

#include <mach/addr-map.h>

#define SMC_VIRT_BASE		(AXI_VIRT_BASE + 0x83800)
#define SMC_REG(x)		(SMC_VIRT_BASE + (x))

#define SMC_MSC0		SMC_REG(0x0020)
#define SMC_MSC1		SMC_REG(0x0024)
#define SMC_SXCNFG0		SMC_REG(0x0030)
#define SMC_SXCNFG1		SMC_REG(0x0034)
#define SMC_MEMCLKCFG		SMC_REG(0x0068)
#define SMC_CSDFICFG0		SMC_REG(0x0090)
#define SMC_CSDFICFG1		SMC_REG(0x0094)
#define SMC_CLK_RET_DEL		SMC_REG(0x00b0)
#define SMC_ADV_RET_DEL		SMC_REG(0x00b4)
#define SMC_CSADRMAP0		SMC_REG(0x00c0)
#define SMC_CSADRMAP1		SMC_REG(0x00c4)
#define SMC_WE_AP0		SMC_REG(0x00e0)
#define SMC_WE_AP1		SMC_REG(0x00e4)
#define SMC_OE_AP0		SMC_REG(0x00f0)
#define SMC_OE_AP1		SMC_REG(0x00f4)
#define SMC_ADV_AP0		SMC_REG(0x0100)
#define SMC_ADV_AP1		SMC_REG(0x0104)

#endif /* __ASM_MACH_REGS_SMC_H */
