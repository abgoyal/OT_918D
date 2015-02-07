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
 * Internal header file for UCC FAST unit routines.
 *
 * Copyright (C) 2006 Freescale Semicondutor, Inc. All rights reserved.
 *
 * Authors: 	Shlomi Gridish <gridish@freescale.com>
 * 		Li Yang <leoli@freescale.com>
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 */
#ifndef __UCC_FAST_H__
#define __UCC_FAST_H__

#include <linux/kernel.h>

#include <asm/immap_qe.h>
#include <asm/qe.h>

#include "ucc.h"

/* Receive BD's status */
#define R_E	0x80000000	/* buffer empty */
#define R_W	0x20000000	/* wrap bit */
#define R_I	0x10000000	/* interrupt on reception */
#define R_L	0x08000000	/* last */
#define R_F	0x04000000	/* first */

/* transmit BD's status */
#define T_R	0x80000000	/* ready bit */
#define T_W	0x20000000	/* wrap bit */
#define T_I	0x10000000	/* interrupt on completion */
#define T_L	0x08000000	/* last */

/* Rx Data buffer must be 4 bytes aligned in most cases */
#define UCC_FAST_RX_ALIGN			4
#define UCC_FAST_MRBLR_ALIGNMENT		4
#define UCC_FAST_VIRT_FIFO_REGS_ALIGNMENT	8

/* Sizes */
#define UCC_FAST_URFS_MIN_VAL				0x88
#define UCC_FAST_RECEIVE_VIRTUAL_FIFO_SIZE_FUDGE_FACTOR	8

/* ucc_fast_channel_protocol_mode - UCC FAST mode */
enum ucc_fast_channel_protocol_mode {
	UCC_FAST_PROTOCOL_MODE_HDLC = 0x00000000,
	UCC_FAST_PROTOCOL_MODE_RESERVED01 = 0x00000001,
	UCC_FAST_PROTOCOL_MODE_RESERVED_QMC = 0x00000002,
	UCC_FAST_PROTOCOL_MODE_RESERVED02 = 0x00000003,
	UCC_FAST_PROTOCOL_MODE_RESERVED_UART = 0x00000004,
	UCC_FAST_PROTOCOL_MODE_RESERVED03 = 0x00000005,
	UCC_FAST_PROTOCOL_MODE_RESERVED_EX_MAC_1 = 0x00000006,
	UCC_FAST_PROTOCOL_MODE_RESERVED_EX_MAC_2 = 0x00000007,
	UCC_FAST_PROTOCOL_MODE_RESERVED_BISYNC = 0x00000008,
	UCC_FAST_PROTOCOL_MODE_RESERVED04 = 0x00000009,
	UCC_FAST_PROTOCOL_MODE_ATM = 0x0000000A,
	UCC_FAST_PROTOCOL_MODE_RESERVED05 = 0x0000000B,
	UCC_FAST_PROTOCOL_MODE_ETHERNET = 0x0000000C,
	UCC_FAST_PROTOCOL_MODE_RESERVED06 = 0x0000000D,
	UCC_FAST_PROTOCOL_MODE_POS = 0x0000000E,
	UCC_FAST_PROTOCOL_MODE_RESERVED07 = 0x0000000F
};

/* ucc_fast_transparent_txrx - UCC Fast Transparent TX & RX */
enum ucc_fast_transparent_txrx {
	UCC_FAST_GUMR_TRANSPARENT_TTX_TRX_NORMAL = 0x00000000,
	UCC_FAST_GUMR_TRANSPARENT_TTX_TRX_TRANSPARENT = 0x18000000
};

/* UCC fast diagnostic mode */
enum ucc_fast_diag_mode {
	UCC_FAST_DIAGNOSTIC_NORMAL = 0x0,
	UCC_FAST_DIAGNOSTIC_LOCAL_LOOP_BACK = 0x40000000,
	UCC_FAST_DIAGNOSTIC_AUTO_ECHO = 0x80000000,
	UCC_FAST_DIAGNOSTIC_LOOP_BACK_AND_ECHO = 0xC0000000
};

/* UCC fast Sync length (transparent mode only) */
enum ucc_fast_sync_len {
	UCC_FAST_SYNC_LEN_NOT_USED = 0x0,
	UCC_FAST_SYNC_LEN_AUTOMATIC = 0x00004000,
	UCC_FAST_SYNC_LEN_8_BIT = 0x00008000,
	UCC_FAST_SYNC_LEN_16_BIT = 0x0000C000
};

/* UCC fast RTS mode */
enum ucc_fast_ready_to_send {
	UCC_FAST_SEND_IDLES_BETWEEN_FRAMES = 0x00000000,
	UCC_FAST_SEND_FLAGS_BETWEEN_FRAMES = 0x00002000
};

/* UCC fast receiver decoding mode */
enum ucc_fast_rx_decoding_method {
	UCC_FAST_RX_ENCODING_NRZ = 0x00000000,
	UCC_FAST_RX_ENCODING_NRZI = 0x00000800,
	UCC_FAST_RX_ENCODING_RESERVED0 = 0x00001000,
	UCC_FAST_RX_ENCODING_RESERVED1 = 0x00001800
};

/* UCC fast transmitter encoding mode */
enum ucc_fast_tx_encoding_method {
	UCC_FAST_TX_ENCODING_NRZ = 0x00000000,
	UCC_FAST_TX_ENCODING_NRZI = 0x00000100,
	UCC_FAST_TX_ENCODING_RESERVED0 = 0x00000200,
	UCC_FAST_TX_ENCODING_RESERVED1 = 0x00000300
};

