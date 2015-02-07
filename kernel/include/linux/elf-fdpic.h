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

/* elf-fdpic.h: FDPIC ELF load map
 *
 * Copyright (C) 2003 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _LINUX_ELF_FDPIC_H
#define _LINUX_ELF_FDPIC_H

#include <linux/elf.h>

#define PT_GNU_STACK    (PT_LOOS + 0x474e551)

/* segment mappings for ELF FDPIC libraries/executables/interpreters */
struct elf32_fdpic_loadseg {
	Elf32_Addr	addr;		/* core address to which mapped */
	Elf32_Addr	p_vaddr;	/* VMA recorded in file */
	Elf32_Word	p_memsz;	/* allocation size recorded in file */
};

struct elf32_fdpic_loadmap {
	Elf32_Half	version;	/* version of these structures, just in case... */
	Elf32_Half	nsegs;		/* number of segments */
	struct elf32_fdpic_loadseg segs[];
};

#define ELF32_FDPIC_LOADMAP_VERSION	0x0000

/*
 * binfmt binary parameters structure
 */
struct elf_fdpic_params {
	struct elfhdr			hdr;		/* ref copy of ELF header */
	struct elf_phdr			*phdrs;		/* ref copy of PT_PHDR table */
	struct elf32_fdpic_loadmap	*loadmap;	/* loadmap to be passed to userspace */
	unsigned long			elfhdr_addr;	/* mapped ELF header user address */
	unsigned long			ph_addr;	/* mapped PT_PHDR user address */
	unsigned long			map_addr;	/* mapped loadmap user address */
	unsigned long			entry_addr;	/* mapped entry user address */
	unsigned long			stack_size;	/* stack size requested (PT_GNU_STACK) */
	unsigned long			dynamic_addr;	/* mapped PT_DYNAMIC user address */
	unsigned long			load_addr;	/* user address at which to map binary */
	unsigned long			flags;
#define ELF_FDPIC_FLAG_ARRANGEMENT	0x0000000f	/* PT_LOAD arrangement flags */
#define ELF_FDPIC_FLAG_INDEPENDENT	0x00000000	/* PT_LOADs can be put anywhere */
#define ELF_FDPIC_FLAG_HONOURVADDR	0x00000001	/* PT_LOAD.vaddr must be honoured */
#define ELF_FDPIC_FLAG_CONSTDISP	0x00000002	/* PT_LOADs require constant
							 * displacement */
#define ELF_FDPIC_FLAG_CONTIGUOUS	0x00000003	/* PT_LOADs should be contiguous */
#define ELF_FDPIC_FLAG_EXEC_STACK	0x00000010	/* T if stack to be executable */
#define ELF_FDPIC_FLAG_NOEXEC_STACK	0x00000020	/* T if stack not to be executable */
#define ELF_FDPIC_FLAG_EXECUTABLE	0x00000040	/* T if this object is the executable */
#define ELF_FDPIC_FLAG_PRESENT		0x80000000	/* T if this object is present */
};

#ifdef __KERNEL__
#ifdef CONFIG_MMU
extern void elf_fdpic_arch_lay_out_mm(struct elf_fdpic_params *exec_params,
				      struct elf_fdpic_params *interp_params,
				      unsigned long *start_stack,
				      unsigned long *start_brk);
#endif
#endif /* __KERNEL__ */

#endif /* _LINUX_ELF_FDPIC_H */
