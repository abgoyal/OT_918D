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
#include "WebNode.h"

#include "Document.h"
#include "Frame.h"
#include "FrameLoaderClientImpl.h"
#include "Node.h"
#include "NodeList.h"

#include "EventListenerWrapper.h"
#include "WebDocument.h"
#include "WebEvent.h"
#include "WebEventListener.h"
#include "WebFrameImpl.h"
#include "WebNodeList.h"
#include "WebString.h"
#include "WebVector.h"

#include "markup.h"

#include <wtf/PassRefPtr.h>

using namespace WebCore;

namespace WebKit {

class WebNodePrivate : public Node {
};

void WebNode::reset()
{
    assign(0);
}

void WebNode::assign(const WebNode& other)
{
    WebNodePrivate* p = const_cast<WebNodePrivate*>(other.m_private);
    if (p)
        p->ref();
    assign(p);
}

WebNode::NodeType WebNode::nodeType() const
{
    return static_cast<NodeType>(m_private->nodeType());
}

WebNode WebNode::parentNode() const
{
    return PassRefPtr<Node>(const_cast<Node*>(m_private->parentNode()));
}

WebString WebNode::nodeName() const
{
    return m_private->nodeName();
}

WebString WebNode::nodeValue() const
{
    return m_private->nodeValue();
}

bool WebNode::setNodeValue(const WebString& value)
{
    ExceptionCode exceptionCode = 0;
    m_private->setNodeValue(value, exceptionCode);
    return !exceptionCode;
}

WebNode::WebNode(const PassRefPtr<Node>& node)
    : m_private(static_cast<WebNodePrivate*>(node.releaseRef()))
{
}

WebNode& WebNode::operator=(const PassRefPtr<Node>& node)
{
    assign(static_cast<WebNodePrivate*>(node.releaseRef()));
    return *this;
}

WebNode::operator PassRefPtr<Node>() const
{
    return PassRefPtr<Node>(const_cast<WebNodePrivate*>(m_private));
}

void WebNode::assign(WebNodePrivate* p)
{
    // p is already ref'd for us by the caller
    if (m_private)
        m_private->deref();
    m_private = p;
}

WebFrame* WebNode::frame() const
{
    return WebFrameImpl::fromFrame(m_private->document()->frame());
}

WebDocument WebNode::document() const
{
    return WebDocument(m_private->document());
}

WebNode WebNode::firstChild() const
{
    return WebNode(m_private->firstChild());
}

WebNode WebNode::lastChild() const
{
    return WebNode(m_private->lastChild());
}

WebNode WebNode::previousSibling() const
{
    return WebNode(m_private->previousSibling());
}

WebNode WebNode::nextSibling() const
{
    return WebNode(m_private->nextSibling());
}

bool WebNode::hasChildNodes() const
{
    return m_private->hasChildNodes();
}

WebNodeList WebNode::childNodes()
{
    return WebNodeList(m_private->childNodes());
}

WebString WebNode::createMarkup() const
{
    return WebCore::createMarkup(m_private);
}

bool WebNode::isTextNode() const
{
    return m_private->isTextNode();
}

bool WebNode::isElementNode() const
{
    return m_private->isElementNode();
}

void WebNode::addEventListener(const WebString& eventType, WebEventListener* listener, bool useCapture)
{
    EventListenerWrapper* listenerWrapper =
        listener->createEventListenerWrapper(eventType, useCapture, m_private);
    // The listenerWrapper is only referenced by the actual Node.  Once it goes
    // away, the wrapper notifies the WebEventListener so it can clear its
    // pointer to it.
    m_private->addEventListener(eventType, adoptRef(listenerWrapper), useCapture);
}

void WebNode::removeEventListener(const WebString& eventType, WebEventListener* listener, bool useCapture)
{
    EventListenerWrapper* listenerWrapper =
        listener->getEventListenerWrapper(eventType, useCapture, m_private);
    m_private->removeEventListener(eventType, listenerWrapper, useCapture);
    // listenerWrapper is now deleted.
}

} // namespace WebKit
