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
 * Copyright (C) 2007, 2008 Apple Inc. All rights reserved.
 * Copyright (C) 2007 Staikos Computing Services Inc. <info@staikos.net>
 * Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)
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
#include "Scrollbar.h"

#include "EventHandler.h"
#include "FrameView.h"
#include "Frame.h"
#include "GraphicsContext.h"
#include "IntRect.h"
#include "PlatformMouseEvent.h"
#include "ScrollbarTheme.h"

#include <QApplication>
#include <QDebug>
#include <QPainter>
#include <QStyle>
#include <QMenu>

using namespace std;

namespace WebCore {

bool Scrollbar::contextMenu(const PlatformMouseEvent& event)
{
#ifndef QT_NO_CONTEXTMENU
    if (!QApplication::style()->styleHint(QStyle::SH_ScrollBar_ContextMenu))
        return true;

    bool horizontal = (m_orientation == HorizontalScrollbar);

    QMenu menu;
    QAction* actScrollHere = menu.addAction(QCoreApplication::translate("QWebPage", "Scroll here"));
    menu.addSeparator();

    QAction* actScrollTop = menu.addAction(horizontal ? QCoreApplication::translate("QWebPage", "Left edge") : QCoreApplication::translate("QWebPage", "Top"));
    QAction* actScrollBottom = menu.addAction(horizontal ? QCoreApplication::translate("QWebPage", "Right edge") : QCoreApplication::translate("QWebPage", "Bottom"));
    menu.addSeparator();

    QAction* actPageUp = menu.addAction(horizontal ? QCoreApplication::translate("QWebPage", "Page left") : QCoreApplication::translate("QWebPage", "Page up"));
    QAction* actPageDown = menu.addAction(horizontal ? QCoreApplication::translate("QWebPage", "Page right") : QCoreApplication::translate("QWebPage", "Page down"));
    menu.addSeparator();

    QAction* actScrollUp = menu.addAction(horizontal ? QCoreApplication::translate("QWebPage", "Scroll left") : QCoreApplication::translate("QWebPage", "Scroll up"));
    QAction* actScrollDown = menu.addAction(horizontal ? QCoreApplication::translate("QWebPage", "Scroll right") : QCoreApplication::translate("QWebPage", "Scroll down"));

    const QPoint globalPos = QPoint(event.globalX(), event.globalY());
    QAction* actionSelected = menu.exec(globalPos);

    if (!actionSelected)
        { /* Do nothing */ }
    else if (actionSelected == actScrollHere) {
        const QPoint pos = convertFromContainingWindow(event.pos());
        moveThumb(horizontal ? pos.x() : pos.y());
    } else if (actionSelected == actScrollTop)
        setValue(0);
    else if (actionSelected == actScrollBottom)
        setValue(maximum());
    else if (actionSelected == actPageUp)
        scroll(horizontal ? ScrollLeft: ScrollUp, ScrollByPage, 1);
    else if (actionSelected == actPageDown)
        scroll(horizontal ? ScrollRight : ScrollDown, ScrollByPage, 1);
    else if (actionSelected == actScrollUp)
        scroll(horizontal ? ScrollLeft : ScrollUp, ScrollByLine, 1);
    else if (actionSelected == actScrollDown)
        scroll(horizontal ? ScrollRight : ScrollDown, ScrollByLine, 1);
#endif // QT_NO_CONTEXTMENU
    return true;
}

}

// vim: ts=4 sw=4 et
