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

#ifndef _M68K_PTRACE_H
#define _M68K_PTRACE_H

#define PT_D1	   0
#define PT_D2	   1
#define PT_D3	   2
#define PT_D4	   3
#define PT_D5	   4
#define PT_D6	   5
#define PT_D7	   6
#define PT_A0	   7
#define PT_A1	   8
#define PT_A2	   9
#define PT_A3	   10
#define PT_A4	   11
#define PT_A5	   12
#define PT_A6	   13
#define PT_D0	   14
#define PT_USP	   15
#define PT_ORIG_D0 16
#define PT_SR	   17
#define PT_PC	   18

#ifndef __ASSEMBLY__

/* this struct defines the way the registers are stored on the
   stack during a system call. */

struct pt_regs {
  long     d1;
  long     d2;
  long     d3;
  long     d4;
  long     d5;
  long     a0;
  long     a1;
  long     a2;
  long     d0;
  long     orig_d0;
  long     stkadj;
#ifdef CONFIG_COLDFIRE
  unsigned format :  4; /* frame format specifier */
  unsigned vector : 12; /* vector offset */
  unsigned short sr;
  unsigned long  pc;
#else
  unsigned short sr;
  unsigned long  pc;
  unsigned format :  4; /* frame format specifier */
  unsigned vector : 12; /* vector offset */
#endif
};

/*
 * This is the extended stack used by signal handlers and the context
 * switcher: it's pushed after the normal "struct pt_regs".
 */
struct switch_stack {
	unsigned long  d6;
	unsigned long  d7;
	unsigned long  a3;
	unsigned long  a4;
	unsigned long  a5;
	unsigned long  a6;
	unsigned long  retpc;
};

/* Arbitrarily choose the same ptrace numbers as used by the Sparc code. */
#define PTRACE_GETREGS            12
#define PTRACE_SETREGS            13
#define PTRACE_GETFPREGS          14
#define PTRACE_SETFPREGS          15

#define PTRACE_GET_THREAD_AREA    25

#define PTRACE_SINGLEBLOCK	33	/* resume execution until next branch */

#ifdef __KERNEL__

#ifndef PS_S
#define PS_S  (0x2000)
#define PS_M  (0x1000)
#endif

#define user_mode(regs) (!((regs)->sr & PS_S))
#define instruction_pointer(regs) ((regs)->pc)
#define profile_pc(regs) instruction_pointer(regs)
extern void show_regs(struct pt_regs *);

#define arch_has_single_step()	(1)

#ifdef CONFIG_MMU
#define arch_has_block_step()	(1)
#endif

#endif /* __KERNEL__ */
#endif /* __ASSEMBLY__ */
#endif /* _M68K_PTRACE_H */
