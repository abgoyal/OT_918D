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
 * arch/arm/mach-spear3xx/include/mach/spear.h
 *
 * SPEAr3xx Machine family specific definition
 *
 * Copyright (C) 2009 ST Microelectronics
 * Viresh Kumar<viresh.kumar@st.com>
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2. This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#ifndef __MACH_SPEAR3XX_H
#define __MACH_SPEAR3XX_H

#include <mach/hardware.h>
#include <mach/spear300.h>
#include <mach/spear310.h>
#include <mach/spear320.h>

#define SPEAR3XX_ML_SDRAM_BASE		0x00000000
#define SPEAR3XX_ML_SDRAM_SIZE		0x40000000

#define SPEAR3XX_ICM9_BASE		0xC0000000
#define SPEAR3XX_ICM9_SIZE		0x10000000

/* ICM1 - Low speed connection */
#define SPEAR3XX_ICM1_2_BASE		0xD0000000
#define SPEAR3XX_ICM1_2_SIZE		0x10000000

#define SPEAR3XX_ICM1_UART_BASE		0xD0000000
#define VA_SPEAR3XX_ICM1_UART_BASE	IO_ADDRESS(SPEAR3XX_ICM1_UART_BASE)
#define SPEAR3XX_ICM1_UART_SIZE		0x00080000

#define SPEAR3XX_ICM1_ADC_BASE		0xD0080000
#define SPEAR3XX_ICM1_ADC_SIZE		0x00080000

#define SPEAR3XX_ICM1_SSP_BASE		0xD0100000
#define SPEAR3XX_ICM1_SSP_SIZE		0x00080000

#define SPEAR3XX_ICM1_I2C_BASE		0xD0180000
#define SPEAR3XX_ICM1_I2C_SIZE		0x00080000

#define SPEAR3XX_ICM1_JPEG_BASE		0xD0800000
#define SPEAR3XX_ICM1_JPEG_SIZE		0x00800000

#define SPEAR3XX_ICM1_IRDA_BASE		0xD1000000
#define SPEAR3XX_ICM1_IRDA_SIZE		0x00080000

#define SPEAR3XX_ICM1_SRAM_BASE		0xD2800000
#define SPEAR3XX_ICM1_SRAM_SIZE		0x05800000

/* ICM2 - Application Subsystem */
#define SPEAR3XX_ICM2_HWACCEL0_BASE	0xD8800000
#define SPEAR3XX_ICM2_HWACCEL0_SIZE	0x00800000

#define SPEAR3XX_ICM2_HWACCEL1_BASE	0xD9000000
#define SPEAR3XX_ICM2_HWACCEL1_SIZE	0x00800000

/* ICM4 - High Speed Connection */
#define SPEAR3XX_ICM4_BASE		0xE0000000
#define SPEAR3XX_ICM4_SIZE		0x08000000

#define SPEAR3XX_ICM4_MII_BASE		0xE0800000
#define SPEAR3XX_ICM4_MII_SIZE		0x00800000

#define SPEAR3XX_ICM4_USBD_FIFO_BASE	0xE1000000
#define SPEAR3XX_ICM4_USBD_FIFO_SIZE	0x00100000

#define SPEAR3XX_ICM4_USBD_CSR_BASE	0xE1100000
#define SPEAR3XX_ICM4_USBD_CSR_SIZE	0x00100000

#define SPEAR3XX_ICM4_USBD_PLDT_BASE	0xE1200000
#define SPEAR3XX_ICM4_USBD_PLDT_SIZE	0x00100000

#define SPEAR3XX_ICM4_USB_EHCI0_1_BASE	0xE1800000
#define SPEAR3XX_ICM4_USB_EHCI0_1_SIZE	0x00100000

#define SPEAR3XX_ICM4_USB_OHCI0_BASE	0xE1900000
#define SPEAR3XX_ICM4_USB_OHCI0_SIZE	0x00100000

