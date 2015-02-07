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

#ifndef __CVMX_MIXX_DEFS_H__
#define __CVMX_MIXX_DEFS_H__

#define CVMX_MIXX_BIST(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100078ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_CTL(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100020ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_INTENA(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100050ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_IRCNT(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100030ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_IRHWM(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100028ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_IRING1(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100010ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_IRING2(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100018ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_ISR(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100048ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_ORCNT(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100040ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_ORHWM(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100038ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_ORING1(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100000ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_ORING2(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100008ull + (((offset) & 1) * 2048))
#define CVMX_MIXX_REMCNT(offset) \
	 CVMX_ADD_IO_SEG(0x0001070000100058ull + (((offset) & 1) * 2048))

union cvmx_mixx_bist {
	uint64_t u64;
	struct cvmx_mixx_bist_s {
		uint64_t reserved_4_63:60;
		uint64_t mrqdat:1;
		uint64_t ipfdat:1;
		uint64_t irfdat:1;
		uint64_t orfdat:1;
	} s;
	struct cvmx_mixx_bist_s cn52xx;
	struct cvmx_mixx_bist_s cn52xxp1;
	struct cvmx_mixx_bist_s cn56xx;
	struct cvmx_mixx_bist_s cn56xxp1;
};

union cvmx_mixx_ctl {
	uint64_t u64;
	struct cvmx_mixx_ctl_s {
		uint64_t reserved_8_63:56;
		uint64_t crc_strip:1;
		uint64_t busy:1;
		uint64_t en:1;
		uint64_t reset:1;
		uint64_t lendian:1;
		uint64_t nbtarb:1;
		uint64_t mrq_hwm:2;
	} s;
	struct cvmx_mixx_ctl_s cn52xx;
	struct cvmx_mixx_ctl_s cn52xxp1;
	struct cvmx_mixx_ctl_s cn56xx;
	struct cvmx_mixx_ctl_s cn56xxp1;
};

union cvmx_mixx_intena {
	uint64_t u64;
	struct cvmx_mixx_intena_s {
		uint64_t reserved_7_63:57;
		uint64_t orunena:1;
		uint64_t irunena:1;
		uint64_t data_drpena:1;
		uint64_t ithena:1;
		uint64_t othena:1;
		uint64_t ivfena:1;
		uint64_t ovfena:1;
	} s;
	struct cvmx_mixx_intena_s cn52xx;
	struct cvmx_mixx_intena_s cn52xxp1;
	struct cvmx_mixx_intena_s cn56xx;
	struct cvmx_mixx_intena_s cn56xxp1;
};

union cvmx_mixx_ircnt {
	uint64_t u64;
	struct cvmx_mixx_ircnt_s {
		uint64_t reserved_20_63:44;
		uint64_t ircnt:20;
	} s;
	struct cvmx_mixx_ircnt_s cn52xx;
	struct cvmx_mixx_ircnt_s cn52xxp1;
	struct cvmx_mixx_ircnt_s cn56xx;
	struct cvmx_mixx_ircnt_s cn56xxp1;
};

union cvmx_mixx_irhwm {
	uint64_t u64;
	struct cvmx_mixx_irhwm_s {
		uint64_t reserved_40_63:24;
		uint64_t ibplwm:20;
		uint64_t irhwm:20;
	} s;
	struct cvmx_mixx_irhwm_s cn52xx;
	struct cvmx_mixx_irhwm_s cn52xxp1;
	struct cvmx_mixx_irhwm_s cn56xx;
	struct cvmx_mixx_irhwm_s cn56xxp1;
};

union cvmx_mixx_iring1 {
	uint64_t u64;
	struct cvmx_mixx_iring1_s {
		uint64_t reserved_60_63:4;
		uint64_t isize:20;
		uint64_t reserved_36_39:4;
		uint64_t ibase:33;
		uint64_t reserved_0_2:3;
	} s;
	struct cvmx_mixx_iring1_s cn52xx;
	struct cvmx_mixx_iring1_s cn52xxp1;
	struct cvmx_mixx_iring1_s cn56xx;
	struct cvmx_mixx_iring1_s cn56xxp1;
};

