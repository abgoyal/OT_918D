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

#ifndef __BFAD_IM_H__
#define __BFAD_IM_H__

#include "fcs/bfa_fcs_fcpim.h"
#include "bfad_im_compat.h"

#define FCPI_NAME " fcpim"

bfa_status_t bfad_im_module_init(void);
void bfad_im_module_exit(void);
bfa_status_t bfad_im_probe(struct bfad_s *bfad);
void bfad_im_probe_undo(struct bfad_s *bfad);
void bfad_im_probe_post(struct bfad_im_s *im);
bfa_status_t bfad_im_port_new(struct bfad_s *bfad, struct bfad_port_s *port);
void bfad_im_port_delete(struct bfad_s *bfad, struct bfad_port_s *port);
void bfad_im_port_online(struct bfad_s *bfad, struct bfad_port_s *port);
void bfad_im_port_offline(struct bfad_s *bfad, struct bfad_port_s *port);
void bfad_im_port_clean(struct bfad_im_port_s *im_port);
int  bfad_im_scsi_host_alloc(struct bfad_s *bfad,
		struct bfad_im_port_s *im_port, struct device *dev);
void bfad_im_scsi_host_free(struct bfad_s *bfad,
				struct bfad_im_port_s *im_port);

#define MAX_FCP_TARGET 1024
#define MAX_FCP_LUN 16384
#define BFAD_TARGET_RESET_TMO 60
#define BFAD_LUN_RESET_TMO 60
#define ScsiResult(host_code, scsi_code) (((host_code) << 16) | scsi_code)
#define BFA_QUEUE_FULL_RAMP_UP_TIME 120
#define BFAD_KOBJ_NAME_LEN 20

/*
 * itnim flags
 */
#define ITNIM_MAPPED		0x00000001

#define SCSI_TASK_MGMT		0x00000001
#define IO_DONE_BIT			0

struct bfad_itnim_data_s {
	struct bfad_itnim_s *itnim;
};

struct bfad_im_port_s {
	struct bfad_s         *bfad;
	struct bfad_port_s    *port;
	struct work_struct port_delete_work;
	int             idr_id;
	u16        cur_scsi_id;
	u16	   flags;
	struct list_head binding_list;
	struct Scsi_Host *shost;
	struct list_head itnim_mapped_list;
	struct fc_vport *fc_vport;
};

enum bfad_itnim_state {
	ITNIM_STATE_NONE,
	ITNIM_STATE_ONLINE,
	ITNIM_STATE_OFFLINE_PENDING,
	ITNIM_STATE_OFFLINE,
	ITNIM_STATE_TIMEOUT,
	ITNIM_STATE_FREE,
};

/*
 * Per itnim data structure
 */
struct bfad_itnim_s {
	struct list_head list_entry;
	struct bfa_fcs_itnim_s fcs_itnim;
	struct work_struct itnim_work;
	u32        flags;
	enum bfad_itnim_state state;
	struct bfad_im_s *im;
	struct bfad_im_port_s *im_port;
	struct bfad_rport_s *drv_rport;
	struct fc_rport *fc_rport;
	struct bfa_itnim_s *bfa_itnim;
	u16        scsi_tgt_id;
	u16        queue_work;
	unsigned long	last_ramp_up_time;
	unsigned long	last_queue_full_time;
};

enum bfad_binding_type {
	FCP_PWWN_BINDING = 0x1,
	FCP_NWWN_BINDING = 0x2,
	FCP_FCID_BINDING = 0x3,
};

struct bfad_fcp_binding {
	struct list_head list_entry;
	enum bfad_binding_type binding_type;
	u16        scsi_target_id;
	u32        fc_id;
	wwn_t           nwwn;
	wwn_t           pwwn;
};

struct bfad_im_s {
	struct bfad_s         *bfad;
	struct workqueue_struct *drv_workq;
	char   drv_workq_name[BFAD_KOBJ_NAME_LEN];
};

struct Scsi_Host *bfad_os_scsi_host_alloc(struct bfad_im_port_s *im_port,
				struct bfad_s *);
bfa_status_t bfad_os_thread_workq(struct bfad_s *bfad);
void bfad_os_destroy_workq(struct bfad_im_s *im);
void bfad_os_itnim_process(struct bfad_itnim_s *itnim_drv);
void bfad_os_fc_host_init(struct bfad_im_port_s *im_port);
void bfad_os_scsi_host_free(struct bfad_s *bfad,
				 struct bfad_im_port_s *im_port);
void bfad_os_ramp_up_qdepth(struct bfad_itnim_s *itnim,
				 struct scsi_device *sdev);
void bfad_os_handle_qfull(struct bfad_itnim_s *itnim, struct scsi_device *sdev);
struct bfad_itnim_s *bfad_os_get_itnim(struct bfad_im_port_s *im_port, int id);
int bfad_os_scsi_add_host(struct Scsi_Host *shost,
		struct bfad_im_port_s *im_port, struct bfad_s *bfad);

void bfad_im_itnim_unmap(struct bfad_im_port_s  *im_port,
			 struct bfad_itnim_s *itnim);

extern struct scsi_host_template bfad_im_scsi_host_template;
extern struct scsi_host_template bfad_im_vport_template;
extern struct fc_function_template bfad_im_fc_function_template;
extern struct fc_function_template bfad_im_vport_fc_function_template;
extern struct scsi_transport_template *bfad_im_scsi_transport_template;
extern struct scsi_transport_template *bfad_im_scsi_vport_transport_template;

#endif
