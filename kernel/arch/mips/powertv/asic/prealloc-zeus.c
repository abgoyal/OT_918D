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
 * Memory pre-allocations for Zeus boxes.
 *
 * Copyright (C) 2005-2009 Scientific-Atlanta, Inc.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
 *
 * Author:       Ken Eppinett
 *               David Schleef <ds@schleef.org>
 */

#include <linux/init.h>
#include <linux/ioport.h>
#include <asm/mach-powertv/asic.h>
#include "prealloc.h"

/*
 * DVR_CAPABLE RESOURCES
 */
struct resource dvr_zeus_resources[] __initdata =
{
	/*
	 * VIDEO1 / LX1
	 */
	/* Delta-Mu 1 image (2MiB) */
	PREALLOC_NORMAL("ST231aImage", 0x20000000, 0x20200000-1,
		IORESOURCE_MEM)
	/* Delta-Mu 1 monitor (8KiB) */
	PREALLOC_NORMAL("ST231aMonitor", 0x20200000, 0x20202000-1,
		IORESOURCE_MEM)
	/* Delta-Mu 1 RAM (~29.9MiB (32MiB - (2MiB + 8KiB))) */
	PREALLOC_NORMAL("MediaMemory1", 0x20202000, 0x22000000-1,
		IORESOURCE_MEM)

	/*
	 * VIDEO2 / LX2
	 */
	/* Delta-Mu 2 image (2MiB) */
	PREALLOC_NORMAL("ST231bImage", 0x30000000, 0x30200000-1,
		IORESOURCE_MEM)
	/* Delta-Mu 2 monitor (8KiB) */
	PREALLOC_NORMAL("ST231bMonitor", 0x30200000, 0x30202000-1,
		IORESOURCE_MEM)
	/* Delta-Mu 2 RAM (~29.9MiB (32MiB - (2MiB + 8KiB))) */
	PREALLOC_NORMAL("MediaMemory2", 0x30202000, 0x32000000-1,
		IORESOURCE_MEM)

