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

#ifndef _ASM_MICROBLAZE_PCI_BRIDGE_H
#define _ASM_MICROBLAZE_PCI_BRIDGE_H
#ifdef __KERNEL__
/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version
 * 2 of the License, or (at your option) any later version.
 */
#include <linux/pci.h>
#include <linux/list.h>
#include <linux/ioport.h>

struct device_node;

enum {
	/* Force re-assigning all resources (ignore firmware
	 * setup completely)
	 */
	PCI_REASSIGN_ALL_RSRC	= 0x00000001,

	/* Re-assign all bus numbers */
	PCI_REASSIGN_ALL_BUS	= 0x00000002,

	/* Do not try to assign, just use existing setup */
	PCI_PROBE_ONLY		= 0x00000004,

	/* Don't bother with ISA alignment unless the bridge has
	 * ISA forwarding enabled
	 */
	PCI_CAN_SKIP_ISA_ALIGN	= 0x00000008,

	/* Enable domain numbers in /proc */
	PCI_ENABLE_PROC_DOMAINS	= 0x00000010,
	/* ... except for domain 0 */
	PCI_COMPAT_DOMAIN_0		= 0x00000020,
};

/*
 * Structure of a PCI controller (host bridge)
 */
struct pci_controller {
	struct pci_bus *bus;
	char is_dynamic;
	struct device_node *dn;
	struct list_head list_node;
	struct device *parent;

	int first_busno;
	int last_busno;

	int self_busno;

	void __iomem *io_base_virt;
	resource_size_t io_base_phys;

	resource_size_t pci_io_size;

	/* Some machines (PReP) have a non 1:1 mapping of
	 * the PCI memory space in the CPU bus space
	 */
	resource_size_t pci_mem_offset;

	/* Some machines have a special region to forward the ISA
	 * "memory" cycles such as VGA memory regions. Left to 0
	 * if unsupported
	 */
	resource_size_t isa_mem_phys;
	resource_size_t isa_mem_size;

	struct pci_ops *ops;
	unsigned int __iomem *cfg_addr;
	void __iomem *cfg_data;

	/*
	 * Used for variants of PCI indirect handling and possible quirks:
	 *  SET_CFG_TYPE - used on 4xx or any PHB that does explicit type0/1
	 *  EXT_REG - provides access to PCI-e extended registers
	 *  SURPRESS_PRIMARY_BUS - we surpress the setting of PCI_PRIMARY_BUS
	 *   on Freescale PCI-e controllers since they used the PCI_PRIMARY_BUS
	 *   to determine which bus number to match on when generating type0
	 *   config cycles
	 *  NO_PCIE_LINK - the Freescale PCI-e controllers have issues with
	 *   hanging if we don't have link and try to do config cycles to
	 *   anything but the PHB.  Only allow talking to the PHB if this is
	 *   set.
	 *  BIG_ENDIAN - cfg_addr is a big endian register
	 *  BROKEN_MRM - the 440EPx/GRx chips have an errata that causes hangs
	 *   on the PLB4.  Effectively disable MRM commands by setting this.
	 */
#define INDIRECT_TYPE_SET_CFG_TYPE		0x00000001
#define INDIRECT_TYPE_EXT_REG		0x00000002
#define INDIRECT_TYPE_SURPRESS_PRIMARY_BUS	0x00000004
#define INDIRECT_TYPE_NO_PCIE_LINK		0x00000008
#define INDIRECT_TYPE_BIG_ENDIAN		0x00000010
#define INDIRECT_TYPE_BROKEN_MRM		0x00000020
	u32 indirect_type;

	/* Currently, we limit ourselves to 1 IO range and 3 mem
	 * ranges since the common pci_bus structure can't handle more
	 */
	struct resource io_resource;
	struct resource mem_resources[3];
	int global_number;	/* PCI domain number */
};

static inline struct pci_controller *pci_bus_to_host(const struct pci_bus *bus)
{
	return bus->sysdata;
}

static inline int isa_vaddr_is_ioport(void __iomem *address)
{
	/* No specific ISA handling on ppc32 at this stage, it
	 * all goes through PCI
	 */
	return 0;
}

/* These are used for config access before all the PCI probing
   has been done. */
extern int early_read_config_byte(struct pci_controller *hose, int bus,
			int dev_fn, int where, u8 *val);
extern int early_read_config_word(struct pci_controller *hose, int bus,
			int dev_fn, int where, u16 *val);
extern int early_read_config_dword(struct pci_controller *hose, int bus,
			int dev_fn, int where, u32 *val);
extern int early_write_config_byte(struct pci_controller *hose, int bus,
			int dev_fn, int where, u8 val);
extern int early_write_config_word(struct pci_controller *hose, int bus,
			int dev_fn, int where, u16 val);
extern int early_write_config_dword(struct pci_controller *hose, int bus,
			int dev_fn, int where, u32 val);

extern int early_find_capability(struct pci_controller *hose, int bus,
				 int dev_fn, int cap);

extern void setup_indirect_pci(struct pci_controller *hose,
			       resource_size_t cfg_addr,
			       resource_size_t cfg_data, u32 flags);

/* Get the PCI host controller for an OF device */
extern struct pci_controller *pci_find_hose_for_OF_device(
			struct device_node *node);

/* Fill up host controller resources from the OF node */
extern void pci_process_bridge_OF_ranges(struct pci_controller *hose,
			struct device_node *dev, int primary);

/* Allocate & free a PCI host bridge structure */
extern struct pci_controller *pcibios_alloc_controller(struct device_node *dev);
extern void pcibios_free_controller(struct pci_controller *phb);
extern void pcibios_setup_phb_resources(struct pci_controller *hose);

#ifdef CONFIG_PCI
extern unsigned int pci_flags;

static inline void pci_set_flags(int flags)
{
	pci_flags = flags;
}

static inline void pci_add_flags(int flags)
{
	pci_flags |= flags;
}

static inline int pci_has_flag(int flag)
{
	return pci_flags & flag;
}

extern struct list_head hose_list;

extern unsigned long pci_address_to_pio(phys_addr_t address);
extern int pcibios_vaddr_is_ioport(void __iomem *address);
#else
static inline unsigned long pci_address_to_pio(phys_addr_t address)
{
	return (unsigned long)-1;
}
static inline int pcibios_vaddr_is_ioport(void __iomem *address)
{
	return 0;
}

static inline void pci_set_flags(int flags) { }
static inline void pci_add_flags(int flags) { }
static inline int pci_has_flag(int flag)
{
	return 0;
}
#endif	/* CONFIG_PCI */

#endif	/* __KERNEL__ */
#endif	/* _ASM_MICROBLAZE_PCI_BRIDGE_H */
