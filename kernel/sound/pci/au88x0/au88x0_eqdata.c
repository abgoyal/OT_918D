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

/* Data structs */

static u16 asEqCoefsZeros[50] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
};

static u16 asEqCoefsPipes[64] = {
	0x0000, 0x0000,
	0x0000, 0x0666, 0x0000, 0x0000, 0x0666,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0666, 0x0000, 0x0000, 0x0666,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0666, 0x0000, 0x0000, 0x0666,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0666, 0x0000, 0x0000, 0x0666,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0666, 0x0000, 0x0000, 0x066a,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,

	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000
};

/* More coef sets can be found in the win2k "inf" file. */
static auxxEqCoeffSet_t asEqCoefsNormal = {
	.LeftCoefs = {
		      0x7e60, 0xc19e, 0x0001, 0x0002, 0x0001,
		      0x7fa0, 0xc05f, 0x004f, 0x0000, 0xffb1,
		      0x7f3f, 0xc0bc, 0x00c2, 0x0000, 0xff3e,
		      0x7e78, 0xc177, 0x011f, 0x0000, 0xfee1,
		      0x7cd6, 0xc2e5, 0x025c, 0x0000, 0xfda4,
		      0x7949, 0xc5aa, 0x0467, 0x0000, 0xfb99,
		      0x7120, 0xcadf, 0x0864, 0x0000, 0xf79c,
		      0x5d33, 0xd430, 0x0f7e, 0x0000, 0xf082,
		      0x2beb, 0xe3ca, 0x1bd3, 0x0000, 0xe42d,
		      0xd740, 0xf01d, 0x2ac5, 0x0000, 0xd53b},

	.RightCoefs = {
		       0x7e60, 0xc19e, 0x0001, 0x0002, 0x0001,
		       0x7fa0, 0xc05f, 0x004f, 0x0000, 0xffb1,
		       0x7f3f, 0xc0bc, 0x00c2, 0x0000, 0xff3e,
		       0x7e78, 0xc177, 0x011f, 0x0000, 0xfee1,
		       0x7cd6, 0xc2e5, 0x025c, 0x0000, 0xfda4,
		       0x7949, 0xc5aa, 0x0467, 0x0000, 0xfb99,
		       0x7120, 0xcadf, 0x0864, 0x0000, 0xf79c,
		       0x5d33, 0xd430, 0x0f7e, 0x0000, 0xf082,
		       0x2beb, 0xe3ca, 0x1bd3, 0x0000, 0xe42d,
		       0xd740, 0xf01d, 0x2ac5, 0x0000, 0xd53b},

	.LeftGains = {
		      0x3e96, 0x3e96, 0x3e96, 0x3e96, 0x3e96,
		      0x3e96, 0x3e96, 0x3e96, 0x3e96, 0x3e96},
	.RightGains = {
		       0x3e96, 0x3e96, 0x3e96, 0x3e96, 0x3e96,
		       0x3e96, 0x3e96, 0x3e96, 0x3e96, 0x3e96}
};

static u16 eq_gains_normal[20] = {
	0x3e96, 0x3e96, 0x3e96, 0x3e96, 0x3e96,
	0x3e96, 0x3e96, 0x3e96, 0x3e96, 0x3e96,
	0x3e96, 0x3e96, 0x3e96, 0x3e96, 0x3e96,
	0x3e96, 0x3e96, 0x3e96, 0x3e96, 0x3e96
};

/* _rodatab60 */
static u16 eq_gains_zero[10] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};

/* _rodatab7c:  ProgramPipe */
static u16 eq_gains_current[12] = {
	0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff, 0x7fff,
	0x7fff,
	0x7fff, 0x7fff, 0x7fff
};

/* _rodatab78 */
static u16 eq_states_zero[2] = { 0x0000, 0x0000 };

static u16 asEqOutStateZeros[48] = {
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000
};

/*_rodataba0:*/
static u16 eq_levels[64] = {
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000,
	0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000
};
