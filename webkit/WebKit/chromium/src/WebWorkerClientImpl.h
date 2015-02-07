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

#ifndef WebWorkerClientImpl_h
#define WebWorkerClientImpl_h

#if ENABLE(WORKERS)

// FIXME: fix to just "WebWorkerClient.h" once nobody in glue depends on us.
#include "../public/WebWorkerClient.h"

#include "WorkerContextProxy.h"
#include <wtf/PassOwnPtr.h>
#include <wtf/RefPtr.h>

namespace WebCore {
class ScriptExecutionContext;
}

namespace WebKit {
class WebWorker;

// The purpose of this class is to provide a WorkerContextProxy
// implementation that we can give to WebKit.  Internally, it converts the
// data types to Chrome compatible ones so that renderer code can use it over
// IPC.
class WebWorkerClientImpl : public WebCore::WorkerContextProxy
                          , public WebWorkerClient {
public:
    WebWorkerClientImpl(WebCore::Worker*);

    // WebCore::WorkerContextProxy Factory.
    static WebCore::WorkerContextProxy* createWorkerContextProxy(WebCore::Worker*);
    void setWebWorker(WebWorker*);

    // WebCore::WorkerContextProxy methods:
    // These are called on the thread that created the worker.  In the renderer
    // process, this will be the main WebKit thread.  In the worker process, this
    // will be the thread of the executing worker (not the main WebKit thread).
    virtual void startWorkerContext(const WebCore::KURL&,
                                    const WebCore::String&,
                                    const WebCore::String&);
    virtual void terminateWorkerContext();
    virtual void postMessageToWorkerContext(
        PassRefPtr<WebCore::SerializedScriptValue> message,
        PassOwnPtr<WebCore::MessagePortChannelArray> channels);
    virtual bool hasPendingActivity() const;
    virtual void workerObjectDestroyed();

    // WebWorkerClient methods:
    // These are called on the main WebKit thread.
    virtual void postMessageToWorkerObject(const WebString&, const WebMessagePortChannelArray&);
    virtual void postExceptionToWorkerObject(const WebString&, int, const WebString&);
    virtual void postConsoleMessageToWorkerObject(int, int, int, int, const WebString&,
                                                  int, const WebString&);
    virtual void confirmMessageFromWorkerObject(bool);
    virtual void reportPendingActivity(bool);
    virtual void workerContextClosed();
    virtual void workerContextDestroyed();
    virtual WebWorker* createWorker(WebWorkerClient*) { return 0; }
    virtual WebNotificationPresenter* notificationPresenter()
    {
        // FIXME: Notifications not yet supported in workers.
        return 0;
    }

private:
    virtual ~WebWorkerClientImpl();

    // Methods used to support WebWorkerClientImpl being constructed on worker
    // threads.
    // These tasks are dispatched on the WebKit thread.
    static void startWorkerContextTask(WebCore::ScriptExecutionContext* context,
                                       WebWorkerClientImpl* thisPtr,
                                       const WebCore::String& scriptURL,
                                       const WebCore::String& userAgent,
                                       const WebCore::String& sourceCode);
    static void terminateWorkerContextTask(WebCore::ScriptExecutionContext* context,
                                           WebWorkerClientImpl* thisPtr);
    static void postMessageToWorkerContextTask(WebCore::ScriptExecutionContext* context,
                                               WebWorkerClientImpl* thisPtr,
                                               const WebCore::String& message,
                                               PassOwnPtr<WebCore::MessagePortChannelArray> channels);
    static void workerObjectDestroyedTask(WebCore::ScriptExecutionContext* context,
                                          WebWorkerClientImpl* thisPtr);

    // These tasks are dispatched on the thread that created the worker (i.e.
    // main WebKit thread in renderer process, and the worker thread in the
    // worker process).
    static void postMessageToWorkerObjectTask(WebCore::ScriptExecutionContext* context,
                                              WebWorkerClientImpl* thisPtr,
                                              const WebCore::String& message,
                                              PassOwnPtr<WebCore::MessagePortChannelArray> channels);
    static void postExceptionToWorkerObjectTask(WebCore::ScriptExecutionContext* context,
                                                WebWorkerClientImpl* thisPtr,
                                                const WebCore::String& message,
                                                int lineNumber,
                                                const WebCore::String& sourceURL);
    static void postConsoleMessageToWorkerObjectTask(WebCore::ScriptExecutionContext* context,
                                                     WebWorkerClientImpl* thisPtr,
                                                     int destinationId,
                                                     int sourceId,
                                                     int messageType,
                                                     int messageLevel,
                                                     const WebCore::String& message,
                                                     int lineNumber,
                                                     const WebCore::String& sourceURL);
    static void confirmMessageFromWorkerObjectTask(WebCore::ScriptExecutionContext* context,
                                                   WebWorkerClientImpl* thisPtr);
    static void reportPendingActivityTask(WebCore::ScriptExecutionContext* context,
                                          WebWorkerClientImpl* thisPtr,
                                          bool hasPendingActivity);

    // Guard against context from being destroyed before a worker exits.
    RefPtr<WebCore::ScriptExecutionContext> m_scriptExecutionContext;

    WebCore::Worker* m_worker;
    WebWorker* m_webWorker;
    bool m_askedToTerminate;
    unsigned m_unconfirmedMessageCount;
    bool m_workerContextHadPendingActivity;
    ThreadIdentifier m_workerThreadId;
};

} // namespace WebKit;

#endif // ENABLE(WORKERS)

#endif
