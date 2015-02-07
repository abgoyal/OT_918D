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

#ifndef __iop_scrc_out_defs_h
#define __iop_scrc_out_defs_h

/*
 * This file is autogenerated from
 *   file:           ../../inst/io_proc/rtl/iop_scrc_out.r
 *     id:           iop_scrc_out.r,v 1.11 2005/02/16 09:13:38 niklaspa Exp
 *     last modfied: Mon Apr 11 16:08:46 2005
 *
 *   by /n/asic/design/tools/rdesc/src/rdes2c --outfile iop_scrc_out_defs.h ../../inst/io_proc/rtl/iop_scrc_out.r
 *      id: $Id: iop_scrc_out_defs.h,v 1.4 2005/04/24 18:31:05 starvik Exp $
 * Any changes here will be lost.
 *
 * -*- buffer-read-only: t -*-
 */
/* Main access macros */
#ifndef REG_RD
#define REG_RD( scope, inst, reg ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR
#define REG_WR( scope, inst, reg, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_VECT
#define REG_RD_VECT( scope, inst, reg, index ) \
  REG_READ( reg_##scope##_##reg, \
            (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_VECT
#define REG_WR_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( reg_##scope##_##reg, \
             (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT
#define REG_RD_INT( scope, inst, reg ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_WR_INT
#define REG_WR_INT( scope, inst, reg, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg, (val) )
#endif

#ifndef REG_RD_INT_VECT
#define REG_RD_INT_VECT( scope, inst, reg, index ) \
  REG_READ( int, (inst) + REG_RD_ADDR_##scope##_##reg + \
	    (index) * STRIDE_##scope##_##reg )
#endif

#ifndef REG_WR_INT_VECT
#define REG_WR_INT_VECT( scope, inst, reg, index, val ) \
  REG_WRITE( int, (inst) + REG_WR_ADDR_##scope##_##reg + \
	     (index) * STRIDE_##scope##_##reg, (val) )
#endif

#ifndef REG_TYPE_CONV
#define REG_TYPE_CONV( type, orgtype, val ) \
  ( { union { orgtype o; type n; } r; r.o = val; r.n; } )
#endif

#ifndef reg_page_size
#define reg_page_size 8192
#endif

#ifndef REG_ADDR
#define REG_ADDR( scope, inst, reg ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg )
#endif

#ifndef REG_ADDR_VECT
#define REG_ADDR_VECT( scope, inst, reg, index ) \
  ( (inst) + REG_RD_ADDR_##scope##_##reg + \
    (index) * STRIDE_##scope##_##reg )
#endif

/* C-code for register scope iop_scrc_out */

/* Register rw_cfg, scope iop_scrc_out, type rw */
typedef struct {
  unsigned int trig    : 2;
  unsigned int inv_crc : 1;
  unsigned int dummy1  : 29;
} reg_iop_scrc_out_rw_cfg;
#define REG_RD_ADDR_iop_scrc_out_rw_cfg 0
#define REG_WR_ADDR_iop_scrc_out_rw_cfg 0

/* Register rw_ctrl, scope iop_scrc_out, type rw */
typedef struct {
  unsigned int strb_src : 1;
  unsigned int out_src  : 1;
  unsigned int dummy1   : 30;
} reg_iop_scrc_out_rw_ctrl;
#define REG_RD_ADDR_iop_scrc_out_rw_ctrl 4
#define REG_WR_ADDR_iop_scrc_out_rw_ctrl 4

/* Register rw_init_crc, scope iop_scrc_out, type rw */
typedef unsigned int reg_iop_scrc_out_rw_init_crc;
#define REG_RD_ADDR_iop_scrc_out_rw_init_crc 8
#define REG_WR_ADDR_iop_scrc_out_rw_init_crc 8

/* Register rw_crc, scope iop_scrc_out, type rw */
typedef unsigned int reg_iop_scrc_out_rw_crc;
#define REG_RD_ADDR_iop_scrc_out_rw_crc 12
#define REG_WR_ADDR_iop_scrc_out_rw_crc 12

/* Register rw_data, scope iop_scrc_out, type rw */
typedef struct {
  unsigned int val : 1;
  unsigned int dummy1 : 31;
} reg_iop_scrc_out_rw_data;
#define REG_RD_ADDR_iop_scrc_out_rw_data 16
#define REG_WR_ADDR_iop_scrc_out_rw_data 16

/* Register r_computed_crc, scope iop_scrc_out, type r */
typedef unsigned int reg_iop_scrc_out_r_computed_crc;
#define REG_RD_ADDR_iop_scrc_out_r_computed_crc 20


/* Constants */
enum {
  regk_iop_scrc_out_crc                    = 0x00000001,
  regk_iop_scrc_out_data                   = 0x00000000,
  regk_iop_scrc_out_dif                    = 0x00000001,
  regk_iop_scrc_out_hi                     = 0x00000000,
  regk_iop_scrc_out_neg                    = 0x00000002,
  regk_iop_scrc_out_no                     = 0x00000000,
  regk_iop_scrc_out_pos                    = 0x00000001,
  regk_iop_scrc_out_pos_neg                = 0x00000003,
  regk_iop_scrc_out_reg                    = 0x00000000,
  regk_iop_scrc_out_rw_cfg_default         = 0x00000000,
  regk_iop_scrc_out_rw_crc_default         = 0x00000000,
  regk_iop_scrc_out_rw_ctrl_default        = 0x00000000,
  regk_iop_scrc_out_rw_data_default        = 0x00000000,
  regk_iop_scrc_out_rw_init_crc_default    = 0x00000000,
  regk_iop_scrc_out_yes                    = 0x00000001
};
#endif /* __iop_scrc_out_defs_h */