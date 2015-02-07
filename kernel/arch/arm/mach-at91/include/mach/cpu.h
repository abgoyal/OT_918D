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
 * arch/arm/mach-at91/include/mach/cpu.h
 *
 *  Copyright (C) 2006 SAN People
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 */

#ifndef __ASM_ARCH_CPU_H
#define __ASM_ARCH_CPU_H

#include <mach/hardware.h>
#include <mach/at91_dbgu.h>


#define ARCH_ID_AT91RM9200	0x09290780
#define ARCH_ID_AT91SAM9260	0x019803a0
#define ARCH_ID_AT91SAM9261	0x019703a0
#define ARCH_ID_AT91SAM9263	0x019607a0
#define ARCH_ID_AT91SAM9G10	0x019903a0
#define ARCH_ID_AT91SAM9G20	0x019905a0
#define ARCH_ID_AT91SAM9RL64	0x019b03a0
#define ARCH_ID_AT91SAM9G45	0x819b05a0
#define ARCH_ID_AT91SAM9G45MRL	0x819b05a2	/* aka 9G45-ES2 & non ES lots */
#define ARCH_ID_AT91SAM9G45ES	0x819b05a1	/* 9G45-ES (Engineering Sample) */
#define ARCH_ID_AT91CAP9	0x039A03A0

#define ARCH_ID_AT91SAM9XE128	0x329973a0
#define ARCH_ID_AT91SAM9XE256	0x329a93a0
#define ARCH_ID_AT91SAM9XE512	0x329aa3a0

#define ARCH_ID_AT572D940HF	0x0e0303e0

#define ARCH_ID_AT91M40800	0x14080044
#define ARCH_ID_AT91R40807	0x44080746
#define ARCH_ID_AT91M40807	0x14080745
#define ARCH_ID_AT91R40008	0x44000840

static inline unsigned long at91_cpu_identify(void)
{
	return (at91_sys_read(AT91_DBGU_CIDR) & ~AT91_CIDR_VERSION);
}

static inline unsigned long at91_cpu_fully_identify(void)
{
	return at91_sys_read(AT91_DBGU_CIDR);
}

#define ARCH_EXID_AT91SAM9M11	0x00000001
#define ARCH_EXID_AT91SAM9M10	0x00000002
#define ARCH_EXID_AT91SAM9G45	0x00000004

static inline unsigned long at91_exid_identify(void)
{
	return at91_sys_read(AT91_DBGU_EXID);
}


#define ARCH_FAMILY_AT91X92	0x09200000
#define ARCH_FAMILY_AT91SAM9	0x01900000
#define ARCH_FAMILY_AT91SAM9XE	0x02900000

static inline unsigned long at91_arch_identify(void)
{
	return (at91_sys_read(AT91_DBGU_CIDR) & AT91_CIDR_ARCH);
}

#ifdef CONFIG_ARCH_AT91CAP9
#include <mach/at91_pmc.h>

#define ARCH_REVISION_CAP9_B	0x399
#define ARCH_REVISION_CAP9_C	0x601

static inline unsigned long at91cap9_rev_identify(void)
{
	return (at91_sys_read(AT91_PMC_VER));
}
#endif

#ifdef CONFIG_ARCH_AT91RM9200
#define cpu_is_at91rm9200()	(at91_cpu_identify() == ARCH_ID_AT91RM9200)
#else
#define cpu_is_at91rm9200()	(0)
#endif

#ifdef CONFIG_ARCH_AT91SAM9260
#define cpu_is_at91sam9xe()	(at91_arch_identify() == ARCH_FAMILY_AT91SAM9XE)
#define cpu_is_at91sam9260()	((at91_cpu_identify() == ARCH_ID_AT91SAM9260) || cpu_is_at91sam9xe())
#else
#define cpu_is_at91sam9xe()	(0)
#define cpu_is_at91sam9260()	(0)
#endif

#ifdef CONFIG_ARCH_AT91SAM9G20
#define cpu_is_at91sam9g20()	(at91_cpu_identify() == ARCH_ID_AT91SAM9G20)
#else
#define cpu_is_at91sam9g20()	(0)
#endif

#ifdef CONFIG_ARCH_AT91SAM9261
#define cpu_is_at91sam9261()	(at91_cpu_identify() == ARCH_ID_AT91SAM9261)
#else
#define cpu_is_at91sam9261()	(0)
#endif

#ifdef CONFIG_ARCH_AT91SAM9G10
#define cpu_is_at91sam9g10()	((at91_cpu_identify() & ~AT91_CIDR_EXT)	== ARCH_ID_AT91SAM9G10)
#else
#define cpu_is_at91sam9g10()	(0)
#endif

#ifdef CONFIG_ARCH_AT91SAM9263
#define cpu_is_at91sam9263()	(at91_cpu_identify() == ARCH_ID_AT91SAM9263)
#else
#define cpu_is_at91sam9263()	(0)
#endif

#ifdef CONFIG_ARCH_AT91SAM9RL
#define cpu_is_at91sam9rl()	(at91_cpu_identify() == ARCH_ID_AT91SAM9RL64)
#else
#define cpu_is_at91sam9rl()	(0)
#endif

#ifdef CONFIG_ARCH_AT91SAM9G45
#define cpu_is_at91sam9g45()	(at91_cpu_identify() == ARCH_ID_AT91SAM9G45)
#define cpu_is_at91sam9g45es()	(at91_cpu_fully_identify() == ARCH_ID_AT91SAM9G45ES)
#else
#define cpu_is_at91sam9g45()	(0)
#define cpu_is_at91sam9g45es()	(0)
#endif

#ifdef CONFIG_ARCH_AT91CAP9
#define cpu_is_at91cap9()	(at91_cpu_identify() == ARCH_ID_AT91CAP9)
#define cpu_is_at91cap9_revB()	(at91cap9_rev_identify() == ARCH_REVISION_CAP9_B)
#define cpu_is_at91cap9_revC()	(at91cap9_rev_identify() == ARCH_REVISION_CAP9_C)
#else
#define cpu_is_at91cap9()	(0)
#define cpu_is_at91cap9_revB()	(0)
#define cpu_is_at91cap9_revC()	(0)
#endif

#ifdef CONFIG_ARCH_AT572D940HF
#define cpu_is_at572d940hf() (at91_cpu_identify() == ARCH_ID_AT572D940HF)
#else
#define cpu_is_at572d940hf() (0)
#endif

/*
 * Since this is ARM, we will never run on any AVR32 CPU. But these
 * definitions may reduce clutter in common drivers.
 */
#define cpu_is_at32ap7000()	(0)

#endif