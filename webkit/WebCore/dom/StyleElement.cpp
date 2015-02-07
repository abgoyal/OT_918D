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
 * Copyright (C) 2006, 2007 Rob Buis
 * Copyright (C) 2008 Apple, Inc. All rights reserved.
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
#include "StyleElement.h"

#include "Document.h"
#include "Element.h"
#include "MappedAttribute.h"
#include "MediaList.h"
#include "MediaQueryEvaluator.h"

namespace WebCore {

StyleElement::StyleElement()
{
}

StyleSheet* StyleElement::sheet(Element* e)
{
    if (!m_sheet)
        createSheet(e);
    return m_sheet.get();
}

void StyleElement::insertedIntoDocument(Document*, Element* element)
{
    process(element);
}

void StyleElement::removedFromDocument(Document* document)
{
    // If we're in document teardown, then we don't need to do any notification of our sheet's removal.
    if (!document->renderer())
        return;

    // FIXME: It's terrible to do a synchronous update of the style selector just because a <style> or <link> element got removed.
    if (m_sheet)
        document->updateStyleSelector();
}

void StyleElement::process(Element* e)
{
    if (!e || !e->inDocument())
        return;

    unsigned resultLength = 0;
    for (Node* c = e->firstChild(); c; c = c->nextSibling()) {
        Node::NodeType nodeType = c->nodeType();
        if (nodeType == Node::TEXT_NODE || nodeType == Node::CDATA_SECTION_NODE || nodeType == Node::COMMENT_NODE) {
            unsigned length = c->nodeValue().length();
            if (length > std::numeric_limits<unsigned>::max() - resultLength)
                CRASH();
            resultLength += length;
        }
    }
    UChar* text;
    String sheetText = String::createUninitialized(resultLength, text);

    UChar* p = text;
    for (Node* c = e->firstChild(); c; c = c->nextSibling()) {
        Node::NodeType nodeType = c->nodeType();
        if (nodeType == Node::TEXT_NODE || nodeType == Node::CDATA_SECTION_NODE || nodeType == Node::COMMENT_NODE) {
            String nodeValue = c->nodeValue();
            unsigned nodeLength = nodeValue.length();
            memcpy(p, nodeValue.characters(), nodeLength * sizeof(UChar));
            p += nodeLength;
        }
    }
    ASSERT(p == text + resultLength);

    createSheet(e, sheetText);
}

void StyleElement::createSheet(Element* e, const String& text)
{
    Document* document = e->document();
    if (m_sheet) {
        if (static_cast<CSSStyleSheet*>(m_sheet.get())->isLoading())
            document->removePendingSheet();
        m_sheet = 0;
    }

    // If type is empty or CSS, this is a CSS style sheet.
    const AtomicString& type = this->type();
    if (type.isEmpty() || (e->isHTMLElement() ? equalIgnoringCase(type, "text/css") : (type == "text/css"))) {
        RefPtr<MediaList> mediaList = MediaList::create(media(), e->isHTMLElement());
        MediaQueryEvaluator screenEval("screen", true);
        MediaQueryEvaluator printEval("print", true);
        if (screenEval.eval(mediaList.get()) || printEval.eval(mediaList.get())) {
            document->addPendingSheet();
            setLoading(true);
            m_sheet = CSSStyleSheet::create(e, String(), KURL(), document->inputEncoding());
            m_sheet->parseString(text, !document->inCompatMode());
            m_sheet->setMedia(mediaList.get());
            m_sheet->setTitle(e->title());
            setLoading(false);
        }
    }

    if (m_sheet)
        m_sheet->checkLoaded();
}

}