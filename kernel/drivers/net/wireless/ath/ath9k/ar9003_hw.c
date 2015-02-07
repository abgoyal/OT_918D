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
 * Copyright (c) 2008-2010 Atheros Communications Inc.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include "hw.h"
#include "ar9003_mac.h"
#include "ar9003_initvals.h"

/* General hardware code for the AR9003 hadware family */

static bool ar9003_hw_macversion_supported(u32 macversion)
{
	switch (macversion) {
	case AR_SREV_VERSION_9300:
		return true;
	default:
		break;
	}
	return false;
}

/* AR9003 2.0 - new INI format (pre, core, post arrays per subsystem) */
/*
 * XXX: move TX/RX gain INI to its own init_mode_gain_regs after
 * ensuring it does not affect hardware bring up
 */
static void ar9003_hw_init_mode_regs(struct ath_hw *ah)
{
	/* mac */
	INIT_INI_ARRAY(&ah->iniMac[ATH_INI_PRE], NULL, 0, 0);
	INIT_INI_ARRAY(&ah->iniMac[ATH_INI_CORE],
		       ar9300_2p0_mac_core,
		       ARRAY_SIZE(ar9300_2p0_mac_core), 2);
	INIT_INI_ARRAY(&ah->iniMac[ATH_INI_POST],
		       ar9300_2p0_mac_postamble,
		       ARRAY_SIZE(ar9300_2p0_mac_postamble), 5);

	/* bb */
	INIT_INI_ARRAY(&ah->iniBB[ATH_INI_PRE], NULL, 0, 0);
	INIT_INI_ARRAY(&ah->iniBB[ATH_INI_CORE],
		       ar9300_2p0_baseband_core,
		       ARRAY_SIZE(ar9300_2p0_baseband_core), 2);
	INIT_INI_ARRAY(&ah->iniBB[ATH_INI_POST],
		       ar9300_2p0_baseband_postamble,
		       ARRAY_SIZE(ar9300_2p0_baseband_postamble), 5);

	/* radio */
	INIT_INI_ARRAY(&ah->iniRadio[ATH_INI_PRE], NULL, 0, 0);
	INIT_INI_ARRAY(&ah->iniRadio[ATH_INI_CORE],
		       ar9300_2p0_radio_core,
		       ARRAY_SIZE(ar9300_2p0_radio_core), 2);
	INIT_INI_ARRAY(&ah->iniRadio[ATH_INI_POST],
		       ar9300_2p0_radio_postamble,
		       ARRAY_SIZE(ar9300_2p0_radio_postamble), 5);

	/* soc */
	INIT_INI_ARRAY(&ah->iniSOC[ATH_INI_PRE],
		       ar9300_2p0_soc_preamble,
		       ARRAY_SIZE(ar9300_2p0_soc_preamble), 2);
	INIT_INI_ARRAY(&ah->iniSOC[ATH_INI_CORE], NULL, 0, 0);
	INIT_INI_ARRAY(&ah->iniSOC[ATH_INI_POST],
		       ar9300_2p0_soc_postamble,
		       ARRAY_SIZE(ar9300_2p0_soc_postamble), 5);

	/* rx/tx gain */
	INIT_INI_ARRAY(&ah->iniModesRxGain,
		       ar9300Common_rx_gain_table_2p0,
		       ARRAY_SIZE(ar9300Common_rx_gain_table_2p0), 2);
	INIT_INI_ARRAY(&ah->iniModesTxGain,
		       ar9300Modes_lowest_ob_db_tx_gain_table_2p0,
		       ARRAY_SIZE(ar9300Modes_lowest_ob_db_tx_gain_table_2p0),
		       5);

	/* Load PCIE SERDES settings from INI */

	/* Awake Setting */

	INIT_INI_ARRAY(&ah->iniPcieSerdes,
		       ar9300PciePhy_pll_on_clkreq_disable_L1_2p0,
		       ARRAY_SIZE(ar9300PciePhy_pll_on_clkreq_disable_L1_2p0),
		       2);

	/* Sleep Setting */

	INIT_INI_ARRAY(&ah->iniPcieSerdesLowPower,
		       ar9300PciePhy_clkreq_enable_L1_2p0,
		       ARRAY_SIZE(ar9300PciePhy_clkreq_enable_L1_2p0),
		       2);

	/* Fast clock modal settings */
	INIT_INI_ARRAY(&ah->iniModesAdditional,
		       ar9300Modes_fast_clock_2p0,
		       ARRAY_SIZE(ar9300Modes_fast_clock_2p0),
		       3);
}

