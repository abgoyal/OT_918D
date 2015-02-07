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

#ifndef _ASM_IA64_SIGCONTEXT_H
#define _ASM_IA64_SIGCONTEXT_H

/*
 * Copyright (C) 1998, 1999, 2001 Hewlett-Packard Co
 * Copyright (C) 1998, 1999, 2001 David Mosberger-Tang <davidm@hpl.hp.com>
 */

#include <asm/fpu.h>

#define IA64_SC_FLAG_ONSTACK_BIT		0	/* is handler running on signal stack? */
#define IA64_SC_FLAG_IN_SYSCALL_BIT		1	/* did signal interrupt a syscall? */
#define IA64_SC_FLAG_FPH_VALID_BIT		2	/* is state in f[32]-f[127] valid? */

#define IA64_SC_FLAG_ONSTACK		(1 << IA64_SC_FLAG_ONSTACK_BIT)
#define IA64_SC_FLAG_IN_SYSCALL		(1 << IA64_SC_FLAG_IN_SYSCALL_BIT)
#define IA64_SC_FLAG_FPH_VALID		(1 << IA64_SC_FLAG_FPH_VALID_BIT)

# ifndef __ASSEMBLY__

/*
 * Note on handling of register backing store: sc_ar_bsp contains the address that would
 * be found in ar.bsp after executing a "cover" instruction the context in which the
 * signal was raised.  If signal delivery required switching to an alternate signal stack
 * (sc_rbs_base is not NULL), the "dirty" partition (as it would exist after executing the
 * imaginary "cover" instruction) is backed by the *alternate* signal stack, not the
 * original one.  In this case, sc_rbs_base contains the base address of the new register
 * backing store.  The number of registers in the dirty partition can be calculated as:
 *
 *   ndirty = ia64_rse_num_regs(sc_rbs_base, sc_rbs_base + (sc_loadrs >> 16))
 *
 */

struct sigcontext {
	unsigned long		sc_flags;	/* see manifest constants above */
	unsigned long		sc_nat;		/* bit i == 1 iff scratch reg gr[i] is a NaT */
	stack_t			sc_stack;	/* previously active stack */

	unsigned long		sc_ip;		/* instruction pointer */
	unsigned long		sc_cfm;		/* current frame marker */
	unsigned long		sc_um;		/* user mask bits */
	unsigned long		sc_ar_rsc;	/* register stack configuration register */
	unsigned long		sc_ar_bsp;	/* backing store pointer */
	unsigned long		sc_ar_rnat;	/* RSE NaT collection register */
	unsigned long		sc_ar_ccv;	/* compare and exchange compare value register */
	unsigned long		sc_ar_unat;	/* ar.unat of interrupted context */
	unsigned long		sc_ar_fpsr;	/* floating-point status register */
	unsigned long		sc_ar_pfs;	/* previous function state */
	unsigned long		sc_ar_lc;	/* loop count register */
	unsigned long		sc_pr;		/* predicate registers */
	unsigned long		sc_br[8];	/* branch registers */
	/* Note: sc_gr[0] is used as the "uc_link" member of ucontext_t */
	unsigned long		sc_gr[32];	/* general registers (static partition) */
	struct ia64_fpreg	sc_fr[128];	/* floating-point registers */

	unsigned long		sc_rbs_base;	/* NULL or new base of sighandler's rbs */
	unsigned long		sc_loadrs;	/* see description above */

	unsigned long		sc_ar25;	/* cmp8xchg16 uses this */
	unsigned long		sc_ar26;	/* rsvd for scratch use */
	unsigned long		sc_rsvd[12];	/* reserved for future use */
	/*
	 * The mask must come last so we can increase _NSIG_WORDS
	 * without breaking binary compatibility.
	 */
	sigset_t		sc_mask;	/* signal mask to restore after handler returns */
};

# endif /* __ASSEMBLY__ */
#endif /* _ASM_IA64_SIGCONTEXT_H */
