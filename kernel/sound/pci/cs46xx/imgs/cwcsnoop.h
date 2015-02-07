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

/* generated from cwcsnoop.osp DO NOT MODIFY */

#ifndef __HEADER_cwcsnoop_H__
#define __HEADER_cwcsnoop_H__

static struct dsp_symbol_entry cwcsnoop_symbols[] = {
  { 0x0500, "OVERLAYBEGINADDRESS",0x00 },
  { 0x0500, "OUTPUTSNOOP",0x03 },
  { 0x051f, "#CODE_END",0x00 },
}; /* cwcsnoop symbols */

static u32 cwcsnoop_code[] = {
/* 0000 */ 0x0007bfb0,0x000b4e40,0x0007c088,0x000c0617,
/* 0002 */ 0x00049705,0x00000000,0x00080630,0x00001028,
/* 0004 */ 0x00076408,0x000efb84,0x00066008,0x00000000,
/* 0006 */ 0x0007c908,0x000c0000,0x00046725,0x000efa44,
/* 0008 */ 0x0005f708,0x00000000,0x0001d402,0x000b2e00,
/* 000A */ 0x0003d418,0x00001000,0x0008d574,0x000c4293,
/* 000C */ 0x00065625,0x000ea30e,0x00096c01,0x000c6f92,
/* 000E */ 0x0006a58a,0x000f6085,0x00002f43,0x00000000,
/* 0010 */ 0x000a83a0,0x00001028,0x0005e608,0x000c0000,
/* 0012 */ 0x00000000,0x00000000,0x000ca108,0x000dcca1,
/* 0014 */ 0x00003bac,0x000fb205,0x00073843,0x00000000,
/* 0016 */ 0x000d8730,0x00001028,0x0006600a,0x000c0000,
/* 0018 */ 0x00057488,0x00000000,0x00000000,0x000e5084,
/* 001A */ 0x00000000,0x000eba44,0x00087401,0x000e4782,
/* 001C */ 0x00000734,0x00001000,0x00010705,0x000a6880,
/* 001E */ 0x00006a88,0x000c75c4
};
/* #CODE_END */

static struct dsp_segment_desc cwcsnoop_segments[] = {
  { SEGTYPE_SP_PROGRAM, 0x00000000, 0x0000003e, cwcsnoop_code },
};

static struct dsp_module_desc cwcsnoop_module = {
  "cwcsnoop",
  {
    3,
    cwcsnoop_symbols
  },
  1,
  cwcsnoop_segments,
};

#endif /* __HEADER_cwcsnoop_H__ */