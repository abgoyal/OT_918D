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
 * Copyright (c) 2007-2008 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
/*                                                                      */
/*  Module Name : wrap_pkt.c                                            */
/*                                                                      */
/*  Abstract                                                            */
/*     This module contains wrapper functions for packet handling       */
/*                                                                      */
/*  NOTES                                                               */
/*     Platform dependent.                                              */
/*                                                                      */
/************************************************************************/

#include "oal_dt.h"
#include "usbdrv.h"

#include <linux/netlink.h>
#include <linux/gfp.h>
#include <net/iw_handler.h>


/* extern struct zsWdsStruct wds[ZM_WDS_PORT_NUMBER]; 	*/
extern struct zsVapStruct vap[ZM_VAP_PORT_NUMBER];


/***** Rx *****/
void zfLnxRecv80211(zdev_t *dev, zbuf_t *buf, struct zsAdditionInfo *addInfo)
{
	u16_t frameType;
	u16_t frameCtrl;
	u16_t frameSubtype;
	zbuf_t *skb1;
	struct usbdrv_private *macp = dev->ml_priv;

	/* frameCtrl = zmw_buf_readb(dev, buf, 0);	*/
	frameCtrl = *(u8_t *)((u8_t *)buf->data);
	frameType = frameCtrl & 0xf;
	frameSubtype = frameCtrl & 0xf0;

	if ((frameType == 0x0) && (macp->forwardMgmt)) {
		switch (frameSubtype) {
			/* Beacon */
		case 0x80:
			/* Probe response */
		case 0x50:
			skb1 = skb_copy(buf, GFP_ATOMIC);
			if (skb1 != NULL) {
				skb1->dev = dev;
				skb_reset_mac_header(skb1);
				skb1->ip_summed = CHECKSUM_NONE;
				skb1->pkt_type = PACKET_OTHERHOST;
				/* ETH_P_80211_RAW */
				skb1->protocol = __constant_htons(0x0019);
				netif_rx(skb1);
			}
			break;
		default:
			break;
		}
	}

	zfiRecv80211(dev, buf, addInfo);
	return;
}

#define ZM_AVOID_UDP_LARGE_PACKET_FAIL
void zfLnxRecvEth(zdev_t *dev, zbuf_t *buf, u16_t port)
{
	struct usbdrv_private *macp = dev->ml_priv;
#ifdef ZM_AVOID_UDP_LARGE_PACKET_FAIL
	zbuf_t *new_buf;

	/* new_buf = dev_alloc_skb(2048);	*/
	new_buf = dev_alloc_skb(buf->len);

	skb_reset_tail_pointer(new_buf);

	skb_put(new_buf, buf->len);
	memcpy(new_buf->data, buf->data, buf->len);

	/* Free buffer */
	dev_kfree_skb_any(buf);

	if (port == 0) {
		new_buf->dev = dev;
		new_buf->protocol = eth_type_trans(new_buf, dev);
	} else {
		/* VAP */
		if (vap[0].dev != NULL) {
			new_buf->dev = vap[0].dev;
			new_buf->protocol = eth_type_trans(new_buf, vap[0].dev);
		} else {
			new_buf->dev = dev;
			new_buf->protocol = eth_type_trans(new_buf, dev);
		}
	}

	new_buf->ip_summed = CHECKSUM_NONE;
	dev->last_rx = jiffies;

	switch (netif_rx(new_buf))
#else
	if (port == 0) {
		buf->dev = dev;
		buf->protocol = eth_type_trans(buf, dev);
	} else {
		/* VAP */
		if (vap[0].dev != NULL) {
			buf->dev = vap[0].dev;
			buf->protocol = eth_type_trans(buf, vap[0].dev);
		} else {
			buf->dev = dev;
			buf->protocol = eth_type_trans(buf, dev);
		}
	}

	buf->ip_summed = CHECKSUM_NONE;
	dev->last_rx = jiffies;

	switch (netif_rx(buf))
#endif
	{
	case NET_RX_DROP:
		break;
	default:
			macp->drv_stats.net_stats.rx_packets++;
			macp->drv_stats.net_stats.rx_bytes += buf->len;
		break;
	}

	return;
}

/* Leave an empty line below to remove warning message on some compiler */
