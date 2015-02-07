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
 * Copyright (c) 1996, 2003 VIA Networking Technologies, Inc.
 * All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 *
 * File: tkip.c
 *
 * Purpose: Implement functions for 802.11i TKIP
 *
 * Author: Jerry Chen
 *
 * Date: Mar. 11, 2003
 *
 * Functions:
 *      TKIPvMixKey - Get TKIP RC4 Key from TK,TA, and TSC
 *
 * Revision History:
 *
 */

#include "tmacro.h"
#include "tkip.h"

/*---------------------  Static Definitions -------------------------*/

/*---------------------  Static Classes  ----------------------------*/

/*---------------------  Static Variables  --------------------------*/

/*---------------------  Static Functions  --------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Static Definitions -------------------------*/

/*---------------------  Static Classes  ----------------------------*/

/*---------------------  Static Variables  --------------------------*/

/* The Sbox is reduced to 2 16-bit wide tables, each with 256 entries. */
/* The 2nd table is the same as the 1st but with the upper and lower   */
/* bytes swapped. To allow an endian tolerant implementation, the byte */
/* halves have been expressed independently here.                      */
const BYTE TKIP_Sbox_Lower[256] = {
    0xA5,0x84,0x99,0x8D,0x0D,0xBD,0xB1,0x54,
    0x50,0x03,0xA9,0x7D,0x19,0x62,0xE6,0x9A,
    0x45,0x9D,0x40,0x87,0x15,0xEB,0xC9,0x0B,
    0xEC,0x67,0xFD,0xEA,0xBF,0xF7,0x96,0x5B,
    0xC2,0x1C,0xAE,0x6A,0x5A,0x41,0x02,0x4F,
    0x5C,0xF4,0x34,0x08,0x93,0x73,0x53,0x3F,
    0x0C,0x52,0x65,0x5E,0x28,0xA1,0x0F,0xB5,
    0x09,0x36,0x9B,0x3D,0x26,0x69,0xCD,0x9F,
    0x1B,0x9E,0x74,0x2E,0x2D,0xB2,0xEE,0xFB,
    0xF6,0x4D,0x61,0xCE,0x7B,0x3E,0x71,0x97,
    0xF5,0x68,0x00,0x2C,0x60,0x1F,0xC8,0xED,
    0xBE,0x46,0xD9,0x4B,0xDE,0xD4,0xE8,0x4A,
    0x6B,0x2A,0xE5,0x16,0xC5,0xD7,0x55,0x94,
    0xCF,0x10,0x06,0x81,0xF0,0x44,0xBA,0xE3,
    0xF3,0xFE,0xC0,0x8A,0xAD,0xBC,0x48,0x04,
    0xDF,0xC1,0x75,0x63,0x30,0x1A,0x0E,0x6D,
    0x4C,0x14,0x35,0x2F,0xE1,0xA2,0xCC,0x39,
    0x57,0xF2,0x82,0x47,0xAC,0xE7,0x2B,0x95,
    0xA0,0x98,0xD1,0x7F,0x66,0x7E,0xAB,0x83,
    0xCA,0x29,0xD3,0x3C,0x79,0xE2,0x1D,0x76,
    0x3B,0x56,0x4E,0x1E,0xDB,0x0A,0x6C,0xE4,
    0x5D,0x6E,0xEF,0xA6,0xA8,0xA4,0x37,0x8B,
    0x32,0x43,0x59,0xB7,0x8C,0x64,0xD2,0xE0,
    0xB4,0xFA,0x07,0x25,0xAF,0x8E,0xE9,0x18,
    0xD5,0x88,0x6F,0x72,0x24,0xF1,0xC7,0x51,
    0x23,0x7C,0x9C,0x21,0xDD,0xDC,0x86,0x85,
    0x90,0x42,0xC4,0xAA,0xD8,0x05,0x01,0x12,
    0xA3,0x5F,0xF9,0xD0,0x91,0x58,0x27,0xB9,
    0x38,0x13,0xB3,0x33,0xBB,0x70,0x89,0xA7,
    0xB6,0x22,0x92,0x20,0x49,0xFF,0x78,0x7A,
    0x8F,0xF8,0x80,0x17,0xDA,0x31,0xC6,0xB8,
    0xC3,0xB0,0x77,0x11,0xCB,0xFC,0xD6,0x3A
};

