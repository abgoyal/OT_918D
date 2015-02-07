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
 * Register definitions for Atmel AC97C
 *
 * Copyright (C) 2005-2009 Atmel Corporation
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */
#ifndef __SOUND_ATMEL_AC97C_H
#define __SOUND_ATMEL_AC97C_H

#define AC97C_MR		0x08
#define AC97C_ICA		0x10
#define AC97C_OCA		0x14
#define AC97C_CARHR		0x20
#define AC97C_CATHR		0x24
#define AC97C_CASR		0x28
#define AC97C_CAMR		0x2c
#define AC97C_CORHR		0x40
#define AC97C_COTHR		0x44
#define AC97C_COSR		0x48
#define AC97C_COMR		0x4c
#define AC97C_SR		0x50
#define AC97C_IER		0x54
#define AC97C_IDR		0x58
#define AC97C_IMR		0x5c
#define AC97C_VERSION		0xfc

#define AC97C_CATPR		PDC_TPR
#define AC97C_CATCR		PDC_TCR
#define AC97C_CATNPR		PDC_TNPR
#define AC97C_CATNCR		PDC_TNCR
#define AC97C_CARPR		PDC_RPR
#define AC97C_CARCR		PDC_RCR
#define AC97C_CARNPR		PDC_RNPR
#define AC97C_CARNCR		PDC_RNCR
#define AC97C_PTCR		PDC_PTCR

#define AC97C_MR_ENA		(1 << 0)
#define AC97C_MR_WRST		(1 << 1)
#define AC97C_MR_VRA		(1 << 2)

#define AC97C_CSR_TXRDY		(1 << 0)
#define AC97C_CSR_TXEMPTY	(1 << 1)
#define AC97C_CSR_UNRUN		(1 << 2)
#define AC97C_CSR_RXRDY		(1 << 4)
#define AC97C_CSR_OVRUN		(1 << 5)
#define AC97C_CSR_ENDTX		(1 << 10)
#define AC97C_CSR_ENDRX		(1 << 14)

#define AC97C_CMR_SIZE_20	(0 << 16)
#define AC97C_CMR_SIZE_18	(1 << 16)
#define AC97C_CMR_SIZE_16	(2 << 16)
#define AC97C_CMR_SIZE_10	(3 << 16)
#define AC97C_CMR_CEM_LITTLE	(1 << 18)
#define AC97C_CMR_CEM_BIG	(0 << 18)
#define AC97C_CMR_CENA		(1 << 21)
#define AC97C_CMR_DMAEN		(1 << 22)

#define AC97C_SR_CAEVT		(1 << 3)
#define AC97C_SR_COEVT		(1 << 2)
#define AC97C_SR_WKUP		(1 << 1)
#define AC97C_SR_SOF		(1 << 0)

#define AC97C_CH_MASK(slot)						\
	(0x7 << (3 * (AC97_SLOT_##slot - 3)))
#define AC97C_CH_ASSIGN(slot, channel)					\
	(AC97C_CHANNEL_##channel << (3 * (AC97_SLOT_##slot - 3)))
#define AC97C_CHANNEL_NONE	0x0
#define AC97C_CHANNEL_A		0x1

#endif /* __SOUND_ATMEL_AC97C_H */