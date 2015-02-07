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

#ifndef _ASM_POWERPC_TOPOLOGY_H
#define _ASM_POWERPC_TOPOLOGY_H
#ifdef __KERNEL__


struct sys_device;
struct device_node;

#ifdef CONFIG_NUMA

/*
 * Before going off node we want the VM to try and reclaim from the local
 * node. It does this if the remote distance is larger than RECLAIM_DISTANCE.
 * With the default REMOTE_DISTANCE of 20 and the default RECLAIM_DISTANCE of
 * 20, we never reclaim and go off node straight away.
 *
 * To fix this we choose a smaller value of RECLAIM_DISTANCE.
 */
#define RECLAIM_DISTANCE 10

/*
 * Before going off node we want the VM to try and reclaim from the local
 * node. It does this if the remote distance is larger than RECLAIM_DISTANCE.
 * With the default REMOTE_DISTANCE of 20 and the default RECLAIM_DISTANCE of
 * 20, we never reclaim and go off node straight away.
 *
 * To fix this we choose a smaller value of RECLAIM_DISTANCE.
 */
#define RECLAIM_DISTANCE 10

#include <asm/mmzone.h>

static inline int cpu_to_node(int cpu)
{
	return numa_cpu_lookup_table[cpu];
}

#define parent_node(node)	(node)

#define cpumask_of_node(node) ((node) == -1 ?				\
			       cpu_all_mask :				\
			       node_to_cpumask_map[node])

int of_node_to_nid(struct device_node *device);

struct pci_bus;
#ifdef CONFIG_PCI
extern int pcibus_to_node(struct pci_bus *bus);
#else
static inline int pcibus_to_node(struct pci_bus *bus)
{
	return -1;
}
#endif

#define cpumask_of_pcibus(bus)	(pcibus_to_node(bus) == -1 ?		\
				 cpu_all_mask :				\
				 cpumask_of_node(pcibus_to_node(bus)))

/* sched_domains SD_NODE_INIT for PPC64 machines */
#define SD_NODE_INIT (struct sched_domain) {				\
	.min_interval		= 8,					\
	.max_interval		= 32,					\
	.busy_factor		= 32,					\
	.imbalance_pct		= 125,					\
	.cache_nice_tries	= 1,					\
	.busy_idx		= 3,					\
	.idle_idx		= 1,					\
	.newidle_idx		= 0,					\
	.wake_idx		= 0,					\
	.forkexec_idx		= 0,					\
									\
	.flags			= 1*SD_LOAD_BALANCE			\
				| 1*SD_BALANCE_NEWIDLE			\
				| 1*SD_BALANCE_EXEC			\
				| 1*SD_BALANCE_FORK			\
				| 0*SD_BALANCE_WAKE			\
				| 0*SD_WAKE_AFFINE			\
				| 0*SD_PREFER_LOCAL			\
				| 0*SD_SHARE_CPUPOWER			\
				| 0*SD_POWERSAVINGS_BALANCE		\
				| 0*SD_SHARE_PKG_RESOURCES		\
				| 1*SD_SERIALIZE			\
				| 0*SD_PREFER_SIBLING			\
				,					\
	.last_balance		= jiffies,				\
	.balance_interval	= 1,					\
}

extern void __init dump_numa_cpu_topology(void);

extern int sysfs_add_device_to_node(struct sys_device *dev, int nid);
extern void sysfs_remove_device_from_node(struct sys_device *dev, int nid);

#else

static inline int of_node_to_nid(struct device_node *device)
{
	return 0;
}

static inline void dump_numa_cpu_topology(void) {}

static inline int sysfs_add_device_to_node(struct sys_device *dev, int nid)
{
	return 0;
}

static inline void sysfs_remove_device_from_node(struct sys_device *dev,
						int nid)
{
}

#endif /* CONFIG_NUMA */

#include <asm-generic/topology.h>

#ifdef CONFIG_SMP
#include <asm/cputable.h>
#define smt_capable()		(cpu_has_feature(CPU_FTR_SMT))

#ifdef CONFIG_PPC64
#include <asm/smp.h>

#define topology_thread_cpumask(cpu)	(per_cpu(cpu_sibling_map, cpu))
#define topology_core_cpumask(cpu)	(per_cpu(cpu_core_map, cpu))
#define topology_core_id(cpu)		(cpu_to_core_id(cpu))
#endif
#endif

#endif /* __KERNEL__ */
#endif	/* _ASM_POWERPC_TOPOLOGY_H */
