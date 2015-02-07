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

/****************************************************************************
 * Driver for Solarflare Solarstorm network controllers and boards
 * Copyright 2006-2009 Solarflare Communications Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation, incorporated herein by reference.
 */

#ifndef EFX_MDIO_10G_H
#define EFX_MDIO_10G_H

#include <linux/mdio.h>

/*
 * Helper functions for doing 10G MDIO as specified in IEEE 802.3 clause 45.
 */

#include "efx.h"

static inline unsigned efx_mdio_id_rev(u32 id) { return id & 0xf; }
static inline unsigned efx_mdio_id_model(u32 id) { return (id >> 4) & 0x3f; }
extern unsigned efx_mdio_id_oui(u32 id);

static inline int efx_mdio_read(struct efx_nic *efx, int devad, int addr)
{
	return efx->mdio.mdio_read(efx->net_dev, efx->mdio.prtad, devad, addr);
}

static inline void
efx_mdio_write(struct efx_nic *efx, int devad, int addr, int value)
{
	efx->mdio.mdio_write(efx->net_dev, efx->mdio.prtad, devad, addr, value);
}

static inline u32 efx_mdio_read_id(struct efx_nic *efx, int mmd)
{
	u16 id_low = efx_mdio_read(efx, mmd, MDIO_DEVID2);
	u16 id_hi = efx_mdio_read(efx, mmd, MDIO_DEVID1);
	return (id_hi << 16) | (id_low);
}

static inline bool efx_mdio_phyxgxs_lane_sync(struct efx_nic *efx)
{
	int i, lane_status;
	bool sync;

	for (i = 0; i < 2; ++i)
		lane_status = efx_mdio_read(efx, MDIO_MMD_PHYXS,
					    MDIO_PHYXS_LNSTAT);

	sync = !!(lane_status & MDIO_PHYXS_LNSTAT_ALIGN);
	if (!sync)
		EFX_LOG(efx, "XGXS lane status: %x\n", lane_status);
	return sync;
}

extern const char *efx_mdio_mmd_name(int mmd);

/*
 * Reset a specific MMD and wait for reset to clear.
 * Return number of spins left (>0) on success, -%ETIMEDOUT on failure.
 *
 * This function will sleep
 */
extern int efx_mdio_reset_mmd(struct efx_nic *efx, int mmd,
			      int spins, int spintime);

/* As efx_mdio_check_mmd but for multiple MMDs */
int efx_mdio_check_mmds(struct efx_nic *efx,
			unsigned int mmd_mask, unsigned int fatal_mask);

/* Check the link status of specified mmds in bit mask */
extern bool efx_mdio_links_ok(struct efx_nic *efx, unsigned int mmd_mask);

/* Generic transmit disable support though PMAPMD */
extern void efx_mdio_transmit_disable(struct efx_nic *efx);

/* Generic part of reconfigure: set/clear loopback bits */
extern void efx_mdio_phy_reconfigure(struct efx_nic *efx);

/* Set the power state of the specified MMDs */
extern void efx_mdio_set_mmds_lpower(struct efx_nic *efx,
				     int low_power, unsigned int mmd_mask);

/* Set (some of) the PHY settings over MDIO */
extern int efx_mdio_set_settings(struct efx_nic *efx, struct ethtool_cmd *ecmd);

/* Push advertising flags and restart autonegotiation */
extern void efx_mdio_an_reconfigure(struct efx_nic *efx);

/* Get pause parameters from AN if available (otherwise return
 * requested pause parameters)
 */
enum efx_fc_type efx_mdio_get_pause(struct efx_nic *efx);

/* Wait for specified MMDs to exit reset within a timeout */
extern int efx_mdio_wait_reset_mmds(struct efx_nic *efx,
				    unsigned int mmd_mask);

/* Set or clear flag, debouncing */
static inline void
efx_mdio_set_flag(struct efx_nic *efx, int devad, int addr,
		  int mask, bool state)
{
	mdio_set_flag(&efx->mdio, efx->mdio.prtad, devad, addr, mask, state);
}

/* Liveness self-test for MDIO PHYs */
extern int efx_mdio_test_alive(struct efx_nic *efx);

#endif /* EFX_MDIO_10G_H */