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
 * libgcc1 routines for Blackfin 5xx
 *
 * Copyright 2004-2009 Analog Devices Inc.
 *
 * Licensed under the ADI BSD license or the GPL-2 (or later)
 */

#ifdef CONFIG_ARITHMETIC_OPS_L1
.section .l1.text
#else
.text
#endif

.extern ___udivsi3;
.type ___udivsi3, STT_FUNC;
.globl	___umodsi3
.type ___umodsi3, STT_FUNC;
___umodsi3:

	CC=R0==0;
	IF CC JUMP .LRETURN_R0;		/* Return 0, if NR == 0 */
	CC= R1==0;
	IF CC JUMP .LRETURN_ZERO_VAL;	/* Return 0, if DR == 0 */
	CC=R0==R1;
	IF CC JUMP .LRETURN_ZERO_VAL;	/* Return 0, if NR == DR */
	CC = R1 == 1;
	IF CC JUMP .LRETURN_ZERO_VAL;	/* Return 0, if  DR == 1 */
	CC = R0<R1 (IU);
	IF CC JUMP .LRETURN_R0;		/* Return dividend (R0),IF NR<DR */

	[--SP] = (R7:6);		/* Push registers and */
	[--SP] = RETS;			/* Return address */
	R7 = R0;			/* Copy of R0 */
	R6 = R1;
	SP += -12;			/* Should always provide this space */
	CALL ___udivsi3;		/* Compute unsigned quotient using ___udiv32()*/
	SP += 12;
	R0 *= R6;			/* Quotient * divisor */
	R0 = R7 - R0;			/* Dividend - (quotient * divisor) */
	RETS = [SP++];			/* Pop return address */
	( R7:6) = [SP++];		/* And registers */
	RTS;				/* Return remainder */
.LRETURN_ZERO_VAL:
	R0 = 0;
.LRETURN_R0:
	RTS;

.size ___umodsi3, .-___umodsi3
