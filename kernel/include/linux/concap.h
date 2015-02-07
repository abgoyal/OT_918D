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

/* $Id: concap.h,v 1.3.2.2 2004/01/12 23:08:35 keil Exp $
 *
 * Copyright 1997 by Henner Eisen <eis@baty.hanse.de>
 *
 * This software may be used and distributed according to the terms
 * of the GNU General Public License, incorporated herein by reference.
 */

#ifndef _LINUX_CONCAP_H
#define _LINUX_CONCAP_H

#include <linux/skbuff.h>
#include <linux/netdevice.h>

/* Stuff to support encapsulation protocols genericly. The encapsulation
   protocol is processed at the uppermost layer of the network interface.

   Based on a ideas developed in a 'synchronous device' thread in the
   linux-x25 mailing list contributed by Alan Cox, Thomasz Motylewski
   and Jonathan Naylor.

   For more documetation on this refer to Documentation/isdn/README.concap
*/

struct concap_proto_ops;
struct concap_device_ops;

/* this manages all data needed by the encapsulation protocol
 */
struct concap_proto{
	struct net_device *net_dev;	/* net device using our service  */
	struct concap_device_ops *dops;	/* callbacks provided by device */
 	struct concap_proto_ops  *pops;	/* callbacks provided by us */
 	spinlock_t lock;
	int flags;
	void *proto_data;		/* protocol specific private data, to
					   be accessed via *pops methods only*/
	/*
	  :
	  whatever 
	  :
	  */
};

/* Operations to be supported by the net device. Called by the encapsulation
 * protocol entity. No receive method is offered because the encapsulation
 * protocol directly calls netif_rx().
 */
struct concap_device_ops{

	/* to request data is submitted by device*/ 
	int (*data_req)(struct concap_proto *, struct sk_buff *);

	/* Control methods must be set to NULL by devices which do not
	   support connection control.*/
	/* to request a connection is set up */ 
	int (*connect_req)(struct concap_proto *);

	/* to request a connection is released */
	int (*disconn_req)(struct concap_proto *);	
};

/* Operations to be supported by the encapsulation protocol. Called by
 * device driver.
 */
struct concap_proto_ops{

	/* create a new encapsulation protocol instance of same type */
	struct concap_proto *  (*proto_new) (void);

	/* delete encapsulation protocol instance and free all its resources.
	   cprot may no loger be referenced after calling this */
	void (*proto_del)(struct concap_proto *cprot);

	/* initialize the protocol's data. To be called at interface startup
	   or when the device driver resets the interface. All services of the
	   encapsulation protocol may be used after this*/
	int (*restart)(struct concap_proto *cprot, 
		       struct net_device *ndev,
		       struct concap_device_ops *dops);

	/* inactivate an encapsulation protocol instance. The encapsulation
	   protocol may not call any *dops methods after this. */
	int (*close)(struct concap_proto *cprot);

	/* process a frame handed down to us by upper layer */
	int (*encap_and_xmit)(struct concap_proto *cprot, struct sk_buff *skb);

	/* to be called for each data entity received from lower layer*/ 
	int (*data_ind)(struct concap_proto *cprot, struct sk_buff *skb);

	/* to be called when a connection was set up/down.
	   Protocols that don't process these primitives might fill in
	   dummy methods here */
	int (*connect_ind)(struct concap_proto *cprot);
	int (*disconn_ind)(struct concap_proto *cprot);
  /*
    Some network device support functions, like net_header(), rebuild_header(),
    and others, that depend solely on the encapsulation protocol, might
    be provided here, too. The net device would just fill them in its
    corresponding fields when it is opened.
    */
};

/* dummy restart/close/connect/reset/disconn methods
 */
extern int concap_nop(struct concap_proto *cprot); 

/* dummy submit method
 */
extern int concap_drop_skb(struct concap_proto *cprot, struct sk_buff *skb);
#endif