#define SPEAR3XX_ICM4_USB_OHCI1_BASE	0xE2100000
#define SPEAR3XX_ICM4_USB_OHCI1_SIZE	0x00100000

#define SPEAR3XX_ICM4_USB_ARB_BASE	0xE2800000
#define SPEAR3XX_ICM4_USB_ARB_SIZE	0x00010000

/* ML1 - Multi Layer CPU Subsystem */
#define SPEAR3XX_ICM3_ML1_2_BASE	0xF0000000
#define SPEAR3XX_ICM3_ML1_2_SIZE	0x0F000000

#define SPEAR3XX_ML1_TMR_BASE		0xF0000000
#define SPEAR3XX_ML1_TMR_SIZE		0x00100000

#define SPEAR3XX_ML1_VIC_BASE		0xF1100000
#define VA_SPEAR3XX_ML1_VIC_BASE	IO_ADDRESS(SPEAR3XX_ML1_VIC_BASE)
#define SPEAR3XX_ML1_VIC_SIZE		0x00100000

/* ICM3 - Basic Subsystem */
#define SPEAR3XX_ICM3_SMEM_BASE		0xF8000000
#define SPEAR3XX_ICM3_SMEM_SIZE		0x04000000

#define SPEAR3XX_ICM3_SMI_CTRL_BASE	0xFC000000
#define SPEAR3XX_ICM3_SMI_CTRL_SIZE	0x00200000

#define SPEAR3XX_ICM3_DMA_BASE		0xFC400000
#define SPEAR3XX_ICM3_DMA_SIZE		0x00200000

#define SPEAR3XX_ICM3_SDRAM_CTRL_BASE	0xFC600000
#define SPEAR3XX_ICM3_SDRAM_CTRL_SIZE	0x00200000

#define SPEAR3XX_ICM3_TMR0_BASE		0xFC800000
#define SPEAR3XX_ICM3_TMR0_SIZE		0x00080000

#define SPEAR3XX_ICM3_WDT_BASE		0xFC880000
#define SPEAR3XX_ICM3_WDT_SIZE		0x00080000

#define SPEAR3XX_ICM3_RTC_BASE		0xFC900000
#define SPEAR3XX_ICM3_RTC_SIZE		0x00080000

#define SPEAR3XX_ICM3_GPIO_BASE		0xFC980000
#define SPEAR3XX_ICM3_GPIO_SIZE		0x00080000

#define SPEAR3XX_ICM3_SYS_CTRL_BASE	0xFCA00000
#define VA_SPEAR3XX_ICM3_SYS_CTRL_BASE	IO_ADDRESS(SPEAR3XX_ICM3_SYS_CTRL_BASE)
#define SPEAR3XX_ICM3_SYS_CTRL_SIZE	0x00080000

#define SPEAR3XX_ICM3_MISC_REG_BASE	0xFCA80000
#define VA_SPEAR3XX_ICM3_MISC_REG_BASE	IO_ADDRESS(SPEAR3XX_ICM3_MISC_REG_BASE)
#define SPEAR3XX_ICM3_MISC_REG_SIZE	0x00080000

#define SPEAR3XX_ICM3_TMR1_BASE		0xFCB00000
#define SPEAR3XX_ICM3_TMR1_SIZE		0x00080000

/* Debug uart for linux, will be used for debug and uncompress messages */
#define SPEAR_DBG_UART_BASE		SPEAR3XX_ICM1_UART_BASE
#define VA_SPEAR_DBG_UART_BASE		VA_SPEAR3XX_ICM1_UART_BASE

/* Sysctl base for spear platform */
#define SPEAR_SYS_CTRL_BASE		SPEAR3XX_ICM3_SYS_CTRL_BASE
#define VA_SPEAR_SYS_CTRL_BASE		VA_SPEAR3XX_ICM3_SYS_CTRL_BASE

#endif /* __MACH_SPEAR3XX_H */
