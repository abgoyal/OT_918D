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

#ifndef CM9780_H_INCLUDED
#define CM9780_H_INCLUDED

#define CM9780_JACK		0x62
#define CM9780_MIXER		0x64
#define CM9780_GPIO_SETUP	0x70
#define CM9780_GPIO_STATUS	0x72

/* jack control */
#define CM9780_RSOE		0x0001
#define CM9780_CBOE		0x0002
#define CM9780_SSOE		0x0004
#define CM9780_FROE		0x0008
#define CM9780_HP2FMICOE	0x0010
#define CM9780_CB2MICOE		0x0020
#define CM9780_FMIC2LI		0x0040
#define CM9780_FMIC2MIC		0x0080
#define CM9780_HP2LI		0x0100
#define CM9780_HP2MIC		0x0200
#define CM9780_MIC2LI		0x0400
#define CM9780_MIC2MIC		0x0800
#define CM9780_LI2LI		0x1000
#define CM9780_LI2MIC		0x2000
#define CM9780_LO2LI		0x4000
#define CM9780_LO2MIC		0x8000

/* mixer control */
#define CM9780_BSTSEL		0x0001
#define CM9780_STRO_MIC		0x0002
#define CM9780_SPDI_FREX	0x0004
#define CM9780_SPDI_SSEX	0x0008
#define CM9780_SPDI_CBEX	0x0010
#define CM9780_SPDI_RSEX	0x0020
#define CM9780_MIX2FR		0x0040
#define CM9780_MIX2SS		0x0080
#define CM9780_MIX2CB		0x0100
#define CM9780_MIX2RS		0x0200
#define CM9780_MIX2FR_EX	0x0400
#define CM9780_MIX2SS_EX	0x0800
#define CM9780_MIX2CB_EX	0x1000
#define CM9780_MIX2RS_EX	0x2000
#define CM9780_P47_IO		0x4000
#define CM9780_PCBSW		0x8000

/* GPIO setup */
#define CM9780_GPI0EN		0x0001
#define CM9780_GPI1EN		0x0002
#define CM9780_SENSE_P		0x0004
#define CM9780_LOCK_P		0x0008
#define CM9780_GPIO0P		0x0010
#define CM9780_GPIO1P		0x0020
#define CM9780_GPIO0IO		0x0100
#define CM9780_GPIO1IO		0x0200

/* GPIO status */
#define CM9780_GPO0		0x0001
#define CM9780_GPO1		0x0002
#define CM9780_GPIO0S		0x0010
#define CM9780_GPIO1S		0x0020
#define CM9780_GPII0S		0x0100
#define CM9780_GPII1S		0x0200

#endif
