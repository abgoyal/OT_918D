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
 * Carsten Langgaard, carstenl@mips.com
 * Copyright (C) 2000 MIPS Technologies, Inc.  All rights reserved.
 *
 * ########################################################################
 *
 *  This program is free software; you can distribute it and/or modify it
 *  under the terms of the GNU General Public License (Version 2) as
 *  published by the Free Software Foundation.
 *
 *  This program is distributed in the hope it will be useful, but WITHOUT
 *  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 *  FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 *  for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  59 Temple Place - Suite 330, Boston MA 02111-1307, USA.
 *
 * ########################################################################
 *
 * Defines for the Malta interrupt controller.
 *
 */
#ifndef _MIPS_MALTAINT_H
#define _MIPS_MALTAINT_H

#include <irq.h>

/*
 * Interrupts 0..15 are used for Malta ISA compatible interrupts
 */
#define MALTA_INT_BASE		0

/* CPU interrupt offsets */
#define MIPSCPU_INT_SW0		0
#define MIPSCPU_INT_SW1		1
#define MIPSCPU_INT_MB0		2
#define MIPSCPU_INT_I8259A	MIPSCPU_INT_MB0
#define MIPSCPU_INT_MB1		3
#define MIPSCPU_INT_SMI		MIPSCPU_INT_MB1
#define MIPSCPU_INT_IPI0	MIPSCPU_INT_MB1	/* GIC IPI */
#define MIPSCPU_INT_MB2		4
#define MIPSCPU_INT_IPI1	MIPSCPU_INT_MB2	/* GIC IPI */
#define MIPSCPU_INT_MB3		5
#define MIPSCPU_INT_COREHI	MIPSCPU_INT_MB3
#define MIPSCPU_INT_MB4		6
#define MIPSCPU_INT_CORELO	MIPSCPU_INT_MB4

/*
 * Interrupts 64..127 are used for Soc-it Classic interrupts
 */
#define MSC01C_INT_BASE		64

/* SOC-it Classic interrupt offsets */
#define MSC01C_INT_TMR		0
#define MSC01C_INT_PCI		1

/*
 * Interrupts 64..127 are used for Soc-it EIC interrupts
 */
#define MSC01E_INT_BASE		64

/* SOC-it EIC interrupt offsets */
#define MSC01E_INT_SW0		1
#define MSC01E_INT_SW1		2
#define MSC01E_INT_MB0		3
#define MSC01E_INT_I8259A	MSC01E_INT_MB0
#define MSC01E_INT_MB1		4
#define MSC01E_INT_SMI		MSC01E_INT_MB1
#define MSC01E_INT_MB2		5
#define MSC01E_INT_MB3		6
#define MSC01E_INT_COREHI	MSC01E_INT_MB3
#define MSC01E_INT_MB4		7
#define MSC01E_INT_CORELO	MSC01E_INT_MB4
#define MSC01E_INT_TMR		8
#define MSC01E_INT_PCI		9
#define MSC01E_INT_PERFCTR	10
#define MSC01E_INT_CPUCTR	11

/* GIC's Nomenclature for Core Interrupt Pins on the Malta */
#define GIC_CPU_INT0		0 /* Core Interrupt 2 	*/
#define GIC_CPU_INT1		1 /* .			*/
#define GIC_CPU_INT2		2 /* .			*/
#define GIC_CPU_INT3		3 /* .			*/
#define GIC_CPU_INT4		4 /* .			*/
#define GIC_CPU_INT5		5 /* Core Interrupt 5   */

#define GIC_EXT_INTR(x)		x

/* Dummy data */
#define X			0xdead

/* External Interrupts used for IPI */
#define GIC_IPI_EXT_INTR_RESCHED_VPE0	16
#define GIC_IPI_EXT_INTR_CALLFNC_VPE0	17
#define GIC_IPI_EXT_INTR_RESCHED_VPE1	18
#define GIC_IPI_EXT_INTR_CALLFNC_VPE1	19
#define GIC_IPI_EXT_INTR_RESCHED_VPE2	20
#define GIC_IPI_EXT_INTR_CALLFNC_VPE2	21
#define GIC_IPI_EXT_INTR_RESCHED_VPE3	22
#define GIC_IPI_EXT_INTR_CALLFNC_VPE3	23

#define MIPS_GIC_IRQ_BASE	(MIPS_CPU_IRQ_BASE + 8)

#ifndef __ASSEMBLY__
extern void maltaint_init(void);
#endif

#endif /* !(_MIPS_MALTAINT_H) */
