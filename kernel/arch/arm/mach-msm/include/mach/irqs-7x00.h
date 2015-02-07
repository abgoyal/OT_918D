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
 * Copyright (C) 2007 Google, Inc.
 * Copyright (c) 2009, Code Aurora Forum. All rights reserved.
 * Author: Brian Swetland <swetland@google.com>
 */

#ifndef __ASM_ARCH_MSM_IRQS_7X00_H
#define __ASM_ARCH_MSM_IRQS_7X00_H

/* MSM ARM11 Interrupt Numbers */
/* See 80-VE113-1 A, pp219-221     */

#define INT_A9_M2A_0         0
#define INT_A9_M2A_1         1
#define INT_A9_M2A_2         2
#define INT_A9_M2A_3         3
#define INT_A9_M2A_4         4
#define INT_A9_M2A_5         5
#define INT_A9_M2A_6         6
#define INT_GP_TIMER_EXP     7
#define INT_DEBUG_TIMER_EXP  8
#define INT_UART1            9
#define INT_UART2            10
#define INT_UART3            11
#define INT_UART1_RX         12
#define INT_UART2_RX         13
#define INT_UART3_RX         14
#define INT_USB_OTG          15
#define INT_MDDI_PRI         16
#define INT_MDDI_EXT         17
#define INT_MDDI_CLIENT      18
#define INT_MDP              19
#define INT_GRAPHICS         20
#define INT_ADM_AARM         21
#define INT_ADSP_A11         22
#define INT_ADSP_A9_A11      23
#define INT_SDC1_0           24
#define INT_SDC1_1           25
#define INT_SDC2_0           26
#define INT_SDC2_1           27
#define INT_KEYSENSE         28
#define INT_TCHSCRN_SSBI     29
#define INT_TCHSCRN1         30
#define INT_TCHSCRN2         31

#define INT_GPIO_GROUP1      (32 + 0)
#define INT_GPIO_GROUP2      (32 + 1)
#define INT_PWB_I2C          (32 + 2)
#define INT_SOFTRESET        (32 + 3)
#define INT_NAND_WR_ER_DONE  (32 + 4)
#define INT_NAND_OP_DONE     (32 + 5)
#define INT_PBUS_ARM11       (32 + 6)
#define INT_AXI_MPU_SMI      (32 + 7)
#define INT_AXI_MPU_EBI1     (32 + 8)
#define INT_AD_HSSD          (32 + 9)
#define INT_ARM11_PMU        (32 + 10)
#define INT_ARM11_DMA        (32 + 11)
#define INT_TSIF_IRQ         (32 + 12)
#define INT_UART1DM_IRQ      (32 + 13)
#define INT_UART1DM_RX       (32 + 14)
#define INT_USB_HS           (32 + 15)
#define INT_SDC3_0           (32 + 16)
#define INT_SDC3_1           (32 + 17)
#define INT_SDC4_0           (32 + 18)
#define INT_SDC4_1           (32 + 19)
#define INT_UART2DM_RX       (32 + 20)
#define INT_UART2DM_IRQ      (32 + 21)

/* 22-31 are reserved */

#define NR_MSM_IRQS 64
#define NR_GPIO_IRQS 122
#define NR_BOARD_IRQS 64

#endif
