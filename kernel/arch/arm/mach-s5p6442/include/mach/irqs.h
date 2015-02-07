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

/* linux/arch/arm/mach-s5p6442/include/mach/irqs.h
 *
 * Copyright (c) 2010 Samsung Electronics Co., Ltd.
 *		http://www.samsung.com/
 *
 * S5P6442 - IRQ definitions
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#ifndef __ASM_ARCH_IRQS_H
#define __ASM_ARCH_IRQS_H __FILE__

#include <plat/irqs.h>

/* VIC0 */
#define IRQ_EINT16_31 		S5P_IRQ_VIC0(16)
#define IRQ_BATF 		S5P_IRQ_VIC0(17)
#define IRQ_MDMA 		S5P_IRQ_VIC0(18)
#define IRQ_PDMA 		S5P_IRQ_VIC0(19)
#define IRQ_TIMER0_VIC		S5P_IRQ_VIC0(21)
#define IRQ_TIMER1_VIC		S5P_IRQ_VIC0(22)
#define IRQ_TIMER2_VIC		S5P_IRQ_VIC0(23)
#define IRQ_TIMER3_VIC		S5P_IRQ_VIC0(24)
#define IRQ_TIMER4_VIC		S5P_IRQ_VIC0(25)
#define IRQ_SYSTIMER		S5P_IRQ_VIC0(26)
#define IRQ_WDT			S5P_IRQ_VIC0(27)
#define IRQ_RTC_ALARM		S5P_IRQ_VIC0(28)
#define IRQ_RTC_TIC		S5P_IRQ_VIC0(29)
#define IRQ_GPIOINT		S5P_IRQ_VIC0(30)

/* VIC1 */
#define IRQ_nPMUIRQ 		S5P_IRQ_VIC1(0)
#define IRQ_ONENAND 		S5P_IRQ_VIC1(7)
#define IRQ_UART0 		S5P_IRQ_VIC1(10)
#define IRQ_UART1 		S5P_IRQ_VIC1(11)
#define IRQ_UART2 		S5P_IRQ_VIC1(12)
#define IRQ_SPI0 		S5P_IRQ_VIC1(15)
#define IRQ_IIC 		S5P_IRQ_VIC1(19)
#define IRQ_IIC1 		S5P_IRQ_VIC1(20)
#define IRQ_IIC2 		S5P_IRQ_VIC1(21)
#define IRQ_OTG 		S5P_IRQ_VIC1(24)
#define IRQ_MSM 		S5P_IRQ_VIC1(25)
#define IRQ_HSMMC0 		S5P_IRQ_VIC1(26)
#define IRQ_HSMMC1 		S5P_IRQ_VIC1(27)
#define IRQ_HSMMC2 		S5P_IRQ_VIC1(28)
#define IRQ_COMMRX 		S5P_IRQ_VIC1(29)
#define IRQ_COMMTX 		S5P_IRQ_VIC1(30)

/* VIC2 */
#define IRQ_LCD0 		S5P_IRQ_VIC2(0)
#define IRQ_LCD1 		S5P_IRQ_VIC2(1)
#define IRQ_LCD2 		S5P_IRQ_VIC2(2)
#define IRQ_LCD3 		S5P_IRQ_VIC2(3)
#define IRQ_ROTATOR 		S5P_IRQ_VIC2(4)
#define IRQ_FIMC0 		S5P_IRQ_VIC2(5)
#define IRQ_FIMC1 		S5P_IRQ_VIC2(6)
#define IRQ_FIMC2 		S5P_IRQ_VIC2(7)
#define IRQ_JPEG 		S5P_IRQ_VIC2(8)
#define IRQ_3D 			S5P_IRQ_VIC2(10)
#define IRQ_Mixer 		S5P_IRQ_VIC2(11)
#define IRQ_MFC 		S5P_IRQ_VIC2(14)
#define IRQ_TVENC 		S5P_IRQ_VIC2(15)
#define IRQ_I2S0 		S5P_IRQ_VIC2(16)
#define IRQ_I2S1 		S5P_IRQ_VIC2(17)
#define IRQ_RP 			S5P_IRQ_VIC2(19)
#define IRQ_PCM0 		S5P_IRQ_VIC2(20)
#define IRQ_PCM1 		S5P_IRQ_VIC2(21)
#define IRQ_ADC 		S5P_IRQ_VIC2(23)
#define IRQ_PENDN 		S5P_IRQ_VIC2(24)
#define IRQ_KEYPAD 		S5P_IRQ_VIC2(25)
#define IRQ_SSS_INT 		S5P_IRQ_VIC2(27)
#define IRQ_SSS_HASH 		S5P_IRQ_VIC2(28)
#define IRQ_VIC_END 		S5P_IRQ_VIC2(31)

#define S5P_IRQ_EINT_BASE	(IRQ_VIC_END + 1)

#define S5P_EINT_BASE1		(S5P_IRQ_VIC0(0))
#define S5P_EINT_BASE2		(S5P_IRQ_EINT_BASE)

/* Set the default NR_IRQS */

#define NR_IRQS 		(IRQ_EINT(31) + 1)

#endif /* __ASM_ARCH_IRQS_H */