static void ar9003_tx_gain_table_apply(struct ath_hw *ah)
{
	switch (ar9003_hw_get_tx_gain_idx(ah)) {
	case 0:
	default:
		INIT_INI_ARRAY(&ah->iniModesTxGain,
			       ar9300Modes_lowest_ob_db_tx_gain_table_2p0,
			       ARRAY_SIZE(ar9300Modes_lowest_ob_db_tx_gain_table_2p0),
			       5);
		break;
	case 1:
		INIT_INI_ARRAY(&ah->iniModesTxGain,
			       ar9300Modes_high_ob_db_tx_gain_table_2p0,
			       ARRAY_SIZE(ar9300Modes_high_ob_db_tx_gain_table_2p0),
			       5);
		break;
	case 2:
		INIT_INI_ARRAY(&ah->iniModesTxGain,
			       ar9300Modes_low_ob_db_tx_gain_table_2p0,
			       ARRAY_SIZE(ar9300Modes_low_ob_db_tx_gain_table_2p0),
			       5);
		break;
	}
}

static void ar9003_rx_gain_table_apply(struct ath_hw *ah)
{
	switch (ar9003_hw_get_rx_gain_idx(ah)) {
	case 0:
	default:
		INIT_INI_ARRAY(&ah->iniModesRxGain, ar9300Common_rx_gain_table_2p0,
			       ARRAY_SIZE(ar9300Common_rx_gain_table_2p0),
			       2);
		break;
	case 1:
		INIT_INI_ARRAY(&ah->iniModesRxGain,
			       ar9300Common_wo_xlna_rx_gain_table_2p0,
			       ARRAY_SIZE(ar9300Common_wo_xlna_rx_gain_table_2p0),
			       2);
		break;
	}
}

/* set gain table pointers according to values read from the eeprom */
static void ar9003_hw_init_mode_gain_regs(struct ath_hw *ah)
{
	ar9003_tx_gain_table_apply(ah);
	ar9003_rx_gain_table_apply(ah);
}

/*
 * Helper for ASPM support.
 *
 * Disable PLL when in L0s as well as receiver clock when in L1.
 * This power saving option must be enabled through the SerDes.
 *
 * Programming the SerDes must go through the same 288 bit serial shift
 * register as the other analog registers.  Hence the 9 writes.
 */
static void ar9003_hw_configpcipowersave(struct ath_hw *ah,
					 int restore,
					 int power_off)
{
	if (ah->is_pciexpress != true)
		return;

	/* Do not touch SerDes registers */
	if (ah->config.pcie_powersave_enable == 2)
		return;

	/* Nothing to do on restore for 11N */
	if (!restore) {
		/* set bit 19 to allow forcing of pcie core into L1 state */
		REG_SET_BIT(ah, AR_PCIE_PM_CTRL, AR_PCIE_PM_CTRL_ENA);

		/* Several PCIe massages to ensure proper behaviour */
		if (ah->config.pcie_waen)
			REG_WRITE(ah, AR_WA, ah->config.pcie_waen);
	}
}

/* Sets up the AR9003 hardware familiy callbacks */
void ar9003_hw_attach_ops(struct ath_hw *ah)
{
	struct ath_hw_private_ops *priv_ops = ath9k_hw_private_ops(ah);
	struct ath_hw_ops *ops = ath9k_hw_ops(ah);

	priv_ops->init_mode_regs = ar9003_hw_init_mode_regs;
	priv_ops->init_mode_gain_regs = ar9003_hw_init_mode_gain_regs;
	priv_ops->macversion_supported = ar9003_hw_macversion_supported;

	ops->config_pci_powersave = ar9003_hw_configpcipowersave;

	ar9003_hw_attach_phy_ops(ah);
	ar9003_hw_attach_calib_ops(ah);
	ar9003_hw_attach_mac_ops(ah);
}
