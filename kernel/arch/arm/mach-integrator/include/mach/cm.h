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
 * update the core module control register.
 */
void cm_control(u32, u32);

#define CM_CTRL_LED			(1 << 0)
#define CM_CTRL_nMBDET			(1 << 1)
#define CM_CTRL_REMAP			(1 << 2)
#define CM_CTRL_RESET			(1 << 3)

/*
 * Integrator/AP,PP2 specific
 */
#define CM_CTRL_HIGHVECTORS		(1 << 4)
#define CM_CTRL_BIGENDIAN		(1 << 5)
#define CM_CTRL_FASTBUS			(1 << 6)
#define CM_CTRL_SYNC			(1 << 7)

/*
 * ARM926/946/966 Integrator/CP specific
 */
#define CM_CTRL_LCDBIASEN		(1 << 8)
#define CM_CTRL_LCDBIASUP		(1 << 9)
#define CM_CTRL_LCDBIASDN		(1 << 10)
#define CM_CTRL_LCDMUXSEL_MASK		(7 << 11)
#define CM_CTRL_LCDMUXSEL_GENLCD	(1 << 11)
#define CM_CTRL_LCDMUXSEL_VGA_16BPP	(2 << 11)
#define CM_CTRL_LCDMUXSEL_SHARPLCD	(3 << 11)
#define CM_CTRL_LCDMUXSEL_VGA_8421BPP	(4 << 11)
#define CM_CTRL_LCDEN0			(1 << 14)
#define CM_CTRL_LCDEN1			(1 << 15)
#define CM_CTRL_STATIC1			(1 << 16)
#define CM_CTRL_STATIC2			(1 << 17)
#define CM_CTRL_STATIC			(1 << 18)
#define CM_CTRL_n24BITEN		(1 << 19)
#define CM_CTRL_EBIWP			(1 << 20)
