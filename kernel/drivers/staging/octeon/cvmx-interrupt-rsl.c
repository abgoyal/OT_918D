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

/***********************license start***************
 * Author: Cavium Networks
 *
 * Contact: support@caviumnetworks.com
 * This file is part of the OCTEON SDK
 *
 * Copyright (c) 2003-2008 Cavium Networks
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License, Version 2, as
 * published by the Free Software Foundation.
 *
 * This file is distributed in the hope that it will be useful, but
 * AS-IS and WITHOUT ANY WARRANTY; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, TITLE, or
 * NONINFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this file; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 * or visit http://www.gnu.org/licenses/.
 *
 * This file may also be available under a different license from Cavium.
 * Contact Cavium Networks for more information
 ***********************license end**************************************/

/*
 * Utility functions to decode Octeon's RSL_INT_BLOCKS
 * interrupts into error messages.
 */

#include <asm/octeon/octeon.h>

#include "cvmx-asxx-defs.h"
#include "cvmx-gmxx-defs.h"

#ifndef PRINT_ERROR
#define PRINT_ERROR(format, ...)
#endif

void __cvmx_interrupt_gmxx_rxx_int_en_enable(int index, int block);

/**
 * Enable ASX error interrupts that exist on CN3XXX, CN50XX, and
 * CN58XX.
 *
 * @block:  Interface to enable 0-1
 */
void __cvmx_interrupt_asxx_enable(int block)
{
	int mask;
	union cvmx_asxx_int_en csr;
	/*
	 * CN38XX and CN58XX have two interfaces with 4 ports per
	 * interface. All other chips have a max of 3 ports on
	 * interface 0
	 */
	if (OCTEON_IS_MODEL(OCTEON_CN38XX) || OCTEON_IS_MODEL(OCTEON_CN58XX))
		mask = 0xf;	/* Set enables for 4 ports */
	else
		mask = 0x7;	/* Set enables for 3 ports */

	/* Enable interface interrupts */
	csr.u64 = cvmx_read_csr(CVMX_ASXX_INT_EN(block));
	csr.s.txpsh = mask;
	csr.s.txpop = mask;
	csr.s.ovrflw = mask;
	cvmx_write_csr(CVMX_ASXX_INT_EN(block), csr.u64);
}
/**
 * Enable GMX error reporting for the supplied interface
 *
 * @interface: Interface to enable
 */
void __cvmx_interrupt_gmxx_enable(int interface)
{
	union cvmx_gmxx_inf_mode mode;
	union cvmx_gmxx_tx_int_en gmx_tx_int_en;
	int num_ports;
	int index;

	mode.u64 = cvmx_read_csr(CVMX_GMXX_INF_MODE(interface));

	if (OCTEON_IS_MODEL(OCTEON_CN56XX) || OCTEON_IS_MODEL(OCTEON_CN52XX)) {
		if (mode.s.en) {
			switch (mode.cn56xx.mode) {
			case 1:	/* XAUI */
				num_ports = 1;
				break;
			case 2:	/* SGMII */
			case 3:	/* PICMG */
				num_ports = 4;
				break;
			default:	/* Disabled */
				num_ports = 0;
				break;
			}
		} else
			num_ports = 0;
	} else {
		if (mode.s.en) {
			if (OCTEON_IS_MODEL(OCTEON_CN38XX)
			    || OCTEON_IS_MODEL(OCTEON_CN58XX)) {
				/*
				 * SPI on CN38XX and CN58XX report all
				 * errors through port 0.  RGMII needs
				 * to check all 4 ports
				 */
				if (mode.s.type)
					num_ports = 1;
				else
					num_ports = 4;
			} else {
				/*
				 * CN30XX, CN31XX, and CN50XX have two
				 * or three ports. GMII and MII has 2,
				 * RGMII has three
				 */
				if (mode.s.type)
					num_ports = 2;
				else
					num_ports = 3;
			}
		} else
			num_ports = 0;
	}

	gmx_tx_int_en.u64 = 0;
	if (num_ports) {
		if (OCTEON_IS_MODEL(OCTEON_CN38XX)
		    || OCTEON_IS_MODEL(OCTEON_CN58XX))
			gmx_tx_int_en.s.ncb_nxa = 1;
		gmx_tx_int_en.s.pko_nxa = 1;
	}
	gmx_tx_int_en.s.undflw = (1 << num_ports) - 1;
	cvmx_write_csr(CVMX_GMXX_TX_INT_EN(interface), gmx_tx_int_en.u64);
	for (index = 0; index < num_ports; index++)
		__cvmx_interrupt_gmxx_rxx_int_en_enable(index, interface);
}
