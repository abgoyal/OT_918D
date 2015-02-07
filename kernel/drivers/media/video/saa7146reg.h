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
    saa7146.h - definitions philips saa7146 based cards
    Copyright (C) 1999 Nathan Laredo (laredo@gnu.org)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/

#ifndef __SAA7146_REG__
#define __SAA7146_REG__
#define SAA7146_BASE_ODD1	0x00
#define SAA7146_BASE_EVEN1	0x04
#define SAA7146_PROT_ADDR1	0x08
#define SAA7146_PITCH1		0x0c
#define SAA7146_PAGE1		0x10
#define SAA7146_NUM_LINE_BYTE1	0x14
#define SAA7146_BASE_ODD2	0x18
#define SAA7146_BASE_EVEN2	0x1c
#define SAA7146_PROT_ADDR2	0x20
#define SAA7146_PITCH2		0x24
#define SAA7146_PAGE2		0x28
#define SAA7146_NUM_LINE_BYTE2	0x2c
#define SAA7146_BASE_ODD3	0x30
#define SAA7146_BASE_EVEN3	0x34
#define SAA7146_PROT_ADDR3	0x38
#define SAA7146_PITCH3		0x3c
#define SAA7146_PAGE3		0x40
#define SAA7146_NUM_LINE_BYTE3	0x44
#define SAA7146_PCI_BT_V1	0x48
#define SAA7146_PCI_BT_V2	0x49
#define SAA7146_PCI_BT_V3	0x4a
#define SAA7146_PCI_BT_DEBI	0x4b
#define SAA7146_PCI_BT_A	0x4c
#define SAA7146_DD1_INIT	0x50
#define SAA7146_DD1_STREAM_B	0x54
#define SAA7146_DD1_STREAM_A	0x56
#define SAA7146_BRS_CTRL	0x58
#define SAA7146_HPS_CTRL	0x5c
#define SAA7146_HPS_V_SCALE	0x60
#define SAA7146_HPS_V_GAIN	0x64
#define SAA7146_HPS_H_PRESCALE	0x68
#define SAA7146_HPS_H_SCALE	0x6c
#define SAA7146_BCS_CTRL	0x70
#define SAA7146_CHROMA_KEY_RANGE	0x74
#define SAA7146_CLIP_FORMAT_CTRL	0x78
#define SAA7146_DEBI_CONFIG	0x7c
#define SAA7146_DEBI_COMMAND	0x80
#define SAA7146_DEBI_PAGE	0x84
#define SAA7146_DEBI_AD		0x88
#define SAA7146_I2C_TRANSFER	0x8c
#define SAA7146_I2C_STATUS	0x90
#define SAA7146_BASE_A1_IN	0x94
#define SAA7146_PROT_A1_IN	0x98
#define SAA7146_PAGE_A1_IN	0x9C
#define SAA7146_BASE_A1_OUT	0xa0
#define SAA7146_PROT_A1_OUT	0xa4
#define SAA7146_PAGE_A1_OUT	0xa8
#define SAA7146_BASE_A2_IN	0xac
#define SAA7146_PROT_A2_IN	0xb0
#define SAA7146_PAGE_A2_IN	0xb4
#define SAA7146_BASE_A2_OUT	0xb8
#define SAA7146_PROT_A2_OUT	0xbc
#define SAA7146_PAGE_A2_OUT	0xc0
#define SAA7146_RPS_PAGE0	0xc4
#define SAA7146_RPS_PAGE1	0xc8
#define SAA7146_RPS_THRESH0	0xcc
#define SAA7146_RPS_THRESH1	0xd0
#define SAA7146_RPS_TOV0	0xd4
#define SAA7146_RPS_TOV1	0xd8
#define SAA7146_IER		0xdc
#define SAA7146_GPIO_CTRL	0xe0
#define SAA7146_EC1SSR		0xe4
#define SAA7146_EC2SSR		0xe8
#define SAA7146_ECT1R		0xec
#define SAA7146_ECT2R		0xf0
#define SAA7146_ACON1		0xf4
#define SAA7146_ACON2		0xf8
#define SAA7146_MC1		0xfc
#define SAA7146_MC2		0x100
#define SAA7146_RPS_ADDR0	0x104
#define SAA7146_RPS_ADDR1	0x108
#define SAA7146_ISR		0x10c
#define SAA7146_PSR		0x110
#define SAA7146_SSR		0x114
#define SAA7146_EC1R		0x118
#define SAA7146_EC2R		0x11c
#define SAA7146_VDP1		0x120
#define SAA7146_VDP2		0x124
#define SAA7146_VDP3		0x128
#define SAA7146_ADP1		0x12c
#define SAA7146_ADP2		0x130
#define SAA7146_ADP3		0x134
#define SAA7146_ADP4		0x138
#define SAA7146_DDP		0x13c
#define SAA7146_LEVEL_REP	0x140
#define SAA7146_FB_BUFFER1	0x144
#define SAA7146_FB_BUFFER2	0x148
#define SAA7146_A_TIME_SLOT1	0x180
#define SAA7146_A_TIME_SLOT2	0x1C0

