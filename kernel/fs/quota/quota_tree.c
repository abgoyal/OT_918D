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
 *	vfsv0 quota IO operations on file
 */

#include <linux/errno.h>
#include <linux/fs.h>
#include <linux/mount.h>
#include <linux/dqblk_v2.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/slab.h>
#include <linux/quotaops.h>

#include <asm/byteorder.h>

#include "quota_tree.h"

MODULE_AUTHOR("Jan Kara");
MODULE_DESCRIPTION("Quota trie support");
MODULE_LICENSE("GPL");

#define __QUOTA_QT_PARANOIA

static int get_index(struct qtree_mem_dqinfo *info, qid_t id, int depth)
{
	unsigned int epb = info->dqi_usable_bs >> 2;

	depth = info->dqi_qtree_depth - depth - 1;
	while (depth--)
		id /= epb;
	return id % epb;
}

/* Number of entries in one blocks */
static int qtree_dqstr_in_blk(struct qtree_mem_dqinfo *info)
{
	return (info->dqi_usable_bs - sizeof(struct qt_disk_dqdbheader))
	       / info->dqi_entry_size;
}

static char *getdqbuf(size_t size)
{
	char *buf = kmalloc(size, GFP_NOFS);
	if (!buf)
		printk(KERN_WARNING
		       "VFS: Not enough memory for quota buffers.\n");
	return buf;
}

static ssize_t read_blk(struct qtree_mem_dqinfo *info, uint blk, char *buf)
{
	struct super_block *sb = info->dqi_sb;

	memset(buf, 0, info->dqi_usable_bs);
	return sb->s_op->quota_read(sb, info->dqi_type, buf,
	       info->dqi_usable_bs, blk << info->dqi_blocksize_bits);
}

static ssize_t write_blk(struct qtree_mem_dqinfo *info, uint blk, char *buf)
{
	struct super_block *sb = info->dqi_sb;
	ssize_t ret;

	ret = sb->s_op->quota_write(sb, info->dqi_type, buf,
	       info->dqi_usable_bs, blk << info->dqi_blocksize_bits);
	if (ret != info->dqi_usable_bs) {
		q_warn(KERN_WARNING "VFS: dquota write failed on "
			"dev %s\n", sb->s_id);
		if (ret >= 0)
			ret = -EIO;
	}
	return ret;
}

/* Remove empty block from list and return it */
static int get_free_dqblk(struct qtree_mem_dqinfo *info)
{
	char *buf = getdqbuf(info->dqi_usable_bs);
	struct qt_disk_dqdbheader *dh = (struct qt_disk_dqdbheader *)buf;
	int ret, blk;

	if (!buf)
		return -ENOMEM;
	if (info->dqi_free_blk) {
		blk = info->dqi_free_blk;
		ret = read_blk(info, blk, buf);
		if (ret < 0)
			goto out_buf;
		info->dqi_free_blk = le32_to_cpu(dh->dqdh_next_free);
	}
	else {
		memset(buf, 0, info->dqi_usable_bs);
		/* Assure block allocation... */
		ret = write_blk(info, info->dqi_blocks, buf);
		if (ret < 0)
			goto out_buf;
		blk = info->dqi_blocks++;
	}
	mark_info_dirty(info->dqi_sb, info->dqi_type);
	ret = blk;
out_buf:
	kfree(buf);
	return ret;
}

/* Insert empty block to the list */
static int put_free_dqblk(struct qtree_mem_dqinfo *info, char *buf, uint blk)
{
	struct qt_disk_dqdbheader *dh = (struct qt_disk_dqdbheader *)buf;
	int err;

	dh->dqdh_next_free = cpu_to_le32(info->dqi_free_blk);
	dh->dqdh_prev_free = cpu_to_le32(0);
	dh->dqdh_entries = cpu_to_le16(0);
	err = write_blk(info, blk, buf);
	if (err < 0)
		return err;
	info->dqi_free_blk = blk;
	mark_info_dirty(info->dqi_sb, info->dqi_type);
	return 0;
}

