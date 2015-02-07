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
 * Copyright (C) 2009 Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "WebSettingsImpl.h"

#include "FontRenderingMode.h"
#include "Settings.h"
#include "WebString.h"
#include "WebURL.h"

#if defined(OS_WIN)
#include "RenderThemeChromiumWin.h"
#endif

using namespace WebCore;

namespace WebKit {

WebSettingsImpl::WebSettingsImpl(Settings* settings)
    : m_settings(settings)
{
    ASSERT(settings);
}

void WebSettingsImpl::setStandardFontFamily(const WebString& font)
{
    m_settings->setStandardFontFamily(font);
}

void WebSettingsImpl::setFixedFontFamily(const WebString& font)
{
    m_settings->setFixedFontFamily((String)font);
}

void WebSettingsImpl::setSerifFontFamily(const WebString& font)
{
    m_settings->setSerifFontFamily((String)font);
}

void WebSettingsImpl::setSansSerifFontFamily(const WebString& font)
{
    m_settings->setSansSerifFontFamily((String)font);
}

void WebSettingsImpl::setCursiveFontFamily(const WebString& font)
{
    m_settings->setCursiveFontFamily((String)font);
}

void WebSettingsImpl::setFantasyFontFamily(const WebString& font)
{
    m_settings->setFantasyFontFamily((String)font);
}

void WebSettingsImpl::setDefaultFontSize(int size)
{
    m_settings->setDefaultFontSize(size);
#if defined(OS_WIN)
    // RenderTheme is a singleton that needs to know the default font size to
    // draw some form controls.  We let it know each time the size changes.
    WebCore::RenderThemeChromiumWin::setDefaultFontSize(size);
#endif
}

void WebSettingsImpl::setDefaultFixedFontSize(int size)
{
    m_settings->setDefaultFixedFontSize(size);
}

void WebSettingsImpl::setMinimumFontSize(int size)
{
    m_settings->setMinimumFontSize(size);
}

void WebSettingsImpl::setMinimumLogicalFontSize(int size)
{
    m_settings->setMinimumLogicalFontSize(size);
}

void WebSettingsImpl::setDefaultTextEncodingName(const WebString& encoding)
{
    m_settings->setDefaultTextEncodingName((String)encoding);
}

void WebSettingsImpl::setJavaScriptEnabled(bool enabled)
{
    m_settings->setJavaScriptEnabled(enabled);
}

void WebSettingsImpl::setWebSecurityEnabled(bool enabled)
{
    m_settings->setWebSecurityEnabled(enabled);
}

void WebSettingsImpl::setJavaScriptCanOpenWindowsAutomatically(bool canOpenWindows)
{
    m_settings->setJavaScriptCanOpenWindowsAutomatically(canOpenWindows);
}

void WebSettingsImpl::setLoadsImagesAutomatically(bool loadsImagesAutomatically)
{
    m_settings->setLoadsImagesAutomatically(loadsImagesAutomatically);
}

void WebSettingsImpl::setImagesEnabled(bool enabled)
{
    m_settings->setImagesEnabled(enabled);
}

void WebSettingsImpl::setPluginsEnabled(bool enabled)
{
    m_settings->setPluginsEnabled(enabled);
}

void WebSettingsImpl::setDOMPasteAllowed(bool enabled)
{
    m_settings->setDOMPasteAllowed(enabled);
}

void WebSettingsImpl::setDeveloperExtrasEnabled(bool enabled)
{
    m_settings->setDeveloperExtrasEnabled(enabled);
}

void WebSettingsImpl::setNeedsSiteSpecificQuirks(bool enabled)
{
    m_settings->setNeedsSiteSpecificQuirks(enabled);
}

void WebSettingsImpl::setShrinksStandaloneImagesToFit(bool shrinkImages)
{
    m_settings->setShrinksStandaloneImagesToFit(shrinkImages);
}

void WebSettingsImpl::setUsesEncodingDetector(bool usesDetector)
{
    m_settings->setUsesEncodingDetector(usesDetector);
}

void WebSettingsImpl::setTextAreasAreResizable(bool areResizable)
{
    m_settings->setTextAreasAreResizable(areResizable);
}

void WebSettingsImpl::setJavaEnabled(bool enabled)
{
    m_settings->setJavaEnabled(enabled);
}

void WebSettingsImpl::setAllowScriptsToCloseWindows(bool allow)
{
    m_settings->setAllowScriptsToCloseWindows(allow);
}

void WebSettingsImpl::setUserStyleSheetLocation(const WebURL& location)
{
    m_settings->setUserStyleSheetLocation(location);
}

void WebSettingsImpl::setUsesPageCache(bool usesPageCache)
{
    m_settings->setUsesPageCache(usesPageCache);
}

void WebSettingsImpl::setDownloadableBinaryFontsEnabled(bool enabled)
{
    m_settings->setDownloadableBinaryFontsEnabled(enabled);
}

void WebSettingsImpl::setXSSAuditorEnabled(bool enabled)
{
    m_settings->setXSSAuditorEnabled(enabled);
}

void WebSettingsImpl::setLocalStorageEnabled(bool enabled)
{
    m_settings->setLocalStorageEnabled(enabled);
}

void WebSettingsImpl::setEditableLinkBehaviorNeverLive()
{
    // FIXME: If you ever need more behaviors than this, then we should probably
    //        define an enum in WebSettings.h and have a switch statement that
    //        translates.  Until then, this is probably fine, though.
    m_settings->setEditableLinkBehavior(WebCore::EditableLinkNeverLive);
}

void WebSettingsImpl::setFontRenderingModeNormal()
{
    // FIXME: If you ever need more behaviors than this, then we should probably
    //        define an enum in WebSettings.h and have a switch statement that
    //        translates.  Until then, this is probably fine, though.
    m_settings->setFontRenderingMode(WebCore::NormalRenderingMode);
}

void WebSettingsImpl::setShouldPaintCustomScrollbars(bool enabled)
{
    m_settings->setShouldPaintCustomScrollbars(enabled);
}

void WebSettingsImpl::setDatabasesEnabled(bool enabled)
{
    m_settings->setDatabasesEnabled(enabled);
}

void WebSettingsImpl::setAllowUniversalAccessFromFileURLs(bool allow)
{
    m_settings->setAllowUniversalAccessFromFileURLs(allow);
}

void WebSettingsImpl::setTextDirectionSubmenuInclusionBehaviorNeverIncluded()
{
    // FIXME: If you ever need more behaviors than this, then we should probably
    //        define an enum in WebSettings.h and have a switch statement that
    //        translates.  Until then, this is probably fine, though.
    m_settings->setTextDirectionSubmenuInclusionBehavior(WebCore::TextDirectionSubmenuNeverIncluded);
}

void WebSettingsImpl::setOfflineWebApplicationCacheEnabled(bool enabled)
{
    m_settings->setOfflineWebApplicationCacheEnabled(enabled);
}

void WebSettingsImpl::setExperimentalWebGLEnabled(bool enabled)
{
    m_settings->setWebGLEnabled(enabled);
}

void WebSettingsImpl::setGeolocationEnabled(bool enabled)
{
    m_settings->setGeolocationEnabled(enabled);
}

} // namespace WebKit