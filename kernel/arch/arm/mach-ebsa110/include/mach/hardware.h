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
 *  arch/arm/mach-ebsa110/include/mach/hardware.h
 *
 *  Copyright (C) 1996-2000 Russell King.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 *
 * This file contains the hardware definitions of the EBSA-110.
 */
#ifndef __ASM_ARCH_HARDWARE_H
#define __ASM_ARCH_HARDWARE_H

/*
 * The EBSA110 has a weird "ISA IO" region:
 *
 * Region 0 (addr = 0xf0000000 + io << 2)
 * --------------------------------------------------------
 * Physical region	IO region
 * f0000fe0 - f0000ffc	3f8 - 3ff  ttyS0
 * f0000e60 - f0000e64	398 - 399
 * f0000de0 - f0000dfc	378 - 37f  lp0
 * f0000be0 - f0000bfc	2f8 - 2ff  ttyS1
 *
 * Region 1 (addr = 0xf0000000 + (io & ~1) << 1 + (io & 1))
 * --------------------------------------------------------
 * Physical region	IO region
 * f00014f1             a79        pnp write data
 * f00007c0 - f00007c1	3e0 - 3e1  pcmcia
 * f00004f1		279        pnp address
 * f0000440 - f000046c  220 - 236  eth0
 * f0000405		203        pnp read data
 */

#define ISAMEM_PHYS		0xe0000000
#define ISAMEM_SIZE		0x10000000

#define ISAIO_PHYS		0xf0000000
#define ISAIO_SIZE		PGDIR_SIZE

#define TRICK0_PHYS		0xf2000000
#define TRICK1_PHYS		0xf2400000
#define TRICK2_PHYS		0xf2800000
#define TRICK3_PHYS		0xf2c00000
#define TRICK4_PHYS		0xf3000000
#define TRICK5_PHYS		0xf3400000
#define TRICK6_PHYS		0xf3800000
#define TRICK7_PHYS		0xf3c00000

#define ISAMEM_BASE		0xe0000000
#define ISAIO_BASE		0xf0000000

#define PIT_BASE		0xfc000000
#define SOFT_BASE		0xfd000000

/*
 * RAM definitions
 */
#define UNCACHEABLE_ADDR	0xff000000	/* IRQ_STAT */

#endif

