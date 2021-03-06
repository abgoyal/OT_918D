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
 * Coldfire generic GPIO support
 *
 * (C) Copyright 2009, Steven King <sfking@fdwdc.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
*/

#include <linux/kernel.h>
#include <linux/init.h>

#include <asm/coldfire.h>
#include <asm/mcfsim.h>
#include <asm/mcfgpio.h>

static struct mcf_gpio_chip mcf_gpio_chips[] = {
	{
		.gpio_chip			= {
			.label			= "NQ",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value,
			.base			= 1,
			.ngpio			= 7,
		},
		.pddr				= MCFEPORT_EPDDR,
		.podr				= MCFEPORT_EPDR,
		.ppdr				= MCFEPORT_EPPDR,
	},
	{
		.gpio_chip			= {
			.label			= "TA",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 8,
			.ngpio			= 4,
		},
		.pddr				= MCFGPTA_GPTDDR,
		.podr				= MCFGPTA_GPTPORT,
		.ppdr				= MCFGPTB_GPTPORT,
	},
	{
		.gpio_chip			= {
			.label			= "TB",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 16,
			.ngpio			= 4,
		},
		.pddr				= MCFGPTB_GPTDDR,
		.podr				= MCFGPTB_GPTPORT,
		.ppdr				= MCFGPTB_GPTPORT,
	},
	{
		.gpio_chip			= {
			.label			= "QA",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 24,
			.ngpio			= 4,
		},
		.pddr				= MCFQADC_DDRQA,
		.podr				= MCFQADC_PORTQA,
		.ppdr				= MCFQADC_PORTQA,
	},
	{
		.gpio_chip			= {
			.label			= "QB",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 32,
			.ngpio			= 4,
		},
		.pddr				= MCFQADC_DDRQB,
		.podr				= MCFQADC_PORTQB,
		.ppdr				= MCFQADC_PORTQB,
	},
	{
		.gpio_chip			= {
			.label			= "A",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 40,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDRA,
		.podr				= MCFGPIO_PORTA,
		.ppdr				= MCFGPIO_PORTAP,
		.setr				= MCFGPIO_SETA,
		.clrr				= MCFGPIO_CLRA,
	},
	{
		.gpio_chip			= {
			.label			= "B",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 48,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDRB,
		.podr				= MCFGPIO_PORTB,
		.ppdr				= MCFGPIO_PORTBP,
		.setr				= MCFGPIO_SETB,
		.clrr				= MCFGPIO_CLRB,
	},
	{
		.gpio_chip			= {
			.label			= "C",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 56,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDRC,
		.podr				= MCFGPIO_PORTC,
		.ppdr				= MCFGPIO_PORTCP,
		.setr				= MCFGPIO_SETC,
		.clrr				= MCFGPIO_CLRC,
	},
	{
		.gpio_chip			= {
			.label			= "D",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 64,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDRD,
		.podr				= MCFGPIO_PORTD,
		.ppdr				= MCFGPIO_PORTDP,
		.setr				= MCFGPIO_SETD,
		.clrr				= MCFGPIO_CLRD,
	},
	{
		.gpio_chip			= {
			.label			= "E",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 72,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDRE,
		.podr				= MCFGPIO_PORTE,
		.ppdr				= MCFGPIO_PORTEP,
		.setr				= MCFGPIO_SETE,
		.clrr				= MCFGPIO_CLRE,
	},
	{
		.gpio_chip			= {
			.label			= "F",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 80,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDRF,
		.podr				= MCFGPIO_PORTF,
		.ppdr				= MCFGPIO_PORTFP,
		.setr				= MCFGPIO_SETF,
		.clrr				= MCFGPIO_CLRF,
	},
	{
		.gpio_chip			= {
			.label			= "G",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 88,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDRG,
		.podr				= MCFGPIO_PORTG,
		.ppdr				= MCFGPIO_PORTGP,
		.setr				= MCFGPIO_SETG,
		.clrr				= MCFGPIO_CLRG,
	},
	{
		.gpio_chip			= {
			.label			= "H",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 96,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDRH,
		.podr				= MCFGPIO_PORTH,
		.ppdr				= MCFGPIO_PORTHP,
		.setr				= MCFGPIO_SETH,
		.clrr				= MCFGPIO_CLRH,
	},
	{
		.gpio_chip			= {
			.label			= "J",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 104,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDRJ,
		.podr				= MCFGPIO_PORTJ,
		.ppdr				= MCFGPIO_PORTJP,
		.setr				= MCFGPIO_SETJ,
		.clrr				= MCFGPIO_CLRJ,
	},
	{
		.gpio_chip			= {
			.label			= "DD",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 112,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDRDD,
		.podr				= MCFGPIO_PORTDD,
		.ppdr				= MCFGPIO_PORTDDP,
		.setr				= MCFGPIO_SETDD,
		.clrr				= MCFGPIO_CLRDD,
	},
	{
		.gpio_chip			= {
			.label			= "EH",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 120,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDREH,
		.podr				= MCFGPIO_PORTEH,
		.ppdr				= MCFGPIO_PORTEHP,
		.setr				= MCFGPIO_SETEH,
		.clrr				= MCFGPIO_CLREH,
	},
	{
		.gpio_chip			= {
			.label			= "EL",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 128,
			.ngpio			= 8,
		},
		.pddr				= MCFGPIO_DDREL,
		.podr				= MCFGPIO_PORTEL,
		.ppdr				= MCFGPIO_PORTELP,
		.setr				= MCFGPIO_SETEL,
		.clrr				= MCFGPIO_CLREL,
	},
	{
		.gpio_chip			= {
			.label			= "AS",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 136,
			.ngpio			= 6,
		},
		.pddr				= MCFGPIO_DDRAS,
		.podr				= MCFGPIO_PORTAS,
		.ppdr				= MCFGPIO_PORTASP,
		.setr				= MCFGPIO_SETAS,
		.clrr				= MCFGPIO_CLRAS,
	},
	{
		.gpio_chip			= {
			.label			= "QS",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 144,
			.ngpio			= 7,
		},
		.pddr				= MCFGPIO_DDRQS,
		.podr				= MCFGPIO_PORTQS,
		.ppdr				= MCFGPIO_PORTQSP,
		.setr				= MCFGPIO_SETQS,
		.clrr				= MCFGPIO_CLRQS,
	},
	{
		.gpio_chip			= {
			.label			= "SD",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 152,
			.ngpio			= 6,
		},
		.pddr				= MCFGPIO_DDRSD,
		.podr				= MCFGPIO_PORTSD,
		.ppdr				= MCFGPIO_PORTSDP,
		.setr				= MCFGPIO_SETSD,
		.clrr				= MCFGPIO_CLRSD,
	},
	{
		.gpio_chip			= {
			.label			= "TC",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 160,
			.ngpio			= 4,
		},
		.pddr				= MCFGPIO_DDRTC,
		.podr				= MCFGPIO_PORTTC,
		.ppdr				= MCFGPIO_PORTTCP,
		.setr				= MCFGPIO_SETTC,
		.clrr				= MCFGPIO_CLRTC,
	},
	{
		.gpio_chip			= {
			.label			= "TD",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 168,
			.ngpio			= 4,
		},
		.pddr				= MCFGPIO_DDRTD,
		.podr				= MCFGPIO_PORTTD,
		.ppdr				= MCFGPIO_PORTTDP,
		.setr				= MCFGPIO_SETTD,
		.clrr				= MCFGPIO_CLRTD,
	},
	{
		.gpio_chip			= {
			.label			= "UA",
			.request		= mcf_gpio_request,
			.free			= mcf_gpio_free,
			.direction_input	= mcf_gpio_direction_input,
			.direction_output	= mcf_gpio_direction_output,
			.get			= mcf_gpio_get_value,
			.set			= mcf_gpio_set_value_fast,
			.base			= 176,
			.ngpio			= 4,
		},
		.pddr				= MCFGPIO_DDRUA,
		.podr				= MCFGPIO_PORTUA,
		.ppdr				= MCFGPIO_PORTUAP,
		.setr				= MCFGPIO_SETUA,
		.clrr				= MCFGPIO_CLRUA,
	},
};

static int __init mcf_gpio_init(void)
{
	unsigned i = 0;
	while (i < ARRAY_SIZE(mcf_gpio_chips))
		(void)gpiochip_add((struct gpio_chip *)&mcf_gpio_chips[i++]);
	return 0;
}

core_initcall(mcf_gpio_init);
