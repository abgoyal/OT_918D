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
 * Copyright (C) 2007 Mathieu Desnoyers
 *
 * Dual LGPL v2.1/GPL v2 license.
 */
#include <linux/module.h>
#include <linux/mutex.h>
#include <linux/immediate.h>
#include <linux/memory.h>
#include <linux/cpu.h>

#include <asm/sections.h>

/*
 * Kernel ready to execute the SMP update that may depend on trap and ipi.
 */
static int imv_early_boot_complete;

extern struct __imv __start___imv[];
extern struct __imv __stop___imv[];

/*
 * imv_mutex nests inside module_mutex. imv_mutex protects builtin
 * immediates and module immediates.
 */
static DEFINE_MUTEX(imv_mutex);

/**
 * imv_update_range - Update immediate values in a range
 * @begin: pointer to the beginning of the range
 * @end: pointer to the end of the range
 *
 * Updates a range of immediates.
 */
void imv_update_range(const struct __imv *begin,
		const struct __imv *end)
{
	const struct __imv *iter;
	int ret;

	mutex_lock(&imv_mutex);
	for (iter = begin; iter < end; iter++) {
		if (!iter->imv) /* Skip removed __init immediate values */
			continue;
		/* workaround on_each_cpu cpu hotplug race */
		get_online_cpus();
		mutex_lock(&text_mutex);
		ret = arch_imv_update(iter, !imv_early_boot_complete);
		mutex_unlock(&text_mutex);
		put_online_cpus();
		if (imv_early_boot_complete && ret)
			printk(KERN_WARNING
				"Invalid immediate value. "
				"Variable at %p, "
				"instruction at %p, size %hu\n",
				(void *)iter->imv,
				(void *)iter->var, iter->size);
	}
	mutex_unlock(&imv_mutex);
}
EXPORT_SYMBOL_GPL(imv_update_range);

/**
 * imv_update - update all immediate values in the kernel
 *
 * Iterate on the kernel core and modules to update the immediate values.
 */
void core_imv_update(void)
{
	/* Core kernel imvs */
	imv_update_range(__start___imv, __stop___imv);
}
EXPORT_SYMBOL_GPL(core_imv_update);

/**
 * imv_unref
 * @begin: pointer to the beginning of the range
 * @end: pointer to the end of the range
 * @start: beginning of the region to consider
 * @size: size of the region to consider
 *
 * Deactivate any immediate value reference pointing into the code region in the
 * range start to start + size.
 */
void imv_unref(struct __imv *begin, struct __imv *end, void *start,
		unsigned long size)
{
	struct __imv *iter;

	for (iter = begin; iter < end; iter++)
		if (iter->imv >= (unsigned long)start
			&& iter->imv < (unsigned long)start + size)
			iter->imv = 0UL;
}

void imv_unref_core_init(void)
{
	imv_unref(__start___imv, __stop___imv, __init_begin,
		(unsigned long)__init_end - (unsigned long)__init_begin);
}

void __init imv_init_complete(void)
{
	imv_early_boot_complete = 1;
}

int imv_module_notify(struct notifier_block *self,
		      unsigned long val, void *data)
{
	struct module *mod = data;

	switch (val) {
	case MODULE_STATE_COMING:
		imv_update_range(mod->immediate,
				 mod->immediate + mod->num_immediate);
		break;
	case MODULE_STATE_GOING:
		/* All references will be gone, no update required. */
		break;
	}
	return 0;
}

struct notifier_block imv_module_nb = {
	.notifier_call = imv_module_notify,
	.priority = 0,
};

static int init_imv(void)
{
	return register_module_notifier(&imv_module_nb);
}
__initcall(init_imv);
