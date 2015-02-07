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
 * Copyright (C) 2007 Apple Inc. All rights reserved.
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
#include "JSSQLTransaction.h"

#if ENABLE(DATABASE)

#include "DOMWindow.h"
#include "ExceptionCode.h"
#include "JSCustomSQLStatementCallback.h"
#include "JSCustomSQLStatementErrorCallback.h"
#include "JSDOMWindowCustom.h"
#include "SQLTransaction.h"

using namespace JSC;

namespace WebCore {
    
JSValue JSSQLTransaction::executeSql(ExecState* exec, const ArgList& args)
{
    if (args.isEmpty()) {
        setDOMException(exec, SYNTAX_ERR);
        return jsUndefined();
    }

    String sqlStatement = args.at(0).toString(exec);
    if (exec->hadException())
        return jsUndefined();

    // Now assemble the list of SQL arguments
    Vector<SQLValue> sqlValues;
    if (!args.at(1).isUndefinedOrNull()) {
        JSObject* object = args.at(1).getObject();
        if (!object) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }

        JSValue lengthValue = object->get(exec, exec->propertyNames().length);
        if (exec->hadException())
            return jsUndefined();
        unsigned length = lengthValue.toUInt32(exec);
        if (exec->hadException())
            return jsUndefined();
        
        for (unsigned i = 0 ; i < length; ++i) {
            JSValue value = object->get(exec, i);
            if (exec->hadException())
                return jsUndefined();
            
            if (value.isNull())
                sqlValues.append(SQLValue());
            else if (value.isNumber())
                sqlValues.append(value.uncheckedGetNumber());
            else {
                // Convert the argument to a string and append it
                sqlValues.append(value.toString(exec));
                if (exec->hadException())
                    return jsUndefined();
            }
        }
    }

    RefPtr<SQLStatementCallback> callback;
    if (!args.at(2).isUndefinedOrNull()) {
        JSObject* object = args.at(2).getObject();
        if (!object) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }
        
        callback = JSCustomSQLStatementCallback::create(object, static_cast<JSDOMGlobalObject*>(exec->dynamicGlobalObject()));
    }
    
    RefPtr<SQLStatementErrorCallback> errorCallback;
    if (!args.at(3).isUndefinedOrNull()) {
        JSObject* object = args.at(3).getObject();
        if (!object) {
            setDOMException(exec, TYPE_MISMATCH_ERR);
            return jsUndefined();
        }
        
        errorCallback = JSCustomSQLStatementErrorCallback::create(object, static_cast<JSDOMGlobalObject*>(exec->dynamicGlobalObject()));
    }
    
    ExceptionCode ec = 0;
    m_impl->executeSQL(sqlStatement, sqlValues, callback.release(), errorCallback.release(), ec);
    setDOMException(exec, ec);
    
    return jsUndefined();
}

}

#endif // ENABLE(DATABASE)