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

   HTML manglizer
   --------------
   Copyright (C) 2004 by Michal Zalewski <lcamtuf@coredump.cx>

   Fault reproduction utility.

 */


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tags.h"

#define R(x) (rand() % (x))

#define MAXTCOUNT 100
#define MAXPCOUNT 20
#define MAXSTR2   80

void make_up_value(void) {
  char c=R(2);

  if (c) putchar('"');

  switch (R(31)) {

    case 0: printf("javascript:"); make_up_value(); break;
//    case 1: printf("jar:"); make_up_value(); break;
    case 2: printf("mk:"); make_up_value(); break;
    case 3: printf("file:"); make_up_value(); break;
    case 4: printf("http:"); make_up_value(); break;
    case 5: printf("about:"); make_up_value(); break;
    case 6: printf("_blank"); break;
    case 7: printf("_self"); break;
    case 8: printf("top"); break;
    case 9: printf("left"); break;
    case 10: putchar('&'); make_up_value(); putchar(';'); break;
    case 11: make_up_value(); make_up_value(); break;

    case 12 ... 20: {
        int c = R(10) ? R(10) : (1 + R(MAXSTR2) * R(MAXSTR2));
        char* x = malloc(c);
        memset(x,R(256),c);
        fwrite(x,c,1,stdout);
        free(x);
        break;
      }

    case 21: printf("%s","%n%n%n%n%n%n"); break;
    case 22: putchar('#'); break;
    case 23: putchar('*'); break;
    default: if (R(2)) putchar('-'); printf("%d",rand()); break;

  }

  if (c) putchar('"');

}
  

void random_tag(void) {
  int tn, tc;
  
  do tn = R(MAXTAGS); while (!tags[tn][0]);
  tc = R(MAXPCOUNT) + 1;
  
  putchar('<');
  
  switch (R(10)) {
    case 0: putchar(R(256)); break;
    case 1: putchar('/');
  }
  
  printf("%s", tags[tn][0]);
  
  while (tc--) {
    int pn;
    switch (R(32)) {
      case 0: putchar(R(256)); 
      case 1: break;
      default: putchar(' ');
    }
    do pn = R(MAXPARS-1) + 1; while (!tags[tn][pn]);
    printf("%s", tags[tn][pn]);
    switch (R(32)) {
      case 0: putchar(R(256)); 
      case 1: break;
      default: putchar('=');
    }
    
    make_up_value();
    
  }
    
  putchar('>');
  
}


int main(int argc,char** argv) {
  int tc,seed; 
  char* x = getenv("QUERY_STRING");

  if (!x || sscanf(x,"%x",&seed) != 1) {
    printf("Content-type: text/plain\n\nMissing or invalid parameter.\n");
    exit(1);
  }

  printf("Content-Type: text/html;charset=utf-8\nRefresh: 0;URL=remangle.cgi?0x%08x\n\n", seed);
  printf("<HTML><HEAD><META HTTP-EQUIV=\"Refresh\" content=\"0;URL=remangle.cgi?0x%08x\">\n", seed);
  printf("<script language=\"javascript\">setTimeout('window.location=\"remangle.cgi?0x%08x\"', 1000);</script>\n", seed);

  srand(seed);
  
  tc = R(MAXTCOUNT) + 1;
  while (tc--) random_tag();
  fflush(0);
  return 0;
}
