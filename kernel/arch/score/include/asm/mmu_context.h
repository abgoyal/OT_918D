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

#ifndef _ASM_SCORE_MMU_CONTEXT_H
#define _ASM_SCORE_MMU_CONTEXT_H

#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/slab.h>
#include <asm-generic/mm_hooks.h>

#include <asm/cacheflush.h>
#include <asm/tlbflush.h>
#include <asm/scoreregs.h>

/*
 * For the fast tlb miss handlers, we keep a per cpu array of pointers
 * to the current pgd for each processor. Also, the proc. id is stuffed
 * into the context register.
 */
extern unsigned long asid_cache;
extern unsigned long pgd_current;

#define TLBMISS_HANDLER_SETUP_PGD(pgd) (pgd_current = (unsigned long)(pgd))

#define TLBMISS_HANDLER_SETUP()				\
do {							\
	write_c0_context(0);				\
	TLBMISS_HANDLER_SETUP_PGD(swapper_pg_dir)	\
} while (0)

/*
 * All unused by hardware upper bits will be considered
 * as a software asid extension.
 */
#define ASID_VERSION_MASK	0xfffff000
#define ASID_FIRST_VERSION	0x1000

/* PEVN    --------- VPN ---------- --ASID--- -NA- */
/* binary: 0000 0000 0000 0000 0000 0000 0001 0000 */
/* binary: 0000 0000 0000 0000 0000 1111 1111 0000 */
#define ASID_INC	0x10
#define ASID_MASK	0xff0

static inline void enter_lazy_tlb(struct mm_struct *mm,
				struct task_struct *tsk)
{}

static inline void
get_new_mmu_context(struct mm_struct *mm)
{
	unsigned long asid = asid_cache + ASID_INC;

	if (!(asid & ASID_MASK)) {
		local_flush_tlb_all();		/* start new asid cycle */
		if (!asid)			/* fix version if needed */
			asid = ASID_FIRST_VERSION;
	}

	mm->context = asid;
	asid_cache = asid;
}

/*
 * Initialize the context related info for a new mm_struct
 * instance.
 */
static inline int
init_new_context(struct task_struct *tsk, struct mm_struct *mm)
{
	mm->context = 0;
	return 0;
}

static inline void switch_mm(struct mm_struct *prev, struct mm_struct *next,
			struct task_struct *tsk)
{
	unsigned long flags;

	local_irq_save(flags);
	if ((next->context ^ asid_cache) & ASID_VERSION_MASK)
		get_new_mmu_context(next);

	pevn_set(next->context);
	TLBMISS_HANDLER_SETUP_PGD(next->pgd);
	local_irq_restore(flags);
}

/*
 * Destroy context related info for an mm_struct that is about
 * to be put to rest.
 */
static inline void destroy_context(struct mm_struct *mm)
{}

static inline void
deactivate_mm(struct task_struct *task, struct mm_struct *mm)
{}

/*
 * After we have set current->mm to a new value, this activates
 * the context for the new mm so we see the new mappings.
 */
static inline void
activate_mm(struct mm_struct *prev, struct mm_struct *next)
{
	unsigned long flags;

	local_irq_save(flags);
	get_new_mmu_context(next);
	pevn_set(next->context);
	TLBMISS_HANDLER_SETUP_PGD(next->pgd);
	local_irq_restore(flags);
}

#endif /* _ASM_SCORE_MMU_CONTEXT_H */
