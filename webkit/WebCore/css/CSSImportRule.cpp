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
 * (C) 1999-2003 Lars Knoll (knoll@kde.org)
 * (C) 2002-2003 Dirk Mueller (mueller@kde.org)
 * Copyright (C) 2002, 2005, 2006, 2008, 2009 Apple Inc. All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */

#include "config.h"
#include "CSSImportRule.h"

#include "CachedCSSStyleSheet.h"
#include "DocLoader.h"
#include "Document.h"
#include "SecurityOrigin.h"
#include "Settings.h"
#include <wtf/StdLibExtras.h>

namespace WebCore {

CSSImportRule::CSSImportRule(CSSStyleSheet* parent, const String& href, PassRefPtr<MediaList> media)
    : CSSRule(parent)
    , m_strHref(href)
    , m_lstMedia(media)
    , m_cachedSheet(0)
    , m_loading(false)
{
    if (m_lstMedia)
        m_lstMedia->setParent(this);
    else
        m_lstMedia = MediaList::create(this, String());
}

CSSImportRule::~CSSImportRule()
{
    if (m_lstMedia)
        m_lstMedia->setParent(0);
    if (m_styleSheet)
        m_styleSheet->setParent(0);
    if (m_cachedSheet)
        m_cachedSheet->removeClient(this);
}

void CSSImportRule::setCSSStyleSheet(const String& href, const KURL& baseURL, const String& charset, const CachedCSSStyleSheet* sheet)
{
    if (m_styleSheet)
        m_styleSheet->setParent(0);
    m_styleSheet = CSSStyleSheet::create(this, href, baseURL, charset);

    bool crossOriginCSS = false;
    bool validMIMEType = false;
    CSSStyleSheet* parent = parentStyleSheet();
    bool strict = !parent || parent->useStrictParsing();
    bool enforceMIMEType = strict;
    bool needsSiteSpecificQuirks = parent && parent->doc() && parent->doc()->settings() && parent->doc()->settings()->needsSiteSpecificQuirks();

#if defined(BUILDING_ON_TIGER) || defined(BUILDING_ON_LEOPARD)
    if (enforceMIMEType && needsSiteSpecificQuirks) {
        // Covers both http and https, with or without "www."
        if (baseURL.string().contains("mcafee.com/japan/", false))
            enforceMIMEType = false;
    }
#endif

    String sheetText = sheet->sheetText(enforceMIMEType, &validMIMEType);
    m_styleSheet->parseString(sheetText, strict);

    if (!parent || !parent->doc() || !parent->doc()->securityOrigin()->canRequest(baseURL))
        crossOriginCSS = true;

    if (crossOriginCSS && !validMIMEType && !m_styleSheet->hasSyntacticallyValidCSSHeader())
        m_styleSheet = CSSStyleSheet::create(this, href, baseURL, charset);

    if (strict && needsSiteSpecificQuirks) {
        // Work around <https://bugs.webkit.org/show_bug.cgi?id=28350>.
        DEFINE_STATIC_LOCAL(const String, slashKHTMLFixesDotCss, ("/KHTMLFixes.css"));
        DEFINE_STATIC_LOCAL(const String, mediaWikiKHTMLFixesStyleSheet, ("/* KHTML fix stylesheet */\n/* work around the horizontal scrollbars */\n#column-content { margin-left: 0; }\n\n"));
        if (baseURL.string().endsWith(slashKHTMLFixesDotCss) && sheetText == mediaWikiKHTMLFixesStyleSheet) {
            ASSERT(m_styleSheet->length() == 1);
            ExceptionCode ec;
            m_styleSheet->deleteRule(0, ec);
        }
    }

    m_loading = false;

    if (parent)
        parent->checkLoaded();
}

bool CSSImportRule::isLoading() const
{
    return m_loading || (m_styleSheet && m_styleSheet->isLoading());
}

void CSSImportRule::insertedIntoParent()
{
    CSSStyleSheet* parentSheet = parentStyleSheet();
    if (!parentSheet)
        return;

    DocLoader* docLoader = parentSheet->doc()->docLoader();
    if (!docLoader)
        return;

    String absHref = m_strHref;
    if (!parentSheet->finalURL().isNull())
        // use parent styleheet's URL as the base URL
        absHref = KURL(parentSheet->finalURL(), m_strHref).string();

    // Check for a cycle in our import chain.  If we encounter a stylesheet
    // in our parent chain with the same URL, then just bail.
    StyleBase* root = this;
    for (StyleBase* curr = parent(); curr; curr = curr->parent()) {
        // FIXME: This is wrong if the finalURL was updated via document::updateBaseURL. 
        if (curr->isCSSStyleSheet() && absHref == static_cast<CSSStyleSheet*>(curr)->finalURL().string())
            return;
        root = curr;
    }

    if (parentSheet->isUserStyleSheet())
        m_cachedSheet = docLoader->requestUserCSSStyleSheet(absHref, parentSheet->charset());
    else
        m_cachedSheet = docLoader->requestCSSStyleSheet(absHref, parentSheet->charset());
    if (m_cachedSheet) {
        // if the import rule is issued dynamically, the sheet may be
        // removed from the pending sheet count, so let the doc know
        // the sheet being imported is pending.
        if (parentSheet && parentSheet->loadCompleted() && root == parentSheet)
            parentSheet->doc()->addPendingSheet();
        m_loading = true;
        m_cachedSheet->addClient(this);
    }
}

String CSSImportRule::cssText() const
{
    String result = "@import url(\"";
    result += m_strHref;
    result += "\")";

    if (m_lstMedia) {
        result += " ";
        result += m_lstMedia->mediaText();
    }
    result += ";";

    return result;
}

void CSSImportRule::addSubresourceStyleURLs(ListHashSet<KURL>& urls)
{
    if (m_styleSheet)
        addSubresourceURL(urls, m_styleSheet->baseURL());
}

#ifdef ANDROID_INSTRUMENT
void* CSSImportRule::operator new(size_t size)
{
    return StyleBase::operator new(size);
}

void* CSSImportRule::operator new[](size_t size)
{
    return StyleBase::operator new[](size);
}

void CSSImportRule::operator delete(void* p, size_t size)
{
    StyleBase::operator delete(p, size);
}

void CSSImportRule::operator delete[](void* p, size_t size)
{
    StyleBase::operator delete[](p, size);
}
#endif

} // namespace WebCore