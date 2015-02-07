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
 * Copyright (C) 2001 - 2007 Jeff Dike (jdike@{addtoit,linux.intel}.com)
 * Licensed under the GPL
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <linux/if_tun.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/uio.h>
#include "kern_constants.h"
#include "kern_util.h"
#include "os.h"
#include "tuntap.h"
#include "user.h"

static int tuntap_user_init(void *data, void *dev)
{
	struct tuntap_data *pri = data;

	pri->dev = dev;
	return 0;
}

static void tuntap_add_addr(unsigned char *addr, unsigned char *netmask,
			    void *data)
{
	struct tuntap_data *pri = data;

	tap_check_ips(pri->gate_addr, addr);
	if ((pri->fd == -1) || pri->fixed_config)
		return;
	open_addr(addr, netmask, pri->dev_name);
}

static void tuntap_del_addr(unsigned char *addr, unsigned char *netmask,
			    void *data)
{
	struct tuntap_data *pri = data;

	if ((pri->fd == -1) || pri->fixed_config)
		return;
	close_addr(addr, netmask, pri->dev_name);
}

struct tuntap_pre_exec_data {
	int stdout;
	int close_me;
};

static void tuntap_pre_exec(void *arg)
{
	struct tuntap_pre_exec_data *data = arg;

	dup2(data->stdout, 1);
	close(data->close_me);
}

static int tuntap_open_tramp(char *gate, int *fd_out, int me, int remote,
			     char *buffer, int buffer_len, int *used_out)
{
	struct tuntap_pre_exec_data data;
	char version_buf[sizeof("nnnnn\0")];
	char *argv[] = { "uml_net", version_buf, "tuntap", "up", gate,
			 NULL };
	char buf[CMSG_SPACE(sizeof(*fd_out))];
	struct msghdr msg;
	struct cmsghdr *cmsg;
	struct iovec iov;
	int pid, n, err;

	sprintf(version_buf, "%d", UML_NET_VERSION);

	data.stdout = remote;
	data.close_me = me;

	pid = run_helper(tuntap_pre_exec, &data, argv);

	if (pid < 0)
		return -pid;

	close(remote);

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	if (buffer != NULL) {
		iov = ((struct iovec) { buffer, buffer_len });
		msg.msg_iov = &iov;
		msg.msg_iovlen = 1;
	}
	else {
		msg.msg_iov = NULL;
		msg.msg_iovlen = 0;
	}
	msg.msg_control = buf;
	msg.msg_controllen = sizeof(buf);
	msg.msg_flags = 0;
	n = recvmsg(me, &msg, 0);
	*used_out = n;
	if (n < 0) {
		err = -errno;
		printk(UM_KERN_ERR "tuntap_open_tramp : recvmsg failed - "
		       "errno = %d\n", errno);
		return err;
	}
	helper_wait(pid);

	cmsg = CMSG_FIRSTHDR(&msg);
	if (cmsg == NULL) {
		printk(UM_KERN_ERR "tuntap_open_tramp : didn't receive a "
		       "message\n");
		return -EINVAL;
	}
	if ((cmsg->cmsg_level != SOL_SOCKET) ||
	   (cmsg->cmsg_type != SCM_RIGHTS)) {
		printk(UM_KERN_ERR "tuntap_open_tramp : didn't receive a "
		       "descriptor\n");
		return -EINVAL;
	}
	*fd_out = ((int *) CMSG_DATA(cmsg))[0];
	os_set_exec_close(*fd_out);
	return 0;
}

static int tuntap_open(void *data)
{
	struct ifreq ifr;
	struct tuntap_data *pri = data;
	char *output, *buffer;
	int err, fds[2], len, used;

	err = tap_open_common(pri->dev, pri->gate_addr);
	if (err < 0)
		return err;

	if (pri->fixed_config) {
		pri->fd = os_open_file("/dev/net/tun",
				       of_cloexec(of_rdwr(OPENFLAGS())), 0);
		if (pri->fd < 0) {
			printk(UM_KERN_ERR "Failed to open /dev/net/tun, "
			       "err = %d\n", -pri->fd);
			return pri->fd;
		}
		memset(&ifr, 0, sizeof(ifr));
		ifr.ifr_flags = IFF_TAP | IFF_NO_PI;
		strlcpy(ifr.ifr_name, pri->dev_name, sizeof(ifr.ifr_name));
		if (ioctl(pri->fd, TUNSETIFF, &ifr) < 0) {
			err = -errno;
			printk(UM_KERN_ERR "TUNSETIFF failed, errno = %d\n",
			       errno);
			close(pri->fd);
			return err;
		}
	}
	else {
		err = socketpair(AF_UNIX, SOCK_DGRAM, 0, fds);
		if (err) {
			err = -errno;
			printk(UM_KERN_ERR "tuntap_open : socketpair failed - "
			       "errno = %d\n", errno);
			return err;
		}

		buffer = get_output_buffer(&len);
		if (buffer != NULL)
			len--;
		used = 0;

		err = tuntap_open_tramp(pri->gate_addr, &pri->fd, fds[0],
					fds[1], buffer, len, &used);

		output = buffer;
		if (err < 0) {
			printk("%s", output);
			free_output_buffer(buffer);
			printk(UM_KERN_ERR "tuntap_open_tramp failed - "
			       "err = %d\n", -err);
			return err;
		}

		pri->dev_name = uml_strdup(buffer);
		output += IFNAMSIZ;
		printk("%s", output);
		free_output_buffer(buffer);

		close(fds[0]);
		iter_addresses(pri->dev, open_addr, pri->dev_name);
	}

	return pri->fd;
}

static void tuntap_close(int fd, void *data)
{
	struct tuntap_data *pri = data;

	if (!pri->fixed_config)
		iter_addresses(pri->dev, close_addr, pri->dev_name);
	close(fd);
	pri->fd = -1;
}

const struct net_user_info tuntap_user_info = {
	.init		= tuntap_user_init,
	.open		= tuntap_open,
	.close	 	= tuntap_close,
	.remove	 	= NULL,
	.add_address	= tuntap_add_addr,
	.delete_address = tuntap_del_addr,
	.mtu		= ETH_MAX_PACKET,
	.max_packet	= ETH_MAX_PACKET + ETH_HEADER_OTHER,
};
