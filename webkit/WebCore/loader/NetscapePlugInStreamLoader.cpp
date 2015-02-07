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
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
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
#include "NetscapePlugInStreamLoader.h"

#include "FrameLoader.h"
#include "DocumentLoader.h"

namespace WebCore {

NetscapePlugInStreamLoader::NetscapePlugInStreamLoader(Frame* frame, NetscapePlugInStreamLoaderClient* client)
    : ResourceLoader(frame, true, true)
    , m_client(client)
{
}

NetscapePlugInStreamLoader::~NetscapePlugInStreamLoader()
{
}

PassRefPtr<NetscapePlugInStreamLoader> NetscapePlugInStreamLoader::create(Frame* frame, NetscapePlugInStreamLoaderClient* client)
{
    return adoptRef(new NetscapePlugInStreamLoader(frame, client));
}

bool NetscapePlugInStreamLoader::isDone() const
{
    return !m_client;
}

void NetscapePlugInStreamLoader::releaseResources()
{
    m_client = 0;
    ResourceLoader::releaseResources();
}

void NetscapePlugInStreamLoader::didReceiveResponse(const ResourceResponse& response)
{
    RefPtr<NetscapePlugInStreamLoader> protect(this);

    m_client->didReceiveResponse(this, response);

    // Don't continue if the stream is cancelled
    if (!m_client)
        return;

    ResourceLoader::didReceiveResponse(response);

    // Don't continue if the stream is cancelled
    if (!m_client)
        return;

    if (!response.isHTTP())
        return;
    
    if (m_client->wantsAllStreams())
        return;
    
    if (response.httpStatusCode() < 100 || response.httpStatusCode() >= 400)
        didCancel(frameLoader()->fileDoesNotExistError(response));
}

void NetscapePlugInStreamLoader::didReceiveData(const char* data, int length, long long lengthReceived, bool allAtOnce)
{
    RefPtr<NetscapePlugInStreamLoader> protect(this);

    m_client->didReceiveData(this, data, length);
    
    ResourceLoader::didReceiveData(data, length, lengthReceived, allAtOnce);
}

void NetscapePlugInStreamLoader::didFinishLoading()
{
    RefPtr<NetscapePlugInStreamLoader> protect(this);

    m_documentLoader->removePlugInStreamLoader(this);
    m_client->didFinishLoading(this);
    ResourceLoader::didFinishLoading();
}

void NetscapePlugInStreamLoader::didFail(const ResourceError& error)
{
    RefPtr<NetscapePlugInStreamLoader> protect(this);

    m_documentLoader->removePlugInStreamLoader(this);
    m_client->didFail(this, error);
    ResourceLoader::didFail(error);
}

void NetscapePlugInStreamLoader::didCancel(const ResourceError& error)
{
    RefPtr<NetscapePlugInStreamLoader> protect(this);

    m_client->didFail(this, error);

    // If calling didFail spins the run loop the stream loader can reach the terminal state.
    // If that's the case we just return early.
    if (reachedTerminalState())
        return;
    
    // We need to remove the stream loader after the call to didFail, since didFail can 
    // spawn a new run loop and if the loader has been removed it won't be deferred when
    // the document loader is asked to defer loading.
    m_documentLoader->removePlugInStreamLoader(this);

    ResourceLoader::didCancel(error);
}

}