/* Remove given block from the list of blocks with free entries */
static int remove_free_dqentry(struct qtree_mem_dqinfo *info, char *buf,
			       uint blk)
{
	char *tmpbuf = getdqbuf(info->dqi_usable_bs);
	struct qt_disk_dqdbheader *dh = (struct qt_disk_dqdbheader *)buf;
	uint nextblk = le32_to_cpu(dh->dqdh_next_free);
	uint prevblk = le32_to_cpu(dh->dqdh_prev_free);
	int err;

	if (!tmpbuf)
		return -ENOMEM;
	if (nextblk) {
		err = read_blk(info, nextblk, tmpbuf);
		if (err < 0)
			goto out_buf;
		((struct qt_disk_dqdbheader *)tmpbuf)->dqdh_prev_free =
							dh->dqdh_prev_free;
		err = write_blk(info, nextblk, tmpbuf);
		if (err < 0)
			goto out_buf;
	}
	if (prevblk) {
		err = read_blk(info, prevblk, tmpbuf);
		if (err < 0)
			goto out_buf;
		((struct qt_disk_dqdbheader *)tmpbuf)->dqdh_next_free =
							dh->dqdh_next_free;
		err = write_blk(info, prevblk, tmpbuf);
		if (err < 0)
			goto out_buf;
	} else {
		info->dqi_free_entry = nextblk;
		mark_info_dirty(info->dqi_sb, info->dqi_type);
	}
	kfree(tmpbuf);
	dh->dqdh_next_free = dh->dqdh_prev_free = cpu_to_le32(0);
	/* No matter whether write succeeds block is out of list */
	if (write_blk(info, blk, buf) < 0)
		q_warn(KERN_ERR
		       "VFS: Can't write block (%u) with free entries.\n",
		       blk);
	return 0;
out_buf:
	kfree(tmpbuf);
	return err;
}

/* Insert given block to the beginning of list with free entries */
static int insert_free_dqentry(struct qtree_mem_dqinfo *info, char *buf,
			       uint blk)
{
	char *tmpbuf = getdqbuf(info->dqi_usable_bs);
	struct qt_disk_dqdbheader *dh = (struct qt_disk_dqdbheader *)buf;
	int err;

	if (!tmpbuf)
		return -ENOMEM;
	dh->dqdh_next_free = cpu_to_le32(info->dqi_free_entry);
	dh->dqdh_prev_free = cpu_to_le32(0);
	err = write_blk(info, blk, buf);
	if (err < 0)
		goto out_buf;
	if (info->dqi_free_entry) {
		err = read_blk(info, info->dqi_free_entry, tmpbuf);
		if (err < 0)
			goto out_buf;
		((struct qt_disk_dqdbheader *)tmpbuf)->dqdh_prev_free =
							cpu_to_le32(blk);
		err = write_blk(info, info->dqi_free_entry, tmpbuf);
		if (err < 0)
			goto out_buf;
	}
	kfree(tmpbuf);
	info->dqi_free_entry = blk;
	mark_info_dirty(info->dqi_sb, info->dqi_type);
	return 0;
out_buf:
	kfree(tmpbuf);
	return err;
}

/* Is the entry in the block free? */
int qtree_entry_unused(struct qtree_mem_dqinfo *info, char *disk)
{
	int i;

	for (i = 0; i < info->dqi_entry_size; i++)
		if (disk[i])
			return 0;
	return 1;
}
EXPORT_SYMBOL(qtree_entry_unused);

/* Find space for dquot */
static uint find_free_dqentry(struct qtree_mem_dqinfo *info,
			      struct dquot *dquot, int *err)
{
	uint blk, i;
	struct qt_disk_dqdbheader *dh;
	char *buf = getdqbuf(info->dqi_usable_bs);
	char *ddquot;

