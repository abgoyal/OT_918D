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
 * Freescale STMP378X SoC pin multiplexing
 *
 * Author: Vladislav Buzov <vbuzov@embeddedalley.com>
 *
 * Copyright 2008 Freescale Semiconductor, Inc. All Rights Reserved.
 * Copyright 2008 Embedded Alley Solutions, Inc All Rights Reserved.
 */

/*
 * The code contained herein is licensed under the GNU General Public
 * License. You may obtain a copy of the GNU General Public License
 * Version 2 or later at the following locations:
 *
 * http://www.opensource.org/licenses/gpl-license.html
 * http://www.gnu.org/copyleft/gpl.html
 */
#ifndef __ASM_ARCH_PINS_H
#define __ASM_ARCH_PINS_H

/*
 * Define all STMP378x pins, a pin name corresponds to a STMP378x hardware
 * interface  this pin belongs to.
 */

/* Bank 0 */
#define PINID_GPMI_D00		STMP3XXX_PINID(0, 0)
#define PINID_GPMI_D01		STMP3XXX_PINID(0, 1)
#define PINID_GPMI_D02		STMP3XXX_PINID(0, 2)
#define PINID_GPMI_D03		STMP3XXX_PINID(0, 3)
#define PINID_GPMI_D04		STMP3XXX_PINID(0, 4)
#define PINID_GPMI_D05		STMP3XXX_PINID(0, 5)
#define PINID_GPMI_D06		STMP3XXX_PINID(0, 6)
#define PINID_GPMI_D07		STMP3XXX_PINID(0, 7)
#define PINID_GPMI_D08		STMP3XXX_PINID(0, 8)
#define PINID_GPMI_D09		STMP3XXX_PINID(0, 9)
#define PINID_GPMI_D10		STMP3XXX_PINID(0, 10)
#define PINID_GPMI_D11		STMP3XXX_PINID(0, 11)
#define PINID_GPMI_D12		STMP3XXX_PINID(0, 12)
#define PINID_GPMI_D13		STMP3XXX_PINID(0, 13)
#define PINID_GPMI_D14		STMP3XXX_PINID(0, 14)
#define PINID_GPMI_D15		STMP3XXX_PINID(0, 15)
#define PINID_GPMI_CLE		STMP3XXX_PINID(0, 16)
#define PINID_GPMI_ALE		STMP3XXX_PINID(0, 17)
#define PINID_GMPI_CE2N		STMP3XXX_PINID(0, 18)
#define PINID_GPMI_RDY0		STMP3XXX_PINID(0, 19)
#define PINID_GPMI_RDY1		STMP3XXX_PINID(0, 20)
#define PINID_GPMI_RDY2		STMP3XXX_PINID(0, 21)
#define PINID_GPMI_RDY3		STMP3XXX_PINID(0, 22)
#define PINID_GPMI_WPN		STMP3XXX_PINID(0, 23)
#define PINID_GPMI_WRN		STMP3XXX_PINID(0, 24)
#define PINID_GPMI_RDN		STMP3XXX_PINID(0, 25)
#define PINID_AUART1_CTS	STMP3XXX_PINID(0, 26)
#define PINID_AUART1_RTS	STMP3XXX_PINID(0, 27)
#define PINID_AUART1_RX		STMP3XXX_PINID(0, 28)
#define PINID_AUART1_TX		STMP3XXX_PINID(0, 29)
#define PINID_I2C_SCL		STMP3XXX_PINID(0, 30)
#define PINID_I2C_SDA		STMP3XXX_PINID(0, 31)

/* Bank 1 */
#define PINID_LCD_D00		STMP3XXX_PINID(1, 0)
#define PINID_LCD_D01		STMP3XXX_PINID(1, 1)
#define PINID_LCD_D02		STMP3XXX_PINID(1, 2)
#define PINID_LCD_D03		STMP3XXX_PINID(1, 3)
#define PINID_LCD_D04		STMP3XXX_PINID(1, 4)
#define PINID_LCD_D05		STMP3XXX_PINID(1, 5)
#define PINID_LCD_D06		STMP3XXX_PINID(1, 6)
#define PINID_LCD_D07		STMP3XXX_PINID(1, 7)
#define PINID_LCD_D08		STMP3XXX_PINID(1, 8)
#define PINID_LCD_D09		STMP3XXX_PINID(1, 9)
#define PINID_LCD_D10		STMP3XXX_PINID(1, 10)
#define PINID_LCD_D11		STMP3XXX_PINID(1, 11)
#define PINID_LCD_D12		STMP3XXX_PINID(1, 12)
#define PINID_LCD_D13		STMP3XXX_PINID(1, 13)
#define PINID_LCD_D14		STMP3XXX_PINID(1, 14)
#define PINID_LCD_D15		STMP3XXX_PINID(1, 15)
#define PINID_LCD_D16		STMP3XXX_PINID(1, 16)
#define PINID_LCD_D17		STMP3XXX_PINID(1, 17)
#define PINID_LCD_RESET		STMP3XXX_PINID(1, 18)
#define PINID_LCD_RS		STMP3XXX_PINID(1, 19)
#define PINID_LCD_WR		STMP3XXX_PINID(1, 20)
#define PINID_LCD_CS		STMP3XXX_PINID(1, 21)
#define PINID_LCD_DOTCK		STMP3XXX_PINID(1, 22)
#define PINID_LCD_ENABLE	STMP3XXX_PINID(1, 23)
#define PINID_LCD_HSYNC		STMP3XXX_PINID(1, 24)
#define PINID_LCD_VSYNC		STMP3XXX_PINID(1, 25)
#define PINID_PWM0		STMP3XXX_PINID(1, 26)
#define PINID_PWM1		STMP3XXX_PINID(1, 27)
#define PINID_PWM2		STMP3XXX_PINID(1, 28)
#define PINID_PWM3		STMP3XXX_PINID(1, 29)
#define PINID_PWM4		STMP3XXX_PINID(1, 30)

