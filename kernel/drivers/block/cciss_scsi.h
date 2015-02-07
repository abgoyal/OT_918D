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
 *    Disk Array driver for HP Smart Array controllers, SCSI Tape module.
 *    (C) Copyright 2001, 2007 Hewlett-Packard Development Company, L.P.
 *
 *    This program is free software; you can redistribute it and/or modify
 *    it under the terms of the GNU General Public License as published by
 *    the Free Software Foundation; version 2 of the License.
 *
 *    This program is distributed in the hope that it will be useful,
 *    but WITHOUT ANY WARRANTY; without even the implied warranty of
 *    MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 *    General Public License for more details.
 *
 *    You should have received a copy of the GNU General Public License
 *    along with this program; if not, write to the Free Software
 *    Foundation, Inc., 59 Temple Place, Suite 300, Boston, MA
 *    02111-1307, USA.
 *
 *    Questions/Comments/Bugfixes to iss_storagedev@hp.com
 *
 */
#ifdef CONFIG_CISS_SCSI_TAPE
#ifndef _CCISS_SCSI_H_
#define _CCISS_SCSI_H_

#include <scsi/scsicam.h> /* possibly irrelevant, since we don't show disks */

		/* the scsi id of the adapter... */
#define SELF_SCSI_ID 15
		/* 15 is somewhat arbitrary, since the scsi-2 bus
		   that's presented by the driver to the OS is
		   fabricated.  The "real" scsi-3 bus the
		   hardware presents is fabricated too.
		   The actual, honest-to-goodness physical
		   bus that the devices are attached to is not
		   addressible natively, and may in fact turn
		   out to be not scsi at all. */

#define SCSI_CCISS_CAN_QUEUE 2

/* 

Note, cmd_per_lun could give us some trouble, so I'm setting it very low.
Likewise, SCSI_CCISS_CAN_QUEUE is set very conservatively.

If the upper scsi layer tries to track how many commands we have 
outstanding, it will be operating under the misapprehension that it is
the only one sending us requests.  We also have the block interface,
which is where most requests must surely come from, so the upper layer's
notion of how many requests we have outstanding will be wrong most or
all of the time. 

Note, the normal SCSI mid-layer error handling doesn't work well
for this driver because 1) it takes the io_request_lock before
calling error handlers and uses a local variable to store flags,
so the io_request_lock cannot be released and interrupts enabled
inside the error handlers, and, the error handlers cannot poll
for command completion because they might get commands from the
block half of the driver completing, and not know what to do
with them.  That's what we get for making a hybrid scsi/block
driver, I suppose.

*/

struct cciss_scsi_dev_t {
	int devtype;
	int bus, target, lun;		/* as presented to the OS */
	unsigned char scsi3addr[8];	/* as presented to the HW */
	unsigned char device_id[16];	/* from inquiry pg. 0x83 */
	unsigned char vendor[8];	/* bytes 8-15 of inquiry data */
	unsigned char model[16];	/* bytes 16-31 of inquiry data */
	unsigned char revision[4];	/* bytes 32-35 of inquiry data */
};

struct cciss_scsi_hba_t {
	char *name;
	int ndevices;
#define CCISS_MAX_SCSI_DEVS_PER_HBA 16
	struct cciss_scsi_dev_t dev[CCISS_MAX_SCSI_DEVS_PER_HBA];
};

#endif /* _CCISS_SCSI_H_ */
#endif /* CONFIG_CISS_SCSI_TAPE */
