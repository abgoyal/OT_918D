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
 * Copyright (C) 2004,2005  ADDI-DATA GmbH for the source code of this module.
 *
 *	ADDI-DATA GmbH
 *	Dieselstrasse 3
 *	D-77833 Ottersweier
 *	Tel: +19(0)7223/9493-0
 *	Fax: +49(0)7223/9493-92
 *	http://www.addi-data-com
 *	info@addi-data.com
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 */

/*********      Definitions for APCI-2200 card  *****/

/* Card Specific information */
#define APCI2200_BOARD_VENDOR_ID                 0x15b8
#define APCI2200_ADDRESS_RANGE                   64

/* DIGITAL INPUT-OUTPUT DEFINE */

#define APCI2200_DIGITAL_OP                 	4
#define APCI2200_DIGITAL_IP                     0

/* TIMER COUNTER WATCHDOG DEFINES */

#define APCI2200_WATCHDOG                          0x08
#define APCI2200_WATCHDOG_ENABLEDISABLE            12
#define APCI2200_WATCHDOG_RELOAD_VALUE             4
#define APCI2200_WATCHDOG_STATUS                   16

/* Hardware Layer  functions for Apci2200 */

/* Digital Input */
int i_APCI2200_ReadMoreDigitalInput(struct comedi_device *dev, struct comedi_subdevice *s,
				    struct comedi_insn *insn, unsigned int *data);
int i_APCI2200_Read1DigitalInput(struct comedi_device *dev, struct comedi_subdevice *s,
				 struct comedi_insn *insn, unsigned int *data);

/* Digital Output */
int i_APCI2200_ConfigDigitalOutput(struct comedi_device *dev, struct comedi_subdevice *s,
				   struct comedi_insn *insn, unsigned int *data);
int i_APCI2200_WriteDigitalOutput(struct comedi_device *dev, struct comedi_subdevice *s,
				  struct comedi_insn *insn, unsigned int *data);
int i_APCI2200_ReadDigitalOutput(struct comedi_device *dev, struct comedi_subdevice *s,
				 struct comedi_insn *insn, unsigned int *data);

/* TIMER */
int i_APCI2200_ConfigWatchdog(struct comedi_device *dev, struct comedi_subdevice *s,
			      struct comedi_insn *insn, unsigned int *data);
int i_APCI2200_StartStopWriteWatchdog(struct comedi_device *dev, struct comedi_subdevice *s,
				      struct comedi_insn *insn, unsigned int *data);
int i_APCI2200_ReadWatchdog(struct comedi_device *dev, struct comedi_subdevice *s,
			    struct comedi_insn *insn, unsigned int *data);

/* reset */
int i_APCI2200_Reset(struct comedi_device *dev);