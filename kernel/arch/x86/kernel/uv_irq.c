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
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file "COPYING" in the main directory of this archive
 * for more details.
 *
 * SGI UV IRQ functions
 *
 * Copyright (C) 2008 Silicon Graphics, Inc. All rights reserved.
 */

#include <linux/module.h>
#include <linux/rbtree.h>
#include <linux/slab.h>
#include <linux/irq.h>

#include <asm/apic.h>
#include <asm/uv/uv_irq.h>
#include <asm/uv/uv_hub.h>

/* MMR offset and pnode of hub sourcing interrupts for a given irq */
struct uv_irq_2_mmr_pnode{
	struct rb_node		list;
	unsigned long		offset;
	int			pnode;
	int			irq;
};

static spinlock_t		uv_irq_lock;
static struct rb_root		uv_irq_root;

static int uv_set_irq_affinity(unsigned int, const struct cpumask *);

static void uv_noop(unsigned int irq)
{
}

static unsigned int uv_noop_ret(unsigned int irq)
{
	return 0;
}

static void uv_ack_apic(unsigned int irq)
{
	ack_APIC_irq();
}

static struct irq_chip uv_irq_chip = {
	.name		= "UV-CORE",
	.startup	= uv_noop_ret,
	.shutdown	= uv_noop,
	.enable		= uv_noop,
	.disable	= uv_noop,
	.ack		= uv_noop,
	.mask		= uv_noop,
	.unmask		= uv_noop,
	.eoi		= uv_ack_apic,
	.end		= uv_noop,
	.set_affinity	= uv_set_irq_affinity,
};

/*
 * Add offset and pnode information of the hub sourcing interrupts to the
 * rb tree for a specific irq.
 */
static int uv_set_irq_2_mmr_info(int irq, unsigned long offset, unsigned blade)
{
	struct rb_node **link = &uv_irq_root.rb_node;
	struct rb_node *parent = NULL;
	struct uv_irq_2_mmr_pnode *n;
	struct uv_irq_2_mmr_pnode *e;
	unsigned long irqflags;

	n = kmalloc_node(sizeof(struct uv_irq_2_mmr_pnode), GFP_KERNEL,
				uv_blade_to_memory_nid(blade));
	if (!n)
		return -ENOMEM;

	n->irq = irq;
	n->offset = offset;
	n->pnode = uv_blade_to_pnode(blade);
	spin_lock_irqsave(&uv_irq_lock, irqflags);
	/* Find the right place in the rbtree: */
	while (*link) {
		parent = *link;
		e = rb_entry(parent, struct uv_irq_2_mmr_pnode, list);

		if (unlikely(irq == e->irq)) {
			/* irq entry exists */
			e->pnode = uv_blade_to_pnode(blade);
			e->offset = offset;
			spin_unlock_irqrestore(&uv_irq_lock, irqflags);
			kfree(n);
			return 0;
		}

		if (irq < e->irq)
			link = &(*link)->rb_left;
		else
			link = &(*link)->rb_right;
	}

	/* Insert the node into the rbtree. */
	rb_link_node(&n->list, parent, link);
	rb_insert_color(&n->list, &uv_irq_root);

	spin_unlock_irqrestore(&uv_irq_lock, irqflags);
	return 0;
}

/* Retrieve offset and pnode information from the rb tree for a specific irq */
int uv_irq_2_mmr_info(int irq, unsigned long *offset, int *pnode)
{
	struct uv_irq_2_mmr_pnode *e;
	struct rb_node *n;
	unsigned long irqflags;

	spin_lock_irqsave(&uv_irq_lock, irqflags);
	n = uv_irq_root.rb_node;
	while (n) {
		e = rb_entry(n, struct uv_irq_2_mmr_pnode, list);

		if (e->irq == irq) {
			*offset = e->offset;
			*pnode = e->pnode;
			spin_unlock_irqrestore(&uv_irq_lock, irqflags);
			return 0;
		}

		if (irq < e->irq)
			n = n->rb_left;
		else
			n = n->rb_right;
	}
	spin_unlock_irqrestore(&uv_irq_lock, irqflags);
	return -1;
}

/*
 * Re-target the irq to the specified CPU and enable the specified MMR located
 * on the specified blade to allow the sending of MSIs to the specified CPU.
 */
