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
    Copyright (C) 2009 Nokia Corporation and/or its subsidiary(-ies)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#include "qscriptengine.h"

#include "qscriptengine_p.h"
#include "qscriptvalue_p.h"

/*!
    Constructs a QScriptEngine object.

    The globalObject() is initialized to have properties as described in ECMA-262, Section 15.1.
*/
QScriptEngine::QScriptEngine()
    : d_ptr(new QScriptEnginePrivate(this))
{
}

/*!
    Destroys this QScriptEngine.
*/
QScriptEngine::~QScriptEngine()
{
}

/*!
    Evaluates \a program, using \a lineNumber as the base line number,
    and returns the result of the evaluation.

    The script code will be evaluated in the current context.

    The evaluation of \a program can cause an exception in the
    engine; in this case the return value will be the exception
    that was thrown (typically an \c{Error} object). You can call
    hasUncaughtException() to determine if an exception occurred in
    the last call to evaluate().

    \a lineNumber is used to specify a starting line number for \a
    program; line number information reported by the engine that pertain
    to this evaluation (e.g. uncaughtExceptionLineNumber()) will be
    based on this argument. For example, if \a program consists of two
    lines of code, and the statement on the second line causes a script
    exception, uncaughtExceptionLineNumber() would return the given \a
    lineNumber plus one. When no starting line number is specified, line
    numbers will be 1-based.

    \a fileName is used for error reporting. For example in error objects
    the file name is accessible through the "fileName" property if it's
    provided with this function.
*/
QScriptValue QScriptEngine::evaluate(const QString& program, const QString& fileName, int lineNumber)
{
    return QScriptValuePrivate::get(d_ptr->evaluate(program, fileName, lineNumber));
}

/*!
    Runs the garbage collector.

    The garbage collector will attempt to reclaim memory by locating and disposing of objects that are
    no longer reachable in the script environment.

    Normally you don't need to call this function; the garbage collector will automatically be invoked
    when the QScriptEngine decides that it's wise to do so (i.e. when a certain number of new objects
    have been created). However, you can call this function to explicitly request that garbage
    collection should be performed as soon as possible.
*/
void QScriptEngine::collectGarbage()
{
    d_ptr->collectGarbage();
}

/*!
  Returns a QScriptValue of the primitive type Null.

  \sa undefinedValue()
*/
QScriptValue QScriptEngine::nullValue()
{
    return QScriptValue(this, QScriptValue::NullValue);
}

/*!
  Returns a QScriptValue of the primitive type Undefined.

  \sa nullValue()
*/
QScriptValue QScriptEngine::undefinedValue()
{
    return QScriptValue(this, QScriptValue::UndefinedValue);
}
