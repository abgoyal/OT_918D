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
 *	IDE tuning and bus mastering support for the CS5510/CS5520
 *	chipsets
 *
 *	The CS5510/CS5520 are slightly unusual devices. Unlike the 
 *	typical IDE controllers they do bus mastering with the drive in
 *	PIO mode and smarter silicon.
 *
 *	The practical upshot of this is that we must always tune the
 *	drive for the right PIO mode. We must also ignore all the blacklists
 *	and the drive bus mastering DMA information.
 *
 *	*** This driver is strictly experimental ***
 *
 *	(c) Copyright Red Hat Inc 2002
 * 
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2, or (at your option) any
 * later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * For the avoidance of doubt the "preferred form" of this code is one which
 * is in an open non patent encumbered format. Where cryptographic key signing
 * forms part of the process of creating an executable the information
 * including keys needed to generate an equivalently functional executable
 * are deemed to be part of the source code.
 *
 */
 
#include <linux/module.h>
#include <linux/types.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/pci.h>
#include <linux/ide.h>
#include <linux/dma-mapping.h>

#define DRV_NAME "cs5520"

struct pio_clocks
{
	int address;
	int assert;
	int recovery;
};

static struct pio_clocks cs5520_pio_clocks[]={
	{3, 6, 11},
	{2, 5, 6},
	{1, 4, 3},
	{1, 3, 2},
	{1, 2, 1}
};

static void cs5520_set_pio_mode(ide_hwif_t *hwif, ide_drive_t *drive)
{
	struct pci_dev *pdev = to_pci_dev(hwif->dev);
	int controller = drive->dn > 1 ? 1 : 0;
	const u8 pio = drive->pio_mode - XFER_PIO_0;

	/* 8bit CAT/CRT - 8bit command timing for channel */
	pci_write_config_byte(pdev, 0x62 + controller, 
		(cs5520_pio_clocks[pio].recovery << 4) |
		(cs5520_pio_clocks[pio].assert));

	/* 0x64 - 16bit Primary, 0x68 - 16bit Secondary */

	/* FIXME: should these use address ? */
	/* Data read timing */
	pci_write_config_byte(pdev, 0x64 + 4*controller + (drive->dn&1),
		(cs5520_pio_clocks[pio].recovery << 4) |
		(cs5520_pio_clocks[pio].assert));
	/* Write command timing */
	pci_write_config_byte(pdev, 0x66 + 4*controller + (drive->dn&1),
		(cs5520_pio_clocks[pio].recovery << 4) |
		(cs5520_pio_clocks[pio].assert));
}

static void cs5520_set_dma_mode(ide_hwif_t *hwif, ide_drive_t *drive)
{
	printk(KERN_ERR "cs55x0: bad ide timing.\n");

	drive->pio_mode = XFER_PIO_0 + 0;
	cs5520_set_pio_mode(hwif, drive);
}

static const struct ide_port_ops cs5520_port_ops = {
	.set_pio_mode		= cs5520_set_pio_mode,
	.set_dma_mode		= cs5520_set_dma_mode,
};

static const struct ide_port_info cyrix_chipset __devinitdata = {
	.name		= DRV_NAME,
	.enablebits	= { { 0x60, 0x01, 0x01 }, { 0x60, 0x02, 0x02 } },
	.port_ops	= &cs5520_port_ops,
	.host_flags	= IDE_HFLAG_ISA_PORTS | IDE_HFLAG_CS5520,
	.pio_mask	= ATA_PIO4,
};

/*
 *	The 5510/5520 are a bit weird. They don't quite set up the way
 *	the PCI helper layer expects so we must do much of the set up 
 *	work longhand.
 */
 
static int __devinit cs5520_init_one(struct pci_dev *dev, const struct pci_device_id *id)
{
	const struct ide_port_info *d = &cyrix_chipset;
	struct ide_hw hw[2], *hws[] = { NULL, NULL };

	ide_setup_pci_noise(dev, d);

	/* We must not grab the entire device, it has 'ISA' space in its
	 * BARS too and we will freak out other bits of the kernel
	 */
	if (pci_enable_device_io(dev)) {
		printk(KERN_WARNING "%s: Unable to enable 55x0.\n", d->name);
		return -ENODEV;
	}
	pci_set_master(dev);
	if (pci_set_dma_mask(dev, DMA_BIT_MASK(32))) {
		printk(KERN_WARNING "%s: No suitable DMA available.\n",
			d->name);
		return -ENODEV;
	}

	/*
	 *	Now the chipset is configured we can let the core
	 *	do all the device setup for us
	 */

	ide_pci_setup_ports(dev, d, &hw[0], &hws[0]);
	hw[0].irq = 14;
	hw[1].irq = 15;

	return ide_host_add(d, hws, 2, NULL);
}

static const struct pci_device_id cs5520_pci_tbl[] = {
	{ PCI_VDEVICE(CYRIX, PCI_DEVICE_ID_CYRIX_5510), 0 },
	{ PCI_VDEVICE(CYRIX, PCI_DEVICE_ID_CYRIX_5520), 1 },
	{ 0, },
};
MODULE_DEVICE_TABLE(pci, cs5520_pci_tbl);

static struct pci_driver cs5520_pci_driver = {
	.name		= "Cyrix_IDE",
	.id_table	= cs5520_pci_tbl,
	.probe		= cs5520_init_one,
	.suspend	= ide_pci_suspend,
	.resume		= ide_pci_resume,
};

static int __init cs5520_ide_init(void)
{
	return ide_pci_register_driver(&cs5520_pci_driver);
}

module_init(cs5520_ide_init);

MODULE_AUTHOR("Alan Cox");
MODULE_DESCRIPTION("PCI driver module for Cyrix 5510/5520 IDE");
MODULE_LICENSE("GPL");
