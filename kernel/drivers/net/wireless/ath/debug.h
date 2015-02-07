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
 * Copyright (c) 2008-2009 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef ATH_DEBUG_H
#define ATH_DEBUG_H

#include "ath.h"

/**
 * enum ath_debug_level - atheros wireless debug level
 *
 * @ATH_DBG_RESET: reset processing
 * @ATH_DBG_QUEUE: hardware queue management
 * @ATH_DBG_EEPROM: eeprom processing
 * @ATH_DBG_CALIBRATE: periodic calibration
 * @ATH_DBG_INTERRUPT: interrupt processing
 * @ATH_DBG_REGULATORY: regulatory processing
 * @ATH_DBG_ANI: adaptive noise immunitive processing
 * @ATH_DBG_XMIT: basic xmit operation
 * @ATH_DBG_BEACON: beacon handling
 * @ATH_DBG_CONFIG: configuration of the hardware
 * @ATH_DBG_FATAL: fatal errors, this is the default, DBG_DEFAULT
 * @ATH_DBG_PS: power save processing
 * @ATH_DBG_HWTIMER: hardware timer handling
 * @ATH_DBG_BTCOEX: bluetooth coexistance
 * @ATH_DBG_ANY: enable all debugging
 *
 * The debug level is used to control the amount and type of debugging output
 * we want to see. Each driver has its own method for enabling debugging and
 * modifying debug level states -- but this is typically done through a
 * module parameter 'debug' along with a respective 'debug' debugfs file
 * entry.
 */
enum ATH_DEBUG {
	ATH_DBG_RESET		= 0x00000001,
	ATH_DBG_QUEUE		= 0x00000002,
	ATH_DBG_EEPROM		= 0x00000004,
	ATH_DBG_CALIBRATE	= 0x00000008,
	ATH_DBG_INTERRUPT	= 0x00000010,
	ATH_DBG_REGULATORY	= 0x00000020,
	ATH_DBG_ANI		= 0x00000040,
	ATH_DBG_XMIT		= 0x00000080,
	ATH_DBG_BEACON		= 0x00000100,
	ATH_DBG_CONFIG		= 0x00000200,
	ATH_DBG_FATAL		= 0x00000400,
	ATH_DBG_PS		= 0x00000800,
	ATH_DBG_HWTIMER		= 0x00001000,
	ATH_DBG_BTCOEX		= 0x00002000,
	ATH_DBG_WMI		= 0x00004000,
	ATH_DBG_ANY		= 0xffffffff
};

#define ATH_DBG_DEFAULT (ATH_DBG_FATAL)

#ifdef CONFIG_ATH_DEBUG
void ath_print(struct ath_common *common, int dbg_mask, const char *fmt, ...)
	__attribute__ ((format (printf, 3, 4)));
#else
static inline void __attribute__ ((format (printf, 3, 4)))
ath_print(struct ath_common *common, int dbg_mask, const char *fmt, ...)
{
}
#endif /* CONFIG_ATH_DEBUG */

#endif /* ATH_DEBUG_H */
