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


# Copyright (C) 2004, 2005, 2006 Nathaniel Smith
# Copyright (C) 2006, 2007 Holger Hans Peter Freyther
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
# 1.  Redistributions of source code must retain the above copyright
#     notice, this list of conditions and the following disclaimer. 
# 2.  Redistributions in binary form must reproduce the above copyright
#     notice, this list of conditions and the following disclaimer in the
#     documentation and/or other materials provided with the distribution. 
# 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
#     its contributors may be used to endorse or promote products derived
#     from this software without specific prior written permission. 
#
# THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
# DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
# (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
# LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
# THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

import csv
import time
import os.path
import shutil

def analyze_coverage(possible_gcov_files, source_files, runid, data_dir, base):

    if not os.path.exists(data_dir):
        os.makedirs(data_dir)

    output = open(os.path.join(data_dir, runid + ".csv"), "w")
    w = csv.writer(output)
    # First row: id and time
    w.writerow([runid, time.time()])

    results = scan_gcov_files(possible_gcov_files, source_files)
    annotated_dir = os.path.join(data_dir, runid + ".annotated")
    if os.path.exists(annotated_dir):
        shutil.rmtree(annotated_dir)

    keys = results.keys()
    keys.sort()
    for path in keys:
        (total, covered, annotated_data) = results[path]
        path = path[path.find(base)+len(base):]
        # Rest of the rows: filename, total_lines, covered_lines
        w.writerow([path, total, covered])

        if path[:1] == "/":
            path = path[1:]
        annotated_path = os.path.join(annotated_dir, path)
        try:
            os.makedirs(os.path.dirname(annotated_path))
        except OSError:
            pass
        a = open(annotated_path, "w")
        a.write(annotated_data)
        a.close()


# zecke's rewrite
STATE_NOT_CODE = -1
STATE_NOT_SEEN = -2
STATE_TEST_CODE = -3

def find_gcov(f, possible_gcovs):
    """
    Find .gcov files that could be of interest for us
    """
    try:
        return possible_gcovs[f]
    except:
        return []


def parse_source_file(file):
    """
    Parse one source file and return a list of lines
    """
    f_source_list = []
    init_state = STATE_NOT_SEEN
    in_test_code = False
    nesting = 0

    for line in open(file, "r"):
        code = line.split(":", 2)[-1]
        if not in_test_code and code.startswith("#ifdef BUILD_UNIT_TESTS"):
            in_test_code = 1
        if in_test_code and code.startswith("#if"):
            nesting += 1
        if in_test_code and code.startswith("#endif"):
            nesting -= 1
            if not nesting:
                in_test_code = True
        if in_test_code:
            init_state = STATE_TEST_CODE
        else:
            init_state = STATE_NOT_SEEN
        f_source_list.append([init_state, line.split(":", 1)[1]])

    return f_source_list

# Runner-up, 3rd annual "write Python that looks like Perl" competition,
# Well, not really.  It doesn't even use regexps.
# He is right so I'm cleaning it up (zecke)
def scan_gcov_files(possible_gcov_files, source_files):
    """Takes a list of gcov filenames and a list of source filenames.

    The gcov files should have names of the form foo.o##foo.cc.gcov, as
    created by 'gcov -l'.

    Returns a dict mapping source filenames to tuples
      (total_lines, tested_lines, gcov_annotated_source)
    which are a number, a number, and a very long string, respectively.

    The fun bit is that we merge .gcov output generated by different object
    files; this way we can provide accurate information for header files and
    for monotone's current unit test system."""
    results = {}
    for f in source_files:
        possible_gcovs = find_gcov(f, possible_gcov_files)
        base_name = os.path.splitext(os.path.basename(f))[0]
        if len(possible_gcovs) == 0:
            print "No gcov files found for: '%s' but it was compiled" % f
            continue

            (garbage,extension) = os.path.splitext(f)
            if extension  in [".cc", ".c", ".moc", ".cpp", ".cxx", ".m", ".mm"]:
                lines = open(f, "r").readlines()
                results[f] = (len(lines), 0, "".join(lines))
            continue
        elif len(possible_gcovs) > 1:
            print "More than one gcov file for %s %d" % (f,len(possible_gcovs))
        base_gcov_lines = parse_source_file(possible_gcovs[0])

        # Now we will try hard to merge the results with others
        # Our requirement is that we have the same amount of lines as
        # as the original file
        for cov_file in possible_gcovs:
            lines = open(cov_file, "r").readlines()

            # e.g. with phonon we have visualisation.h and we can not know
            # which header file (foldername) it is refering to. This is a gcov
            # limitation and i have no workaround yet. We just hope we will pick
            # the right header file...
            if len(lines) != len(base_gcov_lines):
                print "Error Base: %s and Target: %s have different amount of lines" % (possible_gcovs[0],cov_file)
                continue

            # now do the merging of the file. If it has the same basename
            # and the same number of lines things might work out
            # In the future take a look at the header of the file
            i = 0
            for line in lines:
                accumulator = base_gcov_lines[i]
                if accumulator[0] != STATE_TEST_CODE:
                    info = line.split(":", 1)[0]
                    if info.endswith("-"):
                        if accumulator[0] == STATE_NOT_SEEN:
                            accumulator[0] = STATE_NOT_CODE
                    else:
                        if info.endswith("#"):
                            num = 0
                        else:
                            num = int(info)
                        if accumulator[0] in (STATE_NOT_SEEN, STATE_NOT_CODE):
                            accumulator[0] = 0
                        accumulator[0] += num
                i += 1

        # post processing of ths file
        (total_lines, total_covered) = (0, 0)
        annotated_lines = []
        for state, line in base_gcov_lines:
            if state == STATE_NOT_SEEN:
                desc = "?????"
            elif state == STATE_TEST_CODE:
                desc = "+"
            elif state == STATE_NOT_CODE:
                desc = "-"
            elif state == 0:
                desc = "#####"
                total_lines += 1
            else:
                desc = str(state)
                total_lines += 1
                total_covered += 1
            annotated_lines.append(":".join([desc.rjust(9), line]))
        results[f] = (total_lines, total_covered, "".join(annotated_lines))
    return results



    return results
