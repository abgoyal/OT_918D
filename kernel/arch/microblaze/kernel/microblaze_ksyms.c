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
 * Copyright (C) 2008-2009 Michal Simek <monstr@monstr.eu>
 * Copyright (C) 2008-2009 PetaLogix
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/string.h>
#include <linux/cryptohash.h>
#include <linux/delay.h>
#include <linux/in6.h>
#include <linux/syscalls.h>

#include <asm/checksum.h>
#include <linux/io.h>
#include <asm/page.h>
#include <asm/system.h>
#include <linux/ftrace.h>
#include <linux/uaccess.h>

/*
 * libgcc functions - functions that are used internally by the
 * compiler... (prototypes are not correct though, but that
 * doesn't really matter since they're not versioned).
 */
extern void __ashldi3(void);
EXPORT_SYMBOL(__ashldi3);
extern void __ashrdi3(void);
EXPORT_SYMBOL(__ashrdi3);
extern void __divsi3(void);
EXPORT_SYMBOL(__divsi3);
extern void __lshrdi3(void);
EXPORT_SYMBOL(__lshrdi3);
extern void __modsi3(void);
EXPORT_SYMBOL(__modsi3);
extern void __mulsi3(void);
EXPORT_SYMBOL(__mulsi3);
extern void __muldi3(void);
EXPORT_SYMBOL(__muldi3);
extern void __ucmpdi2(void);
EXPORT_SYMBOL(__ucmpdi2);
extern void __udivsi3(void);
EXPORT_SYMBOL(__udivsi3);
extern void __umodsi3(void);
EXPORT_SYMBOL(__umodsi3);
extern char *_ebss;
EXPORT_SYMBOL_GPL(_ebss);
#ifdef CONFIG_FUNCTION_TRACER
extern void _mcount(void);
EXPORT_SYMBOL(_mcount);
#endif

/*
 * Assembly functions that may be used (directly or indirectly) by modules
 */
EXPORT_SYMBOL(__copy_tofrom_user);
EXPORT_SYMBOL(__strncpy_user);

#ifdef CONFIG_OPT_LIB_ASM
EXPORT_SYMBOL(memcpy);
EXPORT_SYMBOL(memmove);
#endif
