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

  fp_trig.c: floating-point math routines for the Linux-m68k
  floating point emulator.

  Copyright (c) 1998-1999 David Huggins-Daines / Roman Zippel.

  I hereby give permission, free of charge, to copy, modify, and
  redistribute this software, in source or binary form, provided that
  the above copyright notice and the following disclaimer are included
  in all such copies.

  THIS SOFTWARE IS PROVIDED "AS IS", WITH ABSOLUTELY NO WARRANTY, REAL
  OR IMPLIED.

*/

#include "fp_emu.h"
#include "fp_trig.h"

struct fp_ext *
fp_fsin(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsin\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_fcos(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fcos\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_ftan(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("ftan\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_fasin(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fasin\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_facos(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("facos\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_fatan(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fatan\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_fsinh(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsinh\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_fcosh(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fcosh\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_ftanh(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("ftanh\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_fatanh(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fatanh\n");

	fp_monadic_check(dest, src);

	return dest;
}

struct fp_ext *
fp_fsincos0(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsincos0\n");

	return dest;
}

struct fp_ext *
fp_fsincos1(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsincos1\n");

	return dest;
}

struct fp_ext *
fp_fsincos2(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsincos2\n");

	return dest;
}

struct fp_ext *
fp_fsincos3(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsincos3\n");

	return dest;
}

struct fp_ext *
fp_fsincos4(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsincos4\n");

	return dest;
}

struct fp_ext *
fp_fsincos5(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsincos5\n");

	return dest;
}

struct fp_ext *
fp_fsincos6(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsincos6\n");

	return dest;
}

struct fp_ext *
fp_fsincos7(struct fp_ext *dest, struct fp_ext *src)
{
	uprint("fsincos7\n");

	return dest;
}
