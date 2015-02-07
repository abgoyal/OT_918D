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
 * linux/drivers/video/nvidia/nv_of.c
 *
 * Copyright 2004 Antonino A. Daplas <adaplas @pol.net>
 *
 * Based on rivafb-i2c.c
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/gfp.h>
#include <linux/pci.h>
#include <linux/fb.h>

#include <asm/io.h>

#include <asm/prom.h>
#include <asm/pci-bridge.h>

#include "nv_type.h"
#include "nv_local.h"
#include "nv_proto.h"

#include "../edid.h"

int nvidia_probe_of_connector(struct fb_info *info, int conn, u8 **out_edid)
{
	struct nvidia_par *par = info->par;
	struct device_node *parent, *dp;
	const unsigned char *pedid = NULL;
	static char *propnames[] = {
		"DFP,EDID", "LCD,EDID", "EDID", "EDID1",
		"EDID,B", "EDID,A", NULL };
	int i;

	parent = pci_device_to_OF_node(par->pci_dev);
	if (parent == NULL)
		return -1;
	if (par->twoHeads) {
		const char *pname;
		int len;

		for (dp = NULL;
		     (dp = of_get_next_child(parent, dp)) != NULL;) {
			pname = of_get_property(dp, "name", NULL);
			if (!pname)
				continue;
			len = strlen(pname);
			if ((pname[len-1] == 'A' && conn == 1) ||
			    (pname[len-1] == 'B' && conn == 2)) {
				for (i = 0; propnames[i] != NULL; ++i) {
					pedid = of_get_property(dp,
							propnames[i], NULL);
					if (pedid != NULL)
						break;
				}
				of_node_put(dp);
				break;
			}
		}
	}
	if (pedid == NULL) {
		for (i = 0; propnames[i] != NULL; ++i) {
			pedid = of_get_property(parent, propnames[i], NULL);
			if (pedid != NULL)
				break;
		}
	}
	if (pedid) {
		*out_edid = kmemdup(pedid, EDID_LENGTH, GFP_KERNEL);
		if (*out_edid == NULL)
			return -1;
		printk(KERN_DEBUG "nvidiafb: Found OF EDID for head %d\n", conn);
		return 0;
	}
	return -1;
}
