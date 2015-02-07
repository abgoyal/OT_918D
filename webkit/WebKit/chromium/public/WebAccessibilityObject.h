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

#ifndef WebAccessibilityObject_h
#define WebAccessibilityObject_h

#include "WebAccessibilityRole.h"
#include "WebCommon.h"

#if WEBKIT_IMPLEMENTATION
namespace WebCore { class AccessibilityObject; }
namespace WTF { template <typename T> class PassRefPtr; }
#endif

namespace WebKit {

class WebAccessibilityObjectPrivate;
class WebString;
struct WebPoint;
struct WebRect;

// A container for passing around a reference to AccessibilityObject.
class WebAccessibilityObject {
public:
    ~WebAccessibilityObject() { reset(); }

    WebAccessibilityObject() : m_private(0) { }
    WebAccessibilityObject(const WebAccessibilityObject& o) : m_private(0) { assign(o); }
    WebAccessibilityObject& operator=(const WebAccessibilityObject& o)
    {
        assign(o);
        return *this;
    }

    WEBKIT_API void reset();
    WEBKIT_API void assign(const WebAccessibilityObject&);

    bool isNull() const { return !m_private; }

    WebString accessibilityDescription() const;
    WebString actionVerb() const;
    bool canSetFocusAttribute() const;
    bool canSetValueAttribute() const;

    unsigned childCount() const;

    WebAccessibilityObject childAt(unsigned) const;
    WebAccessibilityObject firstChild() const;
    WebAccessibilityObject focusedChild() const;
    WebAccessibilityObject lastChild() const;
    WebAccessibilityObject nextSibling() const;
    WebAccessibilityObject parentObject() const;
    WebAccessibilityObject previousSibling() const;

    bool isAnchor() const;
    bool isChecked() const;
    bool isFocused() const;
    bool isEnabled() const;
    bool isHovered() const;
    bool isIndeterminate() const;
    bool isMultiSelectable() const;
    bool isOffScreen() const;
    bool isPasswordField() const;
    bool isPressed() const;
    bool isReadOnly() const;
    bool isVisited() const;

    WebRect boundingBoxRect() const;
    WebString helpText() const;
    WebAccessibilityObject hitTest(const WebPoint&) const;
    WebString keyboardShortcut() const;
    bool performDefaultAction() const;
    WebAccessibilityRole roleValue() const;
    WebString stringValue() const;
    WebString title() const;

#if WEBKIT_IMPLEMENTATION
    WebAccessibilityObject(const WTF::PassRefPtr<WebCore::AccessibilityObject>&);
    WebAccessibilityObject& operator=(const WTF::PassRefPtr<WebCore::AccessibilityObject>&);
    operator WTF::PassRefPtr<WebCore::AccessibilityObject>() const;
#endif

private:
    void assign(WebAccessibilityObjectPrivate*);
    WebAccessibilityObjectPrivate* m_private;
};

} // namespace WebKit

#endif
