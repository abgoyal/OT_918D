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

/**************************************************************************
 *
 * Copyright (c) 2006-2009 VMware, Inc., Palo Alto, CA., USA
 * All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sub license, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice (including the
 * next paragraph) shall be included in all copies or substantial portions
 * of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL
 * THE COPYRIGHT HOLDERS, AUTHORS AND/OR ITS SUPPLIERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 **************************************************************************/
/*
 * Authors: Thomas Hellstrom <thellstrom-at-vmware-dot-com>
 */

#ifndef _TTM_EXECBUF_UTIL_H_
#define _TTM_EXECBUF_UTIL_H_

#include "ttm/ttm_bo_api.h"
#include <linux/list.h>

/**
 * struct ttm_validate_buffer
 *
 * @head:           list head for thread-private list.
 * @bo:             refcounted buffer object pointer.
 * @new_sync_obj_arg: New sync_obj_arg for @bo, to be used once
 * adding a new sync object.
 * @reservied:      Indicates whether @bo has been reserved for validation.
 */

struct ttm_validate_buffer {
	struct list_head head;
	struct ttm_buffer_object *bo;
	void *new_sync_obj_arg;
	bool reserved;
};

/**
 * function ttm_eu_backoff_reservation
 *
 * @list:     thread private list of ttm_validate_buffer structs.
 *
 * Undoes all buffer validation reservations for bos pointed to by
 * the list entries.
 */

extern void ttm_eu_backoff_reservation(struct list_head *list);

/**
 * function ttm_eu_reserve_buffers
 *
 * @list:    thread private list of ttm_validate_buffer structs.
 * @val_seq: A unique sequence number.
 *
 * Tries to reserve bos pointed to by the list entries for validation.
 * If the function returns 0, all buffers are marked as "unfenced",
 * taken off the lru lists and are not synced for write CPU usage.
 *
 * If the function detects a deadlock due to multiple threads trying to
 * reserve the same buffers in reverse order, all threads except one will
 * back off and retry. This function may sleep while waiting for
 * CPU write reservations to be cleared, and for other threads to
 * unreserve their buffers.
 *
 * This function may return -ERESTART or -EAGAIN if the calling process
 * receives a signal while waiting. In that case, no buffers on the list
 * will be reserved upon return.
 *
 * Buffers reserved by this function should be unreserved by
 * a call to either ttm_eu_backoff_reservation() or
 * ttm_eu_fence_buffer_objects() when command submission is complete or
 * has failed.
 */

extern int ttm_eu_reserve_buffers(struct list_head *list, uint32_t val_seq);

/**
 * function ttm_eu_fence_buffer_objects.
 *
 * @list:        thread private list of ttm_validate_buffer structs.
 * @sync_obj:    The new sync object for the buffers.
 *
 * This function should be called when command submission is complete, and
 * it will add a new sync object to bos pointed to by entries on @list.
 * It also unreserves all buffers, putting them on lru lists.
 *
 */

extern void ttm_eu_fence_buffer_objects(struct list_head *list, void *sync_obj);

#endif
