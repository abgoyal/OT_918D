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


#include "ceph_debug.h"

#include <linux/err.h>
#include <linux/module.h>
#include <linux/random.h>
#include <linux/slab.h>

#include "auth_none.h"
#include "auth.h"
#include "decode.h"

static void reset(struct ceph_auth_client *ac)
{
	struct ceph_auth_none_info *xi = ac->private;

	xi->starting = true;
	xi->built_authorizer = false;
}

static void destroy(struct ceph_auth_client *ac)
{
	kfree(ac->private);
	ac->private = NULL;
}

static int is_authenticated(struct ceph_auth_client *ac)
{
	struct ceph_auth_none_info *xi = ac->private;

	return !xi->starting;
}

static int should_authenticate(struct ceph_auth_client *ac)
{
	struct ceph_auth_none_info *xi = ac->private;

	return xi->starting;
}

/*
 * the generic auth code decode the global_id, and we carry no actual
 * authenticate state, so nothing happens here.
 */
static int handle_reply(struct ceph_auth_client *ac, int result,
			void *buf, void *end)
{
	struct ceph_auth_none_info *xi = ac->private;

	xi->starting = false;
	return result;
}

/*
 * build an 'authorizer' with our entity_name and global_id.  we can
 * reuse a single static copy since it is identical for all services
 * we connect to.
 */
static int ceph_auth_none_create_authorizer(
	struct ceph_auth_client *ac, int peer_type,
	struct ceph_authorizer **a,
	void **buf, size_t *len,
	void **reply_buf, size_t *reply_len)
{
	struct ceph_auth_none_info *ai = ac->private;
	struct ceph_none_authorizer *au = &ai->au;
	void *p, *end;
	int ret;

	if (!ai->built_authorizer) {
		p = au->buf;
		end = p + sizeof(au->buf);
		ceph_encode_8(&p, 1);
		ret = ceph_entity_name_encode(ac->name, &p, end - 8);
		if (ret < 0)
			goto bad;
		ceph_decode_need(&p, end, sizeof(u64), bad2);
		ceph_encode_64(&p, ac->global_id);
		au->buf_len = p - (void *)au->buf;
		ai->built_authorizer = true;
		dout("built authorizer len %d\n", au->buf_len);
	}

	*a = (struct ceph_authorizer *)au;
	*buf = au->buf;
	*len = au->buf_len;
	*reply_buf = au->reply_buf;
	*reply_len = sizeof(au->reply_buf);
	return 0;

bad2:
	ret = -ERANGE;
bad:
	return ret;
}

static void ceph_auth_none_destroy_authorizer(struct ceph_auth_client *ac,
				      struct ceph_authorizer *a)
{
	/* nothing to do */
}

static const struct ceph_auth_client_ops ceph_auth_none_ops = {
	.name = "none",
	.reset = reset,
	.destroy = destroy,
	.is_authenticated = is_authenticated,
	.should_authenticate = should_authenticate,
	.handle_reply = handle_reply,
	.create_authorizer = ceph_auth_none_create_authorizer,
	.destroy_authorizer = ceph_auth_none_destroy_authorizer,
};

int ceph_auth_none_init(struct ceph_auth_client *ac)
{
	struct ceph_auth_none_info *xi;

	dout("ceph_auth_none_init %p\n", ac);
	xi = kzalloc(sizeof(*xi), GFP_NOFS);
	if (!xi)
		return -ENOMEM;

	xi->starting = true;
	xi->built_authorizer = false;

	ac->protocol = CEPH_AUTH_NONE;
	ac->private = xi;
	ac->ops = &ceph_auth_none_ops;
	return 0;
}

