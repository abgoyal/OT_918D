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

/***********************************************************************/
/**

    AudioScience HPI driver
    Copyright (C) 1997-2010  AudioScience Inc. <support@audioscience.com>

    This program is free software; you can redistribute it and/or modify
    it under the terms of version 2 of the GNU General Public License as
    published by the Free Software Foundation;

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

\file
Functions for reading DSP code to load into DSP

 hpi_dspcode_defines HPI DSP code loading method
Define exactly one of these to select how the DSP code is supplied to
the adapter.

End users writing applications that use the HPI interface do not have to
use any of the below defines; they are only necessary for building drivers

HPI_DSPCODE_FILE:
DSP code is supplied as a file that is opened and read from by the driver.

HPI_DSPCODE_FIRMWARE:
DSP code is read using the hotplug firmware loader module.
     Only valid when compiling the HPI kernel driver under Linux.
*/
/***********************************************************************/
#ifndef _HPIDSPCD_H_
#define _HPIDSPCD_H_

#include "hpi_internal.h"

#ifndef DISABLE_PRAGMA_PACK1
#pragma pack(push, 1)
#endif

/** Descriptor for dspcode from firmware loader */
struct dsp_code {
	/**  Firmware descriptor */
	const struct firmware *ps_firmware;
	struct pci_dev *ps_dev;
	/** Expected number of words in the whole dsp code,INCL header */
	long int block_length;
	/** Number of words read so far */
	long int word_count;
	/** Version read from dsp code file */
	u32 version;
	/** CRC read from dsp code file */
	u32 crc;
};

#ifndef DISABLE_PRAGMA_PACK1
#pragma pack(pop)
#endif

/** Prepare *psDspCode to refer to the requuested adapter.
 Searches the file, or selects the appropriate linked array

\return 0 for success, or error code if requested code is not available
*/
short hpi_dsp_code_open(
	/** Code identifier, usually adapter family */
	u32 adapter,
	/** Pointer to DSP code control structure */
	struct dsp_code *ps_dsp_code,
	/** Pointer to dword to receive OS specific error code */
	u32 *pos_error_code);

/** Close the DSP code file */
void hpi_dsp_code_close(struct dsp_code *ps_dsp_code);

/** Rewind to the beginning of the DSP code file (for verify) */
void hpi_dsp_code_rewind(struct dsp_code *ps_dsp_code);

/** Read one word from the dsp code file
	\return 0 for success, or error code if eof, or block length exceeded
*/
short hpi_dsp_code_read_word(struct dsp_code *ps_dsp_code,
				      /**< DSP code descriptor */
	u32 *pword /**< where to store the read word */
	);

/** Get a block of dsp code into an internal buffer, and provide a pointer to
that buffer. (If dsp code is already an array in memory, it is referenced,
not copied.)

\return Error if requested number of words are not available
*/
short hpi_dsp_code_read_block(size_t words_requested,
	struct dsp_code *ps_dsp_code,
	/* Pointer to store (Pointer to code buffer) */
	u32 **ppblock);

#endif
