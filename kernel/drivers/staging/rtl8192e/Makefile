# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


NIC_SELECT = RTL8192E

EXTRA_CFLAGS += -DRTL8192E
EXTRA_CFLAGS += -std=gnu89
EXTRA_CFLAGS += -O2
EXTRA_CFLAGS += -DTHOMAS_TURBO
EXTRA_CFLAGS += -DENABLE_DOT11D

EXTRA_CFLAGS += -DENABLE_IPS
EXTRA_CFLAGS += -DENABLE_LPS

r8192e_pci-objs :=		\
	r8192E_core.o		\
	r8180_93cx6.o		\
	r8192E_wx.o		\
	r8190_rtl8256.o		\
	r819xE_phy.o		\
	r819xE_firmware.o	\
	r819xE_cmdpkt.o		\
	r8192E_dm.o		\
	r8192_pm.o		\
	ieee80211/ieee80211_rx.o		\
	ieee80211/ieee80211_softmac.o		\
	ieee80211/ieee80211_tx.o		\
	ieee80211/ieee80211_wx.o		\
	ieee80211/ieee80211_module.o		\
	ieee80211/ieee80211_softmac_wx.o	\
	ieee80211/rtl819x_HTProc.o		\
	ieee80211/rtl819x_TSProc.o		\
	ieee80211/rtl819x_BAProc.o		\
	ieee80211/dot11d.o			\
	ieee80211/ieee80211_crypt.o		\
	ieee80211/ieee80211_crypt_tkip.o	\
	ieee80211/ieee80211_crypt_ccmp.o	\
	ieee80211/ieee80211_crypt_wep.o

obj-$(CONFIG_RTL8192E) += r8192e_pci.o

