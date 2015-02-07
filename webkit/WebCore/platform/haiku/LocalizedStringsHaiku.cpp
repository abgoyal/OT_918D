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
 * Copyright (C) 2009 Maxime Simon <simon.maxime@gmail.com>
 *
 * All rights reserved.
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
#include "LocalizedStrings.h"

#include "NotImplemented.h"
#include "PlatformString.h"


namespace WebCore {
String submitButtonDefaultLabel()
{
    return "Submit";
}

String inputElementAltText()
{
    return String();
}

String resetButtonDefaultLabel()
{
    return "Reset";
}

String defaultLanguage()
{
    return "en";
}

String searchableIndexIntroduction()
{
    return "Searchable Index";
}

String fileButtonChooseFileLabel()
{
    return "Choose File";
}

String fileButtonNoFileSelectedLabel()
{
    return "No file selected";
}

String contextMenuItemTagOpenLinkInNewWindow()
{
    return "Open in new tab";
}

String contextMenuItemTagDownloadLinkToDisk()
{
    return "Download link to disk";
}

String contextMenuItemTagCopyLinkToClipboard()
{
    return "Copy link to clipboard";
}

String contextMenuItemTagOpenImageInNewWindow()
{
    return "Open image in new window";
}

String contextMenuItemTagDownloadImageToDisk()
{
    return "Download image to disk";
}

String contextMenuItemTagCopyImageToClipboard()
{
    return "Copy image to clipboard";
}

String contextMenuItemTagOpenFrameInNewWindow()
{
    return "Open frame in new window";
}

String contextMenuItemTagCopy()
{
    return "Copy";
}

String contextMenuItemTagGoBack()
{
    return "Go back";
}

String contextMenuItemTagGoForward()
{
    return "Go forward";
}

String contextMenuItemTagStop()
{
    return "Stop";
}

String contextMenuItemTagReload()
{
    return "Reload";
}

String contextMenuItemTagCut()
{
    return "Cut";
}

String contextMenuItemTagPaste()
{
    return "Paste";
}

String contextMenuItemTagNoGuessesFound()
{
    return "No guesses found";
}

String contextMenuItemTagIgnoreSpelling()
{
    return "Ignore spelling";
}

String contextMenuItemTagLearnSpelling()
{
    return "Learn spelling";
}

String contextMenuItemTagSearchWeb()
{
    return "Search web";
}

String contextMenuItemTagLookUpInDictionary()
{
    return "Lookup in dictionary";
}

String contextMenuItemTagOpenLink()
{
    return "Open link";
}

String contextMenuItemTagIgnoreGrammar()
{
    return "Ignore grammar";
}

String contextMenuItemTagSpellingMenu()
{
    return "Spelling menu";
}

String contextMenuItemTagShowSpellingPanel(bool show)
{
    return "Show spelling panel";
}

String contextMenuItemTagCheckSpelling()
{
    return "Check spelling";
}

String contextMenuItemTagCheckSpellingWhileTyping()
{
    return "Check spelling while typing";
}

String contextMenuItemTagCheckGrammarWithSpelling()
{
    return "Check for grammar with spelling";
}

String contextMenuItemTagFontMenu()
{
    return "Font menu";
}

String contextMenuItemTagBold()
{
    return "Bold";
}

String contextMenuItemTagItalic()
{
    return "Italic";
}

String contextMenuItemTagUnderline()
{
    return "Underline";
}

String contextMenuItemTagOutline()
{
    return "Outline";
}

String contextMenuItemTagWritingDirectionMenu()
{
    return "Writing direction menu";
}

String contextMenuItemTagDefaultDirection()
{
    return "Default direction";
}

String contextMenuItemTagLeftToRight()
{
    return "Left to right";
}

String contextMenuItemTagRightToLeft()
{
    return "Right to left";
}

String contextMenuItemTagInspectElement()
{
    return "Inspect";
}

String searchMenuNoRecentSearchesText()
{
    return "No recent text searches";
}

String searchMenuRecentSearchesText()
{
    return "Recent text searches";
}

String searchMenuClearRecentSearchesText()
{
    return "Clear recent text searches";
}

String unknownFileSizeText()
{
    return "Unknown";
}

String AXWebAreaText()
{
    return String();
}

String AXLinkText()
{
    return String();
}

String AXListMarkerText()
{
    return String();
}

String AXImageMapText()
{
    return String();
}

String AXHeadingText()
{
    return String();
}

String AXMenuListPopupActionVerb()
{
    return String();
}

String AXMenuListActionVerb()
{
    return String();
}

String imageTitle(const String& filename, const IntSize& size)
{
    return String(filename);
}

String contextMenuItemTagTextDirectionMenu()
{
    return String();
}

String AXButtonActionVerb()
{
    return String();
}

String AXTextFieldActionVerb()
{
    return String();
}

String AXRadioButtonActionVerb()
{
    return String();
}

String AXCheckedCheckBoxActionVerb()
{
    return String();
}

String AXUncheckedCheckBoxActionVerb()
{
    return String();
}

String AXLinkActionVerb()
{
    return String();
}

String AXDefinitionListTermText()
{
    return String();
}

String AXDefinitionListDefinitionText()
{
    return String();
}

String validationMessageValueMissingText()
{
    notImplemented();
    return String();
}

String validationMessageTypeMismatchText()
{
    notImplemented();
    return String();
}

String validationMessagePatternMismatchText()
{
    notImplemented();
    return String();
}

String validationMessageTooLongText()
{
    notImplemented();
    return String();
}

String validationMessageRangeUnderflowText()
{
    notImplemented();
    return String();
}

String validationMessageRangeOverflowText()
{
    notImplemented();
    return String();
}

String validationMessageStepMismatchText()
{
    notImplemented();
    return String();
}

} // namespace WebCore

