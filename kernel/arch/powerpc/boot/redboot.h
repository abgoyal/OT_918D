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

#ifndef _PPC_REDBOOT_H
#define _PPC_REDBOOT_H

//=========================================================================
// include/asm-ppc/redboot.h
//   Copyright (c) 2002, 2003 Gary Thomas (<gary@mlbassoc.com>
//   Copyright (c) 1997 Dan Malek (dmalek@jlc.net)

//
// Board specific details, as provided by RedBoot
//

/* A Board Information structure that is given to a program when
 * RedBoot starts it up.  Note: not all fields make sense for all
 * architectures and it's up to the platform specific code to fill
 * in the details.
 */
typedef struct bd_info {
    unsigned int   bi_tag;        /* Should be 0x42444944 "BDID" */
    unsigned int   bi_size;       /* Size of this structure */
    unsigned int   bi_revision;   /* revision of this structure */
    unsigned int   bi_bdate;      /* bootstrap date, i.e. 0x19971106 */
    unsigned int   bi_memstart;   /* Memory start address */
    unsigned int   bi_memsize;    /* Memory (end) size in bytes */
    unsigned int   bi_intfreq;    /* Internal Freq, in Hz */
    unsigned int   bi_busfreq;    /* Bus Freq, in Hz */
    unsigned int   bi_cpmfreq;    /* CPM Freq, in Hz */
    unsigned int   bi_brgfreq;    /* BRG Freq, in Hz */
    unsigned int   bi_vco;        /* VCO Out from PLL */
    unsigned int   bi_pci_freq;   /* PCI Freq, in Hz */
    unsigned int   bi_baudrate;   /* Default console baud rate */
    unsigned int   bi_immr;       /* IMMR when called from boot rom */
    unsigned char  bi_enetaddr[6];
    unsigned int   bi_flashbase;  /* Physical address of FLASH memory */
    unsigned int   bi_flashsize;  /* Length of FLASH memory */
    int            bi_flashwidth; /* Width (8,16,32,64) */
    unsigned char *bi_cmdline;    /* Pointer to command line */
    unsigned char  bi_esa[3][6];  /* Ethernet station addresses */
    unsigned int   bi_ramdisk_begin, bi_ramdisk_end;
    struct {                      /* Information about [main] video screen */
        short x_res;              /*   Horizontal resolution in pixels */
        short y_res;              /*   Vertical resolution in pixels */
        short bpp;                /*   Bits/pixel */
        short mode;               /*   Type of pixels (packed, indexed) */
        unsigned long fb;         /*   Pointer to frame buffer (pixel) memory */
    } bi_video;
    void         (*bi_cputc)(char);   /* Write a character to the RedBoot console */
    char         (*bi_cgetc)(void);   /* Read a character from the RedBoot console */
    int          (*bi_ctstc)(void);   /* Test for input on the RedBoot console */
} bd_t;

#define BI_REV 0x0102    /* Version 1.02 */

#define bi_pci_busfreq bi_pci_freq
#define bi_immr_base   bi_immr
#endif
