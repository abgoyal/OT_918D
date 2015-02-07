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

#ifndef _ASM_POWERPC_PTE_40x_H
#define _ASM_POWERPC_PTE_40x_H
#ifdef __KERNEL__

/*
 * At present, all PowerPC 400-class processors share a similar TLB
 * architecture. The instruction and data sides share a unified,
 * 64-entry, fully-associative TLB which is maintained totally under
 * software control. In addition, the instruction side has a
 * hardware-managed, 4-entry, fully-associative TLB which serves as a
 * first level to the shared TLB. These two TLBs are known as the UTLB
 * and ITLB, respectively (see "mmu.h" for definitions).
 *
 * There are several potential gotchas here.  The 40x hardware TLBLO
 * field looks like this:
 *
 * 0  1  2  3  4  ... 18 19 20 21 22 23 24 25 26 27 28 29 30 31
 * RPN.....................  0  0 EX WR ZSEL.......  W  I  M  G
 *
 * Where possible we make the Linux PTE bits match up with this
 *
 * - bits 20 and 21 must be cleared, because we use 4k pages (40x can
 *   support down to 1k pages), this is done in the TLBMiss exception
 *   handler.
 * - We use only zones 0 (for kernel pages) and 1 (for user pages)
 *   of the 16 available.  Bit 24-26 of the TLB are cleared in the TLB
 *   miss handler.  Bit 27 is PAGE_USER, thus selecting the correct
 *   zone.
 * - PRESENT *must* be in the bottom two bits because swap cache
 *   entries use the top 30 bits.  Because 40x doesn't support SMP
 *   anyway, M is irrelevant so we borrow it for PAGE_PRESENT.  Bit 30
 *   is cleared in the TLB miss handler before the TLB entry is loaded.
 * - All other bits of the PTE are loaded into TLBLO without
 *   modification, leaving us only the bits 20, 21, 24, 25, 26, 30 for
 *   software PTE bits.  We actually use use bits 21, 24, 25, and
 *   30 respectively for the software bits: ACCESSED, DIRTY, RW, and
 *   PRESENT.
 */

#define	_PAGE_GUARDED	0x001	/* G: page is guarded from prefetch */
#define _PAGE_FILE	0x001	/* when !present: nonlinear file mapping */
#define _PAGE_PRESENT	0x002	/* software: PTE contains a translation */
#define	_PAGE_NO_CACHE	0x004	/* I: caching is inhibited */
#define	_PAGE_WRITETHRU	0x008	/* W: caching is write-through */
#define	_PAGE_USER	0x010	/* matches one of the zone permission bits */
#define	_PAGE_SPECIAL	0x020	/* software: Special page */
#define	_PAGE_RW	0x040	/* software: Writes permitted */
#define	_PAGE_DIRTY	0x080	/* software: dirty page */
#define _PAGE_HWWRITE	0x100	/* hardware: Dirty & RW, set in exception */
#define _PAGE_EXEC	0x200	/* hardware: EX permission */
#define _PAGE_ACCESSED	0x400	/* software: R: page referenced */

#define _PMD_PRESENT	0x400	/* PMD points to page of PTEs */
#define _PMD_BAD	0x802
#define _PMD_SIZE	0x0e0	/* size field, != 0 for large-page PMD entry */
#define _PMD_SIZE_4M	0x0c0
#define _PMD_SIZE_16M	0x0e0

#define PMD_PAGE_SIZE(pmdval)	(1024 << (((pmdval) & _PMD_SIZE) >> 4))

/* Until my rework is finished, 40x still needs atomic PTE updates */
#define PTE_ATOMIC_UPDATES	1

#endif /* __KERNEL__ */
#endif /*  _ASM_POWERPC_PTE_40x_H */
