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

#ifndef __ASM_SH_ILSEL_H
#define __ASM_SH_ILSEL_H

typedef enum {
	ILSEL_NONE,
	ILSEL_LAN,
	ILSEL_USBH_I,
	ILSEL_USBH_S,
	ILSEL_USBH_V,
	ILSEL_RTC,
	ILSEL_USBP_I,
	ILSEL_USBP_S,
	ILSEL_USBP_V,
	ILSEL_KEY,

	/*
	 * ILSEL Aliases - corner cases for interleaved level tables.
	 *
	 * Someone thought this was a good idea and less hassle than
	 * demuxing a shared vector, really.
	 */

	/* ILSEL0 and 2 */
	ILSEL_FPGA0,
	ILSEL_FPGA1,
	ILSEL_EX1,
	ILSEL_EX2,
	ILSEL_EX3,
	ILSEL_EX4,

	/* ILSEL1 and 3 */
	ILSEL_FPGA2 = ILSEL_FPGA0,
	ILSEL_FPGA3 = ILSEL_FPGA1,
	ILSEL_EX5 = ILSEL_EX1,
	ILSEL_EX6 = ILSEL_EX2,
	ILSEL_EX7 = ILSEL_EX3,
	ILSEL_EX8 = ILSEL_EX4,
} ilsel_source_t;

/* arch/sh/boards/renesas/x3proto/ilsel.c */
int ilsel_enable(ilsel_source_t set);
int ilsel_enable_fixed(ilsel_source_t set, unsigned int level);
void ilsel_disable(unsigned int irq);

#endif /* __ASM_SH_ILSEL_H */