	*err = 0;
	if (!buf) {
		*err = -ENOMEM;
		return 0;
	}
	dh = (struct qt_disk_dqdbheader *)buf;
	if (info->dqi_free_entry) {
		blk = info->dqi_free_entry;
		*err = read_blk(info, blk, buf);
		if (*err < 0)
			goto out_buf;
	} else {
		blk = get_free_dqblk(info);
		if ((int)blk < 0) {
			*err = blk;
			kfree(buf);
			return 0;
		}
		memset(buf, 0, info->dqi_usable_bs);
		/* This is enough as the block is already zeroed and the entry
		 * list is empty... */
		info->dqi_free_entry = blk;
		mark_info_dirty(dquot->dq_sb, dquot->dq_type);
	}
	/* Block will be full? */
	if (le16_to_cpu(dh->dqdh_entries) + 1 >= qtree_dqstr_in_blk(info)) {
		*err = remove_free_dqentry(info, buf, blk);
		if (*err < 0) {
			q_warn(KERN_ERR "VFS: find_free_dqentry(): Can't "
			       "remove block (%u) from entry free list.\n",
			       blk);
			goto out_buf;
		}
	}
	le16_add_cpu(&dh->dqdh_entries, 1);
	/* Find free structure in block */
	ddquot = buf + sizeof(struct qt_disk_dqdbheader);
	for (i = 0; i < qtree_dqstr_in_blk(info); i++) {
		if (qtree_entry_unused(info, ddquot))
			break;
		ddquot += info->dqi_entry_size;
	}
#ifdef __QUOTA_QT_PARANOIA
	if (i == qtree_dqstr_in_blk(info)) {
		printk(KERN_ERR "VFS: find_free_dqentry(): Data block full "
				"but it shouldn't.\n");
		*err = -EIO;
		goto out_buf;
	}
#endif
	*err = write_blk(info, blk, buf);
	if (*err < 0) {
		q_warn(KERN_ERR "VFS: find_free_dqentry(): Can't write quota "
				"data block %u.\n", blk);
		goto out_buf;
	}
	dquot->dq_off = (blk << info->dqi_blocksize_bits) +
			sizeof(struct qt_disk_dqdbheader) +
			i * info->dqi_entry_size;
	kfree(buf);
	return blk;
out_buf:
	kfree(buf);
	return 0;
}

/* Insert reference to structure into the trie */
static int do_insert_tree(struct qtree_mem_dqinfo *info, struct dquot *dquot,
			  uint *treeblk, int depth)
{
	char *buf = getdqbuf(info->dqi_usable_bs);
	int ret = 0, newson = 0, newact = 0;
	__le32 *ref;
	uint newblk;

	if (!buf)
		return -ENOMEM;
	if (!*treeblk) {
		ret = get_free_dqblk(info);
		if (ret < 0)
			goto out_buf;
		*treeblk = ret;
		memset(buf, 0, info->dqi_usable_bs);
		newact = 1;
	} else {
		ret = read_blk(info, *treeblk, buf);
		if (ret < 0) {
			q_warn(KERN_ERR "VFS: Can't read tree quota block "
					"%u.\n", *treeblk);
			goto out_buf;
		}
	}
	ref = (__le32 *)buf;
	newblk = le32_to_cpu(ref[get_index(info, dquot->dq_id, depth)]);
	if (!newblk)
		newson = 1;
	if (depth == info->dqi_qtree_depth - 1) {
#ifdef __QUOTA_QT_PARANOIA
		if (newblk) {
			printk(KERN_ERR "VFS: Inserting already present quota "
					"entry (block %u).\n",
			       le32_to_cpu(ref[get_index(info,
						dquot->dq_id, depth)]));
			ret = -EIO;
			goto out_buf;
		}
#endif
		newblk = find_free_dqentry(info, dquot, &ret);
	} else {
		ret = do_insert_tree(info, dquot, &newblk, depth+1);
	}
	if (newson && ret >= 0) {
		ref[get_index(info, dquot->dq_id, depth)] =
							cpu_to_le32(newblk);
		ret = write_blk(info, *treeblk, buf);
	} else if (newact && ret < 0) {
		put_free_dqblk(info, buf, *treeblk);
	}
out_buf:
	kfree(buf);
	return ret;
}

