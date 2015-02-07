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
 * Copyright (C) 2009 Apple Inc.  All rights reserved.
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
#include "HistoryDelegate.h"

#include "DumpRenderTree.h"
#include "DumpRenderTreeWin.h"
#include "LayoutTestController.h"
#include <string>
#include <WebKit/WebKit.h>

using std::wstring;

static inline wstring wstringFromBSTR(BSTR str)
{
    return wstring(str, ::SysStringLen(str));
}

HistoryDelegate::HistoryDelegate()
    : m_refCount(1)
{
}

HistoryDelegate::~HistoryDelegate()
{
}

    // IUnknown
HRESULT HistoryDelegate::QueryInterface(REFIID riid, void** ppvObject)
{
    *ppvObject = 0;
    if (IsEqualGUID(riid, IID_IUnknown))
        *ppvObject = static_cast<IWebHistoryDelegate*>(this);
    else if (IsEqualGUID(riid, IID_IWebHistoryDelegate))
        *ppvObject = static_cast<IWebHistoryDelegate*>(this);
    else
        return E_NOINTERFACE;

    AddRef();
    return S_OK;
}

ULONG HistoryDelegate::AddRef(void)
{
    return ++m_refCount;
}

ULONG HistoryDelegate::Release(void)
{
    ULONG newRef = --m_refCount;
    if (!newRef)
        delete(this);

    return newRef;
}

// IWebHistoryDelegate
HRESULT HistoryDelegate::didNavigateWithNavigationData(IWebView* webView, IWebNavigationData* navigationData, IWebFrame* webFrame)
{
    if (!gLayoutTestController->dumpHistoryDelegateCallbacks())
        return S_OK;

    BSTR urlBSTR;
    if (FAILED(navigationData->url(&urlBSTR)))
        return E_FAIL;
    wstring url;
    if (urlBSTR)
        url = urlSuitableForTestResult(wstringFromBSTR(urlBSTR));
    SysFreeString(urlBSTR);

    BSTR titleBSTR;
    if (FAILED(navigationData->title(&titleBSTR)))
        return E_FAIL;
    wstring title;
    if (titleBSTR)
        title = wstringFromBSTR(titleBSTR);
    SysFreeString(titleBSTR);

    COMPtr<IWebURLRequest> request;
    if (FAILED(navigationData->originalRequest(&request)))
        return E_FAIL;

    BSTR httpMethodBSTR;
    if (FAILED(request->HTTPMethod(&httpMethodBSTR)))
        return E_FAIL;
    wstring httpMethod;
    if (httpMethodBSTR)
        httpMethod = wstringFromBSTR(httpMethodBSTR);
    SysFreeString(httpMethodBSTR);

    COMPtr<IWebURLResponse> response;
    if (FAILED(navigationData->response(&response)))
        return E_FAIL;

    COMPtr<IWebHTTPURLResponse> httpResponse;
    if (FAILED(response->QueryInterface(&httpResponse)))
        return E_FAIL;

    int statusCode = 0;
    if (FAILED(httpResponse->statusCode(&statusCode)))
        return E_FAIL;

    BOOL hasSubstituteData;
    if (FAILED(navigationData->hasSubstituteData(&hasSubstituteData)))
        return E_FAIL;

    BSTR clientRedirectSourceBSTR;
    if (FAILED(navigationData->clientRedirectSource(&clientRedirectSourceBSTR)))
        return E_FAIL;
    bool hasClientRedirect = clientRedirectSourceBSTR && SysStringLen(clientRedirectSourceBSTR);
    wstring redirectSource;
    if (clientRedirectSourceBSTR)
        redirectSource = urlSuitableForTestResult(wstringFromBSTR(clientRedirectSourceBSTR));
    SysFreeString(clientRedirectSourceBSTR);

    bool wasFailure = hasSubstituteData || (httpResponse && statusCode >= 400);
        
    printf("WebView navigated to url \"%S\" with title \"%S\" with HTTP equivalent method \"%S\".  The navigation was %s and was %s%S.\n", 
        url.c_str(), 
        title.c_str(), 
        httpMethod.c_str(),
        wasFailure ? "a failure" : "successful", 
        hasClientRedirect ? "a client redirect from " : "not a client redirect", 
        redirectSource.c_str());

    return S_OK;
}

HRESULT HistoryDelegate::didPerformClientRedirectFromURL(IWebView*, BSTR sourceURL, BSTR destinationURL, IWebFrame*)
{
    if (!gLayoutTestController->dumpHistoryDelegateCallbacks())
        return S_OK;

    wstring source;
    if (sourceURL)
        source = urlSuitableForTestResult(wstringFromBSTR(sourceURL));
    
    wstring destination;
    if (destinationURL)
        destination = urlSuitableForTestResult(wstringFromBSTR(destinationURL));

    printf("WebView performed a client redirect from \"%S\" to \"%S\".\n", source.c_str(), destination.c_str());
    return S_OK;
}
    
HRESULT HistoryDelegate::didPerformServerRedirectFromURL(IWebView* webView, BSTR sourceURL, BSTR destinationURL, IWebFrame* webFrame)
{
    if (!gLayoutTestController->dumpHistoryDelegateCallbacks())
        return S_OK;

    wstring source;
    if (sourceURL)
        source = urlSuitableForTestResult(wstringFromBSTR(sourceURL));
    
    wstring destination;
    if (destinationURL)
        destination = urlSuitableForTestResult(wstringFromBSTR(destinationURL));

    printf("WebView performed a server redirect from \"%S\" to \"%S\".\n", source.c_str(), destination.c_str());
    return S_OK;
}

HRESULT HistoryDelegate::updateHistoryTitle(IWebView* webView, BSTR titleBSTR, BSTR urlBSTR)
{
    if (!gLayoutTestController->dumpHistoryDelegateCallbacks())
        return S_OK;
    
    wstring url;
    if (urlBSTR)
        url = urlSuitableForTestResult(wstringFromBSTR(urlBSTR));

    wstring title;
    if (titleBSTR)
        title = wstringFromBSTR(titleBSTR);

    printf("WebView updated the title for history URL \"%S\" to \"%S\".\n", url.c_str(), title.c_str());
    return S_OK;
}
    
HRESULT HistoryDelegate::populateVisitedLinksForWebView(IWebView* webView)
{
    if (!gLayoutTestController->dumpHistoryDelegateCallbacks())
        return S_OK;

    BSTR urlBSTR;
    if (FAILED(webView->mainFrameURL(&urlBSTR)))
        return E_FAIL;

    wstring url;
    if (urlBSTR)
        url = urlSuitableForTestResult(wstringFromBSTR(urlBSTR));
    SysFreeString(urlBSTR);

    if (gLayoutTestController->dumpVisitedLinksCallback())
        printf("Asked to populate visited links for WebView \"%S\"\n", url.c_str());

    return S_OK;
}
