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
 * File: srom.h
 *
 * Purpose: Implement functions to access eeprom
 *
 * Author: Jerry Chen
 *
 * Date: Jan 29, 2003
 *
 */

#ifndef __SROM_H__
#define __SROM_H__

#include "ttype.h"

/*---------------------  Export Definitions -------------------------*/

#define EEP_MAX_CONTEXT_SIZE    256

#define CB_EEPROM_READBYTE_WAIT 900     //us

#define W_MAX_I2CRETRY          0x0fff

//
// Contents in the EEPROM
//
#define EEP_OFS_PAR         0x00        // physical address
#define EEP_OFS_ANTENNA     0x16
#define EEP_OFS_RADIOCTL    0x17
#define EEP_OFS_RFTYPE      0x1B        // for select RF
#define EEP_OFS_MINCHANNEL  0x1C        // Min Channel #
#define EEP_OFS_MAXCHANNEL  0x1D        // Max Channel #
#define EEP_OFS_SIGNATURE   0x1E        //
#define EEP_OFS_ZONETYPE    0x1F        //
#define EEP_OFS_RFTABLE     0x20        // RF POWER TABLE
#define EEP_OFS_PWR_CCK     0x20
#define EEP_OFS_SETPT_CCK   0x21
#define EEP_OFS_PWR_OFDMG   0x23
#define EEP_OFS_SETPT_OFDMG 0x24
#define EEP_OFS_PWR_FORMULA_OST  0x26   //
#define EEP_OFS_MAJOR_VER 0x2E
#define EEP_OFS_MINOR_VER 0x2F
#define EEP_OFS_CCK_PWR_TBL     0x30
#define EEP_OFS_CCK_PWR_dBm     0x3F
#define EEP_OFS_OFDM_PWR_TBL    0x40
#define EEP_OFS_OFDM_PWR_dBm    0x4F
//{{ RobertYu: 20041124
#define EEP_OFS_SETPT_OFDMA         0x4E
#define EEP_OFS_OFDMA_PWR_TBL       0x50
//}}
#define EEP_OFS_OFDMA_PWR_dBm       0xD2


//----------need to remove --------------------
#define EEP_OFS_BBTAB_LEN   0x70        // BB Table Length
#define EEP_OFS_BBTAB_ADR   0x71        // BB Table Offset
#define EEP_OFS_CHECKSUM    0xFF        // reserved area for baseband 28h ~ 78h

#define EEP_I2C_DEV_ID      0x50        // EEPROM device address on the I2C bus


//
// Bits in EEP_OFS_ANTENNA
//
#define EEP_ANTENNA_MAIN    0x01
#define EEP_ANTENNA_AUX     0x02
#define EEP_ANTINV          0x04

//
// Bits in EEP_OFS_RADIOCTL
//
#define EEP_RADIOCTL_ENABLE 0x80
#define EEP_RADIOCTL_INV    0x01

/*---------------------  Export Types  ------------------------------*/

// AT24C02 eeprom contents
//      2048 bits = 256 bytes = 128 words
//
typedef struct tagSSromReg {
    BYTE    abyPAR[6];                  // 0x00 (WORD)

    WORD    wSUB_VID;                   // 0x03 (WORD)
    WORD    wSUB_SID;

    BYTE    byBCFG0;                    // 0x05 (WORD)
    BYTE    byBCFG1;

    BYTE    byFCR0;                     // 0x06 (WORD)
    BYTE    byFCR1;
    BYTE    byPMC0;                     // 0x07 (WORD)
    BYTE    byPMC1;
    BYTE    byMAXLAT;                   // 0x08 (WORD)
    BYTE    byMINGNT;
    BYTE    byCFG0;                     // 0x09 (WORD)
    BYTE    byCFG1;
    WORD    wCISPTR;                    // 0x0A (WORD)
    WORD    wRsv0;                      // 0x0B (WORD)
    WORD    wRsv1;                      // 0x0C (WORD)
    BYTE    byBBPAIR;                   // 0x0D (WORD)
    BYTE    byRFTYPE;
    BYTE    byMinChannel;               // 0x0E (WORD)
    BYTE    byMaxChannel;
    BYTE    bySignature;                // 0x0F (WORD)
    BYTE    byCheckSum;

    BYTE    abyReserved0[96];           // 0x10 (WORD)
    BYTE    abyCIS[128];                // 0x80 (WORD)
} SSromReg, *PSSromReg;

/*---------------------  Export Macros ------------------------------*/

/*---------------------  Export Classes  ----------------------------*/

/*---------------------  Export Variables  --------------------------*/

/*---------------------  Export Functions  --------------------------*/

BYTE SROMbyReadEmbedded(DWORD_PTR dwIoBase, BYTE byContntOffset);
BOOL SROMbWriteEmbedded(DWORD_PTR dwIoBase, BYTE byContntOffset, BYTE byData);

void SROMvRegBitsOn(DWORD_PTR dwIoBase, BYTE byContntOffset, BYTE byBits);
void SROMvRegBitsOff(DWORD_PTR dwIoBase, BYTE byContntOffset, BYTE byBits);

BOOL SROMbIsRegBitsOn(DWORD_PTR dwIoBase, BYTE byContntOffset, BYTE byTestBits);
BOOL SROMbIsRegBitsOff(DWORD_PTR dwIoBase, BYTE byContntOffset, BYTE byTestBits);

void SROMvReadAllContents(DWORD_PTR dwIoBase, PBYTE pbyEepromRegs);
void SROMvWriteAllContents(DWORD_PTR dwIoBase, PBYTE pbyEepromRegs);

void SROMvReadEtherAddress(DWORD_PTR dwIoBase, PBYTE pbyEtherAddress);
void SROMvWriteEtherAddress(DWORD_PTR dwIoBase, PBYTE pbyEtherAddress);

void SROMvReadSubSysVenId(DWORD_PTR dwIoBase, PDWORD pdwSubSysVenId);

BOOL SROMbAutoLoad (DWORD_PTR dwIoBase);

#endif // __EEPROM_H__