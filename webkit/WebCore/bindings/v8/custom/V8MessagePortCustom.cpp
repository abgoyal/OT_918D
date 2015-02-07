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

#include "ExceptionCode.h"
#include "MessagePort.h"
#include "SerializedScriptValue.h"
#include "V8Binding.h"
#include "V8MessagePortCustom.h"
#include "V8MessagePort.h"
#include "V8Proxy.h"
#include "V8Utilities.h"
#include "WorkerContextExecutionProxy.h"

namespace WebCore {

v8::Handle<v8::Value> V8MessagePort::addEventListenerCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.MessagePort.addEventListener()");
    MessagePort* messagePort = V8MessagePort::toNative(args.Holder());
    RefPtr<EventListener> listener = V8DOMWrapper::getEventListener(messagePort, args[1], false, ListenerFindOrCreate);
    if (listener) {
        String type = toWebCoreString(args[0]);
        bool useCapture = args[2]->BooleanValue();
        messagePort->addEventListener(type, listener, useCapture);

        createHiddenDependency(args.Holder(), args[1], cacheIndex);
    }
    return v8::Undefined();
}

v8::Handle<v8::Value> V8MessagePort::removeEventListenerCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.MessagePort.removeEventListener()");
    MessagePort* messagePort = V8MessagePort::toNative(args.Holder());
    RefPtr<EventListener> listener = V8DOMWrapper::getEventListener(messagePort, args[1], false, ListenerFindOnly);
    if (listener) {
        String type = toWebCoreString(args[0]);
        bool useCapture = args[2]->BooleanValue();
        messagePort->removeEventListener(type, listener.get(), useCapture);

        removeHiddenDependency(args.Holder(), args[1], cacheIndex);
    }

    return v8::Undefined();
}

v8::Handle<v8::Value> V8MessagePort::postMessageCallback(const v8::Arguments& args)
{
    INC_STATS("DOM.MessagePort.postMessage");
    MessagePort* messagePort = V8MessagePort::toNative(args.Holder());
    RefPtr<SerializedScriptValue> message = SerializedScriptValue::create(args[0]);
    MessagePortArray portArray;
    if (args.Length() > 1) {
        if (!getMessagePortArray(args[1], portArray))
            return v8::Undefined();
    }
    ExceptionCode ec = 0;
    messagePort->postMessage(message.release(), &portArray, ec);
    return throwError(ec);
}

bool getMessagePortArray(v8::Local<v8::Value> value, MessagePortArray& portArray)
{
    if (isUndefinedOrNull(value)) {
        portArray.resize(0);
        return true;
    }

    if (!value->IsObject()) {
        throwError("MessagePortArray argument must be an object");
        return false;
    }
    uint32_t length = 0;
    v8::Local<v8::Object> ports = v8::Local<v8::Object>::Cast(value);

    if (value->IsArray()) {
        v8::Local<v8::Array> array = v8::Local<v8::Array>::Cast(value);
        length = array->Length();
    } else {
        // Sequence-type object - get the length attribute
        v8::Local<v8::Value> sequenceLength = ports->Get(v8::String::New("length"));
        if (!sequenceLength->IsNumber()) {
            throwError("MessagePortArray argument has no length attribute");
            return false;
        }
        length = sequenceLength->Uint32Value();
    }
    portArray.resize(length);

    for (unsigned int i = 0; i < length; ++i) {
        v8::Local<v8::Value> port = ports->Get(v8::Integer::New(i));
        // Validation of non-null objects, per HTML5 spec 8.3.3.
        if (isUndefinedOrNull(port)) {
            throwError(INVALID_STATE_ERR);
            return false;
        }
        // Validation of Objects implementing an interface, per WebIDL spec 4.1.15.
        if (!V8MessagePort::HasInstance(port)) {
            throwError("MessagePortArray argument must contain only MessagePorts");
            return false;
        }
        portArray[i] = V8MessagePort::toNative(v8::Handle<v8::Object>::Cast(port));
    }
    return true;
}

} // namespace WebCore