const BYTE TKIP_Sbox_Upper[256] = {
    0xC6,0xF8,0xEE,0xF6,0xFF,0xD6,0xDE,0x91,
    0x60,0x02,0xCE,0x56,0xE7,0xB5,0x4D,0xEC,
    0x8F,0x1F,0x89,0xFA,0xEF,0xB2,0x8E,0xFB,
    0x41,0xB3,0x5F,0x45,0x23,0x53,0xE4,0x9B,
    0x75,0xE1,0x3D,0x4C,0x6C,0x7E,0xF5,0x83,
    0x68,0x51,0xD1,0xF9,0xE2,0xAB,0x62,0x2A,
    0x08,0x95,0x46,0x9D,0x30,0x37,0x0A,0x2F,
    0x0E,0x24,0x1B,0xDF,0xCD,0x4E,0x7F,0xEA,
    0x12,0x1D,0x58,0x34,0x36,0xDC,0xB4,0x5B,
    0xA4,0x76,0xB7,0x7D,0x52,0xDD,0x5E,0x13,
    0xA6,0xB9,0x00,0xC1,0x40,0xE3,0x79,0xB6,
    0xD4,0x8D,0x67,0x72,0x94,0x98,0xB0,0x85,
    0xBB,0xC5,0x4F,0xED,0x86,0x9A,0x66,0x11,
    0x8A,0xE9,0x04,0xFE,0xA0,0x78,0x25,0x4B,
    0xA2,0x5D,0x80,0x05,0x3F,0x21,0x70,0xF1,
    0x63,0x77,0xAF,0x42,0x20,0xE5,0xFD,0xBF,
    0x81,0x18,0x26,0xC3,0xBE,0x35,0x88,0x2E,
    0x93,0x55,0xFC,0x7A,0xC8,0xBA,0x32,0xE6,
    0xC0,0x19,0x9E,0xA3,0x44,0x54,0x3B,0x0B,
    0x8C,0xC7,0x6B,0x28,0xA7,0xBC,0x16,0xAD,
    0xDB,0x64,0x74,0x14,0x92,0x0C,0x48,0xB8,
    0x9F,0xBD,0x43,0xC4,0x39,0x31,0xD3,0xF2,
    0xD5,0x8B,0x6E,0xDA,0x01,0xB1,0x9C,0x49,
    0xD8,0xAC,0xF3,0xCF,0xCA,0xF4,0x47,0x10,
    0x6F,0xF0,0x4A,0x5C,0x38,0x57,0x73,0x97,
    0xCB,0xA1,0xE8,0x3E,0x96,0x61,0x0D,0x0F,
    0xE0,0x7C,0x71,0xCC,0x90,0x06,0xF7,0x1C,
    0xC2,0x6A,0xAE,0x69,0x17,0x99,0x3A,0x27,
    0xD9,0xEB,0x2B,0x22,0xD2,0xA9,0x07,0x33,
    0x2D,0x3C,0x15,0xC9,0x87,0xAA,0x50,0xA5,
    0x03,0x59,0x09,0x1A,0x65,0xD7,0x84,0xD0,
    0x82,0x29,0x5A,0x1E,0x7B,0xA8,0x6D,0x2C
};


//STKIPKeyManagement  sTKIPKeyTable[MAX_TKIP_KEY];

/*---------------------  Static Functions  --------------------------*/
unsigned int tkip_sbox(unsigned int index);
unsigned int rotr1(unsigned int a);

/*---------------------  Export Variables  --------------------------*/

/************************************************************/
/* tkip_sbox()                                              */
/* Returns a 16 bit value from a 64K entry table. The Table */
/* is synthesized from two 256 entry byte wide tables.      */
/************************************************************/
unsigned int tkip_sbox(unsigned int index)
{
    unsigned int index_low;
    unsigned int index_high;
    unsigned int left, right;

    index_low = (index % 256);
    index_high = ((index >> 8) % 256);

    left = TKIP_Sbox_Lower[index_low] + (TKIP_Sbox_Upper[index_low] * 256);
    right = TKIP_Sbox_Upper[index_high] + (TKIP_Sbox_Lower[index_high] * 256);

    return (left ^ right);
};


unsigned int rotr1(unsigned int a)
{
    unsigned int b;

    if ((a & 0x01) == 0x01) {
        b = (a >> 1) | 0x8000;
    } else {
        b = (a >> 1) & 0x7fff;
    }
    b = b % 65536;
    return b;
}


/*
 * Description: Caculate RC4Key fom TK, TA, and TSC
 *
 * Parameters:
 *  In:
 *      pbyTKey         - TKey
 *      pbyTA           - TA
 *      dwTSC           - TSC
 *  Out:
 *      pbyRC4Key       - RC4Key
 *
 * Return Value: none
 *
 */
