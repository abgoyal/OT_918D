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

/* Blackfin on-chip ROM API
 *
 * Copyright 2008 Analog Devices Inc.
 *
 * Licensed under the GPL-2 or later.
 */

#ifndef __BFROM_H__
#define __BFROM_H__

#include <linux/types.h>

/* Possible syscontrol action flags */
#define SYSCTRL_READ        0x00000000    /* read registers */
#define SYSCTRL_WRITE       0x00000001    /* write registers */
#define SYSCTRL_SYSRESET    0x00000002    /* perform system reset */
#define SYSCTRL_CORERESET   0x00000004    /* perform core reset */
#define SYSCTRL_SOFTRESET   0x00000006    /* perform core and system reset */
#define SYSCTRL_VRCTL       0x00000010    /* read/write VR_CTL register */
#define SYSCTRL_EXTVOLTAGE  0x00000020    /* VDDINT supplied externally */
#define SYSCTRL_INTVOLTAGE  0x00000000    /* VDDINT generated by on-chip regulator */
#define SYSCTRL_OTPVOLTAGE  0x00000040    /* For Factory Purposes Only */
#define SYSCTRL_PLLCTL      0x00000100    /* read/write PLL_CTL register */
#define SYSCTRL_PLLDIV      0x00000200    /* read/write PLL_DIV register */
#define SYSCTRL_LOCKCNT     0x00000400    /* read/write PLL_LOCKCNT register */
#define SYSCTRL_PLLSTAT     0x00000800    /* read/write PLL_STAT register */

typedef struct ADI_SYSCTRL_VALUES {
	uint16_t uwVrCtl;
	uint16_t uwPllCtl;
	uint16_t uwPllDiv;
	uint16_t uwPllLockCnt;
	uint16_t uwPllStat;
} ADI_SYSCTRL_VALUES;

static uint32_t (* const bfrom_SysControl)(uint32_t action_flags, ADI_SYSCTRL_VALUES *power_settings, void *reserved) = (void *)0xEF000038;

/* We need a dedicated function since we need to screw with the stack pointer
 * when resetting.  The on-chip ROM will save/restore registers on the stack
 * when doing a system reset, so the stack cannot be outside of the chip.
 */
__attribute__((__noreturn__))
static inline void bfrom_SoftReset(void *new_stack)
{
	while (1)
		/*
		 * We don't declare the SP as clobbered on purpose, since
		 * it confuses the heck out of the compiler, and this function
		 * never returns
		 */
		__asm__ __volatile__(
			"sp = %[stack];"
			"jump (%[bfrom_syscontrol]);"
			: : [bfrom_syscontrol] "p"(bfrom_SysControl),
				"q0"(SYSCTRL_SOFTRESET),
				"q1"(0),
				"q2"(NULL),
				[stack] "p"(new_stack)
		);
}

/* OTP Functions */
static uint32_t (* const bfrom_OtpCommand)(uint32_t command, uint32_t value) = (void *)0xEF000018;
static uint32_t (* const bfrom_OtpRead)(uint32_t page, uint32_t flags, uint64_t *page_content) = (void *)0xEF00001A;
static uint32_t (* const bfrom_OtpWrite)(uint32_t page, uint32_t flags, uint64_t *page_content) = (void *)0xEF00001C;

/* otp command: defines for "command" */
#define OTP_INIT                 0x00000001
#define OTP_CLOSE                0x00000002

/* otp read/write: defines for "flags" */
#define OTP_LOWER_HALF           0x00000000 /* select upper/lower 64-bit half (bit 0) */
#define OTP_UPPER_HALF           0x00000001
#define OTP_NO_ECC               0x00000010 /* do not use ECC */
#define OTP_LOCK                 0x00000020 /* sets page protection bit for page */
#define OTP_CHECK_FOR_PREV_WRITE 0x00000080

/* Return values for all functions */
#define OTP_SUCCESS          0x00000000
#define OTP_MASTER_ERROR     0x001
#define OTP_WRITE_ERROR      0x003
#define OTP_READ_ERROR       0x005
#define OTP_ACC_VIO_ERROR    0x009
#define OTP_DATA_MULT_ERROR  0x011
#define OTP_ECC_MULT_ERROR   0x021
#define OTP_PREV_WR_ERROR    0x041
#define OTP_DATA_SB_WARN     0x100
#define OTP_ECC_SB_WARN      0x200

#endif
