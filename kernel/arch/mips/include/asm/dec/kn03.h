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
 * Hardware info about DECstation 5000/2x0 systems (otherwise known as
 * 3max+) and DECsystem 5900 systems (otherwise known as bigmax) which
 * differ mechanically but are otherwise identical (both are known as
 * KN03).
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1995,1996 by Paul M. Antoine, some code and definitions
 * are by courtesy of Chris Fraser.
 * Copyright (C) 2000, 2002, 2003, 2005  Maciej W. Rozycki
 */
#ifndef __ASM_MIPS_DEC_KN03_H
#define __ASM_MIPS_DEC_KN03_H

#include <asm/dec/ecc.h>
#include <asm/dec/ioasic_addrs.h>

#define KN03_SLOT_BASE		0x1f800000

/*
 * CPU interrupt bits.
 */
#define KN03_CPU_INR_HALT	6	/* HALT button */
#define KN03_CPU_INR_BUS	5	/* memory, I/O bus read/write errors */
#define KN03_CPU_INR_RES_4	4	/* unused */
#define KN03_CPU_INR_RTC	3	/* DS1287 RTC */
#define KN03_CPU_INR_CASCADE	2	/* I/O ASIC cascade */

/*
 * I/O ASIC interrupt bits.  Star marks denote non-IRQ status bits.
 */
#define KN03_IO_INR_3MAXP	15	/* (*) 3max+/bigmax ID */
#define KN03_IO_INR_NVRAM	14	/* (*) NVRAM clear jumper */
#define KN03_IO_INR_TC2		13	/* TURBOchannel slot #2 */
#define KN03_IO_INR_TC1		12	/* TURBOchannel slot #1 */
#define KN03_IO_INR_TC0		11	/* TURBOchannel slot #0 */
#define KN03_IO_INR_NRMOD	10	/* (*) NRMOD manufacturing jumper */
#define KN03_IO_INR_ASC		9	/* ASC (NCR53C94) SCSI */
#define KN03_IO_INR_LANCE	8	/* LANCE (Am7990) Ethernet */
#define KN03_IO_INR_SCC1	7	/* SCC (Z85C30) serial #1 */
#define KN03_IO_INR_SCC0	6	/* SCC (Z85C30) serial #0 */
#define KN03_IO_INR_RTC		5	/* DS1287 RTC */
#define KN03_IO_INR_PSU		4	/* power supply unit warning */
#define KN03_IO_INR_RES_3	3	/* unused */
#define KN03_IO_INR_ASC_DATA	2	/* SCSI data ready (for PIO) */
#define KN03_IO_INR_PBNC	1	/* ~HALT button debouncer */
#define KN03_IO_INR_PBNO	0	/* HALT button debouncer */


/*
 * Memory Control Register bits.
 */
#define KN03_MCR_RES_16		(0xffff<<16)	/* unused */
#define KN03_MCR_DIAGCHK	(1<<15)		/* diagn/norml ECC reads */
#define KN03_MCR_DIAGGEN	(1<<14)		/* diagn/norml ECC writes */
#define KN03_MCR_CORRECT	(1<<13)		/* ECC correct/check */
#define KN03_MCR_RES_11		(0x3<<12)	/* unused */
#define KN03_MCR_BNK32M		(1<<10)		/* 32M/8M stride */
#define KN03_MCR_RES_7		(0x7<<7)	/* unused */
#define KN03_MCR_CHECK		(0x7f<<0)	/* diagnostic check bits */

/*
 * I/O ASIC System Support Register bits.
 */
#define KN03_IO_SSR_TXDIS1	(1<<14)		/* SCC1 transmit disable */
#define KN03_IO_SSR_TXDIS0	(1<<13)		/* SCC0 transmit disable */
#define KN03_IO_SSR_RES_12	(1<<12)		/* unused */

#define KN03_IO_SSR_LEDS	(0xff<<0)	/* ~diagnostic LEDs */

#endif /* __ASM_MIPS_DEC_KN03_H */
