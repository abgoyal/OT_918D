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
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

/*
 * trace binary printk
 *
 * Copyright (C) 2008 Lai Jiangshan <laijs@cn.fujitsu.com>
 *
 */
#include <linux/seq_file.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/kernel.h>
#include <linux/ftrace.h>
#include <linux/string.h>
#include <linux/module.h>
#include <linux/marker.h>
#include <linux/mutex.h>
#include <linux/ctype.h>
#include <linux/list.h>
#include <linux/slab.h>
#include <linux/fs.h>

#include "trace.h"

#if defined(CONFIG_MODULES) && defined(CONFIG_FTRACE_MODULE_SUPPORT)

/*
 * modules trace_printk()'s formats are autosaved in struct trace_bprintk_fmt
 * which are queued on trace_bprintk_fmt_list.
 */
static LIST_HEAD(trace_bprintk_fmt_list);

/* serialize accesses to trace_bprintk_fmt_list */
static DEFINE_MUTEX(btrace_mutex);

struct trace_bprintk_fmt {
	struct list_head list;
	char fmt[0];
};

static inline struct trace_bprintk_fmt *lookup_format(const char *fmt)
{
	struct trace_bprintk_fmt *pos;
	list_for_each_entry(pos, &trace_bprintk_fmt_list, list) {
		if (!strcmp(pos->fmt, fmt))
			return pos;
	}
	return NULL;
}

static
void hold_module_trace_bprintk_format(const char **start, const char **end)
{
	const char **iter;

	mutex_lock(&btrace_mutex);
	for (iter = start; iter < end; iter++) {
		struct trace_bprintk_fmt *tb_fmt = lookup_format(*iter);
		if (tb_fmt) {
			*iter = tb_fmt->fmt;
			continue;
		}

		tb_fmt = kmalloc(offsetof(struct trace_bprintk_fmt, fmt)
				+ strlen(*iter) + 1, GFP_KERNEL);
		if (tb_fmt) {
			list_add_tail(&tb_fmt->list, &trace_bprintk_fmt_list);
			strcpy(tb_fmt->fmt, *iter);
			*iter = tb_fmt->fmt;
		} else
			*iter = NULL;
	}
	mutex_unlock(&btrace_mutex);
}

static int module_trace_bprintk_format_notify(struct notifier_block *self,
		unsigned long val, void *data)
{
	struct module *mod = data;
	if (mod->num_trace_bprintk_fmt) {
		const char **start = mod->trace_bprintk_fmt_start;
		const char **end = start + mod->num_trace_bprintk_fmt;

		if (val == MODULE_STATE_COMING)
			hold_module_trace_bprintk_format(start, end);
	}
	return 0;
}
__initdata_or_module static
struct notifier_block module_trace_bprintk_format_nb = {
	.notifier_call = module_trace_bprintk_format_notify,
};

#endif /* CONFIG_MODULES */



int __trace_bprintk(unsigned long ip, const char *fmt, ...)
 {
	int ret;
	va_list ap;

	if (unlikely(!fmt))
		return 0;

	if (!(trace_flags & TRACE_ITER_PRINTK))
		return 0;

	va_start(ap, fmt);
	ret = trace_vbprintk(ip, fmt, ap);
	va_end(ap);
	return ret;
}
EXPORT_SYMBOL_GPL(__trace_bprintk);

int __ftrace_vbprintk(unsigned long ip, const char *fmt, va_list ap)
 {
	if (unlikely(!fmt))
		return 0;

	if (!(trace_flags & TRACE_ITER_PRINTK))
		return 0;

	return trace_vbprintk(ip, fmt, ap);
}
EXPORT_SYMBOL_GPL(__ftrace_vbprintk);

int __trace_printk(unsigned long ip, const char *fmt, ...)
{
	int ret;
	va_list ap;

	if (!(trace_flags & TRACE_ITER_PRINTK))
		return 0;

	va_start(ap, fmt);
	ret = trace_vprintk(ip, fmt, ap);
	va_end(ap);
	return ret;
}
EXPORT_SYMBOL_GPL(__trace_printk);

int __ftrace_vprintk(unsigned long ip, const char *fmt, va_list ap)
{
	if (!(trace_flags & TRACE_ITER_PRINTK))
		return 0;

	return trace_vprintk(ip, fmt, ap);
}
EXPORT_SYMBOL_GPL(__ftrace_vprintk);

static void *
t_start(struct seq_file *m, loff_t *pos)
{
	const char **fmt = __start___trace_bprintk_fmt + *pos;

	if ((unsigned long)fmt >= (unsigned long)__stop___trace_bprintk_fmt)
		return NULL;
	return fmt;
}

static void *t_next(struct seq_file *m, void * v, loff_t *pos)
{
	(*pos)++;
	return t_start(m, pos);
}

static int t_show(struct seq_file *m, void *v)
{
	const char **fmt = v;
	const char *str = *fmt;
	int i;

	seq_printf(m, "0x%lx : \"", *(unsigned long *)fmt);

	/*
	 * Tabs and new lines need to be converted.
	 */
	for (i = 0; str[i]; i++) {
		switch (str[i]) {
		case '\n':
			seq_puts(m, "\\n");
			break;
		case '\t':
			seq_puts(m, "\\t");
			break;
		case '\\':
			seq_puts(m, "\\");
			break;
		case '"':
			seq_puts(m, "\\\"");
			break;
		default:
			seq_putc(m, str[i]);
		}
	}
	seq_puts(m, "\"\n");

	return 0;
}

static void t_stop(struct seq_file *m, void *p)
{
}

static const struct seq_operations show_format_seq_ops = {
	.start = t_start,
	.next = t_next,
	.show = t_show,
	.stop = t_stop,
};

static int
ftrace_formats_open(struct inode *inode, struct file *file)
{
	return seq_open(file, &show_format_seq_ops);
}

static const struct file_operations ftrace_formats_fops = {
	.open = ftrace_formats_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = seq_release,
};

static __init int init_trace_printk_function_export(void)
{
	struct dentry *d_tracer;

	d_tracer = tracing_init_dentry();
	if (!d_tracer)
		return 0;

	trace_create_file("printk_formats", 0444, d_tracer,
				    NULL, &ftrace_formats_fops);

	return 0;
}

fs_initcall(init_trace_printk_function_export);

#ifdef CONFIG_FTRACE_MODULE_SUPPORT
static __init int init_trace_printk(void)
{
	return register_module_notifier(&module_trace_bprintk_format_nb);
}
early_initcall(init_trace_printk);
#endif