	/*
	 * Sysaudio Driver
	 */
	/* DSP code and data images (1MiB) */
	PREALLOC_NORMAL("DSP_Image_Buff", 0x00000000, 0x00100000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/* ADSC CPU PCM buffer (40KiB) */
	PREALLOC_NORMAL("ADSC_CPU_PCM_Buff", 0x00000000, 0x0000A000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/* ADSC AUX buffer (16KiB) */
	PREALLOC_NORMAL("ADSC_AUX_Buff", 0x00000000, 0x00004000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/* ADSC Main buffer (16KiB) */
	PREALLOC_NORMAL("ADSC_Main_Buff", 0x00000000, 0x00004000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * STAVEM driver/STAPI
	 *
	 *  This memory area is used for allocating buffers for Video decoding
	 *  purposes.  Allocation/De-allocation within this buffer is managed
	 *  by the STAVMEM driver of the STAPI.  They could be Decimated
	 *  Picture Buffers, Intermediate Buffers, as deemed necessary for
	 *  video decoding purposes, for any video decoders on Zeus.
	 */
	/* 12MiB */
	PREALLOC_NORMAL("AVMEMPartition0", 0x00000000, 0x00c00000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * DOCSIS Subsystem
	 */
	/* 7MiB */
	PREALLOC_DOCSIS("Docsis", 0x40100000, 0x40800000-1, IORESOURCE_MEM)

	/*
	 * GHW HAL Driver
	 */
	/* PowerTV Graphics Heap (14MiB) */
	PREALLOC_NORMAL("GraphicsHeap", 0x46900000, 0x47700000-1,
		IORESOURCE_MEM)

	/*
	 * multi com buffer area
	 */
	/* 128KiB */
	PREALLOC_NORMAL("MulticomSHM", 0x47900000, 0x47920000-1,
		IORESOURCE_MEM)

	/*
	 * DMA Ring buffer
	 */
	/* 2.5MiB */
	PREALLOC_NORMAL("BMM_Buffer", 0x00000000, 0x00280000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * Display bins buffer for unit0
	 */
	/* 4KiB */
	PREALLOC_NORMAL("DisplayBins0", 0x00000000, 0x00001000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * Display bins buffer for unit1
	 */
	/* 4KiB */
	PREALLOC_NORMAL("DisplayBins1", 0x00000000, 0x00001000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * ITFS
	 */
	/* 815,104 bytes each for 2 ITFS partitions. */
	PREALLOC_NORMAL("ITFS", 0x00000000, 0x0018E000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * AVFS
	 */
	/* (945K * 8) = (128K * 3) 5 playbacks / 3 server */
	PREALLOC_NORMAL("AvfsDmaMem", 0x00000000, 0x007c2000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/* 4KiB */
	PREALLOC_NORMAL("AvfsFileSys", 0x00000000, 0x00001000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * PMEM
	 */
	/* Persistent memory for diagnostics (64KiB) */
	PREALLOC_PMEM("DiagPersistentMemory", 0x00000000, 0x10000-1,
	     (IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * Smartcard
	 */
	/* Read and write buffers for Internal/External cards (10KiB) */
	PREALLOC_NORMAL("SmartCardInfo", 0x00000000, 0x2800-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * TFTPBuffer
	 *
	 *  This buffer is used in some minimal configurations (e.g. two-way
	 *  loader) for storing software images
	 */
	PREALLOC_TFTP("TFTPBuffer", 0x00000000, MEBIBYTE(80)-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * Add other resources here
	 */

	/*
	 * End of Resource marker
	 */
	{
		.flags  = 0,
	},
};

/*
 * NON_DVR_CAPABLE ZEUS RESOURCES
 */
struct resource non_dvr_zeus_resources[] __initdata =
{
	/*
	 * VIDEO1 / LX1
	 */
	/* Delta-Mu 1 image (2MiB) */
	PREALLOC_NORMAL("ST231aImage", 0x20000000, 0x20200000-1,
		IORESOURCE_MEM)
	/* Delta-Mu 1 monitor (8KiB) */
	PREALLOC_NORMAL("ST231aMonitor", 0x20200000, 0x20202000-1,
		IORESOURCE_MEM)
	/* Delta-Mu 1 RAM (~29.9MiB (32MiB - (2MiB + 8KiB))) */
	PREALLOC_NORMAL("MediaMemory1", 0x20202000, 0x22000000-1,
		IORESOURCE_MEM)

	/*
	 * Sysaudio Driver
	 */
	/* DSP code and data images (1MiB) */
	PREALLOC_NORMAL("DSP_Image_Buff", 0x00000000, 0x00100000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/* ADSC CPU PCM buffer (40KiB) */
	PREALLOC_NORMAL("ADSC_CPU_PCM_Buff", 0x00000000, 0x0000A000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/* ADSC AUX buffer (16KiB) */
	PREALLOC_NORMAL("ADSC_AUX_Buff", 0x00000000, 0x00004000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))
	/* ADSC Main buffer (16KiB) */
	PREALLOC_NORMAL("ADSC_Main_Buff", 0x00000000, 0x00004000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * STAVEM driver/STAPI
	 */
	/* 6MiB */
	PREALLOC_NORMAL("AVMEMPartition0", 0x00000000, 0x00600000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * DOCSIS Subsystem
	 */
	/* 7MiB */
	PREALLOC_DOCSIS("Docsis", 0x40100000, 0x40800000-1, IORESOURCE_MEM)

	/*
	 * GHW HAL Driver
	 */
	/* PowerTV Graphics Heap (14MiB) */
	PREALLOC_NORMAL("GraphicsHeap", 0x46900000, 0x47700000-1,
		IORESOURCE_MEM)

	/*
	 * multi com buffer area
	 */
	/* 128KiB */
	PREALLOC_NORMAL("MulticomSHM", 0x47900000, 0x47920000-1,
		IORESOURCE_MEM)

	/*
	 * DMA Ring buffer
	 */
	/* 2.5MiB */
	PREALLOC_NORMAL("BMM_Buffer", 0x00000000, 0x00280000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * Display bins buffer for unit0
	 */
	/* 4KiB */
	PREALLOC_NORMAL("DisplayBins0", 0x00000000, 0x00001000-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * AVFS: player HAL memory
	 */
	/* 945K * 3 for playback */
	PREALLOC_NORMAL("AvfsDmaMem", 0x00000000, 0x002c4c00-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * PMEM
	 */
	/* Persistent memory for diagnostics (64KiB) */
	PREALLOC_PMEM("DiagPersistentMemory", 0x00000000, 0x10000-1,
	     (IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * Smartcard
	 */
	/* Read and write buffers for Internal/External cards (10KiB) */
	PREALLOC_NORMAL("SmartCardInfo", 0x00000000, 0x2800-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * NAND Flash
	 */
	/* 10KiB */
	PREALLOC_NORMAL("NandFlash", NAND_FLASH_BASE, NAND_FLASH_BASE+0x400-1,
		IORESOURCE_MEM)

	/*
	 * TFTPBuffer
	 *
	 *  This buffer is used in some minimal configurations (e.g. two-way
	 *  loader) for storing software images
	 */
	PREALLOC_TFTP("TFTPBuffer", 0x00000000, MEBIBYTE(80)-1,
		(IORESOURCE_MEM|IORESOURCE_PTV_RES_LOEXT))

	/*
	 * Add other resources here
	 */

	/*
	 * End of Resource marker
	 */
	{
		.flags  = 0,
	},
};
