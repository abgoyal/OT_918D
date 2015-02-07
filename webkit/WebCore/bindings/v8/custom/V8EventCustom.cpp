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
 * Copyright (C) 2007-2009 Google Inc. All rights reserved.
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
#include "V8Event.h"

#include "Clipboard.h"
#include "ClipboardEvent.h"
#include "Event.h"
#include "V8BeforeLoadEvent.h"
#include "V8Binding.h"
#include "V8Clipboard.h"
#include "V8CompositionEvent.h"
#include "V8ErrorEvent.h"
#include "V8KeyboardEvent.h"
#include "V8MessageEvent.h"
#include "V8MouseEvent.h"
#include "V8MutationEvent.h"
#include "V8OverflowEvent.h"
#include "V8PageTransitionEvent.h"
#include "V8PopStateEvent.h"
#include "V8ProgressEvent.h"
#include "V8Proxy.h"
#include "V8SVGZoomEvent.h"
#include "V8StorageEvent.h"
#include "V8TextEvent.h"
#include "V8TouchEvent.h"
#include "V8UIEvent.h"
#include "V8WebKitAnimationEvent.h"
#include "V8WebKitTransitionEvent.h"
#include "V8WheelEvent.h"
#include "V8XMLHttpRequestProgressEvent.h"

namespace WebCore {

void V8Event::valueAccessorSetter(v8::Local<v8::String> name, v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
    Event* event = V8Event::toNative(info.Holder());
    event->setDefaultPrevented(!value->BooleanValue());
}

v8::Handle<v8::Value> V8Event::dataTransferAccessorGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    Event* event = V8Event::toNative(info.Holder());

    if (event->isDragEvent())
        return toV8(static_cast<MouseEvent*>(event)->clipboard());

    return v8::Undefined();
}

v8::Handle<v8::Value> V8Event::clipboardDataAccessorGetter(v8::Local<v8::String> name, const v8::AccessorInfo& info)
{
    Event* event = V8Event::toNative(info.Holder());

    if (event->isClipboardEvent())
        return toV8(static_cast<ClipboardEvent*>(event)->clipboard());

    return v8::Undefined();
}

v8::Handle<v8::Value> toV8(Event* impl)
{
    if (!impl)
        return v8::Null();
    if (impl->isUIEvent()) {
        if (impl->isKeyboardEvent())
            return toV8(static_cast<KeyboardEvent*>(impl));
        if (impl->isTextEvent())
            return toV8(static_cast<TextEvent*>(impl));
        if (impl->isMouseEvent())
            return toV8(static_cast<MouseEvent*>(impl));
        if (impl->isWheelEvent())
            return toV8(static_cast<WheelEvent*>(impl));
#if ENABLE(SVG)
        if (impl->isSVGZoomEvent())
            return toV8(static_cast<SVGZoomEvent*>(impl));
#endif
        if (impl->isCompositionEvent())
            return toV8(static_cast<CompositionEvent*>(impl));
#if ENABLE(TOUCH_EVENTS)
        if (impl->isTouchEvent())
            return toV8(static_cast<TouchEvent*>(impl));
#endif
        return toV8(static_cast<UIEvent*>(impl));
    }
    if (impl->isMutationEvent())
        return toV8(static_cast<MutationEvent*>(impl));
    if (impl->isOverflowEvent())
        return toV8(static_cast<OverflowEvent*>(impl));
    if (impl->isMessageEvent())
        return toV8(static_cast<MessageEvent*>(impl));
    if (impl->isPageTransitionEvent())
        return toV8(static_cast<PageTransitionEvent*>(impl));
    if (impl->isPopStateEvent())
        return toV8(static_cast<PopStateEvent*>(impl));
    if (impl->isProgressEvent()) {
        if (impl->isXMLHttpRequestProgressEvent())
            return toV8(static_cast<XMLHttpRequestProgressEvent*>(impl));
        return toV8(static_cast<ProgressEvent*>(impl));
    }
    if (impl->isWebKitAnimationEvent())
        return toV8(static_cast<WebKitAnimationEvent*>(impl));
    if (impl->isWebKitTransitionEvent())
        return toV8(static_cast<WebKitTransitionEvent*>(impl));
#if ENABLE(WORKERS)
    if (impl->isErrorEvent())
        return toV8(static_cast<ErrorEvent*>(impl));
#endif
#if ENABLE(DOM_STORAGE)
    if (impl->isStorageEvent())
        return toV8(static_cast<StorageEvent*>(impl));
#endif
    if (impl->isBeforeLoadEvent())
        return toV8(static_cast<BeforeLoadEvent*>(impl));
    return V8Event::wrap(impl);
}
} // namespace WebCore