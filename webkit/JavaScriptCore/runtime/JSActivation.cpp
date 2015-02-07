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
 * Copyright (C) 2008, 2009 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
 
#include "config.h"
#include "JSActivation.h"

#include "Arguments.h"
#include "Interpreter.h"
#include "JSFunction.h"

namespace JSC {

ASSERT_CLASS_FITS_IN_CELL(JSActivation);

const ClassInfo JSActivation::info = { "JSActivation", 0, 0, 0 };

JSActivation::JSActivation(CallFrame* callFrame, NonNullPassRefPtr<FunctionExecutable> functionExecutable)
    : Base(callFrame->globalData().activationStructure, new JSActivationData(functionExecutable, callFrame->registers()))
{
}

JSActivation::~JSActivation()
{
    delete d();
}

void JSActivation::markChildren(MarkStack& markStack)
{
    Base::markChildren(markStack);

    Register* registerArray = d()->registerArray.get();
    if (!registerArray)
        return;

    size_t numParametersMinusThis = d()->functionExecutable->parameterCount();

    size_t count = numParametersMinusThis;
    markStack.appendValues(registerArray, count);

    size_t numVars = d()->functionExecutable->variableCount();

    // Skip the call frame, which sits between the parameters and vars.
    markStack.appendValues(registerArray + count + RegisterFile::CallFrameHeaderSize, numVars, MayContainNullValues);
}

bool JSActivation::getOwnPropertySlot(ExecState* exec, const Identifier& propertyName, PropertySlot& slot)
{
    if (symbolTableGet(propertyName, slot))
        return true;

    if (JSValue* location = getDirectLocation(propertyName)) {
        slot.setValueSlot(location);
        return true;
    }

    // Only return the built-in arguments object if it wasn't overridden above.
    if (propertyName == exec->propertyNames().arguments) {
        slot.setCustom(this, getArgumentsGetter());
        return true;
    }

    // We don't call through to JSObject because there's no way to give an 
    // activation object getter properties or a prototype.
    ASSERT(!hasGetterSetterProperties());
    ASSERT(prototype().isNull());
    return false;
}

void JSActivation::put(ExecState*, const Identifier& propertyName, JSValue value, PutPropertySlot& slot)
{
    ASSERT(!Heap::heap(value) || Heap::heap(value) == Heap::heap(this));

    if (symbolTablePut(propertyName, value))
        return;

    // We don't call through to JSObject because __proto__ and getter/setter 
    // properties are non-standard extensions that other implementations do not
    // expose in the activation object.
    ASSERT(!hasGetterSetterProperties());
    putDirect(propertyName, value, 0, true, slot);
}

// FIXME: Make this function honor ReadOnly (const) and DontEnum
void JSActivation::putWithAttributes(ExecState* exec, const Identifier& propertyName, JSValue value, unsigned attributes)
{
    ASSERT(!Heap::heap(value) || Heap::heap(value) == Heap::heap(this));

    if (symbolTablePutWithAttributes(propertyName, value, attributes))
        return;

    // We don't call through to JSObject because __proto__ and getter/setter 
    // properties are non-standard extensions that other implementations do not
    // expose in the activation object.
    ASSERT(!hasGetterSetterProperties());
    PutPropertySlot slot;
    JSObject::putWithAttributes(exec, propertyName, value, attributes, true, slot);
}

bool JSActivation::deleteProperty(ExecState* exec, const Identifier& propertyName)
{
    if (propertyName == exec->propertyNames().arguments)
        return false;

    return Base::deleteProperty(exec, propertyName);
}

JSObject* JSActivation::toThisObject(ExecState* exec) const
{
    return exec->globalThisValue();
}

bool JSActivation::isDynamicScope() const
{
    return d()->functionExecutable->usesEval();
}

JSValue JSActivation::argumentsGetter(ExecState* exec, const Identifier&, const PropertySlot& slot)
{
    JSActivation* activation = asActivation(slot.slotBase());

    if (activation->d()->functionExecutable->usesArguments()) {
        PropertySlot slot;
        activation->symbolTableGet(exec->propertyNames().arguments, slot);
        return slot.getValue(exec, exec->propertyNames().arguments);
    }

    CallFrame* callFrame = CallFrame::create(activation->d()->registers);
    Arguments* arguments = callFrame->optionalCalleeArguments();
    if (!arguments) {
        arguments = new (callFrame) Arguments(callFrame);
        arguments->copyRegisters();
        callFrame->setCalleeArguments(arguments);
    }
    ASSERT(arguments->inherits(&Arguments::info));

    return arguments;
}

// These two functions serve the purpose of isolating the common case from a
// PIC branch.

PropertySlot::GetValueFunc JSActivation::getArgumentsGetter()
{
    return argumentsGetter;
}

} // namespace JSC
