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
 * user-mode-linux networking multicast transport
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Copyright (C) 2001 by Harald Welte <laforge@gnumonks.org>
 *
 * based on the existing uml-networking code, which is
 * Copyright (C) 2001 Lennert Buytenhek (buytenh@gnu.org) and
 * James Leu (jleu@mindspring.net).
 * Copyright (C) 2001 by various other people who didn't put their name here.
 *
 * Licensed under the GPL.
 *
 */

#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include "kern_constants.h"
#include "mcast.h"
#include "net_user.h"
#include "um_malloc.h"
#include "user.h"

static struct sockaddr_in *new_addr(char *addr, unsigned short port)
{
	struct sockaddr_in *sin;

	sin = uml_kmalloc(sizeof(struct sockaddr_in), UM_GFP_KERNEL);
	if (sin == NULL) {
		printk(UM_KERN_ERR "new_addr: allocation of sockaddr_in "
		       "failed\n");
		return NULL;
	}
	sin->sin_family = AF_INET;
	sin->sin_addr.s_addr = in_aton(addr);
	sin->sin_port = htons(port);
	return sin;
}

static int mcast_user_init(void *data, void *dev)
{
	struct mcast_data *pri = data;

	pri->mcast_addr = new_addr(pri->addr, pri->port);
	pri->dev = dev;
	return 0;
}

static void mcast_remove(void *data)
{
	struct mcast_data *pri = data;

	kfree(pri->mcast_addr);
	pri->mcast_addr = NULL;
}

static int mcast_open(void *data)
{
	struct mcast_data *pri = data;
	struct sockaddr_in *sin = pri->mcast_addr;
	struct ip_mreq mreq;
	int fd, yes = 1, err = -EINVAL;


	if ((sin->sin_addr.s_addr == 0) || (sin->sin_port == 0))
		goto out;

	fd = socket(AF_INET, SOCK_DGRAM, 0);

	if (fd < 0) {
		err = -errno;
		printk(UM_KERN_ERR "mcast_open : data socket failed, "
		       "errno = %d\n", errno);
		goto out;
	}

	if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) < 0) {
		err = -errno;
		printk(UM_KERN_ERR "mcast_open: SO_REUSEADDR failed, "
		       "errno = %d\n", errno);
		goto out_close;
	}

	/* set ttl according to config */
	if (setsockopt(fd, SOL_IP, IP_MULTICAST_TTL, &pri->ttl,
		       sizeof(pri->ttl)) < 0) {
		err = -errno;
		printk(UM_KERN_ERR "mcast_open: IP_MULTICAST_TTL failed, "
		       "error = %d\n", errno);
		goto out_close;
	}

	/* set LOOP, so data does get fed back to local sockets */
	if (setsockopt(fd, SOL_IP, IP_MULTICAST_LOOP, &yes, sizeof(yes)) < 0) {
		err = -errno;
		printk(UM_KERN_ERR "mcast_open: IP_MULTICAST_LOOP failed, "
		       "error = %d\n", errno);
		goto out_close;
	}

	/* bind socket to mcast address */
	if (bind(fd, (struct sockaddr *) sin, sizeof(*sin)) < 0) {
		err = -errno;
		printk(UM_KERN_ERR "mcast_open : data bind failed, "
		       "errno = %d\n", errno);
		goto out_close;
	}

	/* subscribe to the multicast group */
	mreq.imr_multiaddr.s_addr = sin->sin_addr.s_addr;
	mreq.imr_interface.s_addr = 0;
	if (setsockopt(fd, SOL_IP, IP_ADD_MEMBERSHIP,
		       &mreq, sizeof(mreq)) < 0) {
		err = -errno;
		printk(UM_KERN_ERR "mcast_open: IP_ADD_MEMBERSHIP failed, "
		       "error = %d\n", errno);
		printk(UM_KERN_ERR "There appears not to be a multicast-"
		       "capable network interface on the host.\n");
		printk(UM_KERN_ERR "eth0 should be configured in order to use "
		       "the multicast transport.\n");
		goto out_close;
	}

	return fd;

 out_close:
	close(fd);
 out:
	return err;
}

static void mcast_close(int fd, void *data)
{
	struct ip_mreq mreq;
	struct mcast_data *pri = data;
	struct sockaddr_in *sin = pri->mcast_addr;

	mreq.imr_multiaddr.s_addr = sin->sin_addr.s_addr;
	mreq.imr_interface.s_addr = 0;
	if (setsockopt(fd, SOL_IP, IP_DROP_MEMBERSHIP,
		       &mreq, sizeof(mreq)) < 0) {
		printk(UM_KERN_ERR "mcast_open: IP_DROP_MEMBERSHIP failed, "
		       "error = %d\n", errno);
	}

	close(fd);
}

int mcast_user_write(int fd, void *buf, int len, struct mcast_data *pri)
{
	struct sockaddr_in *data_addr = pri->mcast_addr;

	return net_sendto(fd, buf, len, data_addr, sizeof(*data_addr));
}

const struct net_user_info mcast_user_info = {
	.init		= mcast_user_init,
	.open		= mcast_open,
	.close	 	= mcast_close,
	.remove	 	= mcast_remove,
	.add_address	= NULL,
	.delete_address = NULL,
	.mtu		= ETH_MAX_PACKET,
	.max_packet	= ETH_MAX_PACKET + ETH_HEADER_OTHER,
};
