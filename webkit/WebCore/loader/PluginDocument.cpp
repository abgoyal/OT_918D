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
 * Copyright (C) 2006, 2008 Apple Inc. All rights reserved.
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
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"
#include "PluginDocument.h"

#include "DocumentLoader.h"
#include "Element.h"
#include "Frame.h"
#include "FrameLoader.h"
#include "FrameLoaderClient.h"
#include "HTMLEmbedElement.h"
#include "HTMLNames.h"
#include "MainResourceLoader.h"
#include "Page.h"
#include "RenderWidget.h"
#include "SegmentedString.h"
#include "Settings.h"
#include "Text.h"
#include "XMLTokenizer.h"

namespace WebCore {
    
using namespace HTMLNames;
    
class PluginTokenizer : public Tokenizer {
public:
    PluginTokenizer(Document* doc) : m_doc(doc), m_embedElement(0) {}
        
private:
    virtual void write(const SegmentedString&, bool appendData);
    virtual void stopParsing();
    virtual void finish();
    virtual bool isWaitingForScripts() const;
        
    virtual bool wantsRawData() const { return true; }
    virtual bool writeRawData(const char* data, int len);
        
    void createDocumentStructure();

    Document* m_doc;
    HTMLEmbedElement* m_embedElement;
};
    
void PluginTokenizer::write(const SegmentedString&, bool)
{
    ASSERT_NOT_REACHED();
}
    
void PluginTokenizer::createDocumentStructure()
{
    ExceptionCode ec;
    RefPtr<Element> rootElement = m_doc->createElement(htmlTag, false);
    m_doc->appendChild(rootElement, ec);

    RefPtr<Element> body = m_doc->createElement(bodyTag, false);
    body->setAttribute(marginwidthAttr, "0");
    body->setAttribute(marginheightAttr, "0");
    body->setAttribute(bgcolorAttr, "rgb(38,38,38)");

    rootElement->appendChild(body, ec);
        
    RefPtr<Element> embedElement = m_doc->createElement(embedTag, false);
        
    m_embedElement = static_cast<HTMLEmbedElement*>(embedElement.get());
    m_embedElement->setAttribute(widthAttr, "100%");
    m_embedElement->setAttribute(heightAttr, "100%");
    
    m_embedElement->setAttribute(nameAttr, "plugin");
    m_embedElement->setAttribute(srcAttr, m_doc->url().string());
    m_embedElement->setAttribute(typeAttr, m_doc->frame()->loader()->responseMIMEType());
    
    body->appendChild(embedElement, ec);    
}
    
bool PluginTokenizer::writeRawData(const char*, int)
{
    ASSERT(!m_embedElement);
    if (m_embedElement)
        return false;
    
    createDocumentStructure();

    if (Frame* frame = m_doc->frame()) {
        Settings* settings = frame->settings();
        if (settings && settings->arePluginsEnabled()) {
            m_doc->updateLayout();

            if (RenderWidget* renderer = toRenderWidget(m_embedElement->renderer())) {
                frame->loader()->client()->redirectDataToPlugin(renderer->widget());
                frame->loader()->activeDocumentLoader()->mainResourceLoader()->setShouldBufferData(false);
            }

            finish();
        }
    }

    return false;
}
    
void PluginTokenizer::stopParsing()
{
    Tokenizer::stopParsing();        
}
    
void PluginTokenizer::finish()
{
    if (!m_parserStopped) 
        m_doc->finishedParsing();            
}
    
bool PluginTokenizer::isWaitingForScripts() const
{
    // A plugin document is never waiting for scripts
    return false;
}
    
PluginDocument::PluginDocument(Frame* frame)
    : HTMLDocument(frame)
{
    setParseMode(Compat);
}
    
Tokenizer* PluginDocument::createTokenizer()
{
    return new PluginTokenizer(this);
}
    
}
