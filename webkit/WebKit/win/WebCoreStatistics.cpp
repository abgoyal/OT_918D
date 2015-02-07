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
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "WebKitDLL.h"
#include "WebCoreStatistics.h"

#include "COMPropertyBag.h"
#include <JavaScriptCore/JSLock.h>
#include <WebCore/FontCache.h>
#include <WebCore/GlyphPageTreeNode.h>
#include <WebCore/IconDatabase.h>
#include <WebCore/JSDOMWindow.h>

using namespace JSC;
using namespace WebCore;

// WebCoreStatistics ---------------------------------------------------------------------------

WebCoreStatistics::WebCoreStatistics()
: m_refCount(0)
{
    gClassCount++;
    gClassNameCount.add("WebCoreStatistics");
}

WebCoreStatistics::~WebCoreStatistics()
{
    gClassCount--;
    gClassNameCount.remove("WebCoreStatistics");
}

WebCoreStatistics* WebCoreStatistics::createInstance()
{
    WebCoreStatistics* instance = new WebCoreStatistics();
    instance->AddRef();
    return instance;
}

// IUnknown -------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE WebCoreStatistics::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IUnknown))
        *ppvObject = static_cast<WebCoreStatistics*>(this);
    else if (IsEqualGUID(riid, IID_IWebCoreStatistics))
        *ppvObject = static_cast<WebCoreStatistics*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG STDMETHODCALLTYPE WebCoreStatistics::AddRef(void)
{
    return ++m_refCount;
}

ULONG STDMETHODCALLTYPE WebCoreStatistics::Release(void)
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete(this);

    return newRef;
}

// IWebCoreStatistics ------------------------------------------------------------------------------

HRESULT STDMETHODCALLTYPE WebCoreStatistics::javaScriptObjectsCount( 
    /* [retval][out] */ UINT* count)
{
    if (!count)
        return E_POINTER;

    JSLock lock(SilenceAssertionsOnly);
    *count = (UINT)JSDOMWindow::commonJSGlobalData()->heap.objectCount();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::javaScriptGlobalObjectsCount( 
    /* [retval][out] */ UINT* count)
{
    if (!count)
        return E_POINTER;

    JSLock lock(SilenceAssertionsOnly);
    *count = (UINT)JSDOMWindow::commonJSGlobalData()->heap.globalObjectCount();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::javaScriptProtectedObjectsCount( 
    /* [retval][out] */ UINT* count)
{
    if (!count)
        return E_POINTER;

    JSLock lock(SilenceAssertionsOnly);
    *count = (UINT)JSDOMWindow::commonJSGlobalData()->heap.protectedObjectCount();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::javaScriptProtectedGlobalObjectsCount( 
    /* [retval][out] */ UINT* count)
{
    if (!count)
        return E_POINTER;

    JSLock lock(SilenceAssertionsOnly);
    *count = (UINT)JSDOMWindow::commonJSGlobalData()->heap.protectedGlobalObjectCount();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::javaScriptProtectedObjectTypeCounts( 
    /* [retval][out] */ IPropertyBag2** typeNamesAndCounts)
{
    JSLock lock(SilenceAssertionsOnly);
    OwnPtr<HashCountedSet<const char*> > jsObjectTypeNames(JSDOMWindow::commonJSGlobalData()->heap.protectedObjectTypeCounts());
    typedef HashCountedSet<const char*>::const_iterator Iterator;
    Iterator end = jsObjectTypeNames->end();
    HashMap<String, int> typeCountMap;
    for (Iterator current = jsObjectTypeNames->begin(); current != end; ++current)
        typeCountMap.set(current->first, current->second);

    COMPtr<IPropertyBag2> results(AdoptCOM, COMPropertyBag<int>::createInstance(typeCountMap));
    results.copyRefTo(typeNamesAndCounts);
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::iconPageURLMappingCount( 
    /* [retval][out] */ UINT* count)
{
    if (!count)
        return E_POINTER;
    *count = (UINT) iconDatabase()->pageURLMappingCount();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::iconRetainedPageURLCount( 
    /* [retval][out] */ UINT *count)
{
    if (!count)
        return E_POINTER;
    *count = (UINT) iconDatabase()->retainedPageURLCount();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::iconRecordCount( 
    /* [retval][out] */ UINT *count)
{
    if (!count)
        return E_POINTER;
    *count = (UINT) iconDatabase()->iconRecordCount();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::iconsWithDataCount( 
    /* [retval][out] */ UINT *count)
{
    if (!count)
        return E_POINTER;
    *count = (UINT) iconDatabase()->iconRecordCountWithData();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::cachedFontDataCount( 
    /* [retval][out] */ UINT *count)
{
    if (!count)
        return E_POINTER;
    *count = (UINT) fontCache()->fontDataCount();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::cachedFontDataInactiveCount( 
    /* [retval][out] */ UINT *count)
{
    if (!count)
        return E_POINTER;
    *count = (UINT) fontCache()->inactiveFontDataCount();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::purgeInactiveFontData(void)
{
    fontCache()->purgeInactiveFontData();
    return S_OK;
}

HRESULT STDMETHODCALLTYPE WebCoreStatistics::glyphPageCount( 
    /* [retval][out] */ UINT *count)
{
    if (!count)
        return E_POINTER;
    *count = (UINT) GlyphPageTreeNode::treeGlyphPageCount();
    return S_OK;
}
