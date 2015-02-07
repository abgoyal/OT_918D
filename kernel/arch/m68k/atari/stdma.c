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
 *  linux/arch/m68k/atari/stmda.c
 *
 *  Copyright (C) 1994 Roman Hodek
 *
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */


/* This file contains some function for controlling the access to the  */
/* ST-DMA chip that may be shared between devices. Currently we have:  */
/*   TT:     Floppy and ACSI bus                                       */
/*   Falcon: Floppy and SCSI                                           */
/*                                                                     */
/* The controlling functions set up a wait queue for access to the     */
/* ST-DMA chip. Callers to stdma_lock() that cannot granted access are */
/* put onto a queue and waked up later if the owner calls              */
/* stdma_release(). Additionally, the caller gives his interrupt       */
/* service routine to stdma_lock().                                    */
/*                                                                     */
/* On the Falcon, the IDE bus uses just the ACSI/Floppy interrupt, but */
/* not the ST-DMA chip itself. So falhd.c needs not to lock the        */
/* chip. The interrupt is routed to falhd.c if IDE is configured, the  */
/* model is a Falcon and the interrupt was caused by the HD controller */
/* (can be determined by looking at its status register).              */


#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/genhd.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/module.h>

#include <asm/atari_stdma.h>
#include <asm/atariints.h>
#include <asm/atarihw.h>
#include <asm/io.h>
#include <asm/irq.h>

static int stdma_locked;			/* the semaphore */
						/* int func to be called */
static irq_handler_t stdma_isr;
static void *stdma_isr_data;			/* data passed to isr */
static DECLARE_WAIT_QUEUE_HEAD(stdma_wait);	/* wait queue for ST-DMA */




/***************************** Prototypes *****************************/

static irqreturn_t stdma_int (int irq, void *dummy);

/************************* End of Prototypes **************************/



/*
 * Function: void stdma_lock( isrfunc isr, void *data )
 *
 * Purpose: Tries to get a lock on the ST-DMA chip that is used by more
 *   then one device driver. Waits on stdma_wait until lock is free.
 *   stdma_lock() may not be called from an interrupt! You have to
 *   get the lock in your main routine and release it when your
 *   request is finished.
 *
 * Inputs: A interrupt function that is called until the lock is
 *   released.
 *
 * Returns: nothing
 *
 */

void stdma_lock(irq_handler_t handler, void *data)
{
	unsigned long flags;

	local_irq_save(flags);		/* protect lock */

	/* Since the DMA is used for file system purposes, we
	 have to sleep uninterruptible (there may be locked
	 buffers) */
	wait_event(stdma_wait, !stdma_locked);

	stdma_locked   = 1;
	stdma_isr      = handler;
	stdma_isr_data = data;
	local_irq_restore(flags);
}
EXPORT_SYMBOL(stdma_lock);


/*
 * Function: void stdma_release( void )
 *
 * Purpose: Releases the lock on the ST-DMA chip.
 *
 * Inputs: none
 *
 * Returns: nothing
 *
 */

void stdma_release(void)
{
	unsigned long flags;

	local_irq_save(flags);

	stdma_locked   = 0;
	stdma_isr      = NULL;
	stdma_isr_data = NULL;
	wake_up(&stdma_wait);

	local_irq_restore(flags);
}
EXPORT_SYMBOL(stdma_release);


/*
 * Function: int stdma_others_waiting( void )
 *
 * Purpose: Check if someone waits for the ST-DMA lock.
 *
 * Inputs: none
 *
 * Returns: 0 if no one is waiting, != 0 otherwise
 *
 */

int stdma_others_waiting(void)
{
	return waitqueue_active(&stdma_wait);
}
EXPORT_SYMBOL(stdma_others_waiting);


/*
 * Function: int stdma_islocked( void )
 *
 * Purpose: Check if the ST-DMA is currently locked.
 * Note: Returned status is only valid if ints are disabled while calling and
 *       as long as they remain disabled.
 *       If called with ints enabled, status can change only from locked to
 *       unlocked, because ints may not lock the ST-DMA.
 *
 * Inputs: none
 *
 * Returns: != 0 if locked, 0 otherwise
 *
 */

int stdma_islocked(void)
{
	return stdma_locked;
}
EXPORT_SYMBOL(stdma_islocked);


/*
 * Function: void stdma_init( void )
 *
 * Purpose: Initialize the ST-DMA chip access controlling.
 *   It sets up the interrupt and its service routine. The int is registered
 *   as slow int, client devices have to live with that (no problem
 *   currently).
 *
 * Inputs: none
 *
 * Return: nothing
 *
 */

void __init stdma_init(void)
{
	stdma_isr = NULL;
	if (request_irq(IRQ_MFP_FDC, stdma_int, IRQ_TYPE_SLOW | IRQF_SHARED,
			"ST-DMA: floppy/ACSI/IDE/Falcon-SCSI", stdma_int))
		pr_err("Couldn't register ST-DMA interrupt\n");
}


/*
 * Function: void stdma_int()
 *
 * Purpose: The interrupt routine for the ST-DMA. It calls the isr
 *   registered by stdma_lock().
 *
 */

static irqreturn_t stdma_int(int irq, void *dummy)
{
  if (stdma_isr)
      (*stdma_isr)(irq, stdma_isr_data);
  return IRQ_HANDLED;
}