/* Bank 2 */
#define PINID_SSP1_CMD		STMP3XXX_PINID(2, 0)
#define PINID_SSP1_DETECT	STMP3XXX_PINID(2, 1)
#define PINID_SSP1_DATA0	STMP3XXX_PINID(2, 2)
#define PINID_SSP1_DATA1	STMP3XXX_PINID(2, 3)
#define PINID_SSP1_DATA2	STMP3XXX_PINID(2, 4)
#define PINID_SSP1_DATA3	STMP3XXX_PINID(2, 5)
#define PINID_SSP1_SCK		STMP3XXX_PINID(2, 6)
#define PINID_ROTARYA		STMP3XXX_PINID(2, 7)
#define PINID_ROTARYB		STMP3XXX_PINID(2, 8)
#define PINID_EMI_A00		STMP3XXX_PINID(2, 9)
#define PINID_EMI_A01		STMP3XXX_PINID(2, 10)
#define PINID_EMI_A02		STMP3XXX_PINID(2, 11)
#define PINID_EMI_A03		STMP3XXX_PINID(2, 12)
#define PINID_EMI_A04		STMP3XXX_PINID(2, 13)
#define PINID_EMI_A05		STMP3XXX_PINID(2, 14)
#define PINID_EMI_A06		STMP3XXX_PINID(2, 15)
#define PINID_EMI_A07		STMP3XXX_PINID(2, 16)
#define PINID_EMI_A08		STMP3XXX_PINID(2, 17)
#define PINID_EMI_A09		STMP3XXX_PINID(2, 18)
#define PINID_EMI_A10		STMP3XXX_PINID(2, 19)
#define PINID_EMI_A11		STMP3XXX_PINID(2, 20)
#define PINID_EMI_A12		STMP3XXX_PINID(2, 21)
#define PINID_EMI_BA0		STMP3XXX_PINID(2, 22)
#define PINID_EMI_BA1		STMP3XXX_PINID(2, 23)
#define PINID_EMI_CASN		STMP3XXX_PINID(2, 24)
#define PINID_EMI_CE0N		STMP3XXX_PINID(2, 25)
#define PINID_EMI_CE1N		STMP3XXX_PINID(2, 26)
#define PINID_GPMI_CE1N		STMP3XXX_PINID(2, 27)
#define PINID_GPMI_CE0N		STMP3XXX_PINID(2, 28)
#define PINID_EMI_CKE		STMP3XXX_PINID(2, 29)
#define PINID_EMI_RASN		STMP3XXX_PINID(2, 30)
#define PINID_EMI_WEN		STMP3XXX_PINID(2, 31)

/* Bank 3 */
#define PINID_EMI_D00		STMP3XXX_PINID(3, 0)
#define PINID_EMI_D01		STMP3XXX_PINID(3, 1)
#define PINID_EMI_D02		STMP3XXX_PINID(3, 2)
#define PINID_EMI_D03		STMP3XXX_PINID(3, 3)
#define PINID_EMI_D04		STMP3XXX_PINID(3, 4)
#define PINID_EMI_D05		STMP3XXX_PINID(3, 5)
#define PINID_EMI_D06		STMP3XXX_PINID(3, 6)
#define PINID_EMI_D07		STMP3XXX_PINID(3, 7)
#define PINID_EMI_D08		STMP3XXX_PINID(3, 8)
#define PINID_EMI_D09		STMP3XXX_PINID(3, 9)
#define PINID_EMI_D10		STMP3XXX_PINID(3, 10)
#define PINID_EMI_D11		STMP3XXX_PINID(3, 11)
#define PINID_EMI_D12		STMP3XXX_PINID(3, 12)
#define PINID_EMI_D13		STMP3XXX_PINID(3, 13)
#define PINID_EMI_D14		STMP3XXX_PINID(3, 14)
#define PINID_EMI_D15		STMP3XXX_PINID(3, 15)
#define PINID_EMI_DQM0		STMP3XXX_PINID(3, 16)
#define PINID_EMI_DQM1		STMP3XXX_PINID(3, 17)
#define PINID_EMI_DQS0		STMP3XXX_PINID(3, 18)
#define PINID_EMI_DQS1		STMP3XXX_PINID(3, 19)
#define PINID_EMI_CLK		STMP3XXX_PINID(3, 20)
#define PINID_EMI_CLKN		STMP3XXX_PINID(3, 21)

#endif /* __ASM_ARCH_PINS_H */
