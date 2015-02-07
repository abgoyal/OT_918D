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
 *  bfa_fcs.c BFA FCS main
 */

#include <fcs/bfa_fcs.h>
#include "fcs_port.h"
#include "fcs_uf.h"
#include "fcs_vport.h"
#include "fcs_rport.h"
#include "fcs_fabric.h"
#include "fcs_fcpim.h"
#include "fcs_fcptm.h"
#include "fcbuild.h"
#include "fcs.h"
#include "bfad_drv.h"
#include <fcb/bfa_fcb.h>

/**
 * FCS sub-modules
 */
struct bfa_fcs_mod_s {
	void		(*attach) (struct bfa_fcs_s *fcs);
	void            (*modinit) (struct bfa_fcs_s *fcs);
	void            (*modexit) (struct bfa_fcs_s *fcs);
};

#define BFA_FCS_MODULE(_mod) { _mod ## _modinit, _mod ## _modexit }

static struct bfa_fcs_mod_s fcs_modules[] = {
	{ bfa_fcs_pport_attach, NULL, NULL },
	{ bfa_fcs_uf_attach, NULL, NULL },
	{ bfa_fcs_fabric_attach, bfa_fcs_fabric_modinit,
	 bfa_fcs_fabric_modexit },
};

/**
 *  fcs_api BFA FCS API
 */

static void
bfa_fcs_exit_comp(void *fcs_cbarg)
{
	struct bfa_fcs_s *fcs = fcs_cbarg;
	struct bfad_s *bfad = fcs->bfad;

	complete(&bfad->comp);
}



/**
 *  fcs_api BFA FCS API
 */

/**
 * fcs attach -- called once to initialize data structures at driver attach time
 */
void
bfa_fcs_attach(struct bfa_fcs_s *fcs, struct bfa_s *bfa, struct bfad_s *bfad,
			bfa_boolean_t min_cfg)
{
	int             i;
	struct bfa_fcs_mod_s  *mod;

	fcs->bfa = bfa;
	fcs->bfad = bfad;
	fcs->min_cfg = min_cfg;

	bfa_attach_fcs(bfa);
	fcbuild_init();

	for (i = 0; i < sizeof(fcs_modules) / sizeof(fcs_modules[0]); i++) {
		mod = &fcs_modules[i];
		if (mod->attach)
			mod->attach(fcs);
	}
}

/**
 * fcs initialization, called once after bfa initialization is complete
 */
void
bfa_fcs_init(struct bfa_fcs_s *fcs)
{
	int             i;
	struct bfa_fcs_mod_s  *mod;

	for (i = 0; i < sizeof(fcs_modules) / sizeof(fcs_modules[0]); i++) {
		mod = &fcs_modules[i];
		if (mod->modinit)
			mod->modinit(fcs);
	}
}

/**
 * Start FCS operations.
 */
void
bfa_fcs_start(struct bfa_fcs_s *fcs)
{
	bfa_fcs_fabric_modstart(fcs);
}

/**
 * 		FCS driver details initialization.
 *
 * 	param[in]		fcs		FCS instance
 * 	param[in]		driver_info	Driver Details
 *
 * 	return None
 */
void
bfa_fcs_driver_info_init(struct bfa_fcs_s *fcs,
			struct bfa_fcs_driver_info_s *driver_info)
{

	fcs->driver_info = *driver_info;

	bfa_fcs_fabric_psymb_init(&fcs->fabric);
}

/**
 *      @brief
 *              FCS FDMI Driver Parameter Initialization
 *
 *      @param[in]              fcs             FCS instance
 *      @param[in]              fdmi_enable     TRUE/FALSE
 *
 *      @return None
 */
void
bfa_fcs_set_fdmi_param(struct bfa_fcs_s *fcs, bfa_boolean_t fdmi_enable)
{

	fcs->fdmi_enabled = fdmi_enable;

}

/**
 * 		FCS instance cleanup and exit.
 *
 * 	param[in]		fcs			FCS instance
 * 	return None
 */
void
bfa_fcs_exit(struct bfa_fcs_s *fcs)
{
	struct bfa_fcs_mod_s  *mod;
	int             nmods, i;

	bfa_wc_init(&fcs->wc, bfa_fcs_exit_comp, fcs);

	nmods = sizeof(fcs_modules) / sizeof(fcs_modules[0]);

	for (i = 0; i < nmods; i++) {

		mod = &fcs_modules[i];
		if (mod->modexit) {
			bfa_wc_up(&fcs->wc);
			mod->modexit(fcs);
		}
	}

	bfa_wc_wait(&fcs->wc);
}


void
bfa_fcs_trc_init(struct bfa_fcs_s *fcs, struct bfa_trc_mod_s *trcmod)
{
	fcs->trcmod = trcmod;
}


void
bfa_fcs_log_init(struct bfa_fcs_s *fcs, struct bfa_log_mod_s *logmod)
{
	fcs->logm = logmod;
}


void
bfa_fcs_aen_init(struct bfa_fcs_s *fcs, struct bfa_aen_s *aen)
{
	fcs->aen = aen;
}

void
bfa_fcs_modexit_comp(struct bfa_fcs_s *fcs)
{
	bfa_wc_down(&fcs->wc);
}

