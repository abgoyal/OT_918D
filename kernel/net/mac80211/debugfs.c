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
 * mac80211 debugfs for wireless PHYs
 *
 * Copyright 2007	Johannes Berg <johannes@sipsolutions.net>
 *
 * GPLv2
 *
 */

#include <linux/debugfs.h>
#include <linux/rtnetlink.h>
#include "ieee80211_i.h"
#include "driver-ops.h"
#include "rate.h"
#include "debugfs.h"

int mac80211_open_file_generic(struct inode *inode, struct file *file)
{
	file->private_data = inode->i_private;
	return 0;
}

#define DEBUGFS_READONLY_FILE(name, buflen, fmt, value...)		\
static ssize_t name## _read(struct file *file, char __user *userbuf,	\
			    size_t count, loff_t *ppos)			\
{									\
	struct ieee80211_local *local = file->private_data;		\
	char buf[buflen];						\
	int res;							\
									\
	res = scnprintf(buf, buflen, fmt "\n", ##value);		\
	return simple_read_from_buffer(userbuf, count, ppos, buf, res);	\
}									\
									\
static const struct file_operations name## _ops = {			\
	.read = name## _read,						\
	.open = mac80211_open_file_generic,				\
};

#define DEBUGFS_ADD(name)						\
	debugfs_create_file(#name, 0400, phyd, local, &name## _ops);

#define DEBUGFS_ADD_MODE(name, mode)					\
	debugfs_create_file(#name, mode, phyd, local, &name## _ops);


DEBUGFS_READONLY_FILE(frequency, 20, "%d",
		      local->hw.conf.channel->center_freq);
DEBUGFS_READONLY_FILE(total_ps_buffered, 20, "%d",
		      local->total_ps_buffered);
DEBUGFS_READONLY_FILE(wep_iv, 20, "%#08x",
		      local->wep_iv & 0xffffff);
DEBUGFS_READONLY_FILE(rate_ctrl_alg, 100, "%s",
	local->rate_ctrl ? local->rate_ctrl->ops->name : "hw/driver");

static ssize_t tsf_read(struct file *file, char __user *user_buf,
			     size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;
	u64 tsf;
	char buf[100];

	tsf = drv_get_tsf(local);

	snprintf(buf, sizeof(buf), "0x%016llx\n", (unsigned long long) tsf);

	return simple_read_from_buffer(user_buf, count, ppos, buf, 19);
}

static ssize_t tsf_write(struct file *file,
                         const char __user *user_buf,
                         size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;
	unsigned long long tsf;
	char buf[100];
	size_t len;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;
	buf[len] = '\0';

	if (strncmp(buf, "reset", 5) == 0) {
		if (local->ops->reset_tsf) {
			drv_reset_tsf(local);
			printk(KERN_INFO "%s: debugfs reset TSF\n", wiphy_name(local->hw.wiphy));
		}
	} else {
		tsf = simple_strtoul(buf, NULL, 0);
		if (local->ops->set_tsf) {
			drv_set_tsf(local, tsf);
			printk(KERN_INFO "%s: debugfs set TSF to %#018llx\n", wiphy_name(local->hw.wiphy), tsf);
		}
	}

	return count;
}

static const struct file_operations tsf_ops = {
	.read = tsf_read,
	.write = tsf_write,
	.open = mac80211_open_file_generic
};

static ssize_t reset_write(struct file *file, const char __user *user_buf,
			   size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;

	rtnl_lock();
	__ieee80211_suspend(&local->hw);
	__ieee80211_resume(&local->hw);
	rtnl_unlock();

	return count;
}

static const struct file_operations reset_ops = {
	.write = reset_write,
	.open = mac80211_open_file_generic,
};

static ssize_t noack_read(struct file *file, char __user *user_buf,
			  size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;
	int res;
	char buf[10];

	res = scnprintf(buf, sizeof(buf), "%d\n", local->wifi_wme_noack_test);

	return simple_read_from_buffer(user_buf, count, ppos, buf, res);
}

static ssize_t noack_write(struct file *file,
			   const char __user *user_buf,
			   size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;
	char buf[10];
	size_t len;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;
	buf[len] = '\0';

	local->wifi_wme_noack_test = !!simple_strtoul(buf, NULL, 0);

	return count;
}

static const struct file_operations noack_ops = {
	.read = noack_read,
	.write = noack_write,
	.open = mac80211_open_file_generic
};

static ssize_t uapsd_queues_read(struct file *file, char __user *user_buf,
				 size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;
	int res;
	char buf[10];

	res = scnprintf(buf, sizeof(buf), "0x%x\n", local->uapsd_queues);

	return simple_read_from_buffer(user_buf, count, ppos, buf, res);
}

static ssize_t uapsd_queues_write(struct file *file,
				  const char __user *user_buf,
				  size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;
	unsigned long val;
	char buf[10];
	size_t len;
	int ret;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;
	buf[len] = '\0';

	ret = strict_strtoul(buf, 0, &val);

	if (ret)
		return -EINVAL;

	if (val & ~IEEE80211_WMM_IE_STA_QOSINFO_AC_MASK)
		return -ERANGE;

	local->uapsd_queues = val;

	return count;
}

static const struct file_operations uapsd_queues_ops = {
	.read = uapsd_queues_read,
	.write = uapsd_queues_write,
	.open = mac80211_open_file_generic
};

static ssize_t uapsd_max_sp_len_read(struct file *file, char __user *user_buf,
				     size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;
	int res;
	char buf[10];

	res = scnprintf(buf, sizeof(buf), "0x%x\n", local->uapsd_max_sp_len);

	return simple_read_from_buffer(user_buf, count, ppos, buf, res);
}

static ssize_t uapsd_max_sp_len_write(struct file *file,
				      const char __user *user_buf,
				      size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;
	unsigned long val;
	char buf[10];
	size_t len;
	int ret;

	len = min(count, sizeof(buf) - 1);
	if (copy_from_user(buf, user_buf, len))
		return -EFAULT;
	buf[len] = '\0';

	ret = strict_strtoul(buf, 0, &val);

	if (ret)
		return -EINVAL;

	if (val & ~IEEE80211_WMM_IE_STA_QOSINFO_SP_MASK)
		return -ERANGE;

	local->uapsd_max_sp_len = val;

	return count;
}

static const struct file_operations uapsd_max_sp_len_ops = {
	.read = uapsd_max_sp_len_read,
	.write = uapsd_max_sp_len_write,
	.open = mac80211_open_file_generic
};

static ssize_t channel_type_read(struct file *file, char __user *user_buf,
		       size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;
	const char *buf;

	switch (local->hw.conf.channel_type) {
	case NL80211_CHAN_NO_HT:
		buf = "no ht\n";
		break;
	case NL80211_CHAN_HT20:
		buf = "ht20\n";
		break;
	case NL80211_CHAN_HT40MINUS:
		buf = "ht40-\n";
		break;
	case NL80211_CHAN_HT40PLUS:
		buf = "ht40+\n";
		break;
	default:
		buf = "???";
		break;
	}

	return simple_read_from_buffer(user_buf, count, ppos, buf, strlen(buf));
}

static const struct file_operations channel_type_ops = {
	.read = channel_type_read,
	.open = mac80211_open_file_generic
};

static ssize_t queues_read(struct file *file, char __user *user_buf,
			   size_t count, loff_t *ppos)
{
	struct ieee80211_local *local = file->private_data;
	unsigned long flags;
	char buf[IEEE80211_MAX_QUEUES * 20];
	int q, res = 0;

	spin_lock_irqsave(&local->queue_stop_reason_lock, flags);
	for (q = 0; q < local->hw.queues; q++)
		res += sprintf(buf + res, "%02d: %#.8lx/%d\n", q,
				local->queue_stop_reasons[q],
				skb_queue_len(&local->pending[q]));
	spin_unlock_irqrestore(&local->queue_stop_reason_lock, flags);

	return simple_read_from_buffer(user_buf, count, ppos, buf, res);
}

static const struct file_operations queues_ops = {
	.read = queues_read,
	.open = mac80211_open_file_generic
};

/* statistics stuff */

#define DEBUGFS_STATS_FILE(name, buflen, fmt, value...)			\
	DEBUGFS_READONLY_FILE(stats_ ##name, buflen, fmt, ##value)

static ssize_t format_devstat_counter(struct ieee80211_local *local,
	char __user *userbuf,
	size_t count, loff_t *ppos,
	int (*printvalue)(struct ieee80211_low_level_stats *stats, char *buf,
			  int buflen))
{
	struct ieee80211_low_level_stats stats;
	char buf[20];
	int res;

	rtnl_lock();
	res = drv_get_stats(local, &stats);
	rtnl_unlock();
	if (res)
		return res;
	res = printvalue(&stats, buf, sizeof(buf));
	return simple_read_from_buffer(userbuf, count, ppos, buf, res);
}

#define DEBUGFS_DEVSTATS_FILE(name)					\
static int print_devstats_##name(struct ieee80211_low_level_stats *stats,\
				 char *buf, int buflen)			\
{									\
	return scnprintf(buf, buflen, "%u\n", stats->name);		\
}									\
static ssize_t stats_ ##name## _read(struct file *file,			\
				     char __user *userbuf,		\
				     size_t count, loff_t *ppos)	\
{									\
	return format_devstat_counter(file->private_data,		\
				      userbuf,				\
				      count,				\
				      ppos,				\
				      print_devstats_##name);		\
}									\
									\
static const struct file_operations stats_ ##name## _ops = {		\
	.read = stats_ ##name## _read,					\
	.open = mac80211_open_file_generic,				\
};

#define DEBUGFS_STATS_ADD(name)						\
	debugfs_create_file(#name, 0400, statsd, local, &stats_ ##name## _ops);

DEBUGFS_STATS_FILE(transmitted_fragment_count, 20, "%u",
		   local->dot11TransmittedFragmentCount);
DEBUGFS_STATS_FILE(multicast_transmitted_frame_count, 20, "%u",
		   local->dot11MulticastTransmittedFrameCount);
DEBUGFS_STATS_FILE(failed_count, 20, "%u",
		   local->dot11FailedCount);
DEBUGFS_STATS_FILE(retry_count, 20, "%u",
		   local->dot11RetryCount);
DEBUGFS_STATS_FILE(multiple_retry_count, 20, "%u",
		   local->dot11MultipleRetryCount);
DEBUGFS_STATS_FILE(frame_duplicate_count, 20, "%u",
		   local->dot11FrameDuplicateCount);
DEBUGFS_STATS_FILE(received_fragment_count, 20, "%u",
		   local->dot11ReceivedFragmentCount);
DEBUGFS_STATS_FILE(multicast_received_frame_count, 20, "%u",
		   local->dot11MulticastReceivedFrameCount);
DEBUGFS_STATS_FILE(transmitted_frame_count, 20, "%u",
		   local->dot11TransmittedFrameCount);
#ifdef CONFIG_MAC80211_DEBUG_COUNTERS
DEBUGFS_STATS_FILE(tx_handlers_drop, 20, "%u",
		   local->tx_handlers_drop);
DEBUGFS_STATS_FILE(tx_handlers_queued, 20, "%u",
		   local->tx_handlers_queued);
DEBUGFS_STATS_FILE(tx_handlers_drop_unencrypted, 20, "%u",
		   local->tx_handlers_drop_unencrypted);
DEBUGFS_STATS_FILE(tx_handlers_drop_fragment, 20, "%u",
		   local->tx_handlers_drop_fragment);
DEBUGFS_STATS_FILE(tx_handlers_drop_wep, 20, "%u",
		   local->tx_handlers_drop_wep);
DEBUGFS_STATS_FILE(tx_handlers_drop_not_assoc, 20, "%u",
		   local->tx_handlers_drop_not_assoc);
DEBUGFS_STATS_FILE(tx_handlers_drop_unauth_port, 20, "%u",
		   local->tx_handlers_drop_unauth_port);
DEBUGFS_STATS_FILE(rx_handlers_drop, 20, "%u",
		   local->rx_handlers_drop);
DEBUGFS_STATS_FILE(rx_handlers_queued, 20, "%u",
		   local->rx_handlers_queued);
DEBUGFS_STATS_FILE(rx_handlers_drop_nullfunc, 20, "%u",
		   local->rx_handlers_drop_nullfunc);
DEBUGFS_STATS_FILE(rx_handlers_drop_defrag, 20, "%u",
		   local->rx_handlers_drop_defrag);
DEBUGFS_STATS_FILE(rx_handlers_drop_short, 20, "%u",
		   local->rx_handlers_drop_short);
DEBUGFS_STATS_FILE(rx_handlers_drop_passive_scan, 20, "%u",
		   local->rx_handlers_drop_passive_scan);
DEBUGFS_STATS_FILE(tx_expand_skb_head, 20, "%u",
		   local->tx_expand_skb_head);
DEBUGFS_STATS_FILE(tx_expand_skb_head_cloned, 20, "%u",
		   local->tx_expand_skb_head_cloned);
DEBUGFS_STATS_FILE(rx_expand_skb_head, 20, "%u",
		   local->rx_expand_skb_head);
DEBUGFS_STATS_FILE(rx_expand_skb_head2, 20, "%u",
		   local->rx_expand_skb_head2);
DEBUGFS_STATS_FILE(rx_handlers_fragments, 20, "%u",
		   local->rx_handlers_fragments);
DEBUGFS_STATS_FILE(tx_status_drop, 20, "%u",
		   local->tx_status_drop);

#endif

DEBUGFS_DEVSTATS_FILE(dot11ACKFailureCount);
DEBUGFS_DEVSTATS_FILE(dot11RTSFailureCount);
DEBUGFS_DEVSTATS_FILE(dot11FCSErrorCount);
DEBUGFS_DEVSTATS_FILE(dot11RTSSuccessCount);


void debugfs_hw_add(struct ieee80211_local *local)
{
	struct dentry *phyd = local->hw.wiphy->debugfsdir;
	struct dentry *statsd;

	if (!phyd)
		return;

	local->debugfs.stations = debugfs_create_dir("stations", phyd);
	local->debugfs.keys = debugfs_create_dir("keys", phyd);

	DEBUGFS_ADD(frequency);
	DEBUGFS_ADD(total_ps_buffered);
	DEBUGFS_ADD(wep_iv);
	DEBUGFS_ADD(tsf);
	DEBUGFS_ADD(queues);
	DEBUGFS_ADD_MODE(reset, 0200);
	DEBUGFS_ADD(noack);
	DEBUGFS_ADD(uapsd_queues);
	DEBUGFS_ADD(uapsd_max_sp_len);
	DEBUGFS_ADD(channel_type);

	statsd = debugfs_create_dir("statistics", phyd);

	/* if the dir failed, don't put all the other things into the root! */
	if (!statsd)
		return;

	DEBUGFS_STATS_ADD(transmitted_fragment_count);
	DEBUGFS_STATS_ADD(multicast_transmitted_frame_count);
	DEBUGFS_STATS_ADD(failed_count);
	DEBUGFS_STATS_ADD(retry_count);
	DEBUGFS_STATS_ADD(multiple_retry_count);
	DEBUGFS_STATS_ADD(frame_duplicate_count);
	DEBUGFS_STATS_ADD(received_fragment_count);
	DEBUGFS_STATS_ADD(multicast_received_frame_count);
	DEBUGFS_STATS_ADD(transmitted_frame_count);
#ifdef CONFIG_MAC80211_DEBUG_COUNTERS
	DEBUGFS_STATS_ADD(tx_handlers_drop);
	DEBUGFS_STATS_ADD(tx_handlers_queued);
	DEBUGFS_STATS_ADD(tx_handlers_drop_unencrypted);
	DEBUGFS_STATS_ADD(tx_handlers_drop_fragment);
	DEBUGFS_STATS_ADD(tx_handlers_drop_wep);
	DEBUGFS_STATS_ADD(tx_handlers_drop_not_assoc);
	DEBUGFS_STATS_ADD(tx_handlers_drop_unauth_port);
	DEBUGFS_STATS_ADD(rx_handlers_drop);
	DEBUGFS_STATS_ADD(rx_handlers_queued);
	DEBUGFS_STATS_ADD(rx_handlers_drop_nullfunc);
	DEBUGFS_STATS_ADD(rx_handlers_drop_defrag);
	DEBUGFS_STATS_ADD(rx_handlers_drop_short);
	DEBUGFS_STATS_ADD(rx_handlers_drop_passive_scan);
	DEBUGFS_STATS_ADD(tx_expand_skb_head);
	DEBUGFS_STATS_ADD(tx_expand_skb_head_cloned);
	DEBUGFS_STATS_ADD(rx_expand_skb_head);
	DEBUGFS_STATS_ADD(rx_expand_skb_head2);
	DEBUGFS_STATS_ADD(rx_handlers_fragments);
	DEBUGFS_STATS_ADD(tx_status_drop);
#endif
	DEBUGFS_STATS_ADD(dot11ACKFailureCount);
	DEBUGFS_STATS_ADD(dot11RTSFailureCount);
	DEBUGFS_STATS_ADD(dot11FCSErrorCount);
	DEBUGFS_STATS_ADD(dot11RTSSuccessCount);
}