union cvmx_mixx_iring2 {
	uint64_t u64;
	struct cvmx_mixx_iring2_s {
		uint64_t reserved_52_63:12;
		uint64_t itlptr:20;
		uint64_t reserved_20_31:12;
		uint64_t idbell:20;
	} s;
	struct cvmx_mixx_iring2_s cn52xx;
	struct cvmx_mixx_iring2_s cn52xxp1;
	struct cvmx_mixx_iring2_s cn56xx;
	struct cvmx_mixx_iring2_s cn56xxp1;
};

union cvmx_mixx_isr {
	uint64_t u64;
	struct cvmx_mixx_isr_s {
		uint64_t reserved_7_63:57;
		uint64_t orun:1;
		uint64_t irun:1;
		uint64_t data_drp:1;
		uint64_t irthresh:1;
		uint64_t orthresh:1;
		uint64_t idblovf:1;
		uint64_t odblovf:1;
	} s;
	struct cvmx_mixx_isr_s cn52xx;
	struct cvmx_mixx_isr_s cn52xxp1;
	struct cvmx_mixx_isr_s cn56xx;
	struct cvmx_mixx_isr_s cn56xxp1;
};

union cvmx_mixx_orcnt {
	uint64_t u64;
	struct cvmx_mixx_orcnt_s {
		uint64_t reserved_20_63:44;
		uint64_t orcnt:20;
	} s;
	struct cvmx_mixx_orcnt_s cn52xx;
	struct cvmx_mixx_orcnt_s cn52xxp1;
	struct cvmx_mixx_orcnt_s cn56xx;
	struct cvmx_mixx_orcnt_s cn56xxp1;
};

union cvmx_mixx_orhwm {
	uint64_t u64;
	struct cvmx_mixx_orhwm_s {
		uint64_t reserved_20_63:44;
		uint64_t orhwm:20;
	} s;
	struct cvmx_mixx_orhwm_s cn52xx;
	struct cvmx_mixx_orhwm_s cn52xxp1;
	struct cvmx_mixx_orhwm_s cn56xx;
	struct cvmx_mixx_orhwm_s cn56xxp1;
};

union cvmx_mixx_oring1 {
	uint64_t u64;
	struct cvmx_mixx_oring1_s {
		uint64_t reserved_60_63:4;
		uint64_t osize:20;
		uint64_t reserved_36_39:4;
		uint64_t obase:33;
		uint64_t reserved_0_2:3;
	} s;
	struct cvmx_mixx_oring1_s cn52xx;
	struct cvmx_mixx_oring1_s cn52xxp1;
	struct cvmx_mixx_oring1_s cn56xx;
	struct cvmx_mixx_oring1_s cn56xxp1;
};

union cvmx_mixx_oring2 {
	uint64_t u64;
	struct cvmx_mixx_oring2_s {
		uint64_t reserved_52_63:12;
		uint64_t otlptr:20;
		uint64_t reserved_20_31:12;
		uint64_t odbell:20;
	} s;
	struct cvmx_mixx_oring2_s cn52xx;
	struct cvmx_mixx_oring2_s cn52xxp1;
	struct cvmx_mixx_oring2_s cn56xx;
	struct cvmx_mixx_oring2_s cn56xxp1;
};

union cvmx_mixx_remcnt {
	uint64_t u64;
	struct cvmx_mixx_remcnt_s {
		uint64_t reserved_52_63:12;
		uint64_t iremcnt:20;
		uint64_t reserved_20_31:12;
		uint64_t oremcnt:20;
	} s;
	struct cvmx_mixx_remcnt_s cn52xx;
	struct cvmx_mixx_remcnt_s cn52xxp1;
	struct cvmx_mixx_remcnt_s cn56xx;
	struct cvmx_mixx_remcnt_s cn56xxp1;
};

#endif
