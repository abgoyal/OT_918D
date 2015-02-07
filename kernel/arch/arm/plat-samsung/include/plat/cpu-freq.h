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

/* arch/arm/plat-samsung/include/plat/cpu-freq.h
 *
 * Copyright (c) 2006-2007 Simtec Electronics
 *	http://armlinux.simtec.co.uk/
 *	Ben Dooks <ben@simtec.co.uk>
 *
 * S3C CPU frequency scaling support - driver and board
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
*/

#include <linux/cpufreq.h>

struct s3c_cpufreq_info;
struct s3c_cpufreq_board;
struct s3c_iotimings;

/**
 * struct s3c_freq - frequency information (mainly for core drivers)
 * @fclk: The FCLK frequency in Hz.
 * @armclk: The ARMCLK frequency in Hz.
 * @hclk_tns: HCLK cycle time in 10ths of nano-seconds.
 * @hclk: The HCLK frequency in Hz.
 * @pclk: The PCLK frequency in Hz.
 *
 * This contains the frequency information about the current configuration
 * mainly for the core drivers to ensure we do not end up passing about
 * a large number of parameters.
 *
 * The @hclk_tns field is a useful cache for the parts of the drivers that
 * need to calculate IO timings and suchlike.
 */
struct s3c_freq {
	unsigned long	fclk;
	unsigned long	armclk;
	unsigned long	hclk_tns;	/* in 10ths of ns */
	unsigned long	hclk;
	unsigned long	pclk;
};

/**
 * struct s3c_cpufreq_freqs - s3c cpufreq notification information.
 * @freqs: The cpufreq setting information.
 * @old: The old clock settings.
 * @new: The new clock settings.
 * @pll_changing: Set if the PLL is changing.
 *
 * Wrapper 'struct cpufreq_freqs' so that any drivers receiving the
 * notification can use this information that is not provided by just
 * having the core frequency alone.
 *
 * The pll_changing flag is used to indicate if the PLL itself is
 * being set during this change. This is important as the clocks
 * will temporarily be set to the XTAL clock during this time, so
 * drivers may want to close down their output during this time.
 *
 * Note, this is not being used by any current drivers and therefore
 * may be removed in the future.
 */
struct s3c_cpufreq_freqs {
	struct cpufreq_freqs	freqs;
	struct s3c_freq		old;
	struct s3c_freq		new;

	unsigned int		pll_changing:1;
};

#define to_s3c_cpufreq(_cf) container_of(_cf, struct s3c_cpufreq_freqs, freqs)

/**
 * struct s3c_clkdivs - clock divisor information
 * @p_divisor: Divisor from FCLK to PCLK.
 * @h_divisor: Divisor from FCLK to HCLK.
 * @arm_divisor: Divisor from FCLK to ARMCLK (not all CPUs).
 * @dvs: Non-zero if using DVS mode for ARMCLK.
 *
 * Divisor settings for the core clocks.
 */
struct s3c_clkdivs {
	int		p_divisor;
	int		h_divisor;
	int		arm_divisor;
	unsigned char	dvs;
};

#define PLLVAL(_m, _p, _s) (((_m) << 12) | ((_p) << 4) | (_s))

/**
 * struct s3c_pllval - PLL value entry.
 * @freq: The frequency for this entry in Hz.
 * @pll_reg: The PLL register setting for this PLL value.
 */
struct s3c_pllval {
	unsigned long		freq;
	unsigned long		pll_reg;
};

/**
 * struct s3c_cpufreq_board - per-board cpu frequency informatin
 * @refresh: The SDRAM refresh period in nanoseconds.
 * @auto_io: Set if the IO timing settings should be generated from the
 *	initialisation time hardware registers.
 * @need_io: Set if the board has external IO on any of the chipselect
 *	lines that will require the hardware timing registers to be
 *	updated on a clock change.
 * @max: The maxium frequency limits for the system. Any field that
 *	is left at zero will use the CPU's settings.
 *
 * This contains the board specific settings that affect how the CPU
 * drivers chose settings. These include the memory refresh and IO
 * timing information.
 *
 * Registration depends on the driver being used, the ARMCLK only
 * implementation does not currently need this but the older style
 * driver requires this to be available.
 */
struct s3c_cpufreq_board {
	unsigned int	refresh;
	unsigned int	auto_io:1;	/* automatically init io timings. */
	unsigned int	need_io:1;	/* set if needs io timing support. */

	/* any non-zero field in here is taken as an upper limit. */
	struct s3c_freq	max;	/* frequency limits */
};

/* Things depending on frequency scaling. */
#ifdef CONFIG_CPU_FREQ_S3C
#define __init_or_cpufreq
#else
#define __init_or_cpufreq __init
#endif

/* Board functions */

#ifdef CONFIG_CPU_FREQ_S3C
extern int s3c_cpufreq_setboard(struct s3c_cpufreq_board *board);
#else

static inline int s3c_cpufreq_setboard(struct s3c_cpufreq_board *board)
{
	return 0;
}
#endif  /* CONFIG_CPU_FREQ_S3C */