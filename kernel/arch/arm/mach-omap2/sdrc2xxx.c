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
 * linux/arch/arm/mach-omap2/sdrc2xxx.c
 *
 * SDRAM timing related functions for OMAP2xxx
 *
 * Copyright (C) 2005, 2008 Texas Instruments Inc.
 * Copyright (C) 2005, 2008 Nokia Corporation
 *
 * Tony Lindgren <tony@atomide.com>
 * Paul Walmsley
 * Richard Woodruff <r-woodruff2@ti.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/device.h>
#include <linux/list.h>
#include <linux/errno.h>
#include <linux/delay.h>
#include <linux/clk.h>
#include <linux/io.h>

#include <plat/common.h>
#include <plat/clock.h>
#include <plat/sram.h>

#include "prm.h"
#include "clock.h"
#include <plat/sdrc.h>
#include "sdrc.h"

/* Memory timing, DLL mode flags */
#define M_DDR		1
#define M_LOCK_CTRL	(1 << 2)
#define M_UNLOCK	0
#define M_LOCK		1


static struct memory_timings mem_timings;
static u32 curr_perf_level = CORE_CLK_SRC_DPLL_X2;

static u32 omap2xxx_sdrc_get_slow_dll_ctrl(void)
{
	return mem_timings.slow_dll_ctrl;
}

static u32 omap2xxx_sdrc_get_fast_dll_ctrl(void)
{
	return mem_timings.fast_dll_ctrl;
}

static u32 omap2xxx_sdrc_get_type(void)
{
	return mem_timings.m_type;
}

/*
 * Check the DLL lock state, and return tue if running in unlock mode.
 * This is needed to compensate for the shifted DLL value in unlock mode.
 */
u32 omap2xxx_sdrc_dll_is_unlocked(void)
{
	/* dlla and dllb are a set */
	u32 dll_state = sdrc_read_reg(SDRC_DLLA_CTRL);

	if ((dll_state & (1 << 2)) == (1 << 2))
		return 1;
	else
		return 0;
}

/*
 * 'level' is the value to store to CM_CLKSEL2_PLL.CORE_CLK_SRC.
 * Practical values are CORE_CLK_SRC_DPLL (for CORE_CLK = DPLL_CLK) or
 * CORE_CLK_SRC_DPLL_X2 (for CORE_CLK = * DPLL_CLK * 2)
 *
 * Used by the clock framework during CORE DPLL changes
 */
u32 omap2xxx_sdrc_reprogram(u32 level, u32 force)
{
	u32 dll_ctrl, m_type;
	u32 prev = curr_perf_level;
	unsigned long flags;

	if ((curr_perf_level == level) && !force)
		return prev;

	if (level == CORE_CLK_SRC_DPLL)
		dll_ctrl = omap2xxx_sdrc_get_slow_dll_ctrl();
	else if (level == CORE_CLK_SRC_DPLL_X2)
		dll_ctrl = omap2xxx_sdrc_get_fast_dll_ctrl();
	else
		return prev;

	m_type = omap2xxx_sdrc_get_type();

	local_irq_save(flags);
	if (cpu_is_omap2420())
		__raw_writel(0xffff, OMAP2420_PRCM_VOLTSETUP);
	else
		__raw_writel(0xffff, OMAP2430_PRCM_VOLTSETUP);
	omap2_sram_reprogram_sdrc(level, dll_ctrl, m_type);
	curr_perf_level = level;
	local_irq_restore(flags);

	return prev;
}

/* Used by the clock framework during CORE DPLL changes */
void omap2xxx_sdrc_init_params(u32 force_lock_to_unlock_mode)
{
	unsigned long dll_cnt;
	u32 fast_dll = 0;

	/* DDR = 1, SDR = 0 */
	mem_timings.m_type = !((sdrc_read_reg(SDRC_MR_0) & 0x3) == 0x1);

	/* 2422 es2.05 and beyond has a single SIP DDR instead of 2 like others.
	 * In the case of 2422, its ok to use CS1 instead of CS0.
	 */
	if (cpu_is_omap2422())
		mem_timings.base_cs = 1;
	else
		mem_timings.base_cs = 0;

	if (mem_timings.m_type != M_DDR)
		return;

	/* With DDR we need to determine the low frequency DLL value */
	if (((mem_timings.fast_dll_ctrl & (1 << 2)) == M_LOCK_CTRL))
		mem_timings.dll_mode = M_UNLOCK;
	else
		mem_timings.dll_mode = M_LOCK;

	if (mem_timings.base_cs == 0) {
		fast_dll = sdrc_read_reg(SDRC_DLLA_CTRL);
		dll_cnt = sdrc_read_reg(SDRC_DLLA_STATUS) & 0xff00;
	} else {
		fast_dll = sdrc_read_reg(SDRC_DLLB_CTRL);
		dll_cnt = sdrc_read_reg(SDRC_DLLB_STATUS) & 0xff00;
	}
	if (force_lock_to_unlock_mode) {
		fast_dll &= ~0xff00;
		fast_dll |= dll_cnt;		/* Current lock mode */
	}
	/* set fast timings with DLL filter disabled */
	mem_timings.fast_dll_ctrl = (fast_dll | (3 << 8));

	/* No disruptions, DDR will be offline & C-ABI not followed */
	omap2_sram_ddr_init(&mem_timings.slow_dll_ctrl,
			    mem_timings.fast_dll_ctrl,
			    mem_timings.base_cs,
			    force_lock_to_unlock_mode);
	mem_timings.slow_dll_ctrl &= 0xff00;	/* Keep lock value */

	/* Turn status into unlock ctrl */
	mem_timings.slow_dll_ctrl |=
		((mem_timings.fast_dll_ctrl & 0xF) | (1 << 2));

	/* 90 degree phase for anything below 133Mhz + disable DLL filter */
	mem_timings.slow_dll_ctrl |= ((1 << 1) | (3 << 8));
}
