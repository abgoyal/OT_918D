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
 * Copyright 1998-2008 VIA Technologies, Inc. All Rights Reserved.
 * Copyright 2001-2008 S3 Graphics, Inc. All Rights Reserved.

 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTIES OR REPRESENTATIONS; without even
 * the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE.See the GNU General Public License
 * for more details.

 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc.,
 * 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */

#ifndef __VIAFBDEV_H__
#define __VIAFBDEV_H__

#include <linux/proc_fs.h>
#include <linux/fb.h>
#include <linux/spinlock.h>

#include "ioctl.h"
#include "share.h"
#include "chip.h"
#include "hw.h"

#define VERSION_MAJOR       2
#define VERSION_KERNEL      6	/* For kernel 2.6 */

#define VERSION_OS          0	/* 0: for 32 bits OS, 1: for 64 bits OS */
#define VERSION_MINOR       4

#define VIAFB_NUM_I2C		5

struct viafb_shared {
	struct proc_dir_entry *proc_entry;	/*viafb proc entry */
	struct viafb_dev *vdev;			/* Global dev info */

	/* All the information will be needed to set engine */
	struct tmds_setting_information tmds_setting_info;
	struct crt_setting_information crt_setting_info;
	struct lvds_setting_information lvds_setting_info;
	struct lvds_setting_information lvds_setting_info2;
	struct chip_information chip_info;

	/* hardware acceleration stuff */
	u32 cursor_vram_addr;
	u32 vq_vram_addr;	/* virtual queue address in video ram */
	int (*hw_bitblt)(void __iomem *engine, u8 op, u32 width, u32 height,
		u8 dst_bpp, u32 dst_addr, u32 dst_pitch, u32 dst_x, u32 dst_y,
		u32 *src_mem, u32 src_addr, u32 src_pitch, u32 src_x, u32 src_y,
		u32 fg_color, u32 bg_color, u8 fill_rop);
};

struct viafb_par {
	u8 depth;
	u32 vram_addr;

	unsigned int fbmem;	/*framebuffer physical memory address */
	unsigned int memsize;	/*size of fbmem */
	u32 fbmem_free;		/* Free FB memory */
	u32 fbmem_used;		/* Use FB memory size */
	u32 iga_path;

	struct viafb_shared *shared;

	/* All the information will be needed to set engine */
	/* depreciated, use the ones in shared directly */
	struct tmds_setting_information *tmds_setting_info;
	struct crt_setting_information *crt_setting_info;
	struct lvds_setting_information *lvds_setting_info;
	struct lvds_setting_information *lvds_setting_info2;
	struct chip_information *chip_info;
};

extern unsigned int viafb_second_virtual_yres;
extern unsigned int viafb_second_virtual_xres;
extern int viafb_SAMM_ON;
extern int viafb_dual_fb;
extern int viafb_LCD2_ON;
extern int viafb_LCD_ON;
extern int viafb_DVI_ON;
extern int viafb_hotplug;

extern int strict_strtoul(const char *cp, unsigned int base,
	unsigned long *res);

u8 viafb_gpio_i2c_read_lvds(struct lvds_setting_information
	*plvds_setting_info, struct lvds_chip_information
	*plvds_chip_info, u8 index);
void viafb_gpio_i2c_write_mask_lvds(struct lvds_setting_information
			      *plvds_setting_info, struct lvds_chip_information
			      *plvds_chip_info, struct IODATA io_data);
int via_fb_pci_probe(struct viafb_dev *vdev);
void via_fb_pci_remove(struct pci_dev *pdev);
/* Temporary */
int viafb_init(void);
void viafb_exit(void);
#endif /* __VIAFBDEV_H__ */