static int
arch_enable_uv_irq(char *irq_name, unsigned int irq, int cpu, int mmr_blade,
		       unsigned long mmr_offset, int limit)
{
	const struct cpumask *eligible_cpu = cpumask_of(cpu);
	struct irq_desc *desc = irq_to_desc(irq);
	struct irq_cfg *cfg;
	int mmr_pnode;
	unsigned long mmr_value;
	struct uv_IO_APIC_route_entry *entry;
	int err;

	BUILD_BUG_ON(sizeof(struct uv_IO_APIC_route_entry) !=
			sizeof(unsigned long));

	cfg = irq_cfg(irq);

	err = assign_irq_vector(irq, cfg, eligible_cpu);
	if (err != 0)
		return err;

	if (limit == UV_AFFINITY_CPU)
		desc->status |= IRQ_NO_BALANCING;
	else
		desc->status |= IRQ_MOVE_PCNTXT;

	set_irq_chip_and_handler_name(irq, &uv_irq_chip, handle_percpu_irq,
				      irq_name);

	mmr_value = 0;
	entry = (struct uv_IO_APIC_route_entry *)&mmr_value;
	entry->vector		= cfg->vector;
	entry->delivery_mode	= apic->irq_delivery_mode;
	entry->dest_mode	= apic->irq_dest_mode;
	entry->polarity		= 0;
	entry->trigger		= 0;
	entry->mask		= 0;
	entry->dest		= apic->cpu_mask_to_apicid(eligible_cpu);

	mmr_pnode = uv_blade_to_pnode(mmr_blade);
	uv_write_global_mmr64(mmr_pnode, mmr_offset, mmr_value);

	if (cfg->move_in_progress)
		send_cleanup_vector(cfg);

	return irq;
}

/*
 * Disable the specified MMR located on the specified blade so that MSIs are
 * longer allowed to be sent.
 */
static void arch_disable_uv_irq(int mmr_pnode, unsigned long mmr_offset)
{
	unsigned long mmr_value;
	struct uv_IO_APIC_route_entry *entry;

	BUILD_BUG_ON(sizeof(struct uv_IO_APIC_route_entry) !=
			sizeof(unsigned long));

	mmr_value = 0;
	entry = (struct uv_IO_APIC_route_entry *)&mmr_value;
	entry->mask = 1;

	uv_write_global_mmr64(mmr_pnode, mmr_offset, mmr_value);
}

static int uv_set_irq_affinity(unsigned int irq, const struct cpumask *mask)
{
	struct irq_desc *desc = irq_to_desc(irq);
	struct irq_cfg *cfg = desc->chip_data;
	unsigned int dest;
	unsigned long mmr_value;
	struct uv_IO_APIC_route_entry *entry;
	unsigned long mmr_offset;
	int mmr_pnode;

	if (set_desc_affinity(desc, mask, &dest))
		return -1;

	mmr_value = 0;
	entry = (struct uv_IO_APIC_route_entry *)&mmr_value;

	entry->vector		= cfg->vector;
	entry->delivery_mode	= apic->irq_delivery_mode;
	entry->dest_mode	= apic->irq_dest_mode;
	entry->polarity		= 0;
	entry->trigger		= 0;
	entry->mask		= 0;
	entry->dest		= dest;

	/* Get previously stored MMR and pnode of hub sourcing interrupts */
	if (uv_irq_2_mmr_info(irq, &mmr_offset, &mmr_pnode))
		return -1;

	uv_write_global_mmr64(mmr_pnode, mmr_offset, mmr_value);

	if (cfg->move_in_progress)
		send_cleanup_vector(cfg);

	return 0;
}

/*
 * Set up a mapping of an available irq and vector, and enable the specified
 * MMR that defines the MSI that is to be sent to the specified CPU when an
 * interrupt is raised.
 */
int uv_setup_irq(char *irq_name, int cpu, int mmr_blade,
		 unsigned long mmr_offset, int limit)
{
	int irq, ret;

	irq = create_irq_nr(NR_IRQS_LEGACY, uv_blade_to_memory_nid(mmr_blade));

	if (irq <= 0)
		return -EBUSY;

	ret = arch_enable_uv_irq(irq_name, irq, cpu, mmr_blade, mmr_offset,
		limit);
	if (ret == irq)
		uv_set_irq_2_mmr_info(irq, mmr_offset, mmr_blade);
	else
		destroy_irq(irq);

	return ret;
}
EXPORT_SYMBOL_GPL(uv_setup_irq);

/*
 * Tear down a mapping of an irq and vector, and disable the specified MMR that
 * defined the MSI that was to be sent to the specified CPU when an interrupt
 * was raised.
 *
 * Set mmr_blade and mmr_offset to what was passed in on uv_setup_irq().
 */
void uv_teardown_irq(unsigned int irq)
{
	struct uv_irq_2_mmr_pnode *e;
	struct rb_node *n;
	unsigned long irqflags;

	spin_lock_irqsave(&uv_irq_lock, irqflags);
	n = uv_irq_root.rb_node;
	while (n) {
		e = rb_entry(n, struct uv_irq_2_mmr_pnode, list);
		if (e->irq == irq) {
			arch_disable_uv_irq(e->pnode, e->offset);
			rb_erase(n, &uv_irq_root);
			kfree(e);
			break;
		}
		if (irq < e->irq)
			n = n->rb_left;
		else
			n = n->rb_right;
	}
	spin_unlock_irqrestore(&uv_irq_lock, irqflags);
	destroy_irq(irq);
}
EXPORT_SYMBOL_GPL(uv_teardown_irq);