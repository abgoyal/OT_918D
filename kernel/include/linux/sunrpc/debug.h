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
 * linux/include/linux/sunrpc/debug.h
 *
 * Debugging support for sunrpc module
 *
 * Copyright (C) 1996, Olaf Kirch <okir@monad.swb.de>
 */

#ifndef _LINUX_SUNRPC_DEBUG_H_
#define _LINUX_SUNRPC_DEBUG_H_

/*
 * RPC debug facilities
 */
#define RPCDBG_XPRT		0x0001
#define RPCDBG_CALL		0x0002
#define RPCDBG_DEBUG		0x0004
#define RPCDBG_NFS		0x0008
#define RPCDBG_AUTH		0x0010
#define RPCDBG_BIND		0x0020
#define RPCDBG_SCHED		0x0040
#define RPCDBG_TRANS		0x0080
#define RPCDBG_SVCXPRT		0x0100
#define RPCDBG_SVCDSP		0x0200
#define RPCDBG_MISC		0x0400
#define RPCDBG_CACHE		0x0800
#define RPCDBG_ALL		0x7fff

#ifdef __KERNEL__

/*
 * Enable RPC debugging/profiling.
 */
#ifdef CONFIG_SYSCTL
#define  RPC_DEBUG
#endif
/* #define  RPC_PROFILE */

/*
 * Debugging macros etc
 */
#ifdef RPC_DEBUG
extern unsigned int		rpc_debug;
extern unsigned int		nfs_debug;
extern unsigned int		nfsd_debug;
extern unsigned int		nlm_debug;
#endif

#define dprintk(args...)	dfprintk(FACILITY, ## args)

#undef ifdebug
#ifdef RPC_DEBUG			
# define ifdebug(fac)		if (unlikely(rpc_debug & RPCDBG_##fac))
# define dfprintk(fac, args...)	do { ifdebug(fac) printk(args); } while(0)
# define RPC_IFDEBUG(x)		x
#else
# define ifdebug(fac)		if (0)
# define dfprintk(fac, args...)	do ; while (0)
# define RPC_IFDEBUG(x)
#endif

/*
 * Sysctl interface for RPC debugging
 */
#ifdef RPC_DEBUG
void		rpc_register_sysctl(void);
void		rpc_unregister_sysctl(void);
#endif

#endif /* __KERNEL__ */

/*
 * Declarations for the sysctl debug interface, which allows to read or
 * change the debug flags for rpc, nfs, nfsd, and lockd. Since the sunrpc
 * module currently registers its sysctl table dynamically, the sysctl path
 * for module FOO is <CTL_SUNRPC, CTL_FOODEBUG>.
 */

enum {
	CTL_RPCDEBUG = 1,
	CTL_NFSDEBUG,
	CTL_NFSDDEBUG,
	CTL_NLMDEBUG,
	CTL_SLOTTABLE_UDP,
	CTL_SLOTTABLE_TCP,
	CTL_MIN_RESVPORT,
	CTL_MAX_RESVPORT,
};

#endif /* _LINUX_SUNRPC_DEBUG_H_ */