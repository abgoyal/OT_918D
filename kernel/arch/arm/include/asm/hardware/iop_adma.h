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
 * Copyright © 2006, Intel Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin St - Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */
#ifndef IOP_ADMA_H
#define IOP_ADMA_H
#include <linux/types.h>
#include <linux/dmaengine.h>
#include <linux/interrupt.h>

#define IOP_ADMA_SLOT_SIZE 32
#define IOP_ADMA_THRESHOLD 4
#ifdef DEBUG
#define IOP_PARANOIA 1
#else
#define IOP_PARANOIA 0
#endif
#define iop_paranoia(x) BUG_ON(IOP_PARANOIA && (x))

/**
 * struct iop_adma_device - internal representation of an ADMA device
 * @pdev: Platform device
 * @id: HW ADMA Device selector
 * @dma_desc_pool: base of DMA descriptor region (DMA address)
 * @dma_desc_pool_virt: base of DMA descriptor region (CPU address)
 * @common: embedded struct dma_device
 */
struct iop_adma_device {
	struct platform_device *pdev;
	int id;
	dma_addr_t dma_desc_pool;
	void *dma_desc_pool_virt;
	struct dma_device common;
};

/**
 * struct iop_adma_chan - internal representation of an ADMA device
 * @pending: allows batching of hardware operations
 * @completed_cookie: identifier for the most recently completed operation
 * @lock: serializes enqueue/dequeue operations to the slot pool
 * @mmr_base: memory mapped register base
 * @chain: device chain view of the descriptors
 * @device: parent device
 * @common: common dmaengine channel object members
 * @last_used: place holder for allocation to continue from where it left off
 * @all_slots: complete domain of slots usable by the channel
 * @slots_allocated: records the actual size of the descriptor slot pool
 * @irq_tasklet: bottom half where iop_adma_slot_cleanup runs
 */
struct iop_adma_chan {
	int pending;
	dma_cookie_t completed_cookie;
	spinlock_t lock; /* protects the descriptor slot pool */
	void __iomem *mmr_base;
	struct list_head chain;
	struct iop_adma_device *device;
	struct dma_chan common;
	struct iop_adma_desc_slot *last_used;
	struct list_head all_slots;
	int slots_allocated;
	struct tasklet_struct irq_tasklet;
};

/**
 * struct iop_adma_desc_slot - IOP-ADMA software descriptor
 * @slot_node: node on the iop_adma_chan.all_slots list
 * @chain_node: node on the op_adma_chan.chain list
 * @hw_desc: virtual address of the hardware descriptor chain
 * @phys: hardware address of the hardware descriptor chain
 * @group_head: first operation in a transaction
 * @slot_cnt: total slots used in an transaction (group of operations)
 * @slots_per_op: number of slots per operation
 * @idx: pool index
 * @unmap_src_cnt: number of xor sources
 * @unmap_len: transaction bytecount
 * @tx_list: list of descriptors that are associated with one operation
 * @async_tx: support for the async_tx api
 * @group_list: list of slots that make up a multi-descriptor transaction
 *	for example transfer lengths larger than the supported hw max
 * @xor_check_result: result of zero sum
 * @crc32_result: result crc calculation
 */
struct iop_adma_desc_slot {
	struct list_head slot_node;
	struct list_head chain_node;
	void *hw_desc;
	struct iop_adma_desc_slot *group_head;
	u16 slot_cnt;
	u16 slots_per_op;
	u16 idx;
	u16 unmap_src_cnt;
	size_t unmap_len;
	struct list_head tx_list;
	struct dma_async_tx_descriptor async_tx;
	union {
		u32 *xor_check_result;
		u32 *crc32_result;
		u32 *pq_check_result;
	};
};

struct iop_adma_platform_data {
	int hw_id;
	dma_cap_mask_t cap_mask;
	size_t pool_size;
};

#define to_iop_sw_desc(addr_hw_desc) \
	container_of(addr_hw_desc, struct iop_adma_desc_slot, hw_desc)
#define iop_hw_desc_slot_idx(hw_desc, idx) \
	( (void *) (((unsigned long) hw_desc) + ((idx) << 5)) )
#endif