/* Wrapper for inserting quota structure into tree */
static inline int dq_insert_tree(struct qtree_mem_dqinfo *info,
				 struct dquot *dquot)
{
	int tmp = QT_TREEOFF;
	return do_insert_tree(info, dquot, &tmp, 0);
}

/*
 * We don't have to be afraid of deadlocks as we never have quotas on quota
 * files...
 */
int qtree_write_dquot(struct qtree_mem_dqinfo *info, struct dquot *dquot)
{
	int type = dquot->dq_type;
	struct super_block *sb = dquot->dq_sb;
	ssize_t ret;
	char *ddquot = getdqbuf(info->dqi_entry_size);

	if (!ddquot)
		return -ENOMEM;

	/* dq_off is guarded by dqio_mutex */
	if (!dquot->dq_off) {
		ret = dq_insert_tree(info, dquot);
		if (ret < 0) {
			q_warn(KERN_ERR "VFS: Error %zd occurred while "
					"creating quota.\n", ret);
			kfree(ddquot);
			return ret;
		}
	}
	spin_lock(&dq_data_lock);
	info->dqi_ops->mem2disk_dqblk(ddquot, dquot);
	spin_unlock(&dq_data_lock);
	ret = sb->s_op->quota_write(sb, type, ddquot, info->dqi_entry_size,
				    dquot->dq_off);
	if (ret != info->dqi_entry_size) {
		q_warn(KERN_WARNING "VFS: dquota write failed on dev %s\n",
		       sb->s_id);
		if (ret >= 0)
			ret = -ENOSPC;
	} else {
		ret = 0;
	}
	dqstats_inc(DQST_WRITES);
	kfree(ddquot);

	return ret;
}
EXPORT_SYMBOL(qtree_write_dquot);

/* Free dquot entry in data block */
static int free_dqentry(struct qtree_mem_dqinfo *info, struct dquot *dquot,
			uint blk)
{
	struct qt_disk_dqdbheader *dh;
	char *buf = getdqbuf(info->dqi_usable_bs);
	int ret = 0;

	if (!buf)
		return -ENOMEM;
	if (dquot->dq_off >> info->dqi_blocksize_bits != blk) {
		q_warn(KERN_ERR "VFS: Quota structure has offset to other "
		  "block (%u) than it should (%u).\n", blk,
		  (uint)(dquot->dq_off >> info->dqi_blocksize_bits));
		goto out_buf;
	}
	ret = read_blk(info, blk, buf);
	if (ret < 0) {
		q_warn(KERN_ERR "VFS: Can't read quota data block %u\n", blk);
		goto out_buf;
	}
	dh = (struct qt_disk_dqdbheader *)buf;
	le16_add_cpu(&dh->dqdh_entries, -1);
	if (!le16_to_cpu(dh->dqdh_entries)) {	/* Block got free? */
		ret = remove_free_dqentry(info, buf, blk);
		if (ret >= 0)
			ret = put_free_dqblk(info, buf, blk);
		if (ret < 0) {
			q_warn(KERN_ERR "VFS: Can't move quota data block (%u) "
			  "to free list.\n", blk);
			goto out_buf;
		}
	} else {
		memset(buf +
		       (dquot->dq_off & ((1 << info->dqi_blocksize_bits) - 1)),
		       0, info->dqi_entry_size);
		if (le16_to_cpu(dh->dqdh_entries) ==
		    qtree_dqstr_in_blk(info) - 1) {
			/* Insert will write block itself */
			ret = insert_free_dqentry(info, buf, blk);
			if (ret < 0) {
				q_warn(KERN_ERR "VFS: Can't insert quota data "
				       "block (%u) to free entry list.\n", blk);
				goto out_buf;
			}
		} else {
			ret = write_blk(info, blk, buf);
			if (ret < 0) {
				q_warn(KERN_ERR "VFS: Can't write quota data "
				  "block %u\n", blk);
				goto out_buf;
			}
		}
	}
	dquot->dq_off = 0;	/* Quota is now unattached */
out_buf:
	kfree(buf);
	return ret;
}

