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
 * arch/arm/plat-omap/include/mach/board-ams-delta.h
 *
 * Copyright (C) 2006 Jonathan McDowell <noodles@earth.li>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN
 * NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 * USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#ifndef __ASM_ARCH_OMAP_AMS_DELTA_H
#define __ASM_ARCH_OMAP_AMS_DELTA_H

#if defined (CONFIG_MACH_AMS_DELTA)

#define AMS_DELTA_LATCH1_PHYS		0x01000000
#define AMS_DELTA_LATCH1_VIRT		0xEA000000
#define AMS_DELTA_MODEM_PHYS		0x04000000
#define AMS_DELTA_MODEM_VIRT		0xEB000000
#define AMS_DELTA_LATCH2_PHYS		0x08000000
#define AMS_DELTA_LATCH2_VIRT		0xEC000000

#define AMS_DELTA_LATCH1_LED_CAMERA	0x01
#define AMS_DELTA_LATCH1_LED_ADVERT	0x02
#define AMS_DELTA_LATCH1_LED_EMAIL	0x04
#define AMS_DELTA_LATCH1_LED_HANDSFREE	0x08
#define AMS_DELTA_LATCH1_LED_VOICEMAIL	0x10
#define AMS_DELTA_LATCH1_LED_VOICE	0x20

#define AMS_DELTA_LATCH2_LCD_VBLEN	0x0001
#define AMS_DELTA_LATCH2_LCD_NDISP	0x0002
#define AMS_DELTA_LATCH2_NAND_NCE	0x0004
#define AMS_DELTA_LATCH2_NAND_NRE	0x0008
#define AMS_DELTA_LATCH2_NAND_NWP	0x0010
#define AMS_DELTA_LATCH2_NAND_NWE	0x0020
#define AMS_DELTA_LATCH2_NAND_ALE	0x0040
#define AMS_DELTA_LATCH2_NAND_CLE	0x0080
#define AMD_DELTA_LATCH2_KEYBRD_PWR	0x0100
#define AMD_DELTA_LATCH2_KEYBRD_DATA	0x0200
#define AMD_DELTA_LATCH2_SCARD_RSTIN	0x0400
#define AMD_DELTA_LATCH2_SCARD_CMDVCC	0x0800
#define AMS_DELTA_LATCH2_MODEM_NRESET	0x1000
#define AMS_DELTA_LATCH2_MODEM_CODEC	0x2000

#define AMS_DELTA_GPIO_PIN_KEYBRD_DATA	0
#define AMS_DELTA_GPIO_PIN_KEYBRD_CLK	1
#define AMS_DELTA_GPIO_PIN_MODEM_IRQ	2
#define AMS_DELTA_GPIO_PIN_HOOK_SWITCH	4
#define AMS_DELTA_GPIO_PIN_SCARD_NOFF	6
#define AMS_DELTA_GPIO_PIN_SCARD_IO	7
#define AMS_DELTA_GPIO_PIN_CONFIG	11
#define AMS_DELTA_GPIO_PIN_NAND_RB	12

#ifndef __ASSEMBLY__
void ams_delta_latch1_write(u8 mask, u8 value);
void ams_delta_latch2_write(u16 mask, u16 value);
#endif

#endif /* CONFIG_MACH_AMS_DELTA */

#endif /* __ASM_ARCH_OMAP_AMS_DELTA_H */
