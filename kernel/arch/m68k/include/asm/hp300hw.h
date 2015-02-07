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

#ifndef _M68K_HP300HW_H
#define _M68K_HP300HW_H

extern unsigned long hp300_model;

/* This information was taken from NetBSD */
#define	HP_320		(0)	/* 16MHz 68020+HP MMU+16K external cache */
#define	HP_330		(1)	/* 16MHz 68020+68851 MMU */
#define	HP_340		(2)	/* 16MHz 68030 */
#define	HP_345		(3)	/* 50MHz 68030+32K external cache */
#define	HP_350		(4)	/* 25MHz 68020+HP MMU+32K external cache */
#define	HP_360		(5)	/* 25MHz 68030 */
#define	HP_370		(6)	/* 33MHz 68030+64K external cache */
#define	HP_375		(7)	/* 50MHz 68030+32K external cache */
#define	HP_380		(8)	/* 25MHz 68040 */
#define	HP_385		(9)	/* 33MHz 68040 */

#define	HP_400		(10)	/* 50MHz 68030+32K external cache */
#define	HP_425T		(11)	/* 25MHz 68040 - model 425t */
#define	HP_425S		(12)	/* 25MHz 68040 - model 425s */
#define HP_425E		(13)	/* 25MHz 68040 - model 425e */
#define HP_433T		(14)	/* 33MHz 68040 - model 433t */
#define HP_433S		(15)	/* 33MHz 68040 - model 433s */

#endif /* _M68K_HP300HW_H */
