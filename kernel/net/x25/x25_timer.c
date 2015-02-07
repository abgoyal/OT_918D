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
 *	X.25 Packet Layer release 002
 *
 *	This is ALPHA test software. This code may break your machine,
 *	randomly fail to work with new releases, misbehave and/or generally
 *	screw up. It might even work.
 *
 *	This code REQUIRES 2.1.15 or higher
 *
 *	This module:
 *		This module is free software; you can redistribute it and/or
 *		modify it under the terms of the GNU General Public License
 *		as published by the Free Software Foundation; either version
 *		2 of the License, or (at your option) any later version.
 *
 *	History
 *	X.25 001	Jonathan Naylor	Started coding.
 *	X.25 002	Jonathan Naylor	New timer architecture.
 *					Centralised disconnection processing.
 */

#include <linux/errno.h>
#include <linux/jiffies.h>
#include <linux/timer.h>
#include <net/sock.h>
#include <net/tcp_states.h>
#include <net/x25.h>

static void x25_heartbeat_expiry(unsigned long);
static void x25_timer_expiry(unsigned long);

void x25_init_timers(struct sock *sk)
{
	struct x25_sock *x25 = x25_sk(sk);

	setup_timer(&x25->timer, x25_timer_expiry, (unsigned long)sk);

	/* initialized by sock_init_data */
	sk->sk_timer.data     = (unsigned long)sk;
	sk->sk_timer.function = &x25_heartbeat_expiry;
}

void x25_start_heartbeat(struct sock *sk)
{
	mod_timer(&sk->sk_timer, jiffies + 5 * HZ);
}

void x25_stop_heartbeat(struct sock *sk)
{
	del_timer(&sk->sk_timer);
}

void x25_start_t2timer(struct sock *sk)
{
	struct x25_sock *x25 = x25_sk(sk);

	mod_timer(&x25->timer, jiffies + x25->t2);
}

void x25_start_t21timer(struct sock *sk)
{
	struct x25_sock *x25 = x25_sk(sk);

	mod_timer(&x25->timer, jiffies + x25->t21);
}

void x25_start_t22timer(struct sock *sk)
{
	struct x25_sock *x25 = x25_sk(sk);

	mod_timer(&x25->timer, jiffies + x25->t22);
}

void x25_start_t23timer(struct sock *sk)
{
	struct x25_sock *x25 = x25_sk(sk);

	mod_timer(&x25->timer, jiffies + x25->t23);
}

void x25_stop_timer(struct sock *sk)
{
	del_timer(&x25_sk(sk)->timer);
}

unsigned long x25_display_timer(struct sock *sk)
{
	struct x25_sock *x25 = x25_sk(sk);

	if (!timer_pending(&x25->timer))
		return 0;

	return x25->timer.expires - jiffies;
}

static void x25_heartbeat_expiry(unsigned long param)
{
	struct sock *sk = (struct sock *)param;

	bh_lock_sock(sk);
	if (sock_owned_by_user(sk)) /* can currently only occur in state 3 */
		goto restart_heartbeat;

	switch (x25_sk(sk)->state) {

		case X25_STATE_0:
			/*
			 * Magic here: If we listen() and a new link dies
			 * before it is accepted() it isn't 'dead' so doesn't
			 * get removed.
			 */
			if (sock_flag(sk, SOCK_DESTROY) ||
			    (sk->sk_state == TCP_LISTEN &&
			     sock_flag(sk, SOCK_DEAD))) {
				bh_unlock_sock(sk);
				x25_destroy_socket_from_timer(sk);
				return;
			}
			break;

		case X25_STATE_3:
			/*
			 * Check for the state of the receive buffer.
			 */
			x25_check_rbuf(sk);
			break;
	}
restart_heartbeat:
	x25_start_heartbeat(sk);
	bh_unlock_sock(sk);
}

/*
 *	Timer has expired, it may have been T2, T21, T22, or T23. We can tell
 *	by the state machine state.
 */
static inline void x25_do_timer_expiry(struct sock * sk)
{
	struct x25_sock *x25 = x25_sk(sk);

	switch (x25->state) {

		case X25_STATE_3:	/* T2 */
			if (x25->condition & X25_COND_ACK_PENDING) {
				x25->condition &= ~X25_COND_ACK_PENDING;
				x25_enquiry_response(sk);
			}
			break;

		case X25_STATE_1:	/* T21 */
		case X25_STATE_4:	/* T22 */
			x25_write_internal(sk, X25_CLEAR_REQUEST);
			x25->state = X25_STATE_2;
			x25_start_t23timer(sk);
			break;

		case X25_STATE_2:	/* T23 */
			x25_disconnect(sk, ETIMEDOUT, 0, 0);
			break;
	}
}

static void x25_timer_expiry(unsigned long param)
{
	struct sock *sk = (struct sock *)param;

	bh_lock_sock(sk);
	if (sock_owned_by_user(sk)) { /* can currently only occur in state 3 */
		if (x25_sk(sk)->state == X25_STATE_3)
			x25_start_t2timer(sk);
	} else
		x25_do_timer_expiry(sk);
	bh_unlock_sock(sk);
}