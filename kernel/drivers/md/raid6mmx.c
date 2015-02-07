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

/* -*- linux-c -*- ------------------------------------------------------- *
 *
 *   Copyright 2002 H. Peter Anvin - All Rights Reserved
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, Inc., 53 Temple Place Ste 330,
 *   Boston MA 02111-1307, USA; either version 2 of the License, or
 *   (at your option) any later version; incorporated herein by reference.
 *
 * ----------------------------------------------------------------------- */

/*
 * raid6mmx.c
 *
 * MMX implementation of RAID-6 syndrome functions
 */

#if defined(__i386__) && !defined(__arch_um__)

#include <linux/raid/pq.h>
#include "raid6x86.h"

/* Shared with raid6sse1.c */
const struct raid6_mmx_constants {
	u64 x1d;
} raid6_mmx_constants = {
	0x1d1d1d1d1d1d1d1dULL,
};

static int raid6_have_mmx(void)
{
	/* Not really "boot_cpu" but "all_cpus" */
	return boot_cpu_has(X86_FEATURE_MMX);
}

/*
 * Plain MMX implementation
 */
static void raid6_mmx1_gen_syndrome(int disks, size_t bytes, void **ptrs)
{
	u8 **dptr = (u8 **)ptrs;
	u8 *p, *q;
	int d, z, z0;

	z0 = disks - 3;		/* Highest data disk */
	p = dptr[z0+1];		/* XOR parity */
	q = dptr[z0+2];		/* RS syndrome */

	kernel_fpu_begin();

	asm volatile("movq %0,%%mm0" : : "m" (raid6_mmx_constants.x1d));
	asm volatile("pxor %mm5,%mm5");	/* Zero temp */

	for ( d = 0 ; d < bytes ; d += 8 ) {
		asm volatile("movq %0,%%mm2" : : "m" (dptr[z0][d])); /* P[0] */
		asm volatile("movq %mm2,%mm4");	/* Q[0] */
		for ( z = z0-1 ; z >= 0 ; z-- ) {
			asm volatile("movq %0,%%mm6" : : "m" (dptr[z][d]));
			asm volatile("pcmpgtb %mm4,%mm5");
			asm volatile("paddb %mm4,%mm4");
			asm volatile("pand %mm0,%mm5");
			asm volatile("pxor %mm5,%mm4");
			asm volatile("pxor %mm5,%mm5");
			asm volatile("pxor %mm6,%mm2");
			asm volatile("pxor %mm6,%mm4");
		}
		asm volatile("movq %%mm2,%0" : "=m" (p[d]));
		asm volatile("pxor %mm2,%mm2");
		asm volatile("movq %%mm4,%0" : "=m" (q[d]));
		asm volatile("pxor %mm4,%mm4");
	}

	kernel_fpu_end();
}

const struct raid6_calls raid6_mmxx1 = {
	raid6_mmx1_gen_syndrome,
	raid6_have_mmx,
	"mmxx1",
	0
};

/*
 * Unrolled-by-2 MMX implementation
 */
static void raid6_mmx2_gen_syndrome(int disks, size_t bytes, void **ptrs)
{
	u8 **dptr = (u8 **)ptrs;
	u8 *p, *q;
	int d, z, z0;

	z0 = disks - 3;		/* Highest data disk */
	p = dptr[z0+1];		/* XOR parity */
	q = dptr[z0+2];		/* RS syndrome */

	kernel_fpu_begin();

	asm volatile("movq %0,%%mm0" : : "m" (raid6_mmx_constants.x1d));
	asm volatile("pxor %mm5,%mm5");	/* Zero temp */
	asm volatile("pxor %mm7,%mm7"); /* Zero temp */

	for ( d = 0 ; d < bytes ; d += 16 ) {
		asm volatile("movq %0,%%mm2" : : "m" (dptr[z0][d])); /* P[0] */
		asm volatile("movq %0,%%mm3" : : "m" (dptr[z0][d+8]));
		asm volatile("movq %mm2,%mm4"); /* Q[0] */
		asm volatile("movq %mm3,%mm6"); /* Q[1] */
		for ( z = z0-1 ; z >= 0 ; z-- ) {
			asm volatile("pcmpgtb %mm4,%mm5");
			asm volatile("pcmpgtb %mm6,%mm7");
			asm volatile("paddb %mm4,%mm4");
			asm volatile("paddb %mm6,%mm6");
			asm volatile("pand %mm0,%mm5");
			asm volatile("pand %mm0,%mm7");
			asm volatile("pxor %mm5,%mm4");
			asm volatile("pxor %mm7,%mm6");
			asm volatile("movq %0,%%mm5" : : "m" (dptr[z][d]));
			asm volatile("movq %0,%%mm7" : : "m" (dptr[z][d+8]));
			asm volatile("pxor %mm5,%mm2");
			asm volatile("pxor %mm7,%mm3");
			asm volatile("pxor %mm5,%mm4");
			asm volatile("pxor %mm7,%mm6");
			asm volatile("pxor %mm5,%mm5");
			asm volatile("pxor %mm7,%mm7");
		}
		asm volatile("movq %%mm2,%0" : "=m" (p[d]));
		asm volatile("movq %%mm3,%0" : "=m" (p[d+8]));
		asm volatile("movq %%mm4,%0" : "=m" (q[d]));
		asm volatile("movq %%mm6,%0" : "=m" (q[d+8]));
	}

	kernel_fpu_end();
}

const struct raid6_calls raid6_mmxx2 = {
	raid6_mmx2_gen_syndrome,
	raid6_have_mmx,
	"mmxx2",
	0
};

#endif
