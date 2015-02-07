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
 * Copyright 2006, The Android Open Source Project
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *  * Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
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

#ifndef RenderThemeAndroid_h
#define RenderThemeAndroid_h

#include "RenderTheme.h"

namespace WebCore {

class RenderSkinButton;
class RenderSkinRadio;
class RenderSkinCombo;

struct ThemeData {
    ThemeData()
        : m_part(0)
        , m_state(0)
    {
    }

    unsigned m_part;
    unsigned m_state;
};

class RenderThemeAndroid : public RenderTheme {
public:
    static PassRefPtr<RenderTheme> create();
    ~RenderThemeAndroid();
    
    virtual bool stateChanged(RenderObject*, ControlState) const;
       
    virtual bool supportsFocusRing(const RenderStyle*) const;
    // A method asking if the theme's controls actually care about redrawing when hovered.
    virtual bool supportsHover(const RenderStyle* style) const { return style->affectedByHoverRules(); }

    virtual int baselinePosition(const RenderObject*) const;

    virtual Color platformActiveSelectionBackgroundColor() const;
    virtual Color platformInactiveSelectionBackgroundColor() const;
    virtual Color platformActiveSelectionForegroundColor() const;
    virtual Color platformInactiveSelectionForegroundColor() const;
    virtual Color platformTextSearchHighlightColor() const;

    virtual Color platformActiveListBoxSelectionBackgroundColor() const;
    virtual Color platformInactiveListBoxSelectionBackgroundColor() const;
    virtual Color platformActiveListBoxSelectionForegroundColor() const;
    virtual Color platformInactiveListBoxSelectionForegroundColor() const;

    virtual void systemFont(int, WebCore::FontDescription&) const {}

    virtual int minimumMenuListSize(RenderStyle*) const { return 0; }

protected:
    virtual bool paintCheckbox(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);
    virtual void setCheckboxSize(RenderStyle*) const;

#if ENABLE(VIDEO)
    virtual String extraMediaControlsStyleSheet();
    virtual void adjustSliderThumbSize(RenderObject* o) const;
    virtual bool shouldRenderMediaControlPart(ControlPart part, Element* e);
    bool paintMediaMuteButton(RenderObject* o, const RenderObject::PaintInfo& paintInfo, const IntRect& r);
    bool paintMediaPlayButton(RenderObject* o, const RenderObject::PaintInfo& paintInfo, const IntRect& r);
    bool paintMediaSeekBackButton(RenderObject* o, const RenderObject::PaintInfo& paintInfo, const IntRect& r);
    bool paintMediaSeekForwardButton(RenderObject* o, const RenderObject::PaintInfo& paintInfo, const IntRect& r);
    bool paintMediaSliderTrack(RenderObject* o, const RenderObject::PaintInfo& paintInfo, const IntRect& r);
    bool paintMediaSliderThumb(RenderObject* o, const RenderObject::PaintInfo& paintInfo, const IntRect& r);
    virtual bool paintMediaControlsBackground(RenderObject* object, const RenderObject::PaintInfo& paintInfo, const IntRect& rect);
#endif

    virtual bool paintRadio(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);
    virtual void setRadioSize(RenderStyle*) const;

    virtual void adjustButtonStyle(CSSStyleSelector*, RenderStyle*, WebCore::Element*) const;
    virtual bool paintButton(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

    virtual void adjustTextFieldStyle(CSSStyleSelector*, RenderStyle*, WebCore::Element*) const;
    virtual bool paintTextField(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

    virtual void adjustTextAreaStyle(CSSStyleSelector*, RenderStyle*, WebCore::Element*) const;
    virtual bool paintTextArea(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);
    
    bool paintCombo(RenderObject*, const RenderObject::PaintInfo&,  const IntRect&);

    virtual void adjustListboxStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual void adjustMenuListStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual bool paintMenuList(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

    virtual void adjustMenuListButtonStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual bool paintMenuListButton(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);
    
    virtual void adjustSearchFieldStyle(CSSStyleSelector*, RenderStyle*, Element*) const;
    virtual bool paintSearchField(RenderObject*, const RenderObject::PaintInfo&, const IntRect&);

private:
    RenderThemeAndroid();
    void addIntrinsicMargins(RenderStyle*) const;
    void close();

    bool supportsFocus(ControlPart);
    friend RenderTheme* theme();
};

} // namespace WebCore

#endif // RenderThemeAndroid_h
