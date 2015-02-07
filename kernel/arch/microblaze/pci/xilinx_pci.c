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
 * PCI support for Xilinx plbv46_pci soft-core which can be used on
 * Xilinx Virtex ML410 / ML510 boards.
 *
 * Copyright 2009 Roderick Colenbrander
 * Copyright 2009 Secret Lab Technologies Ltd.
 *
 * The pci bridge fixup code was copied from ppc4xx_pci.c and was written
 * by Benjamin Herrenschmidt.
 * Copyright 2007 Ben. Herrenschmidt <benh@kernel.crashing.org>, IBM Corp.
 *
 * This file is licensed under the terms of the GNU General Public License
 * version 2. This program is licensed "as is" without any warranty of any
 * kind, whether express or implied.
 */

#include <linux/ioport.h>
#include <linux/of.h>
#include <linux/pci.h>
#include <asm/io.h>

#define XPLB_PCI_ADDR 0x10c
#define XPLB_PCI_DATA 0x110
#define XPLB_PCI_BUS  0x114

#define PCI_HOST_ENABLE_CMD (PCI_COMMAND_SERR | PCI_COMMAND_PARITY | \
				PCI_COMMAND_MASTER | PCI_COMMAND_MEMORY)

static struct of_device_id xilinx_pci_match[] = {
	{ .compatible = "xlnx,plbv46-pci-1.03.a", },
	{}
};

/**
 * xilinx_pci_fixup_bridge - Block Xilinx PHB configuration.
 */
static void xilinx_pci_fixup_bridge(struct pci_dev *dev)
{
	struct pci_controller *hose;
	int i;

	if (dev->devfn || dev->bus->self)
		return;

	hose = pci_bus_to_host(dev->bus);
	if (!hose)
		return;

	if (!of_match_node(xilinx_pci_match, hose->dn))
		return;

	/* Hide the PCI host BARs from the kernel as their content doesn't
	 * fit well in the resource management
	 */
	for (i = 0; i < DEVICE_COUNT_RESOURCE; i++) {
		dev->resource[i].start = 0;
		dev->resource[i].end = 0;
		dev->resource[i].flags = 0;
	}

	dev_info(&dev->dev, "Hiding Xilinx plb-pci host bridge resources %s\n",
		 pci_name(dev));
}
DECLARE_PCI_FIXUP_HEADER(PCI_ANY_ID, PCI_ANY_ID, xilinx_pci_fixup_bridge);

#ifdef DEBUG
/**
 * xilinx_pci_exclude_device - Don't do config access for non-root bus
 *
 * This is a hack.  Config access to any bus other than bus 0 does not
 * currently work on the ML510 so we prevent it here.
 */
static int
xilinx_pci_exclude_device(struct pci_controller *hose, u_char bus, u8 devfn)
{
	return (bus != 0);
}

/**
 * xilinx_early_pci_scan - List pci config space for available devices
 *
 * List pci devices in very early phase.
 */
void __init xilinx_early_pci_scan(struct pci_controller *hose)
{
	u32 bus = 0;
	u32 val, dev, func, offset;

	/* Currently we have only 2 device connected - up-to 32 devices */
	for (dev = 0; dev < 2; dev++) {
		/* List only first function number - up-to 8 functions */
		for (func = 0; func < 1; func++) {
			printk(KERN_INFO "%02x:%02x:%02x", bus, dev, func);
			/* read the first 64 standardized bytes */
			/* Up-to 192 bytes can be list of capabilities */
			for (offset = 0; offset < 64; offset += 4) {
				early_read_config_dword(hose, bus,
					PCI_DEVFN(dev, func), offset, &val);
				if (offset == 0 && val == 0xFFFFFFFF) {
					printk(KERN_CONT "\nABSENT");
					break;
				}
				if (!(offset % 0x10))
					printk(KERN_CONT "\n%04x:    ", offset);

				printk(KERN_CONT "%08x  ", val);
			}
			printk(KERN_INFO "\n");
		}
	}
}
#else
void __init xilinx_early_pci_scan(struct pci_controller *hose)
{
}
#endif

/**
 * xilinx_pci_init - Find and register a Xilinx PCI host bridge
 */
void __init xilinx_pci_init(void)
{
	struct pci_controller *hose;
	struct resource r;
	void __iomem *pci_reg;
	struct device_node *pci_node;

	pci_node = of_find_matching_node(NULL, xilinx_pci_match);
	if (!pci_node)
		return;

	if (of_address_to_resource(pci_node, 0, &r)) {
		pr_err("xilinx-pci: cannot resolve base address\n");
		return;
	}

	hose = pcibios_alloc_controller(pci_node);
	if (!hose) {
		pr_err("xilinx-pci: pcibios_alloc_controller() failed\n");
		return;
	}

	/* Setup config space */
	setup_indirect_pci(hose, r.start + XPLB_PCI_ADDR,
			   r.start + XPLB_PCI_DATA,
			   INDIRECT_TYPE_SET_CFG_TYPE);

	/* According to the xilinx plbv46_pci documentation the soft-core starts
	 * a self-init when the bus master enable bit is set. Without this bit
	 * set the pci bus can't be scanned.
	 */
	early_write_config_word(hose, 0, 0, PCI_COMMAND, PCI_HOST_ENABLE_CMD);

	/* Set the max latency timer to 255 */
	early_write_config_byte(hose, 0, 0, PCI_LATENCY_TIMER, 0xff);

	/* Set the max bus number to 255, and bus/subbus no's to 0 */
	pci_reg = of_iomap(pci_node, 0);
	out_be32(pci_reg + XPLB_PCI_BUS, 0x000000ff);
	iounmap(pci_reg);

	/* Register the host bridge with the linux kernel! */
	pci_process_bridge_OF_ranges(hose, pci_node,
					INDIRECT_TYPE_SET_CFG_TYPE);

	pr_info("xilinx-pci: Registered PCI host bridge\n");
	xilinx_early_pci_scan(hose);
}