/* Remove reference to dquot from tree */
static int remove_tree(struct qtree_mem_dqinfo *info, struct dquot *dquot,
		       uint *blk, int depth)
{
	char *buf = getdqbuf(info->dqi_usable_bs);
	int ret = 0;
	uint newblk;
	__le32 *ref = (__le32 *)buf;

	if (!buf)
		return -ENOMEM;
	ret = read_blk(info, *blk, buf);
	if (ret < 0) {
		q_warn(KERN_ERR "VFS: Can't read quota data block %u\n", *blk);
		goto out_buf;
	}
	newblk = le32_to_cpu(ref[get_index(info, dquot->dq_id, depth)]);
	if (depth == info->dqi_qtree_depth - 1) {
		ret = free_dqentry(info, dquot, newblk);
		newblk = 0;
	} else {
		ret = remove_tree(info, dquot, &newblk, depth+1);
	}
	if (ret >= 0 && !newblk) {
		int i;
		ref[get_index(info, dquot->dq_id, depth)] = cpu_to_le32(0);
		/* Block got empty? */
		for (i = 0; i < (info->dqi_usable_bs >> 2) && !ref[i]; i++)
			;
		/* Don't put the root block into the free block list */
		if (i == (info->dqi_usable_bs >> 2)
		    && *blk != QT_TREEOFF) {
			put_free_dqblk(info, buf, *blk);
			*blk = 0;
		} else {
			ret = write_blk(info, *blk, buf);
			if (ret < 0)
				q_warn(KERN_ERR "VFS: Can't write quota tree "
				  "block %u.\n", *blk);
		}
	}
out_buf:
	kfree(buf);
	return ret;
}

/* Delete dquot from tree */
int qtree_delete_dquot(struct qtree_mem_dqinfo *info, struct dquot *dquot)
{
	uint tmp = QT_TREEOFF;

	if (!dquot->dq_off)	/* Even not allocated? */
		return 0;
	return remove_tree(info, dquot, &tmp, 0);
}
EXPORT_SYMBOL(qtree_delete_dquot);

/* Find entry in block */
static loff_t find_block_dqentry(struct qtree_mem_dqinfo *info,
				 struct dquot *dquot, uint blk)
{
	char *buf = getdqbuf(info->dqi_usable_bs);
	loff_t ret = 0;
	int i;
	char *ddquot;

	if (!buf)
		return -ENOMEM;
	ret = read_blk(info, blk, buf);
	if (ret < 0) {
		q_warn(KERN_ERR "VFS: Can't read quota tree block %u.\n", blk);
		goto out_buf;
	}
	ddquot = buf + sizeof(struct qt_disk_dqdbheader);
	for (i = 0; i < qtree_dqstr_in_blk(info); i++) {
		if (info->dqi_ops->is_id(ddquot, dquot))
			break;
		ddquot += info->dqi_entry_size;
	}
	if (i == qtree_dqstr_in_blk(info)) {
		q_warn(KERN_ERR "VFS: Quota for id %u referenced "
		  "but not present.\n", dquot->dq_id);
		ret = -EIO;
		goto out_buf;
	} else {
		ret = (blk << info->dqi_blocksize_bits) + sizeof(struct
		  qt_disk_dqdbheader) + i * info->dqi_entry_size;
	}
out_buf:
	kfree(buf);
	return ret;
}

/* Find entry for given id in the tree */
static loff_t find_tree_dqentry(struct qtree_mem_dqinfo *info,
				struct dquot *dquot, uint blk, int depth)
{
	char *buf = getdqbuf(info->dqi_usable_bs);
	loff_t ret = 0;
	__le32 *ref = (__le32 *)buf;

