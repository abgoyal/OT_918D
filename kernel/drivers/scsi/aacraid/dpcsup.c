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
 *	Adaptec AAC series RAID controller driver
 *	(c) Copyright 2001 Red Hat Inc.
 *
 * based on the old aacraid driver that is..
 * Adaptec aacraid device driver for Linux.
 *
 * Copyright (c) 2000-2007 Adaptec, Inc. (aacraid@adaptec.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file COPYING.  If not, write to
 * the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Module Name:
 *  dpcsup.c
 *
 * Abstract: All DPC processing routines for the cyclone board occur here.
 *
 *
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/types.h>
#include <linux/spinlock.h>
#include <linux/slab.h>
#include <linux/completion.h>
#include <linux/blkdev.h>
#include <linux/semaphore.h>

#include "aacraid.h"

/**
 *	aac_response_normal	-	Handle command replies
 *	@q: Queue to read from
 *
 *	This DPC routine will be run when the adapter interrupts us to let us
 *	know there is a response on our normal priority queue. We will pull off
 *	all QE there are and wake up all the waiters before exiting. We will
 *	take a spinlock out on the queue before operating on it.
 */

unsigned int aac_response_normal(struct aac_queue * q)
{
	struct aac_dev * dev = q->dev;
	struct aac_entry *entry;
	struct hw_fib * hwfib;
	struct fib * fib;
	int consumed = 0;
	unsigned long flags, mflags;

	spin_lock_irqsave(q->lock, flags);
	/*
	 *	Keep pulling response QEs off the response queue and waking
	 *	up the waiters until there are no more QEs. We then return
	 *	back to the system. If no response was requesed we just
	 *	deallocate the Fib here and continue.
	 */
	while(aac_consumer_get(dev, q, &entry))
	{
		int fast;
		u32 index = le32_to_cpu(entry->addr);
		fast = index & 0x01;
		fib = &dev->fibs[index >> 2];
		hwfib = fib->hw_fib_va;
		
		aac_consumer_free(dev, q, HostNormRespQueue);
		/*
		 *	Remove this fib from the Outstanding I/O queue.
		 *	But only if it has not already been timed out.
		 *
		 *	If the fib has been timed out already, then just 
		 *	continue. The caller has already been notified that
		 *	the fib timed out.
		 */
		dev->queues->queue[AdapNormCmdQueue].numpending--;

		if (unlikely(fib->flags & FIB_CONTEXT_FLAG_TIMED_OUT)) {
			spin_unlock_irqrestore(q->lock, flags);
			aac_fib_complete(fib);
			aac_fib_free(fib);
			spin_lock_irqsave(q->lock, flags);
			continue;
		}
		spin_unlock_irqrestore(q->lock, flags);

		if (fast) {
			/*
			 *	Doctor the fib
			 */
			*(__le32 *)hwfib->data = cpu_to_le32(ST_OK);
			hwfib->header.XferState |= cpu_to_le32(AdapterProcessed);
		}

		FIB_COUNTER_INCREMENT(aac_config.FibRecved);

		if (hwfib->header.Command == cpu_to_le16(NuFileSystem))
		{
			__le32 *pstatus = (__le32 *)hwfib->data;
			if (*pstatus & cpu_to_le32(0xffff0000))
				*pstatus = cpu_to_le32(ST_OK);
		}
		if (hwfib->header.XferState & cpu_to_le32(NoResponseExpected | Async)) 
		{
	        	if (hwfib->header.XferState & cpu_to_le32(NoResponseExpected))
				FIB_COUNTER_INCREMENT(aac_config.NoResponseRecved);
			else 
				FIB_COUNTER_INCREMENT(aac_config.AsyncRecved);
			/*
			 *	NOTE:  we cannot touch the fib after this
			 *	    call, because it may have been deallocated.
			 */
			fib->flags = 0;
			fib->callback(fib->callback_data, fib);
		} else {
			unsigned long flagv;
			spin_lock_irqsave(&fib->event_lock, flagv);
			if (!fib->done) {
				fib->done = 1;
				up(&fib->event_wait);
			}
			spin_unlock_irqrestore(&fib->event_lock, flagv);

			spin_lock_irqsave(&dev->manage_lock, mflags);
			dev->management_fib_count--;
			spin_unlock_irqrestore(&dev->manage_lock, mflags);

			FIB_COUNTER_INCREMENT(aac_config.NormalRecved);
			if (fib->done == 2) {
				spin_lock_irqsave(&fib->event_lock, flagv);
				fib->done = 0;
				spin_unlock_irqrestore(&fib->event_lock, flagv);
				aac_fib_complete(fib);
				aac_fib_free(fib);
			}
		}
		consumed++;
		spin_lock_irqsave(q->lock, flags);
	}

	if (consumed > aac_config.peak_fibs)
		aac_config.peak_fibs = consumed;
	if (consumed == 0) 
		aac_config.zero_fibs++;

	spin_unlock_irqrestore(q->lock, flags);
	return 0;
}


/**
 *	aac_command_normal	-	handle commands
 *	@q: queue to process
 *
 *	This DPC routine will be queued when the adapter interrupts us to 
 *	let us know there is a command on our normal priority queue. We will 
 *	pull off all QE there are and wake up all the waiters before exiting.
 *	We will take a spinlock out on the queue before operating on it.
 */
 
unsigned int aac_command_normal(struct aac_queue *q)
{
	struct aac_dev * dev = q->dev;
	struct aac_entry *entry;
	unsigned long flags;

	spin_lock_irqsave(q->lock, flags);

	/*
	 *	Keep pulling response QEs off the response queue and waking
	 *	up the waiters until there are no more QEs. We then return
	 *	back to the system.
	 */
	while(aac_consumer_get(dev, q, &entry))
	{
		struct fib fibctx;
		struct hw_fib * hw_fib;
		u32 index;
		struct fib *fib = &fibctx;
		
		index = le32_to_cpu(entry->addr) / sizeof(struct hw_fib);
		hw_fib = &dev->aif_base_va[index];
		
		/*
		 *	Allocate a FIB at all costs. For non queued stuff
		 *	we can just use the stack so we are happy. We need
		 *	a fib object in order to manage the linked lists
		 */
		if (dev->aif_thread)
			if((fib = kmalloc(sizeof(struct fib), GFP_ATOMIC)) == NULL)
				fib = &fibctx;
		
		memset(fib, 0, sizeof(struct fib));
		INIT_LIST_HEAD(&fib->fiblink);
		fib->type = FSAFS_NTC_FIB_CONTEXT;
		fib->size = sizeof(struct fib);
		fib->hw_fib_va = hw_fib;
		fib->data = hw_fib->data;
		fib->dev = dev;
		
				
		if (dev->aif_thread && fib != &fibctx) {
		        list_add_tail(&fib->fiblink, &q->cmdq);
	 	        aac_consumer_free(dev, q, HostNormCmdQueue);
		        wake_up_interruptible(&q->cmdready);
		} else {
	 	        aac_consumer_free(dev, q, HostNormCmdQueue);
			spin_unlock_irqrestore(q->lock, flags);
			/*
			 *	Set the status of this FIB
			 */
			*(__le32 *)hw_fib->data = cpu_to_le32(ST_OK);
			aac_fib_adapter_complete(fib, sizeof(u32));
			spin_lock_irqsave(q->lock, flags);
		}		
	}
	spin_unlock_irqrestore(q->lock, flags);
	return 0;
}


/**
 *	aac_intr_normal	-	Handle command replies
 *	@dev: Device
 *	@index: completion reference
 *
 *	This DPC routine will be run when the adapter interrupts us to let us
 *	know there is a response on our normal priority queue. We will pull off
 *	all QE there are and wake up all the waiters before exiting.
 */

unsigned int aac_intr_normal(struct aac_dev * dev, u32 index)
{
	unsigned long mflags;
	dprintk((KERN_INFO "aac_intr_normal(%p,%x)\n", dev, index));
	if ((index & 0x00000002L)) {
		struct hw_fib * hw_fib;
		struct fib * fib;
		struct aac_queue *q = &dev->queues->queue[HostNormCmdQueue];
		unsigned long flags;

		if (index == 0xFFFFFFFEL) /* Special Case */
			return 0;	  /* Do nothing */
		/*
		 *	Allocate a FIB. For non queued stuff we can just use
		 * the stack so we are happy. We need a fib object in order to
		 * manage the linked lists.
		 */
		if ((!dev->aif_thread)
		 || (!(fib = kzalloc(sizeof(struct fib),GFP_ATOMIC))))
			return 1;
		if (!(hw_fib = kzalloc(sizeof(struct hw_fib),GFP_ATOMIC))) {
			kfree (fib);
			return 1;
		}
		memcpy(hw_fib, (struct hw_fib *)(((uintptr_t)(dev->regs.sa)) +
		  (index & ~0x00000002L)), sizeof(struct hw_fib));
		INIT_LIST_HEAD(&fib->fiblink);
		fib->type = FSAFS_NTC_FIB_CONTEXT;
		fib->size = sizeof(struct fib);
		fib->hw_fib_va = hw_fib;
		fib->data = hw_fib->data;
		fib->dev = dev;
	
		spin_lock_irqsave(q->lock, flags);
		list_add_tail(&fib->fiblink, &q->cmdq);
	        wake_up_interruptible(&q->cmdready);
		spin_unlock_irqrestore(q->lock, flags);
		return 1;
	} else {
		int fast = index & 0x01;
		struct fib * fib = &dev->fibs[index >> 2];
		struct hw_fib * hwfib = fib->hw_fib_va;

		/*
		 *	Remove this fib from the Outstanding I/O queue.
		 *	But only if it has not already been timed out.
		 *
		 *	If the fib has been timed out already, then just 
		 *	continue. The caller has already been notified that
		 *	the fib timed out.
		 */
		dev->queues->queue[AdapNormCmdQueue].numpending--;

		if (unlikely(fib->flags & FIB_CONTEXT_FLAG_TIMED_OUT)) {
			aac_fib_complete(fib);
			aac_fib_free(fib);
			return 0;
		}

		if (fast) {
			/*
			 *	Doctor the fib
			 */
			*(__le32 *)hwfib->data = cpu_to_le32(ST_OK);
			hwfib->header.XferState |= cpu_to_le32(AdapterProcessed);
		}

		FIB_COUNTER_INCREMENT(aac_config.FibRecved);

		if (hwfib->header.Command == cpu_to_le16(NuFileSystem))
		{
			__le32 *pstatus = (__le32 *)hwfib->data;
			if (*pstatus & cpu_to_le32(0xffff0000))
				*pstatus = cpu_to_le32(ST_OK);
		}
		if (hwfib->header.XferState & cpu_to_le32(NoResponseExpected | Async)) 
		{
	        	if (hwfib->header.XferState & cpu_to_le32(NoResponseExpected))
				FIB_COUNTER_INCREMENT(aac_config.NoResponseRecved);
			else 
				FIB_COUNTER_INCREMENT(aac_config.AsyncRecved);
			/*
			 *	NOTE:  we cannot touch the fib after this
			 *	    call, because it may have been deallocated.
			 */
			fib->flags = 0;
			fib->callback(fib->callback_data, fib);
		} else {
			unsigned long flagv;
	  		dprintk((KERN_INFO "event_wait up\n"));
			spin_lock_irqsave(&fib->event_lock, flagv);
			if (!fib->done) {
				fib->done = 1;
				up(&fib->event_wait);
			}
			spin_unlock_irqrestore(&fib->event_lock, flagv);

			spin_lock_irqsave(&dev->manage_lock, mflags);
			dev->management_fib_count--;
			spin_unlock_irqrestore(&dev->manage_lock, mflags);

			FIB_COUNTER_INCREMENT(aac_config.NormalRecved);
			if (fib->done == 2) {
				spin_lock_irqsave(&fib->event_lock, flagv);
				fib->done = 0;
				spin_unlock_irqrestore(&fib->event_lock, flagv);
				aac_fib_complete(fib);
				aac_fib_free(fib);
			}

		}
		return 0;
	}
}