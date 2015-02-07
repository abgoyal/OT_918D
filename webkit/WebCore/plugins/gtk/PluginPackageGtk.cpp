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
 * Copyright (C) 2006, 2007 Apple Inc.  All rights reserved.
 * Copyright (C) 2008 Collabora Ltd. All rights reserved.
 * Copyright (C) 2008 Nuanti Ltd.
 * Copyright (C) 2008 Novell Inc. All rights reserved.
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

#include "config.h"
#include "PluginPackage.h"

#include <stdio.h>

#include "CString.h"
#include "MIMETypeRegistry.h"
#include "NotImplemented.h"
#include "npruntime_impl.h"
#include "PluginDebug.h"

namespace WebCore {

bool PluginPackage::fetchInfo()
{
#if defined(XP_UNIX)
    if (!load())
        return false;

    NP_GetMIMEDescriptionFuncPtr NP_GetMIMEDescription = 0;
    NPP_GetValueProcPtr NPP_GetValue = 0;

    g_module_symbol(m_module, "NP_GetMIMEDescription", (void**)&NP_GetMIMEDescription);
    g_module_symbol(m_module, "NP_GetValue", (void**)&NPP_GetValue);

    if (!NP_GetMIMEDescription || !NPP_GetValue)
        return false;

    char* buffer = 0;
    NPError err = NPP_GetValue(0, NPPVpluginNameString, &buffer);
    if (err == NPERR_NO_ERROR)
        m_name = buffer;

    buffer = 0;
    err = NPP_GetValue(0, NPPVpluginDescriptionString, &buffer);
    if (err == NPERR_NO_ERROR) {
        m_description = buffer;
        determineModuleVersionFromDescription();
    }

    const gchar* types = NP_GetMIMEDescription();
    gchar** mimeDescs = g_strsplit(types, ";", -1);
    for (int i = 0; mimeDescs[i] && mimeDescs[i][0]; i++) {
        gchar** mimeData = g_strsplit(mimeDescs[i], ":", 3);
        if (g_strv_length(mimeData) < 3) {
            g_strfreev(mimeData);
            continue;
        }

        String description = String::fromUTF8(mimeData[2]);
        gchar** extensions = g_strsplit(mimeData[1], ",", -1);

        Vector<String> extVector;
        for (int j = 0; extensions[j]; j++)
            extVector.append(String::fromUTF8(extensions[j]));

        determineQuirks(mimeData[0]);
        m_mimeToExtensions.add(mimeData[0], extVector);
        m_mimeToDescriptions.add(mimeData[0], description);

        g_strfreev(extensions);
        g_strfreev(mimeData);
    }
    g_strfreev(mimeDescs);

    return true;
#else
    notImplemented();
    return false;
#endif
}

bool PluginPackage::load()
{
    if (m_isLoaded) {
        m_loadCount++;
        return true;
    }

    m_module = g_module_open((m_path.utf8()).data(), G_MODULE_BIND_LOCAL);

    if (!m_module) {
        LOG(Plugins,"Module Load Failed :%s, Error:%s\n", (m_path.utf8()).data(), g_module_error());
        return false;
    }

    m_isLoaded = true;

    NP_InitializeFuncPtr NP_Initialize = 0;
    m_NPP_Shutdown = 0;

    NPError npErr;

    g_module_symbol(m_module, "NP_Initialize", (void**)&NP_Initialize);
    g_module_symbol(m_module, "NP_Shutdown", (void**)&m_NPP_Shutdown);

    if (!NP_Initialize || !m_NPP_Shutdown)
        goto abort;

    memset(&m_pluginFuncs, 0, sizeof(m_pluginFuncs));
    m_pluginFuncs.size = sizeof(m_pluginFuncs);

    initializeBrowserFuncs();

#if defined(XP_UNIX)
    npErr = NP_Initialize(&m_browserFuncs, &m_pluginFuncs);
#else
    npErr = NP_Initialize(&m_browserFuncs);
#endif
    if (npErr != NPERR_NO_ERROR)
        goto abort;

    m_loadCount++;
    return true;

abort:
    unloadWithoutShutdown();
    return false;
}

}