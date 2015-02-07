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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * Copyright (C) 1999, 2000, 2003 Ralf Baechle
 * Copyright (C) 1999, 2000 Silicon Graphics, Inc.
 */
#ifndef _ASM_SIM_H
#define _ASM_SIM_H


#include <asm/asm-offsets.h>

#define __str2(x) #x
#define __str(x) __str2(x)

#ifdef CONFIG_32BIT

#define save_static_function(symbol)					\
__asm__(								\
	".text\n\t"							\
	".globl\t" #symbol "\n\t"					\
	".align\t2\n\t"							\
	".type\t" #symbol ", @function\n\t"				\
	".ent\t" #symbol ", 0\n"					\
	#symbol":\n\t"							\
	".frame\t$29, 0, $31\n\t"					\
	"sw\t$16,"__str(PT_R16)"($29)\t\t\t# save_static_function\n\t"	\
	"sw\t$17,"__str(PT_R17)"($29)\n\t"				\
	"sw\t$18,"__str(PT_R18)"($29)\n\t"				\
	"sw\t$19,"__str(PT_R19)"($29)\n\t"				\
	"sw\t$20,"__str(PT_R20)"($29)\n\t"				\
	"sw\t$21,"__str(PT_R21)"($29)\n\t"				\
	"sw\t$22,"__str(PT_R22)"($29)\n\t"				\
	"sw\t$23,"__str(PT_R23)"($29)\n\t"				\
	"sw\t$30,"__str(PT_R30)"($29)\n\t"				\
	"j\t_" #symbol "\n\t"						\
	".end\t" #symbol "\n\t"						\
	".size\t" #symbol",. - " #symbol)

#define nabi_no_regargs

#endif /* CONFIG_32BIT */

#ifdef CONFIG_64BIT

#define save_static_function(symbol)					\
__asm__(								\
	".text\n\t"							\
	".globl\t" #symbol "\n\t"					\
	".align\t2\n\t"							\
	".type\t" #symbol ", @function\n\t"				\
	".ent\t" #symbol ", 0\n"					\
	#symbol":\n\t"							\
	".frame\t$29, 0, $31\n\t"					\
	"sd\t$16,"__str(PT_R16)"($29)\t\t\t# save_static_function\n\t"	\
	"sd\t$17,"__str(PT_R17)"($29)\n\t"				\
	"sd\t$18,"__str(PT_R18)"($29)\n\t"				\
	"sd\t$19,"__str(PT_R19)"($29)\n\t"				\
	"sd\t$20,"__str(PT_R20)"($29)\n\t"				\
	"sd\t$21,"__str(PT_R21)"($29)\n\t"				\
	"sd\t$22,"__str(PT_R22)"($29)\n\t"				\
	"sd\t$23,"__str(PT_R23)"($29)\n\t"				\
	"sd\t$30,"__str(PT_R30)"($29)\n\t"				\
	"j\t_" #symbol "\n\t"						\
	".end\t" #symbol "\n\t"						\
	".size\t" #symbol",. - " #symbol)

#define nabi_no_regargs							\
	unsigned long __dummy0,						\
	unsigned long __dummy1,						\
	unsigned long __dummy2,						\
	unsigned long __dummy3,						\
	unsigned long __dummy4,						\
	unsigned long __dummy5,						\
	unsigned long __dummy6,						\
	unsigned long __dummy7,

#endif /* CONFIG_64BIT */

#endif /* _ASM_SIM_H */
