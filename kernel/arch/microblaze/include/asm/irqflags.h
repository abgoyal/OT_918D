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
 * Copyright (C) 2006 Atmark Techno, Inc.
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License. See the file "COPYING" in the main directory of this archive
 * for more details.
 */

#ifndef _ASM_MICROBLAZE_IRQFLAGS_H
#define _ASM_MICROBLAZE_IRQFLAGS_H

#include <linux/irqflags.h>
#include <asm/registers.h>

# if CONFIG_XILINX_MICROBLAZE0_USE_MSR_INSTR

# define raw_local_irq_save(flags)			\
	do {						\
		asm volatile ("	msrclr %0, %1;		\
				nop;"			\
				: "=r"(flags)		\
				: "i"(MSR_IE)		\
				: "memory");		\
	} while (0)

# define raw_local_irq_disable()			\
	do {						\
		asm volatile ("	msrclr r0, %0;		\
				nop;"			\
				:			\
				: "i"(MSR_IE)		\
				: "memory");		\
	} while (0)

# define raw_local_irq_enable()				\
	do {						\
		asm volatile ("	msrset	r0, %0;		\
				nop;"			\
				:			\
				: "i"(MSR_IE)		\
				: "memory");		\
	} while (0)

# else /* CONFIG_XILINX_MICROBLAZE0_USE_MSR_INSTR == 0 */

# define raw_local_irq_save(flags)				\
	do {							\
		register unsigned tmp;				\
		asm volatile ("	mfs	%0, rmsr;		\
				nop;				\
				andi	%1, %0, %2;		\
				mts	rmsr, %1;		\
				nop;"				\
				: "=r"(flags), "=r" (tmp)	\
				: "i"(~MSR_IE)			\
				: "memory");			\
	} while (0)

# define raw_local_irq_disable()				\
	do {							\
		register unsigned tmp;				\
		asm volatile ("	mfs	%0, rmsr;		\
				nop;				\
				andi	%0, %0, %1;		\
				mts	rmsr, %0;		\
				nop;"			\
				: "=r"(tmp)			\
				: "i"(~MSR_IE)			\
				: "memory");			\
	} while (0)

# define raw_local_irq_enable()					\
	do {							\
		register unsigned tmp;				\
		asm volatile ("	mfs	%0, rmsr;		\
				nop;				\
				ori	%0, %0, %1;		\
				mts	rmsr, %0;		\
				nop;"				\
				: "=r"(tmp)			\
				: "i"(MSR_IE)			\
				: "memory");			\
	} while (0)

# endif /* CONFIG_XILINX_MICROBLAZE0_USE_MSR_INSTR */

#define raw_local_irq_restore(flags)				\
	do {							\
		asm volatile ("	mts	rmsr, %0;		\
				nop;"				\
				:				\
				: "r"(flags)			\
				: "memory");			\
	} while (0)

static inline unsigned long get_msr(void)
{
	unsigned long flags;
	asm volatile ("	mfs	%0, rmsr;	\
			nop;"			\
			: "=r"(flags)		\
			:			\
			: "memory");		\
	return flags;
}

#define raw_local_save_flags(flags)	((flags) = get_msr())
#define raw_irqs_disabled()		((get_msr() & MSR_IE) == 0)
#define raw_irqs_disabled_flags(flags)	((flags & MSR_IE) == 0)

#endif /* _ASM_MICROBLAZE_IRQFLAGS_H */
