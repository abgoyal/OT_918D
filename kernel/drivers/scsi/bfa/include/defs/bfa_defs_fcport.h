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
 *  bfa_defs_fcport.h
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
#ifndef __BFA_DEFS_FCPORT_H__
#define __BFA_DEFS_FCPORT_H__

#include <defs/bfa_defs_types.h>
#include <protocol/types.h>

#pragma pack(1)

/**
 * FCoE statistics
 */
struct bfa_fcoe_stats_s {
	u64	secs_reset;	/*  Seconds since stats reset	     */
	u64	cee_linkups;	/*  CEE link up		     */
	u64	cee_linkdns;	/*  CEE link down		     */
	u64	fip_linkups;	/*  FIP link up		     */
	u64	fip_linkdns;	/*  FIP link down		     */
	u64	fip_fails;	/*  FIP failures		     */
	u64	mac_invalids;	/*  Invalid mac assignments	     */
	u64	vlan_req;	/*  Vlan requests		     */
	u64	vlan_notify;	/*  Vlan notifications		     */
	u64	vlan_err;	/*  Vlan notification errors	     */
	u64	vlan_timeouts;	/*  Vlan request timeouts	     */
	u64	vlan_invalids;	/*  Vlan invalids		     */
	u64	disc_req;	/*  Discovery requests		     */
	u64	disc_rsp;	/*  Discovery responses	     */
	u64	disc_err;	/*  Discovery error frames	     */
	u64	disc_unsol;	/*  Discovery unsolicited	     */
	u64	disc_timeouts;	/*  Discovery timeouts		     */
	u64	disc_fcf_unavail; /*  Discovery FCF not avail	     */
	u64	linksvc_unsupp;	/*  FIP link service req unsupp.    */
	u64	linksvc_err;	/*  FIP link service req errors     */
	u64	logo_req;	/*  FIP logo			     */
	u64	clrvlink_req;	/*  Clear virtual link requests     */
	u64	op_unsupp;	/*  FIP operation unsupp.	     */
	u64	untagged;	/*  FIP untagged frames	     */
	u64	txf_ucast;	/*  Tx FCoE unicast frames	     */
	u64	txf_ucast_vlan;	/*  Tx FCoE unicast vlan frames     */
	u64	txf_ucast_octets; /*  Tx FCoE unicast octets	     */
	u64	txf_mcast;	/*  Tx FCoE mutlicast frames	     */
	u64	txf_mcast_vlan;	/*  Tx FCoE mutlicast vlan frames   */
	u64	txf_mcast_octets; /*  Tx FCoE multicast octets	     */
	u64	txf_bcast;	/*  Tx FCoE broadcast frames	     */
	u64	txf_bcast_vlan;	/*  Tx FCoE broadcast vlan frames   */
	u64	txf_bcast_octets; /*  Tx FCoE broadcast octets	     */
	u64	txf_timeout;	/*  Tx timeouts		     */
	u64	txf_parity_errors; /*  Transmit parity err	     */
	u64	txf_fid_parity_errors; /*  Transmit FID parity err  */
	u64	tx_pause;	/*  Tx pause frames		     */
	u64	tx_zero_pause;	/*  Tx zero pause frames	     */
	u64	tx_first_pause;	/*  Tx first pause frames	     */
	u64	rx_pause;	/*  Rx pause frames		     */
	u64	rx_zero_pause;	/*  Rx zero pause frames	     */
	u64	rx_first_pause;	/*  Rx first pause frames	     */
	u64	rxf_ucast_octets; /*  Rx unicast octets	     */
	u64	rxf_ucast;	/*  Rx unicast frames		     */
	u64	rxf_ucast_vlan;	/*  Rx unicast vlan frames	     */
	u64	rxf_mcast_octets; /*  Rx multicast octets	     */
	u64	rxf_mcast;	/*  Rx multicast frames	     */
	u64	rxf_mcast_vlan;	/*  Rx multicast vlan frames	     */
	u64	rxf_bcast_octets; /*  Rx broadcast octests	     */
	u64	rxf_bcast;	/*  Rx broadcast frames	     */
	u64	rxf_bcast_vlan;	/*  Rx broadcast vlan frames	     */
};

/**
 * QoS or FCoE stats (fcport stats excluding physical FC port stats)
 */
union bfa_fcport_stats_u {
	struct bfa_qos_stats_s	fcqos;
	struct bfa_fcoe_stats_s	fcoe;
};

#pragma pack()

#endif  /* __BFA_DEFS_FCPORT_H__ */
