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
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "MIMETypeRegistry.h"

#include <shlwapi.h>
#include <wtf/HashMap.h>

namespace WebCore 
{

static String mimeTypeForExtension(const String& extension)
{
    String ext = "." + extension;
    WCHAR contentTypeStr[256];
    DWORD contentTypeStrLen = sizeof(contentTypeStr);
    DWORD keyType;

    HRESULT result = SHGetValue(HKEY_CLASSES_ROOT, ext.charactersWithNullTermination(), L"Content Type", &keyType, (LPVOID)contentTypeStr, &contentTypeStrLen);

    if (result == ERROR_SUCCESS && keyType == REG_SZ) 
        return String(contentTypeStr, contentTypeStrLen / sizeof(contentTypeStr[0]) - 1);

    return String();
}
   
String MIMETypeRegistry::getPreferredExtensionForMIMEType(const String& type)
{
    String path = "MIME\\Database\\Content Type\\" + type;
    WCHAR extStr[MAX_PATH];
    DWORD extStrLen = sizeof(extStr);
    DWORD keyType;

    HRESULT result = SHGetValueW(HKEY_CLASSES_ROOT, path.charactersWithNullTermination(), L"Extension", &keyType, (LPVOID)extStr, &extStrLen);

    if (result == ERROR_SUCCESS && keyType == REG_SZ) 
        return String(extStr + 1, extStrLen / sizeof(extStr[0]) - 2);

    return String();
}

String MIMETypeRegistry::getMIMETypeForExtension(const String &ext)
{
    if (ext.isEmpty())
        return String();

    static HashMap<String, String> mimetypeMap;
    if (mimetypeMap.isEmpty()) {
        //fill with initial values
        mimetypeMap.add("txt", "text/plain");
        mimetypeMap.add("pdf", "application/pdf");
        mimetypeMap.add("ps", "application/postscript");
        mimetypeMap.add("html", "text/html");
        mimetypeMap.add("htm", "text/html");
        mimetypeMap.add("xml", "text/xml");
        mimetypeMap.add("xsl", "text/xsl");
        mimetypeMap.add("js", "application/x-javascript");
        mimetypeMap.add("xhtml", "application/xhtml+xml");
        mimetypeMap.add("rss", "application/rss+xml");
        mimetypeMap.add("webarchive", "application/x-webarchive");
        mimetypeMap.add("svg", "image/svg+xml");
        mimetypeMap.add("svgz", "image/svg+xml");
        mimetypeMap.add("jpg", "image/jpeg");
        mimetypeMap.add("jpeg", "image/jpeg");
        mimetypeMap.add("png", "image/png");
        mimetypeMap.add("tif", "image/tiff");
        mimetypeMap.add("tiff", "image/tiff");
        mimetypeMap.add("ico", "image/ico");
        mimetypeMap.add("cur", "image/ico");
        mimetypeMap.add("bmp", "image/bmp");
        mimetypeMap.add("wml", "text/vnd.wap.wml");
        mimetypeMap.add("wmlc", "application/vnd.wap.wmlc");
    }
    String result = mimetypeMap.get(ext);
    if (result.isEmpty()) {
        result = mimeTypeForExtension(ext);
        if (!result.isEmpty())
            mimetypeMap.add(ext, result);
    }
    return result;
}

}