void TKIPvMixKey(
    PBYTE   pbyTKey,
    PBYTE   pbyTA,
    WORD    wTSC15_0,
    DWORD   dwTSC47_16,
    PBYTE   pbyRC4Key
    )
{
    unsigned int p1k[5];
//    unsigned int ttak0, ttak1, ttak2, ttak3, ttak4;
    unsigned int tsc0, tsc1, tsc2;
    unsigned int ppk0, ppk1, ppk2, ppk3, ppk4, ppk5;
    unsigned long int pnl,pnh;

    int i, j;

    pnl = wTSC15_0;
    pnh = dwTSC47_16;

    tsc0 = (unsigned int)((pnh >> 16) % 65536); /* msb */
    tsc1 = (unsigned int)(pnh % 65536);
    tsc2 = (unsigned int)(pnl % 65536); /* lsb */

    /* Phase 1, step 1 */
    p1k[0] = tsc1;
    p1k[1] = tsc0;
    p1k[2] = (unsigned int)(pbyTA[0] + (pbyTA[1]*256));
    p1k[3] = (unsigned int)(pbyTA[2] + (pbyTA[3]*256));
    p1k[4] = (unsigned int)(pbyTA[4] + (pbyTA[5]*256));

    /* Phase 1, step 2 */
    for (i=0; i<8; i++) {
        j = 2*(i & 1);
        p1k[0] = (p1k[0] + tkip_sbox( (p1k[4] ^ ((256*pbyTKey[1+j]) + pbyTKey[j])) % 65536 )) % 65536;
        p1k[1] = (p1k[1] + tkip_sbox( (p1k[0] ^ ((256*pbyTKey[5+j]) + pbyTKey[4+j])) % 65536 )) % 65536;
        p1k[2] = (p1k[2] + tkip_sbox( (p1k[1] ^ ((256*pbyTKey[9+j]) + pbyTKey[8+j])) % 65536 )) % 65536;
        p1k[3] = (p1k[3] + tkip_sbox( (p1k[2] ^ ((256*pbyTKey[13+j]) + pbyTKey[12+j])) % 65536 )) % 65536;
        p1k[4] = (p1k[4] + tkip_sbox( (p1k[3] ^ (((256*pbyTKey[1+j]) + pbyTKey[j]))) % 65536 )) % 65536;
        p1k[4] = (p1k[4] + i) % 65536;
    }
    /* Phase 2, Step 1 */
    ppk0 = p1k[0];
    ppk1 = p1k[1];
    ppk2 = p1k[2];
    ppk3 = p1k[3];
    ppk4 = p1k[4];
    ppk5 = (p1k[4] + tsc2) % 65536;

    /* Phase2, Step 2 */
    ppk0 = ppk0 + tkip_sbox( (ppk5 ^ ((256*pbyTKey[1]) + pbyTKey[0])) % 65536);
    ppk1 = ppk1 + tkip_sbox( (ppk0 ^ ((256*pbyTKey[3]) + pbyTKey[2])) % 65536);
    ppk2 = ppk2 + tkip_sbox( (ppk1 ^ ((256*pbyTKey[5]) + pbyTKey[4])) % 65536);
    ppk3 = ppk3 + tkip_sbox( (ppk2 ^ ((256*pbyTKey[7]) + pbyTKey[6])) % 65536);
    ppk4 = ppk4 + tkip_sbox( (ppk3 ^ ((256*pbyTKey[9]) + pbyTKey[8])) % 65536);
    ppk5 = ppk5 + tkip_sbox( (ppk4 ^ ((256*pbyTKey[11]) + pbyTKey[10])) % 65536);

    ppk0 = ppk0 + rotr1(ppk5 ^ ((256*pbyTKey[13]) + pbyTKey[12]));
    ppk1 = ppk1 + rotr1(ppk0 ^ ((256*pbyTKey[15]) + pbyTKey[14]));
    ppk2 = ppk2 + rotr1(ppk1);
    ppk3 = ppk3 + rotr1(ppk2);
    ppk4 = ppk4 + rotr1(ppk3);
    ppk5 = ppk5 + rotr1(ppk4);

    /* Phase 2, Step 3 */
    pbyRC4Key[0] = (tsc2 >> 8) % 256;
    pbyRC4Key[1] = (((tsc2 >> 8) % 256) | 0x20) & 0x7f;
    pbyRC4Key[2] = tsc2 % 256;
    pbyRC4Key[3] = ((ppk5 ^ ((256*pbyTKey[1]) + pbyTKey[0])) >> 1) % 256;

    pbyRC4Key[4] = ppk0 % 256;
    pbyRC4Key[5] = (ppk0 >> 8) % 256;

    pbyRC4Key[6] = ppk1 % 256;
    pbyRC4Key[7] = (ppk1 >> 8) % 256;

    pbyRC4Key[8] = ppk2 % 256;
    pbyRC4Key[9] = (ppk2 >> 8) % 256;

    pbyRC4Key[10] = ppk3 % 256;
    pbyRC4Key[11] = (ppk3 >> 8) % 256;

    pbyRC4Key[12] = ppk4 % 256;
    pbyRC4Key[13] = (ppk4 >> 8) % 256;

    pbyRC4Key[14] = ppk5 % 256;
    pbyRC4Key[15] = (ppk5 >> 8) % 256;
}
