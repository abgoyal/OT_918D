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

/**
 * Copyright (C) 2008, Creative Technology Ltd. All Rights Reserved.
 *
 * This source file is released under GPL v2 license (no other versions).
 * See the COPYING file included in the main directory of this source
 * distribution for the license terms and conditions.
 */

#ifndef _20K2REGISTERS_H_
#define _20K2REGISTERS_H_


/* Timer Registers */
#define WC		0x1b7000
#define TIMR		0x1b7004
# define	TIMR_IE		(1<<15)
# define	TIMR_IP		(1<<14)
#define GIP		0x1b7010
#define GIE		0x1b7014

/* I2C Registers */
#define I2C_IF_ADDRESS   0x1B9000
#define I2C_IF_WDATA     0x1B9004
#define I2C_IF_RDATA     0x1B9008
#define I2C_IF_STATUS    0x1B900C
#define I2C_IF_WLOCK     0x1B9010

/* Global Control Registers */
#define GLOBAL_CNTL_GCTL    0x1B7090

/* PLL Registers */
#define PLL_CTL 		0x1B7080
#define PLL_STAT		0x1B7084
#define PLL_ENB			0x1B7088

/* SRC Registers */
#define SRC_CTL             0x1A0000 /* 0x1A0000 + (256 * Chn) */
#define SRC_CCR             0x1A0004 /* 0x1A0004 + (256 * Chn) */
#define SRC_IMAP            0x1A0008 /* 0x1A0008 + (256 * Chn) */
#define SRC_CA              0x1A0010 /* 0x1A0010 + (256 * Chn) */
#define SRC_CF              0x1A0014 /* 0x1A0014 + (256 * Chn) */
#define SRC_SA              0x1A0018 /* 0x1A0018 + (256 * Chn) */
#define SRC_LA              0x1A001C /* 0x1A001C + (256 * Chn) */
#define SRC_CTLSWR	    0x1A0020 /* 0x1A0020 + (256 * Chn) */
#define SRC_CD		    0x1A0080 /* 0x1A0080 + (256 * Chn) + (4 * Regn) */
#define SRC_MCTL		0x1A012C
#define SRC_IP			0x1A102C /* 0x1A102C + (256 * Regn) */
#define SRC_ENB			0x1A282C /* 0x1A282C + (256 * Regn) */
#define SRC_ENBSTAT		0x1A202C
#define SRC_ENBSA		0x1A232C
#define SRC_DN0Z		0x1A0030
#define SRC_DN1Z		0x1A0040
#define SRC_UPZ			0x1A0060

/* GPIO Registers */
#define GPIO_DATA           0x1B7020
#define GPIO_CTRL           0x1B7024

/* Virtual memory registers */
#define VMEM_PTPAL          0x1C6300 /* 0x1C6300 + (16 * Chn) */
#define VMEM_PTPAH          0x1C6304 /* 0x1C6304 + (16 * Chn) */
#define VMEM_CTL            0x1C7000

/* Transport Registers */
#define TRANSPORT_ENB       0x1B6000
#define TRANSPORT_CTL       0x1B6004
#define TRANSPORT_INT       0x1B6008

/* Audio IO */
#define AUDIO_IO_AIM        0x1B5000 /* 0x1B5000 + (0x04 * Chn) */
#define AUDIO_IO_TX_CTL     0x1B5400 /* 0x1B5400 + (0x40 * Chn) */
#define AUDIO_IO_TX_CSTAT_L 0x1B5408 /* 0x1B5408 + (0x40 * Chn) */
#define AUDIO_IO_TX_CSTAT_H 0x1B540C /* 0x1B540C + (0x40 * Chn) */
#define AUDIO_IO_RX_CTL     0x1B5410 /* 0x1B5410 + (0x40 * Chn) */
#define AUDIO_IO_RX_SRT_CTL 0x1B5420 /* 0x1B5420 + (0x40 * Chn) */
#define AUDIO_IO_MCLK       0x1B5600
#define AUDIO_IO_TX_BLRCLK  0x1B5604
#define AUDIO_IO_RX_BLRCLK  0x1B5608

/* Mixer */
#define MIXER_AMOPLO		0x130000 /* 0x130000 + (8 * Chn) [4095 : 0] */
#define MIXER_AMOPHI		0x130004 /* 0x130004 + (8 * Chn) [4095 : 0] */
#define MIXER_PRING_LO_HI	0x188000 /* 0x188000 + (4 * Chn) [4095 : 0] */
#define MIXER_PMOPLO		0x138000 /* 0x138000 + (8 * Chn) [4095 : 0] */
#define MIXER_PMOPHI		0x138004 /* 0x138004 + (8 * Chn) [4095 : 0] */
#define MIXER_AR_ENABLE		0x19000C

#endif
