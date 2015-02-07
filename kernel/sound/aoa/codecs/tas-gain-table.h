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
 This is the program used to generate below table.

#include <stdio.h>
#include <math.h>
int main() {
  int dB2;
  printf("/" "* This file is only included exactly once!\n");
  printf(" *\n");
  printf(" * If they'd only tell us that generating this table was\n");
  printf(" * as easy as calculating\n");
  printf(" *      hwvalue = 1048576.0*exp(0.057564628*dB*2)\n");
  printf(" * :) *" "/\n");
  printf("static int tas_gaintable[] = {\n");
  printf("	0x000000, /" "* -infinity dB *" "/\n");
  for (dB2=-140;dB2<=36;dB2++)
    printf("	0x%.6x, /" "* %-02.1f dB *" "/\n", (int)(1048576.0*exp(0.057564628*dB2)), dB2/2.0);
  printf("};\n\n");
}

*/

/* This file is only included exactly once!
 *
 * If they'd only tell us that generating this table was
 * as easy as calculating
 *      hwvalue = 1048576.0*exp(0.057564628*dB*2)
 * :) */
static int tas_gaintable[] = {
	0x000000, /* -infinity dB */
	0x00014b, /* -70.0 dB */
	0x00015f, /* -69.5 dB */
	0x000174, /* -69.0 dB */
	0x00018a, /* -68.5 dB */
	0x0001a1, /* -68.0 dB */
	0x0001ba, /* -67.5 dB */
	0x0001d4, /* -67.0 dB */
	0x0001f0, /* -66.5 dB */
	0x00020d, /* -66.0 dB */
	0x00022c, /* -65.5 dB */
	0x00024d, /* -65.0 dB */
	0x000270, /* -64.5 dB */
	0x000295, /* -64.0 dB */
	0x0002bc, /* -63.5 dB */
	0x0002e6, /* -63.0 dB */
	0x000312, /* -62.5 dB */
	0x000340, /* -62.0 dB */
	0x000372, /* -61.5 dB */
	0x0003a6, /* -61.0 dB */
	0x0003dd, /* -60.5 dB */
	0x000418, /* -60.0 dB */
	0x000456, /* -59.5 dB */
	0x000498, /* -59.0 dB */
	0x0004de, /* -58.5 dB */
	0x000528, /* -58.0 dB */
	0x000576, /* -57.5 dB */
	0x0005c9, /* -57.0 dB */
	0x000620, /* -56.5 dB */
	0x00067d, /* -56.0 dB */
	0x0006e0, /* -55.5 dB */
	0x000748, /* -55.0 dB */
	0x0007b7, /* -54.5 dB */
	0x00082c, /* -54.0 dB */
	0x0008a8, /* -53.5 dB */
	0x00092b, /* -53.0 dB */
	0x0009b6, /* -52.5 dB */
	0x000a49, /* -52.0 dB */
	0x000ae5, /* -51.5 dB */
	0x000b8b, /* -51.0 dB */
	0x000c3a, /* -50.5 dB */
	0x000cf3, /* -50.0 dB */
	0x000db8, /* -49.5 dB */
	0x000e88, /* -49.0 dB */
	0x000f64, /* -48.5 dB */
	0x00104e, /* -48.0 dB */
	0x001145, /* -47.5 dB */
	0x00124b, /* -47.0 dB */
	0x001361, /* -46.5 dB */
	0x001487, /* -46.0 dB */
	0x0015be, /* -45.5 dB */
	0x001708, /* -45.0 dB */
	0x001865, /* -44.5 dB */
	0x0019d8, /* -44.0 dB */
	0x001b60, /* -43.5 dB */
	0x001cff, /* -43.0 dB */
	0x001eb7, /* -42.5 dB */
	0x002089, /* -42.0 dB */
	0x002276, /* -41.5 dB */
	0x002481, /* -41.0 dB */
	0x0026ab, /* -40.5 dB */
	0x0028f5, /* -40.0 dB */
	0x002b63, /* -39.5 dB */
	0x002df5, /* -39.0 dB */
	0x0030ae, /* -38.5 dB */
	0x003390, /* -38.0 dB */
	0x00369e, /* -37.5 dB */
	0x0039db, /* -37.0 dB */
	0x003d49, /* -36.5 dB */
	0x0040ea, /* -36.0 dB */
	0x0044c3, /* -35.5 dB */
	0x0048d6, /* -35.0 dB */
	0x004d27, /* -34.5 dB */
	0x0051b9, /* -34.0 dB */
	0x005691, /* -33.5 dB */
	0x005bb2, /* -33.0 dB */
	0x006121, /* -32.5 dB */
	0x0066e3, /* -32.0 dB */
	0x006cfb, /* -31.5 dB */
	0x007370, /* -31.0 dB */
	0x007a48, /* -30.5 dB */
	0x008186, /* -30.0 dB */
	0x008933, /* -29.5 dB */
	0x009154, /* -29.0 dB */
	0x0099f1, /* -28.5 dB */
	0x00a310, /* -28.0 dB */
	0x00acba, /* -27.5 dB */
	0x00b6f6, /* -27.0 dB */
	0x00c1cd, /* -26.5 dB */
	0x00cd49, /* -26.0 dB */
	0x00d973, /* -25.5 dB */
	0x00e655, /* -25.0 dB */
	0x00f3fb, /* -24.5 dB */
	0x010270, /* -24.0 dB */
	0x0111c0, /* -23.5 dB */
	0x0121f9, /* -23.0 dB */
	0x013328, /* -22.5 dB */
	0x01455b, /* -22.0 dB */
	0x0158a2, /* -21.5 dB */
	0x016d0e, /* -21.0 dB */
	0x0182af, /* -20.5 dB */
	0x019999, /* -20.0 dB */
	0x01b1de, /* -19.5 dB */
	0x01cb94, /* -19.0 dB */
	0x01e6cf, /* -18.5 dB */
	0x0203a7, /* -18.0 dB */
	0x022235, /* -17.5 dB */
	0x024293, /* -17.0 dB */
	0x0264db, /* -16.5 dB */
	0x02892c, /* -16.0 dB */
	0x02afa3, /* -15.5 dB */
	0x02d862, /* -15.0 dB */
	0x03038a, /* -14.5 dB */
	0x033142, /* -14.0 dB */
	0x0361af, /* -13.5 dB */
	0x0394fa, /* -13.0 dB */
	0x03cb50, /* -12.5 dB */
	0x0404de, /* -12.0 dB */
	0x0441d5, /* -11.5 dB */
	0x048268, /* -11.0 dB */
	0x04c6d0, /* -10.5 dB */
	0x050f44, /* -10.0 dB */
	0x055c04, /* -9.5 dB */
	0x05ad50, /* -9.0 dB */
	0x06036e, /* -8.5 dB */
	0x065ea5, /* -8.0 dB */
	0x06bf44, /* -7.5 dB */
	0x07259d, /* -7.0 dB */
	0x079207, /* -6.5 dB */
	0x0804dc, /* -6.0 dB */
	0x087e80, /* -5.5 dB */
	0x08ff59, /* -5.0 dB */
	0x0987d5, /* -4.5 dB */
	0x0a1866, /* -4.0 dB */
	0x0ab189, /* -3.5 dB */
	0x0b53be, /* -3.0 dB */
	0x0bff91, /* -2.5 dB */
	0x0cb591, /* -2.0 dB */
	0x0d765a, /* -1.5 dB */
	0x0e4290, /* -1.0 dB */
	0x0f1adf, /* -0.5 dB */
	0x100000, /* 0.0 dB */
	0x10f2b4, /* 0.5 dB */
	0x11f3c9, /* 1.0 dB */
	0x13041a, /* 1.5 dB */
	0x14248e, /* 2.0 dB */
	0x15561a, /* 2.5 dB */
	0x1699c0, /* 3.0 dB */
	0x17f094, /* 3.5 dB */
	0x195bb8, /* 4.0 dB */
	0x1adc61, /* 4.5 dB */
	0x1c73d5, /* 5.0 dB */
	0x1e236d, /* 5.5 dB */
	0x1fec98, /* 6.0 dB */
	0x21d0d9, /* 6.5 dB */
	0x23d1cd, /* 7.0 dB */
	0x25f125, /* 7.5 dB */
	0x2830af, /* 8.0 dB */
	0x2a9254, /* 8.5 dB */
	0x2d1818, /* 9.0 dB */
	0x2fc420, /* 9.5 dB */
	0x3298b0, /* 10.0 dB */
	0x35982f, /* 10.5 dB */
	0x38c528, /* 11.0 dB */
	0x3c224c, /* 11.5 dB */
	0x3fb278, /* 12.0 dB */
	0x4378b0, /* 12.5 dB */
	0x477829, /* 13.0 dB */
	0x4bb446, /* 13.5 dB */
	0x5030a1, /* 14.0 dB */
	0x54f106, /* 14.5 dB */
	0x59f980, /* 15.0 dB */
	0x5f4e52, /* 15.5 dB */
	0x64f403, /* 16.0 dB */
	0x6aef5e, /* 16.5 dB */
	0x714575, /* 17.0 dB */
	0x77fbaa, /* 17.5 dB */
	0x7f17af, /* 18.0 dB */
};

