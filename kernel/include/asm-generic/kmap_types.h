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

#ifndef _ASM_GENERIC_KMAP_TYPES_H
#define _ASM_GENERIC_KMAP_TYPES_H

#ifdef __WITH_KM_FENCE
# define KMAP_D(n) __KM_FENCE_##n ,
#else
# define KMAP_D(n)
#endif

enum km_type {
KMAP_D(0)	KM_BOUNCE_READ,
KMAP_D(1)	KM_SKB_SUNRPC_DATA,
KMAP_D(2)	KM_SKB_DATA_SOFTIRQ,
KMAP_D(3)	KM_USER0,
KMAP_D(4)	KM_USER1,
KMAP_D(5)	KM_BIO_SRC_IRQ,
KMAP_D(6)	KM_BIO_DST_IRQ,
KMAP_D(7)	KM_PTE0,
KMAP_D(8)	KM_PTE1,
KMAP_D(9)	KM_IRQ0,
KMAP_D(10)	KM_IRQ1,
KMAP_D(11)	KM_SOFTIRQ0,
KMAP_D(12)	KM_SOFTIRQ1,
KMAP_D(13)	KM_SYNC_ICACHE,
KMAP_D(14)	KM_SYNC_DCACHE,
/* UML specific, for copy_*_user - used in do_op_one_page */
KMAP_D(15)	KM_UML_USERCOPY,
KMAP_D(16)	KM_IRQ_PTE,
KMAP_D(17)	KM_NMI,
KMAP_D(18)	KM_NMI_PTE,
KMAP_D(19)	KM_KDB,
/*
 * Remember to update debug_kmap_atomic() when adding new kmap types!
 */
KMAP_D(20)	KM_TYPE_NR
};

#undef KMAP_D

#endif
