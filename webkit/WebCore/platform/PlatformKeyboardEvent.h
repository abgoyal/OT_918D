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
 * Copyright (C) 2004, 2005, 2006 Apple Computer, Inc.  All rights reserved.
 * Copyright (C) 2008 Collabora, Ltd.  All rights reserved.
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

#ifndef PlatformKeyboardEvent_h
#define PlatformKeyboardEvent_h

#include "PlatformString.h"
#include <wtf/Platform.h>

#if PLATFORM(MAC)
#include <wtf/RetainPtr.h>
#ifdef __OBJC__
@class NSEvent;
#else
class NSEvent;
#endif
#endif

#if PLATFORM(WIN)
typedef struct HWND__ *HWND;
typedef unsigned WPARAM;
typedef long LPARAM;
#endif

#if PLATFORM(GTK)
typedef struct _GdkEventKey GdkEventKey;
#endif

#if PLATFORM(QT)
QT_BEGIN_NAMESPACE
class QKeyEvent;
QT_END_NAMESPACE
#endif

#if PLATFORM(WX)
class wxKeyEvent;
#endif

#if PLATFORM(HAIKU)
class BMessage;
#endif

namespace WebCore {

    class PlatformKeyboardEvent : public FastAllocBase {
    public:
        enum Type {
            // KeyDown is sent by platforms such as Mac OS X, gtk and Qt, and has information about both physical pressed key, and its translation.
            // For DOM processing, it needs to be disambiguated as RawKeyDown or Char event.
            KeyDown,

            // KeyUp is sent by all platforms.
            KeyUp,

            // These events are sent by platforms such as Windows and wxWidgets. RawKeyDown only has information about a physical key, and Char
            // only has information about a character it was translated into.
            RawKeyDown,
            Char
        };

        enum ModifierKey {
            AltKey = 1 << 0,
            CtrlKey = 1 << 1,
            MetaKey = 1 << 2,
            ShiftKey = 1 << 3,
        };

        Type type() const { return m_type; }
        void disambiguateKeyDownEvent(Type, bool backwardCompatibilityMode = false); // Only used on platforms that need it, i.e. those that generate KeyDown events.

        // Text as as generated by processing a virtual key code with a keyboard layout
        // (in most cases, just a character code, but the layout can emit several
        // characters in a single keypress event on some platforms).
        // This may bear no resemblance to the ultimately inserted text if an input method
        // processes the input.
        // Will be null for KeyUp and RawKeyDown events.
        String text() const { return m_text; }

        // Text that would have been generated by the keyboard if no modifiers were pressed
        // (except for Shift); useful for shortcut (accelerator) key handling.
        // Otherwise, same as text().
        String unmodifiedText() const { return m_unmodifiedText; }

        // Most compatible Windows virtual key code associated with the event.
        // Zero for Char events.
        int windowsVirtualKeyCode() const { return m_windowsVirtualKeyCode; }
        void setWindowsVirtualKeyCode(int code) { m_windowsVirtualKeyCode = code; }

        int nativeVirtualKeyCode() const { return m_nativeVirtualKeyCode; }
        void setNativeVirtualKeyCode(int code) { m_nativeVirtualKeyCode = code; }

        String keyIdentifier() const { return m_keyIdentifier; }
        bool isAutoRepeat() const { return m_autoRepeat; }
        void setIsAutoRepeat(bool in) { m_autoRepeat = in; }
        bool isKeypad() const { return m_isKeypad; }
        bool shiftKey() const { return m_shiftKey; }
        bool ctrlKey() const { return m_ctrlKey; }
        bool altKey() const { return m_altKey; }
        bool metaKey() const { return m_metaKey; }
        unsigned modifiers() const {
            return (altKey() ? AltKey : 0)
                | (ctrlKey() ? CtrlKey : 0)
                | (metaKey() ? MetaKey : 0)
                | (shiftKey() ? ShiftKey : 0);
        }

        static bool currentCapsLockState();

#if PLATFORM(MAC)
        PlatformKeyboardEvent();
        PlatformKeyboardEvent(NSEvent*);
        NSEvent* macEvent() const { return m_macEvent.get(); }
#endif

#if PLATFORM(WIN)
        PlatformKeyboardEvent(HWND, WPARAM, LPARAM, Type, bool);
#endif

#if PLATFORM(GTK)
        PlatformKeyboardEvent(GdkEventKey*);
        GdkEventKey* gdkEventKey() const;
#endif

#if PLATFORM(ANDROID)
        PlatformKeyboardEvent(int keyCode, UChar32 unichar, int repeatCount,
                    bool down, bool cap, bool alt, bool sym);
        UChar32 unichar() const { return m_unichar; }
        int repeatCount() const { return m_repeatCount; }
#endif

#if PLATFORM(QT)
        PlatformKeyboardEvent(QKeyEvent*);
        QKeyEvent* qtEvent() const { return m_qtEvent; }
#endif

#if PLATFORM(WX)
        PlatformKeyboardEvent(wxKeyEvent&);
#endif

#if PLATFORM(HAIKU)
        PlatformKeyboardEvent(BMessage*);
#endif

#if PLATFORM(WIN) || PLATFORM(CHROMIUM)
        bool isSystemKey() const { return m_isSystemKey; }
#endif

    protected:
        Type m_type;
        String m_text;
        String m_unmodifiedText;
        String m_keyIdentifier;
        bool m_autoRepeat;
        int m_windowsVirtualKeyCode;
        int m_nativeVirtualKeyCode;
        bool m_isKeypad;
        bool m_shiftKey;
        bool m_ctrlKey;
        bool m_altKey;
        bool m_metaKey;

#if PLATFORM(ANDROID)
        /*  the actual repeatCount (as opposed to just a bool m_autoRepeat)
            0 for initial down and up
         */
        int     m_repeatCount;
        /*  The originall unichar value. Sometimes the m_text/m_unmodifiedText
            fields are stripped (e.g. for RawKeyDown), so we record it also here
            in case someone (e.g. plugins) want to sniff it w/o waiting for a
            Char event type.
         */
        UChar32 m_unichar;
#endif

#if PLATFORM(MAC)
        RetainPtr<NSEvent> m_macEvent;
#endif
#if PLATFORM(WIN) || PLATFORM(CHROMIUM)
        bool m_isSystemKey;
#endif
#if PLATFORM(GTK)
        GdkEventKey* m_gdkEventKey;
#endif
#if PLATFORM(QT)
        QKeyEvent* m_qtEvent;
#endif
    };

} // namespace WebCore

#endif // PlatformKeyboardEvent_h
