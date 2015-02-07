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
 * Magic syscall break down functions
 *
 * Copyright 2010 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __ASM_BLACKFIN_SYSCALL_H__
#define __ASM_BLACKFIN_SYSCALL_H__

/*
 * Blackfin syscalls are simple:
 *	enter:
 *		p0: syscall number
 *		r{0,1,2,3,4,5}: syscall args 0,1,2,3,4,5
 *	exit:
 *		r0: return/error value
 */

#include <linux/err.h>
#include <linux/sched.h>
#include <asm/ptrace.h>

static inline long
syscall_get_nr(struct task_struct *task, struct pt_regs *regs)
{
	return regs->p0;
}

static inline void
syscall_rollback(struct task_struct *task, struct pt_regs *regs)
{
	regs->p0 = regs->orig_p0;
}

static inline long
syscall_get_error(struct task_struct *task, struct pt_regs *regs)
{
	return IS_ERR_VALUE(regs->r0) ? regs->r0 : 0;
}

static inline long
syscall_get_return_value(struct task_struct *task, struct pt_regs *regs)
{
	return regs->r0;
}

static inline void
syscall_set_return_value(struct task_struct *task, struct pt_regs *regs,
                         int error, long val)
{
	regs->r0 = error ? -error : val;
}

/**
 *	syscall_get_arguments()
 *	@task:   unused
 *	@regs:   the register layout to extract syscall arguments from
 *	@i:      first syscall argument to extract
 *	@n:      number of syscall arguments to extract
 *	@args:   array to return the syscall arguments in
 *
 * args[0] gets i'th argument, args[n - 1] gets the i+n-1'th argument
 */
static inline void
syscall_get_arguments(struct task_struct *task, struct pt_regs *regs,
                      unsigned int i, unsigned int n, unsigned long *args)
{
	/*
	 * Assume the ptrace layout doesn't change -- r5 is first in memory,
	 * then r4, ..., then r0.  So we simply reverse the ptrace register
	 * array in memory to store into the args array.
	 */
	long *aregs = &regs->r0 - i;

	BUG_ON(i > 5 || i + n > 6);

	while (n--)
		*args++ = *aregs--;
}

/* See syscall_get_arguments() comments */
static inline void
syscall_set_arguments(struct task_struct *task, struct pt_regs *regs,
                      unsigned int i, unsigned int n, const unsigned long *args)
{
	long *aregs = &regs->r0 - i;

	BUG_ON(i > 5 || i + n > 6);

	while (n--)
		*aregs-- = *args++;
}

#endif