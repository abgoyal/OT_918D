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
 *  linux/fs/hfs/string.c
 *
 * Copyright (C) 1995-1997  Paul H. Hargrove
 * (C) 2003 Ardis Technologies <roman@ardistech.com>
 * This file may be distributed under the terms of the GNU General Public License.
 *
 * This file contains the string comparison function for the
 * Macintosh character set.
 *
 * The code in this file is derived from code which is copyright
 * 1986, 1989, 1990 by Abacus Research and Development, Inc. (ARDI)
 * It is used here by the permission of ARDI's president Cliff Matthews.
 */

#include "hfs_fs.h"
#include <linux/dcache.h>

/*================ File-local variables ================*/

/*
 * unsigned char caseorder[]
 *
 * Defines the lexical ordering of characters on the Macintosh
 *
 * Composition of the 'casefold' and 'order' tables from ARDI's code
 * with the entry for 0x20 changed to match that for 0xCA to remove
 * special case for those two characters.
 */
static unsigned char caseorder[256] = {
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x22,0x23,0x28,0x29,0x2A,0x2B,0x2C,0x2F,0x30,0x31,0x32,0x33,0x34,0x35,0x36,
	0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,0x40,0x41,0x42,0x43,0x44,0x45,0x46,
	0x47,0x48,0x57,0x59,0x5D,0x5F,0x66,0x68,0x6A,0x6C,0x72,0x74,0x76,0x78,0x7A,0x7E,
	0x8C,0x8E,0x90,0x92,0x95,0x97,0x9E,0xA0,0xA2,0xA4,0xA7,0xA9,0xAA,0xAB,0xAC,0xAD,
	0x4E,0x48,0x57,0x59,0x5D,0x5F,0x66,0x68,0x6A,0x6C,0x72,0x74,0x76,0x78,0x7A,0x7E,
	0x8C,0x8E,0x90,0x92,0x95,0x97,0x9E,0xA0,0xA2,0xA4,0xA7,0xAF,0xB0,0xB1,0xB2,0xB3,
	0x4A,0x4C,0x5A,0x60,0x7B,0x7F,0x98,0x4F,0x49,0x51,0x4A,0x4B,0x4C,0x5A,0x60,0x63,
	0x64,0x65,0x6E,0x6F,0x70,0x71,0x7B,0x84,0x85,0x86,0x7F,0x80,0x9A,0x9B,0x9C,0x98,
	0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0x94,0xBB,0xBC,0xBD,0xBE,0xBF,0xC0,0x4D,0x81,
	0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0x55,0x8A,0xCC,0x4D,0x81,
	0xCD,0xCE,0xCF,0xD0,0xD1,0xD2,0xD3,0x26,0x27,0xD4,0x20,0x49,0x4B,0x80,0x82,0x82,
	0xD5,0xD6,0x24,0x25,0x2D,0x2E,0xD7,0xD8,0xA6,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
	0xE0,0xE1,0xE2,0xE3,0xE4,0xE5,0xE6,0xE7,0xE8,0xE9,0xEA,0xEB,0xEC,0xED,0xEE,0xEF,
	0xF0,0xF1,0xF2,0xF3,0xF4,0xF5,0xF6,0xF7,0xF8,0xF9,0xFA,0xFB,0xFC,0xFD,0xFE,0xFF
};

/*================ Global functions ================*/

/*
 * Hash a string to an integer in a case-independent way
 */
int hfs_hash_dentry(struct dentry *dentry, struct qstr *this)
{
	const unsigned char *name = this->name;
	unsigned int hash, len = this->len;

	if (len > HFS_NAMELEN)
		len = HFS_NAMELEN;

	hash = init_name_hash();
	for (; len; len--)
		hash = partial_name_hash(caseorder[*name++], hash);
	this->hash = end_name_hash(hash);
	return 0;
}

/*
 * Compare two strings in the HFS filename character ordering
 * Returns positive, negative, or zero, not just 0 or (+/-)1
 *
 * Equivalent to ARDI's call:
 *	ROMlib_RelString(s1+1, s2+1, true, false, (s1[0]<<16) | s2[0])
 */
int hfs_strcmp(const unsigned char *s1, unsigned int len1,
	       const unsigned char *s2, unsigned int len2)
{
	int len, tmp;

	len = (len1 > len2) ? len2 : len1;

	while (len--) {
		tmp = (int)caseorder[*(s1++)] - (int)caseorder[*(s2++)];
		if (tmp)
			return tmp;
	}
	return len1 - len2;
}

/*
 * Test for equality of two strings in the HFS filename character ordering.
 * return 1 on failure and 0 on success
 */
int hfs_compare_dentry(struct dentry *dentry, struct qstr *s1, struct qstr *s2)
{
	const unsigned char *n1, *n2;
	int len;

	len = s1->len;
	if (len >= HFS_NAMELEN) {
		if (s2->len < HFS_NAMELEN)
			return 1;
		len = HFS_NAMELEN;
	} else if (len != s2->len)
		return 1;

	n1 = s1->name;
	n2 = s2->name;
	while (len--) {
		if (caseorder[*n1++] != caseorder[*n2++])
			return 1;
	}
	return 0;
}
