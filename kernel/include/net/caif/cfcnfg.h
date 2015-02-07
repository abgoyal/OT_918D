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
 * Copyright (C) ST-Ericsson AB 2010
 * Author:	Sjur Brendeland/sjur.brandeland@stericsson.com
 * License terms: GNU General Public License (GPL) version 2
 */

#ifndef CFCNFG_H_
#define CFCNFG_H_
#include <linux/spinlock.h>
#include <net/caif/caif_layer.h>
#include <net/caif/cfctrl.h>

struct cfcnfg;

/**
 * enum cfcnfg_phy_type -  Types of physical layers defined in CAIF Stack
 *
 * @CFPHYTYPE_FRAG:	Fragmented frames physical interface.
 * @CFPHYTYPE_CAIF:	Generic CAIF physical interface
 */
enum cfcnfg_phy_type {
	CFPHYTYPE_FRAG = 1,
	CFPHYTYPE_CAIF,
	CFPHYTYPE_MAX
};

/**
 * enum cfcnfg_phy_preference - Physical preference HW Abstraction
 *
 * @CFPHYPREF_UNSPECIFIED:	Default physical interface
 *
 * @CFPHYPREF_LOW_LAT:		Default physical interface for low-latency
 *				traffic
 * @CFPHYPREF_HIGH_BW:		Default physical interface for high-bandwidth
 *				traffic
 * @CFPHYPREF_LOOP:		TEST only Loopback interface simulating modem
 *				responses.
 *
 */
enum cfcnfg_phy_preference {
	CFPHYPREF_UNSPECIFIED,
	CFPHYPREF_LOW_LAT,
	CFPHYPREF_HIGH_BW,
	CFPHYPREF_LOOP
};

/**
 * cfcnfg_create() - Create the CAIF configuration object.
 */
struct cfcnfg *cfcnfg_create(void);

/**
 * cfcnfg_remove() -  Remove the CFCNFG object
 * @cfg: config object
 */
void cfcnfg_remove(struct cfcnfg *cfg);

/**
 * cfcnfg_add_phy_layer() - Adds a physical layer to the CAIF stack.
 * @cnfg:	Pointer to a CAIF configuration object, created by
 *		cfcnfg_create().
 * @phy_type:	Specifies the type of physical interface, e.g.
 *			CFPHYTYPE_FRAG.
 * @dev:	Pointer to link layer device
 * @phy_layer:	Specify the physical layer. The transmit function
 *		MUST be set in the structure.
 * @phyid:	The assigned physical ID for this layer, used in
 *		cfcnfg_add_adapt_layer to specify PHY for the link.
 * @pref:	The phy (link layer) preference.
 * @fcs:	Specify if checksum is used in CAIF Framing Layer.
 * @stx:	Specify if Start Of Frame eXtention is used.
 */

void
cfcnfg_add_phy_layer(struct cfcnfg *cnfg, enum cfcnfg_phy_type phy_type,
		     void *dev, struct cflayer *phy_layer, u16 *phyid,
		     enum cfcnfg_phy_preference pref,
		     bool fcs, bool stx);

/**
 * cfcnfg_del_phy_layer - Deletes an phy layer from the CAIF stack.
 *
 * @cnfg:	Pointer to a CAIF configuration object, created by
 *		cfcnfg_create().
 * @phy_layer:	Adaptation layer to be removed.
 */
int cfcnfg_del_phy_layer(struct cfcnfg *cnfg, struct cflayer *phy_layer);

/**
 * cfcnfg_disconn_adapt_layer - Disconnects an adaptation layer.
 *
 * @cnfg:	Pointer to a CAIF configuration object, created by
 *		cfcnfg_create().
 * @adap_layer: Adaptation layer to be removed.
 */
int cfcnfg_disconn_adapt_layer(struct cfcnfg *cnfg,
			struct cflayer *adap_layer);

/**
 * cfcnfg_release_adap_layer - Used by client to release the adaptation layer.
 *
 * @adap_layer: Adaptation layer.
 */
void cfcnfg_release_adap_layer(struct cflayer *adap_layer);

/**
 * cfcnfg_add_adaptation_layer - Add an adaptation layer to the CAIF stack.
 *
 * The adaptation Layer is where the interface to application or higher-level
 * driver functionality is implemented.
 *
 * @cnfg:		Pointer to a CAIF configuration object, created by
 *			cfcnfg_create().
 * @param:		Link setup parameters.
 * @adap_layer:		Specify the adaptation layer; the receive and
 *			flow-control functions MUST be set in the structure.
 *
 */
int cfcnfg_add_adaptation_layer(struct cfcnfg *cnfg,
			    struct cfctrl_link_param *param,
			    struct cflayer *adap_layer);

/**
 * cfcnfg_get_phyid() - Get physical ID, given type.
 * Returns one of the physical interfaces matching the given type.
 * Zero if no match is found.
 * @cnfg:	Configuration object
 * @phy_pref:	Caif Link Layer preference
 */
struct dev_info *cfcnfg_get_phyid(struct cfcnfg *cnfg,
		     enum cfcnfg_phy_preference phy_pref);

/**
 * cfcnfg_get_named() - Get the Physical Identifier of CAIF Link Layer
 * @cnfg:	Configuration object
 * @name:	Name of the Physical Layer (Caif Link Layer)
 */
int cfcnfg_get_named(struct cfcnfg *cnfg, char *name);

#endif				/* CFCNFG_H_ */