	if (!buf)
		return -ENOMEM;
	ret = read_blk(info, blk, buf);
	if (ret < 0) {
		q_warn(KERN_ERR "VFS: Can't read quota tree block %u.\n", blk);
		goto out_buf;
	}
	ret = 0;
	blk = le32_to_cpu(ref[get_index(info, dquot->dq_id, depth)]);
	if (!blk)	/* No reference? */
		goto out_buf;
	if (depth < info->dqi_qtree_depth - 1)
		ret = find_tree_dqentry(info, dquot, blk, depth+1);
	else
		ret = find_block_dqentry(info, dquot, blk);
out_buf:
	kfree(buf);
	return ret;
}

/* Find entry for given id in the tree - wrapper function */
static inline loff_t find_dqentry(struct qtree_mem_dqinfo *info,
				  struct dquot *dquot)
{
	return find_tree_dqentry(info, dquot, QT_TREEOFF, 0);
}

int qtree_read_dquot(struct qtree_mem_dqinfo *info, struct dquot *dquot)
{
	int type = dquot->dq_type;
	struct super_block *sb = dquot->dq_sb;
	loff_t offset;
	char *ddquot;
	int ret = 0;

#ifdef __QUOTA_QT_PARANOIA
	/* Invalidated quota? */
	if (!sb_dqopt(dquot->dq_sb)->files[type]) {
		printk(KERN_ERR "VFS: Quota invalidated while reading!\n");
		return -EIO;
	}
#endif
	/* Do we know offset of the dquot entry in the quota file? */
	if (!dquot->dq_off) {
		offset = find_dqentry(info, dquot);
		if (offset <= 0) {	/* Entry not present? */
			if (offset < 0)
				q_warn(KERN_ERR "VFS: Can't read quota "
				  "structure for id %u.\n", dquot->dq_id);
			dquot->dq_off = 0;
			set_bit(DQ_FAKE_B, &dquot->dq_flags);
			memset(&dquot->dq_dqb, 0, sizeof(struct mem_dqblk));
			ret = offset;
			goto out;
		}
		dquot->dq_off = offset;
	}
	ddquot = getdqbuf(info->dqi_entry_size);
	if (!ddquot)
		return -ENOMEM;
	ret = sb->s_op->quota_read(sb, type, ddquot, info->dqi_entry_size,
				   dquot->dq_off);
	if (ret != info->dqi_entry_size) {
		if (ret >= 0)
			ret = -EIO;
		q_warn(KERN_ERR "VFS: Error while reading quota "
				"structure for id %u.\n", dquot->dq_id);
		set_bit(DQ_FAKE_B, &dquot->dq_flags);
		memset(&dquot->dq_dqb, 0, sizeof(struct mem_dqblk));
		kfree(ddquot);
		goto out;
	}
	spin_lock(&dq_data_lock);
	info->dqi_ops->disk2mem_dqblk(dquot, ddquot);
	if (!dquot->dq_dqb.dqb_bhardlimit &&
	    !dquot->dq_dqb.dqb_bsoftlimit &&
	    !dquot->dq_dqb.dqb_ihardlimit &&
	    !dquot->dq_dqb.dqb_isoftlimit)
		set_bit(DQ_FAKE_B, &dquot->dq_flags);
	spin_unlock(&dq_data_lock);
	kfree(ddquot);
out:
	dqstats_inc(DQST_READS);
	return ret;
}
EXPORT_SYMBOL(qtree_read_dquot);

/* Check whether dquot should not be deleted. We know we are
 * the only one operating on dquot (thanks to dq_lock) */
int qtree_release_dquot(struct qtree_mem_dqinfo *info, struct dquot *dquot)
{
	if (test_bit(DQ_FAKE_B, &dquot->dq_flags) &&
	    !(dquot->dq_dqb.dqb_curinodes | dquot->dq_dqb.dqb_curspace))
		return qtree_delete_dquot(info, dquot);
	return 0;
}
EXPORT_SYMBOL(qtree_release_dquot);