/* bitfield defines */
#define MASK_31			0x80000000
#define MASK_30			0x40000000
#define MASK_29			0x20000000
#define MASK_28			0x10000000
#define MASK_27			0x08000000
#define MASK_26			0x04000000
#define MASK_25			0x02000000
#define MASK_24			0x01000000
#define MASK_23			0x00800000
#define MASK_22			0x00400000
#define MASK_21			0x00200000
#define MASK_20			0x00100000
#define MASK_19			0x00080000
#define MASK_18			0x00040000
#define MASK_17			0x00020000
#define MASK_16			0x00010000
#define MASK_15			0x00008000
#define MASK_14			0x00004000
#define MASK_13			0x00002000
#define MASK_12			0x00001000
#define MASK_11			0x00000800
#define MASK_10			0x00000400
#define MASK_09			0x00000200
#define MASK_08			0x00000100
#define MASK_07			0x00000080
#define MASK_06			0x00000040
#define MASK_05			0x00000020
#define MASK_04			0x00000010
#define MASK_03			0x00000008
#define MASK_02			0x00000004
#define MASK_01			0x00000002
#define MASK_00			0x00000001
#define MASK_B0			0x000000ff
#define MASK_B1			0x0000ff00
#define MASK_B2			0x00ff0000
#define MASK_B3			0xff000000
#define MASK_W0			0x0000ffff
#define MASK_W1			0xffff0000
#define MASK_PA			0xfffffffc
#define MASK_PR			0xfffffffe
#define MASK_ER			0xffffffff
#define MASK_NONE		0x00000000

