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
 *	vmlinux.lds.S -- master linker script for m68knommu arch
 *
 *	(C) Copyright 2002-2006, Greg Ungerer <gerg@snapgear.com>
 *
 *	This linker script is equiped to build either ROM loaded or RAM
 *	run kernels.
 */

#include <asm-generic/vmlinux.lds.h>
#include <asm/page.h>
#include <asm/thread_info.h>

#if defined(CONFIG_RAMKERNEL)
#define	RAM_START	CONFIG_KERNELBASE
#define	RAM_LENGTH	(CONFIG_RAMBASE + CONFIG_RAMSIZE - CONFIG_KERNELBASE)
#define	TEXT		ram
#define	DATA		ram
#define	INIT		ram
#define	BSSS		ram
#endif
#if defined(CONFIG_ROMKERNEL) || defined(CONFIG_HIMEMKERNEL)
#define	RAM_START	CONFIG_RAMBASE
#define	RAM_LENGTH	CONFIG_RAMSIZE
#define	ROMVEC_START	CONFIG_ROMVEC
#define	ROMVEC_LENGTH	CONFIG_ROMVECSIZE
#define	ROM_START	CONFIG_ROMSTART
#define	ROM_LENGTH	CONFIG_ROMSIZE
#define	TEXT		rom
#define	DATA		ram
#define	INIT		ram
#define	BSSS		ram
#endif

#ifndef DATA_ADDR
#define	DATA_ADDR
#endif


OUTPUT_ARCH(m68k)
ENTRY(_start)

MEMORY {
	ram	: ORIGIN = RAM_START, LENGTH = RAM_LENGTH
#ifdef ROM_START
	romvec	: ORIGIN = ROMVEC_START, LENGTH = ROMVEC_LENGTH
	rom	: ORIGIN = ROM_START, LENGTH = ROM_LENGTH
#endif
}

jiffies = jiffies_64 + 4;

SECTIONS {

#ifdef ROMVEC_START
	. = ROMVEC_START ;
	.romvec : {
		__rom_start = . ;
		_romvec = .;
		*(.data..initvect)
	} > romvec
#endif

	.text : {
		_text = .;
		_stext = . ;
		HEAD_TEXT
		TEXT_TEXT
		SCHED_TEXT
		LOCK_TEXT
		*(.text..lock)

		. = ALIGN(16);          /* Exception table              */
		__start___ex_table = .;
		*(__ex_table)
		__stop___ex_table = .;

		*(.rodata) *(.rodata.*)
		*(__vermagic)		/* Kernel version magic */
		*(__markers_strings)
		*(.rodata1)
		*(.rodata.str1.1)

		/* Kernel symbol table: Normal symbols */
		. = ALIGN(4);
		__start___ksymtab = .;
		*(__ksymtab)
		__stop___ksymtab = .;

		/* Kernel symbol table: GPL-only symbols */
		__start___ksymtab_gpl = .;
		*(__ksymtab_gpl)
		__stop___ksymtab_gpl = .;

		/* Kernel symbol table: Normal unused symbols */
		__start___ksymtab_unused = .;
		*(__ksymtab_unused)
		__stop___ksymtab_unused = .;

		/* Kernel symbol table: GPL-only unused symbols */
		__start___ksymtab_unused_gpl = .;
		*(__ksymtab_unused_gpl)
		__stop___ksymtab_unused_gpl = .;

		/* Kernel symbol table: GPL-future symbols */
		__start___ksymtab_gpl_future = .;
		*(__ksymtab_gpl_future)
		__stop___ksymtab_gpl_future = .;

		/* Kernel symbol table: Normal symbols */
		__start___kcrctab = .;
		*(__kcrctab)
		__stop___kcrctab = .;

		/* Kernel symbol table: GPL-only symbols */
		__start___kcrctab_gpl = .;
		*(__kcrctab_gpl)
		__stop___kcrctab_gpl = .;

		/* Kernel symbol table: Normal unused symbols */
		__start___kcrctab_unused = .;
		*(__kcrctab_unused)
		__stop___kcrctab_unused = .;

		/* Kernel symbol table: GPL-only unused symbols */
		__start___kcrctab_unused_gpl = .;
		*(__kcrctab_unused_gpl)
		__stop___kcrctab_unused_gpl = .;

		/* Kernel symbol table: GPL-future symbols */
		__start___kcrctab_gpl_future = .;
		*(__kcrctab_gpl_future)
		__stop___kcrctab_gpl_future = .;

		/* Kernel symbol table: strings */
		*(__ksymtab_strings)

		/* Built-in module parameters */
		. = ALIGN(4) ;
		__start___param = .;
		*(__param)
		__stop___param = .;

		. = ALIGN(4) ;
		_etext = . ;
	} > TEXT

	.data DATA_ADDR : {
		. = ALIGN(4);
		_sdata = . ;
		DATA_DATA
		CACHELINE_ALIGNED_DATA(32)
		INIT_TASK_DATA(THREAD_SIZE)
		_edata = . ;
	} > DATA

	.init.text : {
		. = ALIGN(PAGE_SIZE);
		__init_begin = .;
	} > INIT
	INIT_TEXT_SECTION(PAGE_SIZE) > INIT
	INIT_DATA_SECTION(16) > INIT
	.init.data : {
		. = ALIGN(PAGE_SIZE);
		__init_end = .;
	} > INIT

	.bss : {
		. = ALIGN(4);
		_sbss = . ;
		*(.bss)
		*(COMMON)
		. = ALIGN(4) ;
		_ebss = . ;
	 	_end = . ;
	} > BSSS

	DISCARDS
}

