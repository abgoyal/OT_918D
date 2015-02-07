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

/**
 * Copyright (C) 1997 Martin Jones (mjones@kde.org)
 *           (C) 1997 Torben Weis (weis@kde.org)
 *           (C) 1998 Waldo Bastian (bastian@kde.org)
 *           (C) 1999 Lars Knoll (knoll@kde.org)
 *           (C) 1999 Antti Koivisto (koivisto@kde.org)
 * Copyright (C) 2003, 2004, 2005, 2006 Apple Computer, Inc.
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
#include "HTMLTablePartElement.h"

#include "CSSHelper.h"
#include "CSSPropertyNames.h"
#include "CSSValueKeywords.h"
#include "Document.h"
#include "HTMLNames.h"
#include "MappedAttribute.h"

namespace WebCore {

using namespace HTMLNames;

bool HTMLTablePartElement::mapToEntry(const QualifiedName& attrName, MappedAttributeEntry& result) const
{
    if (attrName == backgroundAttr) {
        result = (MappedAttributeEntry)(eLastEntry + document()->docID());
        return false;
    }
    
    if (attrName == bgcolorAttr ||
        attrName == bordercolorAttr ||
        attrName == valignAttr ||
        attrName == heightAttr) {
        result = eUniversal;
        return false;
    }
    
    if (attrName == alignAttr) {
        result = eCell; // All table parts will just share in the TD space.
        return false;
    }

    return HTMLElement::mapToEntry(attrName, result);
}

void HTMLTablePartElement::parseMappedAttribute(MappedAttribute *attr)
{
    if (attr->name() == bgcolorAttr)
        addCSSColor(attr, CSSPropertyBackgroundColor, attr->value());
    else if (attr->name() == backgroundAttr) {
        String url = deprecatedParseURL(attr->value());
        if (!url.isEmpty())
            addCSSImageProperty(attr, CSSPropertyBackgroundImage, document()->completeURL(url).string());
    } else if (attr->name() == bordercolorAttr) {
        if (!attr->value().isEmpty()) {
            addCSSColor(attr, CSSPropertyBorderColor, attr->value());
            addCSSProperty(attr, CSSPropertyBorderTopStyle, CSSValueSolid);
            addCSSProperty(attr, CSSPropertyBorderBottomStyle, CSSValueSolid);
            addCSSProperty(attr, CSSPropertyBorderLeftStyle, CSSValueSolid);
            addCSSProperty(attr, CSSPropertyBorderRightStyle, CSSValueSolid);
        }
    } else if (attr->name() == valignAttr) {
        if (!attr->value().isEmpty())
            addCSSProperty(attr, CSSPropertyVerticalAlign, attr->value());
    } else if (attr->name() == alignAttr) {
        const AtomicString& v = attr->value();
        if (equalIgnoringCase(v, "middle") || equalIgnoringCase(v, "center"))
            addCSSProperty(attr, CSSPropertyTextAlign, CSSValueWebkitCenter);
        else if (equalIgnoringCase(v, "absmiddle"))
            addCSSProperty(attr, CSSPropertyTextAlign, CSSValueCenter);
        else if (equalIgnoringCase(v, "left"))
            addCSSProperty(attr, CSSPropertyTextAlign, CSSValueWebkitLeft);
        else if (equalIgnoringCase(v, "right"))
            addCSSProperty(attr, CSSPropertyTextAlign, CSSValueWebkitRight);
        else
            addCSSProperty(attr, CSSPropertyTextAlign, v);
    } else if (attr->name() == heightAttr) {
        if (!attr->value().isEmpty())
            addCSSLength(attr, CSSPropertyHeight, attr->value());
    } else
        HTMLElement::parseMappedAttribute(attr);
}

}