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
 * arch/arm/mach-sa1100/include/mach/neponset.h
 *
 * Created 2000/06/05 by Nicolas Pitre <nico@fluxnic.net>
 *
 * This file contains the hardware specific definitions for Assabet
 * Only include this file from SA1100-specific files.
 *
 * 2000/05/23 John Dorsey <john+@cs.cmu.edu>
 *      Definitions for Neponset added.
 */
#ifndef __ASM_ARCH_NEPONSET_H
#define __ASM_ARCH_NEPONSET_H

/*
 * Neponset definitions: 
 */

#define NEPONSET_CPLD_BASE      (0x10000000)
#define Nep_p2v( x )            ((x) - NEPONSET_CPLD_BASE + 0xf3000000)
#define Nep_v2p( x )            ((x) - 0xf3000000 + NEPONSET_CPLD_BASE)

#define _IRR                    0x10000024      /* Interrupt Reason Register */
#define _AUD_CTL                0x100000c0      /* Audio controls (RW)       */
#define _MDM_CTL_0              0x100000b0      /* Modem control 0 (RW)      */
#define _MDM_CTL_1              0x100000b4      /* Modem control 1 (RW)      */
#define _NCR_0	                0x100000a0      /* Control Register (RW)     */
#define _KP_X_OUT               0x10000090      /* Keypad row write (RW)     */
#define _KP_Y_IN                0x10000080      /* Keypad column read (RO)   */
#define _SWPK                   0x10000020      /* Switch pack (RO)          */
#define _WHOAMI                 0x10000000      /* System ID Register (RO)   */

#define _LEDS                   0x10000010      /* LEDs [31:0] (WO)          */

#define IRR                     (*((volatile u_char *) Nep_p2v(_IRR)))
#define AUD_CTL                 (*((volatile u_char *) Nep_p2v(_AUD_CTL)))
#define MDM_CTL_0               (*((volatile u_char *) Nep_p2v(_MDM_CTL_0)))
#define MDM_CTL_1               (*((volatile u_char *) Nep_p2v(_MDM_CTL_1)))
#define NCR_0			(*((volatile u_char *) Nep_p2v(_NCR_0)))
#define KP_X_OUT                (*((volatile u_char *) Nep_p2v(_KP_X_OUT)))
#define KP_Y_IN                 (*((volatile u_char *) Nep_p2v(_KP_Y_IN)))
#define SWPK                    (*((volatile u_char *) Nep_p2v(_SWPK)))
#define WHOAMI                  (*((volatile u_char *) Nep_p2v(_WHOAMI)))

#define LEDS                    (*((volatile Word   *) Nep_p2v(_LEDS)))

#define IRR_ETHERNET		(1<<0)
#define IRR_USAR		(1<<1)
#define IRR_SA1111		(1<<2)

#define AUD_SEL_1341            (1<<0)
#define AUD_MUTE_1341           (1<<1)

#define MDM_CTL0_RTS1		(1 << 0)
#define MDM_CTL0_DTR1		(1 << 1)
#define MDM_CTL0_RTS2		(1 << 2)
#define MDM_CTL0_DTR2		(1 << 3)

#define MDM_CTL1_CTS1		(1 << 0)
#define MDM_CTL1_DSR1		(1 << 1)
#define MDM_CTL1_DCD1		(1 << 2)
#define MDM_CTL1_CTS2		(1 << 3)
#define MDM_CTL1_DSR2		(1 << 4)
#define MDM_CTL1_DCD2		(1 << 5)

#define NCR_GP01_OFF		(1<<0)
#define NCR_TP_PWR_EN		(1<<1)
#define NCR_MS_PWR_EN		(1<<2)
#define NCR_ENET_OSC_EN		(1<<3)
#define NCR_SPI_KB_WK_UP	(1<<4)
#define NCR_A0VPP		(1<<5)
#define NCR_A1VPP		(1<<6)

#endif
