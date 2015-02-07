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

/* cpu-irqs.h: on-CPU peripheral irqs
 *
 * Copyright (C) 2004 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#ifndef _ASM_CPU_IRQS_H
#define _ASM_CPU_IRQS_H

#ifndef __ASSEMBLY__

/* IRQ to level mappings */
#define IRQ_GDBSTUB_LEVEL	15
#define IRQ_UART_LEVEL		13

#ifdef CONFIG_GDBSTUB_UART0
#define IRQ_UART0_LEVEL		IRQ_GDBSTUB_LEVEL
#else
#define IRQ_UART0_LEVEL		IRQ_UART_LEVEL
#endif

#ifdef CONFIG_GDBSTUB_UART1
#define IRQ_UART1_LEVEL		IRQ_GDBSTUB_LEVEL
#else
#define IRQ_UART1_LEVEL		IRQ_UART_LEVEL
#endif

#define IRQ_DMA0_LEVEL		14
#define IRQ_DMA1_LEVEL		14
#define IRQ_DMA2_LEVEL		14
#define IRQ_DMA3_LEVEL		14
#define IRQ_DMA4_LEVEL		14
#define IRQ_DMA5_LEVEL		14
#define IRQ_DMA6_LEVEL		14
#define IRQ_DMA7_LEVEL		14

#define IRQ_TIMER0_LEVEL	12
#define IRQ_TIMER1_LEVEL	11
#define IRQ_TIMER2_LEVEL	10

#define IRQ_XIRQ0_LEVEL		1
#define IRQ_XIRQ1_LEVEL		2
#define IRQ_XIRQ2_LEVEL		3
#define IRQ_XIRQ3_LEVEL		4
#define IRQ_XIRQ4_LEVEL		5
#define IRQ_XIRQ5_LEVEL		6
#define IRQ_XIRQ6_LEVEL		7
#define IRQ_XIRQ7_LEVEL		8

/* IRQ IDs presented to drivers */
#define IRQ_CPU__UNUSED		IRQ_BASE_CPU
#define IRQ_CPU_UART0		(IRQ_BASE_CPU + IRQ_UART0_LEVEL)
#define IRQ_CPU_UART1		(IRQ_BASE_CPU + IRQ_UART1_LEVEL)
#define IRQ_CPU_TIMER0		(IRQ_BASE_CPU + IRQ_TIMER0_LEVEL)
#define IRQ_CPU_TIMER1		(IRQ_BASE_CPU + IRQ_TIMER1_LEVEL)
#define IRQ_CPU_TIMER2		(IRQ_BASE_CPU + IRQ_TIMER2_LEVEL)
#define IRQ_CPU_DMA0		(IRQ_BASE_CPU + IRQ_DMA0_LEVEL)
#define IRQ_CPU_DMA1		(IRQ_BASE_CPU + IRQ_DMA1_LEVEL)
#define IRQ_CPU_DMA2		(IRQ_BASE_CPU + IRQ_DMA2_LEVEL)
#define IRQ_CPU_DMA3		(IRQ_BASE_CPU + IRQ_DMA3_LEVEL)
#define IRQ_CPU_DMA4		(IRQ_BASE_CPU + IRQ_DMA4_LEVEL)
#define IRQ_CPU_DMA5		(IRQ_BASE_CPU + IRQ_DMA5_LEVEL)
#define IRQ_CPU_DMA6		(IRQ_BASE_CPU + IRQ_DMA6_LEVEL)
#define IRQ_CPU_DMA7		(IRQ_BASE_CPU + IRQ_DMA7_LEVEL)
#define IRQ_CPU_EXTERNAL0	(IRQ_BASE_CPU + IRQ_XIRQ0_LEVEL)
#define IRQ_CPU_EXTERNAL1	(IRQ_BASE_CPU + IRQ_XIRQ1_LEVEL)
#define IRQ_CPU_EXTERNAL2	(IRQ_BASE_CPU + IRQ_XIRQ2_LEVEL)
#define IRQ_CPU_EXTERNAL3	(IRQ_BASE_CPU + IRQ_XIRQ3_LEVEL)
#define IRQ_CPU_EXTERNAL4	(IRQ_BASE_CPU + IRQ_XIRQ4_LEVEL)
#define IRQ_CPU_EXTERNAL5	(IRQ_BASE_CPU + IRQ_XIRQ5_LEVEL)
#define IRQ_CPU_EXTERNAL6	(IRQ_BASE_CPU + IRQ_XIRQ6_LEVEL)
#define IRQ_CPU_EXTERNAL7	(IRQ_BASE_CPU + IRQ_XIRQ7_LEVEL)

#endif /* !__ASSEMBLY__ */

#endif /* _ASM_CPU_IRQS_H */
