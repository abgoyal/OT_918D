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
 * Copyright (C) 2009 Company 100, Inc. All rights reserved.
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
#include "PlatformMouseEvent.h"

#include <AEEEvent.h>
#include <AEEPointerHelpers.h>
#include <AEEStdDef.h>
#include <AEEVCodes.h>

namespace WebCore {

PlatformMouseEvent::PlatformMouseEvent(AEEEvent event, uint16 wParam, uint32 dwParam)
{
    switch (event) {
    case EVT_POINTER_DOWN:
        m_eventType = MouseEventPressed;
        break;
    case EVT_POINTER_UP:
        m_eventType = MouseEventReleased;
        break;
    case EVT_POINTER_MOVE:
    case EVT_POINTER_STALE_MOVE:
        m_eventType = MouseEventMoved;
        break;
    default:
        m_eventType = MouseEventMoved;
        break;
    };

    char* dwParamStr = reinterpret_cast<char*>(dwParam);

    int x, y;
    AEE_POINTER_GET_XY(dwParamStr, &x, &y);
    m_position = IntPoint(x, y);
    // Use IDisplay, so position and global position are the same.
    m_globalPosition = m_position;

    uint32 keyModifiers = AEE_POINTER_GET_KEY_MODIFIERS(dwParamStr);
    m_shiftKey = keyModifiers & (KB_LSHIFT | KB_RSHIFT);
    m_ctrlKey  = keyModifiers & (KB_LCTRL | KB_RCTRL);
    m_altKey   = keyModifiers & (KB_LALT | KB_RALT);
    m_metaKey  = m_altKey;

    uint16 mouseModifiers = AEE_POINTER_GET_MOUSE_MODIFIERS(dwParamStr);
    if (mouseModifiers & AEE_POINTER_MOUSE_LBUTTON)
        m_button = LeftButton;
    else if (mouseModifiers & AEE_POINTER_MOUSE_RBUTTON)
        m_button = RightButton;
    else if (mouseModifiers & AEE_POINTER_MOUSE_MBUTTON)
        m_button = MiddleButton;
    else
        m_button = NoButton;

    // AEE_POINTER_GET_TIME returns milliseconds
    m_timestamp = AEE_POINTER_GET_TIME(dwParamStr) * 0.001;

    m_clickCount = AEE_POINTER_GET_CLICKCOUNT(dwParamStr);
}

} // namespace WebCore