#define SAA7146_PAGE_MAP_EN	MASK_11
/* main control register 1 */
#define SAA7146_MC1_MRST_N	MASK_15
#define SAA7146_MC1_ERPS1	MASK_13
#define SAA7146_MC1_ERPS0	MASK_12
#define SAA7146_MC1_EDP		MASK_11
#define SAA7146_MC1_EVP		MASK_10
#define SAA7146_MC1_EAP		MASK_09
#define SAA7146_MC1_EI2C	MASK_08
#define SAA7146_MC1_TR_E_DEBI	MASK_07
#define SAA7146_MC1_TR_E_1	MASK_06
#define SAA7146_MC1_TR_E_2	MASK_05
#define SAA7146_MC1_TR_E_3	MASK_04
#define SAA7146_MC1_TR_E_A2_OUT	MASK_03
#define SAA7146_MC1_TR_E_A2_IN	MASK_02
#define SAA7146_MC1_TR_E_A1_OUT	MASK_01
#define SAA7146_MC1_TR_E_A1_IN	MASK_00
/* main control register 2 */
#define SAA7146_MC2_RPS_SIG4	MASK_15
#define SAA7146_MC2_RPS_SIG3	MASK_14
#define SAA7146_MC2_RPS_SIG2	MASK_13
#define SAA7146_MC2_RPS_SIG1	MASK_12
#define SAA7146_MC2_RPS_SIG0	MASK_11
#define SAA7146_MC2_UPLD_D1_B	MASK_10
#define SAA7146_MC2_UPLD_D1_A	MASK_09
#define SAA7146_MC2_UPLD_BRS	MASK_08
#define SAA7146_MC2_UPLD_HPS_H	MASK_06
#define SAA7146_MC2_UPLD_HPS_V	MASK_05
#define SAA7146_MC2_UPLD_DMA3	MASK_04
#define SAA7146_MC2_UPLD_DMA2	MASK_03
#define SAA7146_MC2_UPLD_DMA1	MASK_02
#define SAA7146_MC2_UPLD_DEBI	MASK_01
#define SAA7146_MC2_UPLD_I2C	MASK_00
/* Primary Status Register and Interrupt Enable/Status Registers */
#define SAA7146_PSR_PPEF	MASK_31
#define SAA7146_PSR_PABO	MASK_30
#define SAA7146_PSR_PPED	MASK_29
#define SAA7146_PSR_RPS_I1	MASK_28
#define SAA7146_PSR_RPS_I0	MASK_27
#define SAA7146_PSR_RPS_LATE1	MASK_26
#define SAA7146_PSR_RPS_LATE0	MASK_25
#define SAA7146_PSR_RPS_E1	MASK_24
#define SAA7146_PSR_RPS_E0	MASK_23
#define SAA7146_PSR_RPS_TO1	MASK_22
#define SAA7146_PSR_RPS_TO0	MASK_21
#define SAA7146_PSR_UPLD	MASK_20
#define SAA7146_PSR_DEBI_S	MASK_19
#define SAA7146_PSR_DEBI_E	MASK_18
#define SAA7146_PSR_I2C_S	MASK_17
#define SAA7146_PSR_I2C_E	MASK_16
#define SAA7146_PSR_A2_IN	MASK_15
#define SAA7146_PSR_A2_OUT	MASK_14
#define SAA7146_PSR_A1_IN	MASK_13
#define SAA7146_PSR_A1_OUT	MASK_12
#define SAA7146_PSR_AFOU	MASK_11
#define SAA7146_PSR_V_PE	MASK_10
#define SAA7146_PSR_VFOU	MASK_09
#define SAA7146_PSR_FIDA	MASK_08
#define SAA7146_PSR_FIDB	MASK_07
#define SAA7146_PSR_PIN3	MASK_06
#define SAA7146_PSR_PIN2	MASK_05
#define SAA7146_PSR_PIN1	MASK_04
#define SAA7146_PSR_PIN0	MASK_03
#define SAA7146_PSR_ECS		MASK_02
#define SAA7146_PSR_EC3S	MASK_01
#define SAA7146_PSR_EC0S	MASK_00
/* Secondary Status Register */
#define SAA7146_SSR_PRQ		MASK_31
#define SAA7146_SSR_PMA		MASK_30
#define SAA7146_SSR_RPS_RE1	MASK_29
#define SAA7146_SSR_RPS_PE1	MASK_28
#define SAA7146_SSR_RPS_A1	MASK_27
#define SAA7146_SSR_RPS_RE0	MASK_26
#define SAA7146_SSR_RPS_PE0	MASK_25
#define SAA7146_SSR_RPS_A0	MASK_24
#define SAA7146_SSR_DEBI_TO	MASK_23
#define SAA7146_SSR_DEBI_EF	MASK_22
#define SAA7146_SSR_I2C_EA	MASK_21
#define SAA7146_SSR_I2C_EW	MASK_20
#define SAA7146_SSR_I2C_ER	MASK_19
#define SAA7146_SSR_I2C_EL	MASK_18
#define SAA7146_SSR_I2C_EF	MASK_17
#define SAA7146_SSR_V3P		MASK_16
#define SAA7146_SSR_V2P		MASK_15
#define SAA7146_SSR_V1P		MASK_14
#define SAA7146_SSR_VF3		MASK_13
#define SAA7146_SSR_VF2		MASK_12
#define SAA7146_SSR_VF1		MASK_11
#define SAA7146_SSR_AF2_IN	MASK_10
#define SAA7146_SSR_AF2_OUT	MASK_09
#define SAA7146_SSR_AF1_IN	MASK_08
#define SAA7146_SSR_AF1_OUT	MASK_07
#define SAA7146_SSR_VGT		MASK_05
#define SAA7146_SSR_LNQG	MASK_04
#define SAA7146_SSR_EC5S	MASK_03
#define SAA7146_SSR_EC4S	MASK_02
#define SAA7146_SSR_EC2S	MASK_01
#define SAA7146_SSR_EC1S	MASK_00
/* I2C status register */
#define SAA7146_I2C_ABORT	MASK_07
#define SAA7146_I2C_SPERR	MASK_06
#define SAA7146_I2C_APERR	MASK_05
#define SAA7146_I2C_DTERR	MASK_04
#define SAA7146_I2C_DRERR	MASK_03
#define SAA7146_I2C_AL		MASK_02
#define SAA7146_I2C_ERR		MASK_01
#define SAA7146_I2C_BUSY	MASK_00
/* output formats */
#define SAA7146_YUV422	0
#define SAA7146_RGB16	0
#define SAA7146_YUV444	1
#define SAA7146_RGB24	1
#define SAA7146_ARGB32	2
#define SAA7146_YUV411	3
#define SAA7146_ARGB15  3
#define SAA7146_YUV2	4
#define SAA7146_RGAB15	4
#define SAA7146_Y8	6
#define SAA7146_YUV8	7
#define SAA7146_RGB8	7
#define SAA7146_YUV444p	8
#define SAA7146_YUV422p	9
#define SAA7146_YUV420p	10
#define SAA7146_YUV1620	11
#define SAA7146_Y1	13
#define SAA7146_Y2	14
#define SAA7146_YUV1	15
#endif
