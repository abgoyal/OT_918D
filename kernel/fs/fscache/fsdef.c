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

/* Filesystem index definition
 *
 * Copyright (C) 2004-2007 Red Hat, Inc. All Rights Reserved.
 * Written by David Howells (dhowells@redhat.com)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */

#define FSCACHE_DEBUG_LEVEL CACHE
#include <linux/module.h>
#include "internal.h"

static uint16_t fscache_fsdef_netfs_get_key(const void *cookie_netfs_data,
					    void *buffer, uint16_t bufmax);

static uint16_t fscache_fsdef_netfs_get_aux(const void *cookie_netfs_data,
					    void *buffer, uint16_t bufmax);

static
enum fscache_checkaux fscache_fsdef_netfs_check_aux(void *cookie_netfs_data,
						    const void *data,
						    uint16_t datalen);

/*
 * The root index is owned by FS-Cache itself.
 *
 * When a netfs requests caching facilities, FS-Cache will, if one doesn't
 * already exist, create an entry in the root index with the key being the name
 * of the netfs ("AFS" for example), and the auxiliary data holding the index
 * structure version supplied by the netfs:
 *
 *				     FSDEF
 *				       |
 *				 +-----------+
 *				 |           |
 *				NFS         AFS
 *			       [v=1]       [v=1]
 *
 * If an entry with the appropriate name does already exist, the version is
 * compared.  If the version is different, the entire subtree from that entry
 * will be discarded and a new entry created.
 *
 * The new entry will be an index, and a cookie referring to it will be passed
 * to the netfs.  This is then the root handle by which the netfs accesses the
 * cache.  It can create whatever objects it likes in that index, including
 * further indices.
 */
static struct fscache_cookie_def fscache_fsdef_index_def = {
	.name		= ".FS-Cache",
	.type		= FSCACHE_COOKIE_TYPE_INDEX,
};

struct fscache_cookie fscache_fsdef_index = {
	.usage		= ATOMIC_INIT(1),
	.lock		= __SPIN_LOCK_UNLOCKED(fscache_fsdef_index.lock),
	.backing_objects = HLIST_HEAD_INIT,
	.def		= &fscache_fsdef_index_def,
};
EXPORT_SYMBOL(fscache_fsdef_index);

/*
 * Definition of an entry in the root index.  Each entry is an index, keyed to
 * a specific netfs and only applicable to a particular version of the index
 * structure used by that netfs.
 */
struct fscache_cookie_def fscache_fsdef_netfs_def = {
	.name		= "FSDEF.netfs",
	.type		= FSCACHE_COOKIE_TYPE_INDEX,
	.get_key	= fscache_fsdef_netfs_get_key,
	.get_aux	= fscache_fsdef_netfs_get_aux,
	.check_aux	= fscache_fsdef_netfs_check_aux,
};

/*
 * get the key data for an FSDEF index record - this is the name of the netfs
 * for which this entry is created
 */
static uint16_t fscache_fsdef_netfs_get_key(const void *cookie_netfs_data,
					    void *buffer, uint16_t bufmax)
{
	const struct fscache_netfs *netfs = cookie_netfs_data;
	unsigned klen;

	_enter("{%s.%u},", netfs->name, netfs->version);

	klen = strlen(netfs->name);
	if (klen > bufmax)
		return 0;

	memcpy(buffer, netfs->name, klen);
	return klen;
}

/*
 * get the auxiliary data for an FSDEF index record - this is the index
 * structure version number of the netfs for which this version is created
 */
static uint16_t fscache_fsdef_netfs_get_aux(const void *cookie_netfs_data,
					    void *buffer, uint16_t bufmax)
{
	const struct fscache_netfs *netfs = cookie_netfs_data;
	unsigned dlen;

	_enter("{%s.%u},", netfs->name, netfs->version);

	dlen = sizeof(uint32_t);
	if (dlen > bufmax)
		return 0;

	memcpy(buffer, &netfs->version, dlen);
	return dlen;
}

/*
 * check that the index structure version number stored in the auxiliary data
 * matches the one the netfs gave us
 */
static enum fscache_checkaux fscache_fsdef_netfs_check_aux(
	void *cookie_netfs_data,
	const void *data,
	uint16_t datalen)
{
	struct fscache_netfs *netfs = cookie_netfs_data;
	uint32_t version;

	_enter("{%s},,%hu", netfs->name, datalen);

	if (datalen != sizeof(version)) {
		_leave(" = OBSOLETE [dl=%d v=%zu]", datalen, sizeof(version));
		return FSCACHE_CHECKAUX_OBSOLETE;
	}

	memcpy(&version, data, sizeof(version));
	if (version != netfs->version) {
		_leave(" = OBSOLETE [ver=%x net=%x]", version, netfs->version);
		return FSCACHE_CHECKAUX_OBSOLETE;
	}

	_leave(" = OKAY");
	return FSCACHE_CHECKAUX_OKAY;
}
