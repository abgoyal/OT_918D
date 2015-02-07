#!/usr/bin/python
# Copyright Statement:
#
# This software/firmware and related documentation ("MediaTek Software") are
# protected under relevant copyright laws. The information contained herein
# is confidential and proprietary to MediaTek Inc. and/or its licensors.
# Without the prior written permission of MediaTek inc. and/or its licensors,
# any reproduction, modification, use or disclosure of MediaTek Software,
# and information contained herein, in whole or in part, shall be strictly prohibited.
#
# MediaTek Inc. (C) 2010. All rights reserved.
#
# BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
# THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
# RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
# AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
# EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
# MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
# NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
# SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
# SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
# THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
# THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
# CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
# SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
# STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
# CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
# AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
# OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
# MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.


#
# Usage: unwcheck.py FILE
#
# This script checks the unwind info of each function in file FILE
# and verifies that the sum of the region-lengths matches the total
# length of the function.
#
# Based on a shell/awk script originally written by Harish Patil,
# which was converted to Perl by Matthew Chapman, which was converted
# to Python by David Mosberger.
#
import os
import re
import sys

if len(sys.argv) != 2:
    print "Usage: %s FILE" % sys.argv[0]
    sys.exit(2)

readelf = os.getenv("READELF", "readelf")

start_pattern = re.compile("<([^>]*)>: \[0x([0-9a-f]+)-0x([0-9a-f]+)\]")
rlen_pattern  = re.compile(".*rlen=([0-9]+)")

def check_func (func, slots, rlen_sum):
    if slots != rlen_sum:
        global num_errors
        num_errors += 1
        if not func: func = "[%#x-%#x]" % (start, end)
        print "ERROR: %s: %lu slots, total region length = %lu" % (func, slots, rlen_sum)
    return

num_funcs = 0
num_errors = 0
func = False
slots = 0
rlen_sum = 0
for line in os.popen("%s -u %s" % (readelf, sys.argv[1])):
    m = start_pattern.match(line)
    if m:
        check_func(func, slots, rlen_sum)

        func  = m.group(1)
        start = long(m.group(2), 16)
        end   = long(m.group(3), 16)
        slots = 3 * (end - start) / 16
        rlen_sum = 0L
        num_funcs += 1
    else:
        m = rlen_pattern.match(line)
        if m:
            rlen_sum += long(m.group(1))
check_func(func, slots, rlen_sum)

if num_errors == 0:
    print "No errors detected in %u functions." % num_funcs
else:
    if num_errors > 1:
        err="errors"
    else:
        err="error"
    print "%u %s detected in %u functions." % (num_errors, err, num_funcs)
    sys.exit(1)
