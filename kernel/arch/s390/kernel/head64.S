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
 * arch/s390/kernel/head64.S
 *
 * Copyright (C) IBM Corp. 1999,2010
 *
 *   Author(s):	Hartmut Penner <hp@de.ibm.com>
 *		Martin Schwidefsky <schwidefsky@de.ibm.com>
 *		Rob van der Heij <rvdhei@iae.nl>
 *		Heiko Carstens <heiko.carstens@de.ibm.com>
 *
 */

#include <linux/init.h>
#include <asm/asm-offsets.h>
#include <asm/thread_info.h>
#include <asm/page.h>

__HEAD
	.globl	startup_continue
startup_continue:
	larl	%r1,sched_clock_base_cc
	mvc	0(8,%r1),__LC_LAST_UPDATE_CLOCK
	larl	%r13,.LPG1		# get base
	lctlg	%c0,%c15,.Lctl-.LPG1(%r13)	# load control registers
	lg	%r12,.Lparmaddr-.LPG1(%r13)	# pointer to parameter area
					# move IPL device to lowcore
	lghi	%r0,__LC_PASTE
	stg	%r0,__LC_VDSO_PER_CPU
#
# Setup stack
#
	larl	%r15,init_thread_union
	stg	%r15,__LC_THREAD_INFO	# cache thread info in lowcore
	lg	%r14,__TI_task(%r15)	# cache current in lowcore
	stg	%r14,__LC_CURRENT
	aghi	%r15,1<<(PAGE_SHIFT+THREAD_ORDER) # init_task_union + THREAD_SIZE
	stg	%r15,__LC_KERNEL_STACK	# set end of kernel stack
	aghi	%r15,-160
#
# Save ipl parameters, clear bss memory, initialize storage key for kernel pages,
# and create a kernel NSS if the SAVESYS= parm is defined
#
	brasl	%r14,startup_init
	lpswe	.Lentry-.LPG1(13)	# jump to _stext in primary-space,
					# virtual and never return ...
	.align	16
.LPG1:
.Lentry:.quad	0x0000000180000000,_stext
.Lctl:	.quad	0x04350002		# cr0: various things
	.quad	0			# cr1: primary space segment table
	.quad	.Lduct			# cr2: dispatchable unit control table
	.quad	0			# cr3: instruction authorization
	.quad	0			# cr4: instruction authorization
	.quad	.Lduct			# cr5: primary-aste origin
	.quad	0			# cr6:	I/O interrupts
	.quad	0			# cr7:	secondary space segment table
	.quad	0			# cr8:	access registers translation
	.quad	0			# cr9:	tracing off
	.quad	0			# cr10: tracing off
	.quad	0			# cr11: tracing off
	.quad	0			# cr12: tracing off
	.quad	0			# cr13: home space segment table
	.quad	0xc0000000		# cr14: machine check handling off
	.quad	0			# cr15: linkage stack operations
.Lpcmsk:.quad	0x0000000180000000
.L4malign:.quad 0xffffffffffc00000
.Lscan2g:.quad	0x80000000 + 0x20000 - 8	# 2GB + 128K - 8
.Lnop:	.long	0x07000700
.Lparmaddr:
	.quad	PARMAREA
	.align	64
.Lduct: .long	0,0,0,0,.Lduald,0,0,0
	.long	0,0,0,0,0,0,0,0
	.align	128
.Lduald:.rept	8
	.long	0x80000000,0,0,0	# invalid access-list entries
	.endr

	.globl	_ehead
_ehead:

#ifdef CONFIG_SHARED_KERNEL
	.org	0x100000 - 0x11000	# head.o ends at 0x11000
#endif

#
# startup-code, running in absolute addressing mode
#
	.globl	_stext
_stext:	basr	%r13,0			# get base
.LPG3:
# check control registers
	stctg	%c0,%c15,0(%r15)
	oi	6(%r15),0x40		# enable sigp emergency signal
	oi	4(%r15),0x10		# switch on low address proctection
	lctlg	%c0,%c15,0(%r15)

	lam	0,15,.Laregs-.LPG3(%r13)	# load acrs needed by uaccess
	brasl	%r14,start_kernel	# go to C code
#
# We returned from start_kernel ?!? PANIK
#
	basr	%r13,0
	lpswe	.Ldw-.(%r13)		# load disabled wait psw

	.align	8
.Ldw:	.quad	0x0002000180000000,0x0000000000000000
.Laregs:.long	0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