/* UCC fast CRC length */
enum ucc_fast_transparent_tcrc {
	UCC_FAST_16_BIT_CRC = 0x00000000,
	UCC_FAST_CRC_RESERVED0 = 0x00000040,
	UCC_FAST_32_BIT_CRC = 0x00000080,
	UCC_FAST_CRC_RESERVED1 = 0x000000C0
};

/* Fast UCC initialization structure */
struct ucc_fast_info {
	int ucc_num;
	enum qe_clock rx_clock;
	enum qe_clock tx_clock;
	u32 regs;
	int irq;
	u32 uccm_mask;
	int bd_mem_part;
	int brkpt_support;
	int grant_support;
	int tsa;
	int cdp;
	int cds;
	int ctsp;
	int ctss;
	int tci;
	int txsy;
	int rtsm;
	int revd;
	int rsyn;
	u16 max_rx_buf_length;
	u16 urfs;
	u16 urfet;
	u16 urfset;
	u16 utfs;
	u16 utfet;
	u16 utftt;
	u16 ufpt;
	enum ucc_fast_channel_protocol_mode mode;
	enum ucc_fast_transparent_txrx ttx_trx;
	enum ucc_fast_tx_encoding_method tenc;
	enum ucc_fast_rx_decoding_method renc;
	enum ucc_fast_transparent_tcrc tcrc;
	enum ucc_fast_sync_len synl;
};

struct ucc_fast_private {
	struct ucc_fast_info *uf_info;
	struct ucc_fast __iomem *uf_regs; /* a pointer to the UCC regs. */
	u32 __iomem *p_ucce;	/* a pointer to the event register in memory. */
	u32 __iomem *p_uccm;	/* a pointer to the mask register in memory. */
#ifdef CONFIG_UGETH_TX_ON_DEMAND
	u16 __iomem *p_utodr;	/* pointer to the transmit on demand register */
#endif
	int enabled_tx;		/* Whether channel is enabled for Tx (ENT) */
	int enabled_rx;		/* Whether channel is enabled for Rx (ENR) */
	int stopped_tx;		/* Whether channel has been stopped for Tx
				   (STOP_TX, etc.) */
	int stopped_rx;		/* Whether channel has been stopped for Rx */
	u32 ucc_fast_tx_virtual_fifo_base_offset;/* pointer to base of Tx
						    virtual fifo */
	u32 ucc_fast_rx_virtual_fifo_base_offset;/* pointer to base of Rx
						    virtual fifo */
#ifdef STATISTICS
	u32 tx_frames;		/* Transmitted frames counter. */
	u32 rx_frames;		/* Received frames counter (only frames
				   passed to application). */
	u32 tx_discarded;	/* Discarded tx frames counter (frames that
				   were discarded by the driver due to errors).
				   */
	u32 rx_discarded;	/* Discarded rx frames counter (frames that
				   were discarded by the driver due to errors).
				   */
#endif				/* STATISTICS */
	u16 mrblr;		/* maximum receive buffer length */
};

/* ucc_fast_init
 * Initializes Fast UCC according to user provided parameters.
 *
 * uf_info  - (In) pointer to the fast UCC info structure.
 * uccf_ret - (Out) pointer to the fast UCC structure.
 */
int ucc_fast_init(struct ucc_fast_info * uf_info, struct ucc_fast_private ** uccf_ret);

/* ucc_fast_free
 * Frees all resources for fast UCC.
 *
 * uccf - (In) pointer to the fast UCC structure.
 */
void ucc_fast_free(struct ucc_fast_private * uccf);

/* ucc_fast_enable
 * Enables a fast UCC port.
 * This routine enables Tx and/or Rx through the General UCC Mode Register.
 *
 * uccf - (In) pointer to the fast UCC structure.
 * mode - (In) TX, RX, or both.
 */
void ucc_fast_enable(struct ucc_fast_private * uccf, enum comm_dir mode);

/* ucc_fast_disable
 * Disables a fast UCC port.
 * This routine disables Tx and/or Rx through the General UCC Mode Register.
 *
 * uccf - (In) pointer to the fast UCC structure.
 * mode - (In) TX, RX, or both.
 */
void ucc_fast_disable(struct ucc_fast_private * uccf, enum comm_dir mode);

/* ucc_fast_irq
 * Handles interrupts on fast UCC.
 * Called from the general interrupt routine to handle interrupts on fast UCC.
 *
 * uccf - (In) pointer to the fast UCC structure.
 */
void ucc_fast_irq(struct ucc_fast_private * uccf);

/* ucc_fast_transmit_on_demand
 * Immediately forces a poll of the transmitter for data to be sent.
 * Typically, the hardware performs a periodic poll for data that the
 * transmit routine has set up to be transmitted. In cases where
 * this polling cycle is not soon enough, this optional routine can
 * be invoked to force a poll right away, instead. Proper use for
 * each transmission for which this functionality is desired is to
 * call the transmit routine and then this routine right after.
 *
 * uccf - (In) pointer to the fast UCC structure.
 */
void ucc_fast_transmit_on_demand(struct ucc_fast_private * uccf);

u32 ucc_fast_get_qe_cr_subblock(int uccf_num);

void ucc_fast_dump_regs(struct ucc_fast_private * uccf);

#endif				/* __UCC_FAST_H__ */
