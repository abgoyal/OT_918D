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

/* -*- mode: c; c-basic-offset: 8; -*-
 * vim: noexpandtab sw=8 ts=8 sts=0:
 *
 * ocfs1_fs_compat.h
 *
 * OCFS1 volume header definitions.  OCFS2 creates valid but unmountable
 * OCFS1 volume headers on the first two sectors of an OCFS2 volume.
 * This allows an OCFS1 volume to see the partition and cleanly fail to
 * mount it.
 *
 * Copyright (C) 2002, 2004 Oracle.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License, version 2,  as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#ifndef _OCFS1_FS_COMPAT_H
#define _OCFS1_FS_COMPAT_H

#define OCFS1_MAX_VOL_SIGNATURE_LEN          128
#define OCFS1_MAX_MOUNT_POINT_LEN            128
#define OCFS1_MAX_VOL_ID_LENGTH               16
#define OCFS1_MAX_VOL_LABEL_LEN               64
#define OCFS1_MAX_CLUSTER_NAME_LEN            64

#define OCFS1_MAJOR_VERSION              (2)
#define OCFS1_MINOR_VERSION              (0)
#define OCFS1_VOLUME_SIGNATURE		 "OracleCFS"

/*
 * OCFS1 superblock.  Lives at sector 0.
 */
struct ocfs1_vol_disk_hdr
{
/*00*/	__u32 minor_version;
	__u32 major_version;
/*08*/	__u8 signature[OCFS1_MAX_VOL_SIGNATURE_LEN];
/*88*/	__u8 mount_point[OCFS1_MAX_MOUNT_POINT_LEN];
/*108*/	__u64 serial_num;
/*110*/	__u64 device_size;
	__u64 start_off;
/*120*/	__u64 bitmap_off;
	__u64 publ_off;
/*130*/	__u64 vote_off;
	__u64 root_bitmap_off;
/*140*/	__u64 data_start_off;
	__u64 root_bitmap_size;
/*150*/	__u64 root_off;
	__u64 root_size;
/*160*/	__u64 cluster_size;
	__u64 num_nodes;
/*170*/	__u64 num_clusters;
	__u64 dir_node_size;
/*180*/	__u64 file_node_size;
	__u64 internal_off;
/*190*/	__u64 node_cfg_off;
	__u64 node_cfg_size;
/*1A0*/	__u64 new_cfg_off;
	__u32 prot_bits;
	__s32 excl_mount;
/*1B0*/
};


struct ocfs1_disk_lock
{
/*00*/	__u32 curr_master;
	__u8 file_lock;
	__u8 compat_pad[3];  /* Not in original definition.  Used to
				make the already existing alignment
				explicit */
	__u64 last_write_time;
/*10*/	__u64 last_read_time;
	__u32 writer_node_num;
	__u32 reader_node_num;
/*20*/	__u64 oin_node_map;
	__u64 dlock_seq_num;
/*30*/
};

/*
 * OCFS1 volume label.  Lives at sector 1.
 */
struct ocfs1_vol_label
{
/*00*/	struct ocfs1_disk_lock disk_lock;
/*30*/	__u8 label[OCFS1_MAX_VOL_LABEL_LEN];
/*70*/	__u16 label_len;
/*72*/	__u8 vol_id[OCFS1_MAX_VOL_ID_LENGTH];
/*82*/	__u16 vol_id_len;
/*84*/	__u8 cluster_name[OCFS1_MAX_CLUSTER_NAME_LEN];
/*A4*/	__u16 cluster_name_len;
/*A6*/
};


#endif /* _OCFS1_FS_COMPAT_H */
