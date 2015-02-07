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
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

// This all-in-one cpp file cuts down on template bloat to allow us to build our Windows release build.

#include "GCController.cpp"
#include "JSAbstractWorkerCustom.cpp"
#include "JSAttrCustom.cpp"
#include "JSAudioConstructor.cpp"
#include "JSCDATASectionCustom.cpp"
#include "JSCSSRuleCustom.cpp"
#include "JSCSSRuleListCustom.cpp"
#include "JSCSSStyleDeclarationCustom.cpp"
#include "JSCSSValueCustom.cpp"
#include "JSCallbackData.cpp"
#include "JSCanvasRenderingContext2DCustom.cpp"
#include "JSCanvasRenderingContextCustom.cpp"
#include "JSClipboardCustom.cpp"
#include "JSConsoleCustom.cpp"
#include "JSCoordinatesCustom.cpp"
#include "JSCustomSQLStatementCallback.cpp"
#include "JSCustomSQLStatementErrorCallback.cpp"
#include "JSCustomSQLTransactionCallback.cpp"
#include "JSCustomSQLTransactionErrorCallback.cpp"
#include "JSCustomVoidCallback.cpp"
#include "JSCustomXPathNSResolver.cpp"
#include "JSDOMApplicationCacheCustom.cpp"
#include "JSDOMBinding.cpp"
#include "JSDOMGlobalObject.cpp"
#include "JSDOMWindowBase.cpp"
#include "JSDOMWindowCustom.cpp"
#include "JSDOMWindowShell.cpp"
#include "JSDataGridColumnListCustom.cpp"
#include "JSDataGridDataSource.cpp"
#include "JSDatabaseCustom.cpp"
#include "JSDedicatedWorkerContextCustom.cpp"
#include "JSDesktopNotificationsCustom.cpp"
#include "JSDocumentCustom.cpp"
#include "JSDocumentFragmentCustom.cpp"
#include "JSElementCustom.cpp"
#include "JSEventCustom.cpp"
#include "JSEventListener.cpp"
#include "JSEventSourceConstructor.cpp"
#include "JSEventSourceCustom.cpp"
#include "JSEventTarget.cpp"
#include "JSExceptionBase.cpp"
#include "JSHTMLAllCollectionCustom.cpp"
#include "JSHTMLAppletElementCustom.cpp"
#include "JSHTMLCanvasElementCustom.cpp"
#include "JSHTMLCollectionCustom.cpp"
#include "JSHTMLDataGridElementCustom.cpp"
#include "JSHTMLDocumentCustom.cpp"
#include "JSHTMLElementCustom.cpp"
#include "JSHTMLEmbedElementCustom.cpp"
#include "JSHTMLFormElementCustom.cpp"
#include "JSHTMLFrameElementCustom.cpp"
#include "JSHTMLFrameSetElementCustom.cpp"
#include "JSHTMLIFrameElementCustom.cpp"
#include "JSHTMLInputElementCustom.cpp"
#include "JSHTMLObjectElementCustom.cpp"
#include "JSHTMLOptionsCollectionCustom.cpp"
#include "JSHTMLSelectElementCustom.cpp"
#include "JSHistoryCustom.cpp"
#include "JSImageConstructor.cpp"
#include "JSImageDataCustom.cpp"
#include "JSInjectedScriptHostCustom.cpp"
#include "JSInspectorFrontendHostCustom.cpp"
#include "JSJavaScriptCallFrameCustom.cpp"
#include "JSLazyEventListener.cpp"
#include "JSLocationCustom.cpp"
#include "JSMessageChannelConstructor.cpp"
#include "JSMessageChannelCustom.cpp"
#include "JSMessageEventCustom.cpp"
#include "JSMessagePortCustom.cpp"
#include "JSMimeTypeArrayCustom.cpp"
#include "JSNamedNodeMapCustom.cpp"
#include "JSNavigatorCustom.cpp"
#include "JSNodeCustom.cpp"
#include "JSNodeFilterCondition.cpp"
#include "JSNodeFilterCustom.cpp"
#include "JSNodeIteratorCustom.cpp"
#include "JSNodeListCustom.cpp"
#include "JSOptionConstructor.cpp"
#include "JSPluginArrayCustom.cpp"
#include "JSPluginCustom.cpp"
#include "JSPluginElementFunctions.cpp"
#include "JSSQLResultSetRowListCustom.cpp"
#include "JSSQLTransactionCustom.cpp"
#include "JSSVGElementInstanceCustom.cpp"
#include "JSSVGLengthCustom.cpp"
#include "JSSVGMatrixCustom.cpp"
#include "JSSVGPathSegCustom.cpp"
#include "JSSVGPathSegListCustom.cpp"
#include "JSSharedWorkerConstructor.cpp"
#include "JSSharedWorkerCustom.cpp"
#include "JSStorageCustom.cpp"
#include "JSStyleSheetCustom.cpp"
#include "JSStyleSheetListCustom.cpp"
#include "JSTextCustom.cpp"
#include "JSTouchCustom.cpp"
#include "JSTouchListCustom.cpp"
#include "JSTreeWalkerCustom.cpp"
#include "JSWebKitCSSMatrixConstructor.cpp"
#include "JSWebKitPointConstructor.cpp"
#include "JSWebSocketConstructor.cpp"
#include "JSWebSocketCustom.cpp"
#include "JSWorkerConstructor.cpp"
#include "JSWorkerContextBase.cpp"
#include "JSWorkerContextCustom.cpp"
#include "JSWorkerCustom.cpp"
#include "JSXMLHttpRequestConstructor.cpp"
#include "JSXMLHttpRequestCustom.cpp"
#include "JSXMLHttpRequestUploadCustom.cpp"
#include "JSXSLTProcessorConstructor.cpp"
#include "JSXSLTProcessorCustom.cpp"
#include "ScheduledAction.cpp"
#include "ScriptArray.cpp"
#include "ScriptCachedFrameData.cpp"
#include "ScriptCallFrame.cpp"
#include "ScriptCallStack.cpp"
#include "ScriptController.cpp"
#include "ScriptControllerWin.cpp"
#include "ScriptDebugServer.cpp"
#include "ScriptEventListener.cpp"
#include "ScriptFunctionCall.cpp"
#include "ScriptProfiler.cpp"
#include "ScriptState.cpp"
#include "SerializedScriptValue.cpp"
#include "WorkerScriptController.cpp"
