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

/* GSPCA subdrivers for Genesys Logic webcams with the GL860 chip
 * Subdriver declarations
 *
 * 2009/10/14 Olivier LORIN <o.lorin@laposte.net>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef GL860_DEV_H
#define GL860_DEV_H
#include <linux/version.h>

#include "gspca.h"

#define MODULE_NAME "gspca_gl860"
#define DRIVER_VERSION "0.9d10"

#define ctrl_in  gl860_RTx
#define ctrl_out gl860_RTx

#define ID_MI1320   1
#define ID_OV2640   2
#define ID_OV9655   4
#define ID_MI2020   8
#define ID_MI2020b 16

#define _MI1320_  (((struct sd *) gspca_dev)->sensor == ID_MI1320)
#define _MI2020_  (((struct sd *) gspca_dev)->sensor == ID_MI2020)
#define _MI2020b_ (((struct sd *) gspca_dev)->sensor == ID_MI2020b)
#define _MI2020c_ 0
#define _OV2640_  (((struct sd *) gspca_dev)->sensor == ID_OV2640)
#define _OV9655_  (((struct sd *) gspca_dev)->sensor == ID_OV9655)

#define IMAGE_640   0
#define IMAGE_800   1
#define IMAGE_1280  2
#define IMAGE_1600 3

struct sd_gl860 {
	u16 backlight;
	u16 brightness;
	u16 sharpness;
	u16 contrast;
	u16 gamma;
	u16 hue;
	u16 saturation;
	u16 whitebal;
	u8  mirror;
	u8  flip;
	u8  AC50Hz;
};

/* Specific webcam descriptor */
struct sd {
	struct gspca_dev gspca_dev;	/* !! must be the first item */

	struct sd_gl860 vcur;
	struct sd_gl860 vold;
	struct sd_gl860 vmax;

	int  (*dev_configure_alt)  (struct gspca_dev *);
	int  (*dev_init_at_startup)(struct gspca_dev *);
	int  (*dev_init_pre_alt)   (struct gspca_dev *);
	void (*dev_post_unset_alt) (struct gspca_dev *);
	int  (*dev_camera_settings)(struct gspca_dev *);

	u8   swapRB;
	u8  mirrorMask;
	u8  sensor;
	s32 nbIm;
	s32 nbRightUp;
	u8   waitSet;
};

struct validx {
	u16 val;
	u16 idx;
};

struct idxdata {
	u8 idx;
	u8 data[3];
};

int fetch_validx(struct gspca_dev *gspca_dev, struct validx *tbl, int len);
int keep_on_fetching_validx(struct gspca_dev *gspca_dev, struct validx *tbl,
				int len, int n);
void fetch_idxdata(struct gspca_dev *gspca_dev, struct idxdata *tbl, int len);

int gl860_RTx(struct gspca_dev *gspca_dev,
			unsigned char pref, u32 req, u16 val, u16 index,
			s32 len, void *pdata);

void mi1320_init_settings(struct gspca_dev *);
void ov2640_init_settings(struct gspca_dev *);
void ov9655_init_settings(struct gspca_dev *);
void mi2020_init_settings(struct gspca_dev *);

#endif
