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
 * arch/arm/mach-orion5x/rd88f6183-ap-ge-setup.c
 *
 * Marvell Orion-1-90 AP GE Reference Design Setup
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/pci.h>
#include <linux/irq.h>
#include <linux/mtd/physmap.h>
#include <linux/mv643xx_eth.h>
#include <linux/spi/spi.h>
#include <linux/spi/orion_spi.h>
#include <linux/spi/flash.h>
#include <linux/ethtool.h>
#include <net/dsa.h>
#include <asm/mach-types.h>
#include <asm/gpio.h>
#include <asm/leds.h>
#include <asm/mach/arch.h>
#include <asm/mach/pci.h>
#include <mach/orion5x.h>
#include "common.h"
#include "mpp.h"

static struct mv643xx_eth_platform_data rd88f6183ap_ge_eth_data = {
	.phy_addr	= -1,
	.speed		= SPEED_1000,
	.duplex		= DUPLEX_FULL,
};

static struct dsa_chip_data rd88f6183ap_ge_switch_chip_data = {
	.port_names[0]	= "lan1",
	.port_names[1]	= "lan2",
	.port_names[2]	= "lan3",
	.port_names[3]	= "lan4",
	.port_names[4]	= "wan",
	.port_names[5]	= "cpu",
};

static struct dsa_platform_data rd88f6183ap_ge_switch_plat_data = {
	.nr_chips	= 1,
	.chip		= &rd88f6183ap_ge_switch_chip_data,
};

static struct mtd_partition rd88f6183ap_ge_partitions[] = {
	{
		.name	= "kernel",
		.offset	= 0x00000000,
		.size	= 0x00200000,
	}, {
		.name	= "rootfs",
		.offset	= 0x00200000,
		.size	= 0x00500000,
	}, {
		.name	= "nvram",
		.offset	= 0x00700000,
		.size	= 0x00080000,
	},
};

static struct flash_platform_data rd88f6183ap_ge_spi_slave_data = {
	.type		= "m25p64",
	.nr_parts	= ARRAY_SIZE(rd88f6183ap_ge_partitions),
	.parts		= rd88f6183ap_ge_partitions,
};

static struct spi_board_info __initdata rd88f6183ap_ge_spi_slave_info[] = {
	{
		.modalias	= "m25p80",
		.platform_data	= &rd88f6183ap_ge_spi_slave_data,
		.irq		= NO_IRQ,
		.max_speed_hz	= 20000000,
		.bus_num	= 0,
		.chip_select	= 0,
	},
};

static void __init rd88f6183ap_ge_init(void)
{
	/*
	 * Setup basic Orion functions. Need to be called early.
	 */
	orion5x_init();

	/*
	 * Configure peripherals.
	 */
	orion5x_ehci0_init();
	orion5x_eth_init(&rd88f6183ap_ge_eth_data);
	orion5x_eth_switch_init(&rd88f6183ap_ge_switch_plat_data,
				gpio_to_irq(3));
	spi_register_board_info(rd88f6183ap_ge_spi_slave_info,
				ARRAY_SIZE(rd88f6183ap_ge_spi_slave_info));
	orion5x_spi_init();
	orion5x_uart0_init();
}

static struct hw_pci rd88f6183ap_ge_pci __initdata = {
	.nr_controllers	= 2,
	.swizzle	= pci_std_swizzle,
	.setup		= orion5x_pci_sys_setup,
	.scan		= orion5x_pci_sys_scan_bus,
	.map_irq	= orion5x_pci_map_irq,
};

static int __init rd88f6183ap_ge_pci_init(void)
{
	if (machine_is_rd88f6183ap_ge()) {
		orion5x_pci_disable();
		pci_common_init(&rd88f6183ap_ge_pci);
	}

	return 0;
}
subsys_initcall(rd88f6183ap_ge_pci_init);

MACHINE_START(RD88F6183AP_GE, "Marvell Orion-1-90 AP GE Reference Design")
	/* Maintainer: Lennert Buytenhek <buytenh@marvell.com> */
	.phys_io	= ORION5X_REGS_PHYS_BASE,
	.io_pg_offst	= ((ORION5X_REGS_VIRT_BASE) >> 18) & 0xFFFC,
	.boot_params	= 0x00000100,
	.init_machine	= rd88f6183ap_ge_init,
	.map_io		= orion5x_map_io,
	.init_irq	= orion5x_init_irq,
	.timer		= &orion5x_timer,
	.fixup		= tag_fixup_mem32,
MACHINE_END