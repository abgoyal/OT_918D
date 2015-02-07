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
 * Copyright (c) 2005-2009 Brocade Communications Systems, Inc.
 * All rights reserved
 * www.brocade.com
 *
 * Linux driver for Brocade Fibre Channel Host Bus Adapter.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License (GPL) Version 2 as
 * published by the Free Software Foundation
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 */

/**
* : bfad_fcpim.h - BFA FCS initiator mode remote port callbacks
 */

#ifndef __BFAD_FCB_FCPIM_H__
#define __BFAD_FCB_FCPIM_H__

struct bfad_itnim_s;

/*
 * RPIM callbacks
 */

/**
 * 	Memory allocation for remote port instance. Called before PRLI is
 * 	initiated to the remote target port.
 *
 * @param[in] bfad		- driver instance
 * @param[out] itnim		- FCS remote port (IM) instance
 * @param[out] itnim_drv	- driver remote port (IM) instance
 *
 * @return None
 */
void bfa_fcb_itnim_alloc(struct bfad_s *bfad, struct bfa_fcs_itnim_s **itnim,
				    struct bfad_itnim_s **itnim_drv);

/**
 * 		Free remote port (IM) instance.
 *
 * @param[in] bfad	- driver instance
 * @param[in] itnim_drv	- driver remote port instance
 *
 * @return None
 */
void            bfa_fcb_itnim_free(struct bfad_s *bfad,
				   struct bfad_itnim_s *itnim_drv);

/**
 * 	Notification of when login with a remote target device is complete.
 *
 * @param[in] itnim_drv	- driver remote port instance
 *
 * @return None
 */
void            bfa_fcb_itnim_online(struct bfad_itnim_s *itnim_drv);

/**
 * 	Notification when login with the remote device is severed.
 *
 * @param[in] itnim_drv	- driver remote port instance
 *
 * @return None
 */
void            bfa_fcb_itnim_offline(struct bfad_itnim_s *itnim_drv);

void            bfa_fcb_itnim_tov(struct bfad_itnim_s *itnim_drv);

#endif /* __BFAD_FCB_FCPIM_H__ */
