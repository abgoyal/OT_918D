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
 * Copyright (C) 2009 Apple Inc. All rights reserved.
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
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef JSWebGLArrayBufferConstructor_h
#define JSWebGLArrayBufferConstructor_h

#include "JSDOMBinding.h"
#include "JSDocument.h"
#include "JSWebGLArrayBuffer.h"
#include <runtime/Error.h>

namespace WebCore {

    class WebGLArray;

    // Template function used by CanvasXXXArrayConstructors
    template<class C, typename T>
    PassRefPtr<WebGLArray> construct(JSC::ExecState* exec, const JSC::ArgList& args)
    {
        // There are 3 constructors:
        //
        //  1) (in int size)
        //  2) (in WebGLArrayBuffer buffer, [Optional] in int offset, [Optional] in unsigned int length)
        //  3) (in sequence<T>) - This ends up being a JS "array-like" object
        //
        RefPtr<C> arrayObject;
        
        // For the 0 args case, just create an object without a buffer 
        if (args.size() < 1)
            return C::create(0, 0, 0);
        
        if (args.at(0).isObject()) {
            RefPtr<WebGLArrayBuffer> buffer = toWebGLArrayBuffer(args.at(0));
            if (buffer) {
                int offset = (args.size() > 1) ? args.at(1).toInt32(exec) : 0;
                unsigned int length = (args.size() > 2) ? static_cast<unsigned int>(args.at(2).toInt32(exec)) : 0;
                return C::create(buffer, offset, length);
            }
            
            JSC::JSObject* array = asObject(args.at(0));
            int length = array->get(exec, JSC::Identifier(exec, "length")).toInt32(exec);
            void* tempValues;
            if (!tryFastMalloc(length * sizeof(T)).getValue(tempValues)) {
                throwError(exec, JSC::GeneralError);
                return 0;
            }
            
            OwnFastMallocPtr<T> values(static_cast<T*>(tempValues));
            for (int i = 0; i < length; ++i) {
                JSC::JSValue v = array->get(exec, i);
                if (exec->hadException())
                    return 0;
                values.get()[i] = static_cast<T>(v.toNumber(exec));
            }
            
            return C::create(values.get(), length);
        }
        
        unsigned size = static_cast<unsigned>(args.at(0).toInt32(exec));
        return C::create(size);
    }

    class JSWebGLArrayBufferConstructor : public DOMConstructorObject {
    public:
        JSWebGLArrayBufferConstructor(JSC::ExecState*, JSDOMGlobalObject*);
        static const JSC::ClassInfo s_info;

    private:
        virtual JSC::ConstructType getConstructData(JSC::ConstructData&);
        virtual const JSC::ClassInfo* classInfo() const { return &s_info; }
    };

}

#endif // JSWebGLArrayBufferConstructor_h
