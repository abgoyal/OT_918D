#! /usr/bin/perl -w
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




# convert an Intel HEX file into a set of C records usable by the firmware
# loading code in usb-serial.c (or others)

# accepts the .hex file(s) on stdin, a basename (to name the initialized
# array) as an argument, and prints the .h file to stdout. Typical usage:
#  perl ezusb_convert.pl foo <foo.hex >fw_foo.h


my $basename = $ARGV[0];
die "no base name specified" unless $basename;

while (<STDIN>) {
    # ':' <len> <addr> <type> <len-data> <crc> '\r'
    #  len, type, crc are 2-char hex, addr is 4-char hex. type is 00 for
    # normal records, 01 for EOF
    my($lenstring, $addrstring, $typestring, $reststring, $doscrap) =
      /^:(\w\w)(\w\w\w\w)(\w\w)(\w+)(\r?)$/;
    die "malformed line: $_" unless $reststring;
    last if $typestring eq '01';
    my($len) = hex($lenstring);
    my($addr) = hex($addrstring);
    my(@bytes) = unpack("C*", pack("H".(2*$len), $reststring));
    #pop(@bytes); # last byte is a CRC
    push(@records, [$addr, \@bytes]);
}

@sorted_records = sort { $a->[0] <=> $b->[0] } @records;

print <<"EOF";
/*
 * ${basename}_fw.h
 *
 * Generated from ${basename}.s by ezusb_convert.pl
 * This file is presumed to be under the same copyright as the source file
 * from which it was derived.
 */

EOF

print "static const struct ezusb_hex_record ${basename}_firmware[] = {\n";
foreach $r (@sorted_records) {
    printf("{ 0x%04x,\t%d,\t{", $r->[0], scalar(@{$r->[1]}));
    print join(", ", map {sprintf('0x%02x', $_);} @{$r->[1]});
    print "} },\n";
}
print "{ 0xffff,\t0,\t{0x00} }\n";
print "};\n";
