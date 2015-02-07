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
 * PCI Express Hot Plug Controller Driver
 *
 * Copyright (C) 1995,2001 Compaq Computer Corporation
 * Copyright (C) 2001 Greg Kroah-Hartman (greg@kroah.com)
 * Copyright (C) 2001 IBM Corp.
 * Copyright (C) 2003-2004 Intel Corporation
 *
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE, GOOD TITLE or
 * NON INFRINGEMENT.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 * Send feedback to <greg@kroah.com>, <kristen.c.accardi@intel.com>
 *
 */
#ifndef _PCIEHP_H
#define _PCIEHP_H

#include <linux/types.h>
#include <linux/pci.h>
#include <linux/pci_hotplug.h>
#include <linux/delay.h>
#include <linux/sched.h>		/* signal_pending() */
#include <linux/pcieport_if.h>
#include <linux/mutex.h>

#define MY_NAME	"pciehp"

extern int pciehp_poll_mode;
extern int pciehp_poll_time;
extern int pciehp_debug;
extern int pciehp_force;
extern struct workqueue_struct *pciehp_wq;

#define dbg(format, arg...)						\
do {									\
	if (pciehp_debug)						\
		printk(KERN_DEBUG "%s: " format, MY_NAME , ## arg);	\
} while (0)
#define err(format, arg...)						\
	printk(KERN_ERR "%s: " format, MY_NAME , ## arg)
#define info(format, arg...)						\
	printk(KERN_INFO "%s: " format, MY_NAME , ## arg)
#define warn(format, arg...)						\
	printk(KERN_WARNING "%s: " format, MY_NAME , ## arg)

#define ctrl_dbg(ctrl, format, arg...)					\
	do {								\
		if (pciehp_debug)					\
			dev_printk(KERN_DEBUG, &ctrl->pcie->device,	\
					format, ## arg);		\
	} while (0)
#define ctrl_err(ctrl, format, arg...)					\
	dev_err(&ctrl->pcie->device, format, ## arg)
#define ctrl_info(ctrl, format, arg...)					\
	dev_info(&ctrl->pcie->device, format, ## arg)
#define ctrl_warn(ctrl, format, arg...)					\
	dev_warn(&ctrl->pcie->device, format, ## arg)

#define SLOT_NAME_SIZE 10
struct slot {
	u8 state;
	struct controller *ctrl;
	struct hotplug_slot *hotplug_slot;
	struct delayed_work work;	/* work for button event */
	struct mutex lock;
};

struct event_info {
	u32 event_type;
	struct slot *p_slot;
	struct work_struct work;
};

struct controller {
	struct mutex ctrl_lock;		/* controller lock */
	struct pcie_device *pcie;	/* PCI Express port service */
	struct slot *slot;
	wait_queue_head_t queue;	/* sleep & wake process */
	u32 slot_cap;
	struct timer_list poll_timer;
	unsigned int cmd_busy:1;
	unsigned int no_cmd_complete:1;
	unsigned int link_active_reporting:1;
	unsigned int notification_enabled:1;
	unsigned int power_fault_detected;
};

#define INT_BUTTON_IGNORE		0
#define INT_PRESENCE_ON			1
#define INT_PRESENCE_OFF		2
#define INT_SWITCH_CLOSE		3
#define INT_SWITCH_OPEN			4
#define INT_POWER_FAULT			5
#define INT_POWER_FAULT_CLEAR		6
#define INT_BUTTON_PRESS		7
#define INT_BUTTON_RELEASE		8
#define INT_BUTTON_CANCEL		9

#define STATIC_STATE			0
#define BLINKINGON_STATE		1
#define BLINKINGOFF_STATE		2
#define POWERON_STATE			3
#define POWEROFF_STATE			4

#define ATTN_BUTTN(ctrl)	((ctrl)->slot_cap & PCI_EXP_SLTCAP_ABP)
#define POWER_CTRL(ctrl)	((ctrl)->slot_cap & PCI_EXP_SLTCAP_PCP)
#define MRL_SENS(ctrl)		((ctrl)->slot_cap & PCI_EXP_SLTCAP_MRLSP)
#define ATTN_LED(ctrl)		((ctrl)->slot_cap & PCI_EXP_SLTCAP_AIP)
#define PWR_LED(ctrl)		((ctrl)->slot_cap & PCI_EXP_SLTCAP_PIP)
#define HP_SUPR_RM(ctrl)	((ctrl)->slot_cap & PCI_EXP_SLTCAP_HPS)
#define EMI(ctrl)		((ctrl)->slot_cap & PCI_EXP_SLTCAP_EIP)
#define NO_CMD_CMPL(ctrl)	((ctrl)->slot_cap & PCI_EXP_SLTCAP_NCCS)
#define PSN(ctrl)		((ctrl)->slot_cap >> 19)

extern int pciehp_sysfs_enable_slot(struct slot *slot);
extern int pciehp_sysfs_disable_slot(struct slot *slot);
extern u8 pciehp_handle_attention_button(struct slot *p_slot);
extern u8 pciehp_handle_switch_change(struct slot *p_slot);
extern u8 pciehp_handle_presence_change(struct slot *p_slot);
extern u8 pciehp_handle_power_fault(struct slot *p_slot);
extern int pciehp_configure_device(struct slot *p_slot);
extern int pciehp_unconfigure_device(struct slot *p_slot);
extern void pciehp_queue_pushbutton_work(struct work_struct *work);
struct controller *pcie_init(struct pcie_device *dev);
int pcie_init_notification(struct controller *ctrl);
int pciehp_enable_slot(struct slot *p_slot);
int pciehp_disable_slot(struct slot *p_slot);
int pcie_enable_notification(struct controller *ctrl);
int pciehp_power_on_slot(struct slot *slot);
int pciehp_power_off_slot(struct slot *slot);
int pciehp_get_power_status(struct slot *slot, u8 *status);
int pciehp_get_attention_status(struct slot *slot, u8 *status);

int pciehp_set_attention_status(struct slot *slot, u8 status);
int pciehp_get_latch_status(struct slot *slot, u8 *status);
int pciehp_get_adapter_status(struct slot *slot, u8 *status);
int pciehp_get_max_link_speed(struct slot *slot, enum pci_bus_speed *speed);
int pciehp_get_max_link_width(struct slot *slot, enum pcie_link_width *val);
int pciehp_get_cur_link_speed(struct slot *slot, enum pci_bus_speed *speed);
int pciehp_get_cur_link_width(struct slot *slot, enum pcie_link_width *val);
int pciehp_query_power_fault(struct slot *slot);
void pciehp_green_led_on(struct slot *slot);
void pciehp_green_led_off(struct slot *slot);
void pciehp_green_led_blink(struct slot *slot);
int pciehp_check_link_status(struct controller *ctrl);
void pciehp_release_ctrl(struct controller *ctrl);

static inline const char *slot_name(struct slot *slot)
{
	return hotplug_slot_name(slot->hotplug_slot);
}

#ifdef CONFIG_ACPI
#include <acpi/acpi.h>
#include <acpi/acpi_bus.h>
#include <linux/pci-acpi.h>

extern void __init pciehp_acpi_slot_detection_init(void);
extern int pciehp_acpi_slot_detection_check(struct pci_dev *dev);

static inline void pciehp_firmware_init(void)
{
	pciehp_acpi_slot_detection_init();
}

static inline int pciehp_get_hp_hw_control_from_firmware(struct pci_dev *dev)
{
	int retval;
	u32 flags = (OSC_PCI_EXPRESS_NATIVE_HP_CONTROL |
		     OSC_PCI_EXPRESS_CAP_STRUCTURE_CONTROL);
	retval = acpi_get_hp_hw_control_from_firmware(dev, flags);
	if (retval)
		return retval;
	return pciehp_acpi_slot_detection_check(dev);
}
#else
#define pciehp_firmware_init()				do {} while (0)
#define pciehp_get_hp_hw_control_from_firmware(dev) 	0
#endif 				/* CONFIG_ACPI */
#endif				/* _PCIEHP_H */
