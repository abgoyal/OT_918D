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

/**
 * Copyright (C) 2008 Torch Mobile Inc. All rights reserved. (http://www.torchmobile.com/)
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 *
 */

#include "config.h"

#if ENABLE(WML)
#include "WMLVariables.h"

#include "WMLDocument.h"
#include <wtf/ASCIICType.h>

namespace WebCore {

// WML variables specification, excluding the
// pre-WML 1.0 deprecated variable syntax
//
// varname = ("_" | alpha) ("_" | alpha | digit)* 
// conv = ":" ("e" ("scape")? | "n" ("oesc")? | "u" ("nesc")?)
// var = ("$" varname) | ("$(" varname (conv)? ")")

static bool isValidFirstVariableNameCharacter(const UChar& character)
{
    return WTF::isASCIIAlpha(character)
           || character == '_';
}

static bool isValidVariableNameCharacter(const UChar& character)
{
    return WTF::isASCIIAlpha(character)
           || WTF::isASCIIDigit(character)
           || character == '_';
}

static bool isValidVariableEscapingModeString(const String& mode, WMLVariableEscapingMode& escapeMode)
{
    if (mode == "e" || mode == "escape")
        escapeMode = WMLVariableEscapingEscape; 
    else if (mode == "u" || mode == "unesc")
        escapeMode = WMLVariableEscapingUnescape;
    else if (mode == "n" || mode == "noesc")
        escapeMode = WMLVariableEscapingNone;
    else
        return false;

    return true;
}

bool isValidVariableName(const String& name)
{
    if (name.isEmpty())
        return false;

    const UChar* characters = name.characters();
    if (!isValidFirstVariableNameCharacter(characters[0]))
        return false;

    unsigned length = name.length();
    for (unsigned i = 1; i < length; ++i) {
        if (!isValidVariableNameCharacter(characters[i]))
            return false;
    }

    return true;
}

bool containsVariableReference(const String& text, bool& isValid)
{
    isValid = true;
    bool foundReference = false;
    bool finished = false;
    int currentPosition = 0;
    const UChar* characters = text.characters();

    while (!finished) {
        // Find beginning of variable reference
        int referenceStartPosition = text.find('$', currentPosition);
        if (referenceStartPosition == -1) {
            finished = true;
            break;
        }

        foundReference = true;

        int nameStartPosition = referenceStartPosition + 1;
        int nameEndPosition = -1;

        if (characters[nameStartPosition] == '(') {
            // If the input string contains an open brace, a close brace must exist as well
            nameEndPosition = text.find(')', nameStartPosition + 1);
            if (nameEndPosition == -1) {
                finished = true;
                isValid = false;
                break;
            }

            ++nameStartPosition;
        } else {
            int length = text.length();
            for (nameEndPosition = nameStartPosition; nameEndPosition < length; ++nameEndPosition) {
                if (!isValidVariableNameCharacter(text[nameEndPosition]))
                    break;
            }
        }

        if (nameEndPosition < nameStartPosition) {
            finished = true;
            isValid = false;
            break;
        }
    
        // Eventually split of conversion string, and check its syntax afterwards
        String conversionString;
        String variableName = text.substring(nameStartPosition, nameEndPosition - nameStartPosition);

        int conversionStringStart = variableName.find(':');
        if (conversionStringStart != -1) {
            conversionString = variableName.substring(conversionStringStart + 1, variableName.length() - (conversionStringStart + 1));
            variableName = variableName.left(conversionStringStart);
        }

        isValid = isValidVariableName(variableName);
        if (!isValid) {
            finished = true;
            break;
        }

        if (!conversionString.isEmpty()) {
            isValid = isValidVariableName(conversionString);
            if (!isValid) {
                finished = true;
                break;
            }

            WMLVariableEscapingMode escapeMode = WMLVariableEscapingNone;
            isValid = isValidVariableEscapingModeString(conversionString, escapeMode);
            if (!isValid) {
                finished = true;
                break;
            }
        }

        currentPosition = nameEndPosition;
    }

    return foundReference;
}

String substituteVariableReferences(const String& reference, Document* document, WMLVariableEscapingMode escapeMode)
{
    ASSERT(document);

    if (reference.isEmpty())
        return reference;

    WMLPageState* pageState = wmlPageStateForDocument(document);
    if (!pageState)
        return reference;

    bool isValid = true; 
    String remainingInput = reference;
    String result;

    while (!remainingInput.isEmpty()) {
        ASSERT(isValid);

        int start = remainingInput.find("$");
        if (start == -1) {
            // Consume all remaining characters, as there's nothing more to substitute
            result += remainingInput;
            break;
        }

        // Consume all characters until the variable reference beginning
        result += remainingInput.left(start);
        remainingInput.remove(0, start);

        // Transform adjacent dollar signs into a single dollar sign as string literal
        if (remainingInput[1] == '$') {
            result += "$";
            remainingInput.remove(0, 2);
            continue;
        }

        String variableName;
        String conversionMode;

        if (remainingInput[1] == '(') {
            int referenceEndPosition = remainingInput.find(")");
            if (referenceEndPosition == -1) {
                isValid = false;
                break;
            }

            variableName = remainingInput.substring(2, referenceEndPosition - 2);
            remainingInput.remove(0, referenceEndPosition + 1);

            // Determine variable conversion mode string
            int pos = variableName.find(':');
            if (pos != -1) {
                conversionMode = variableName.substring(pos + 1, variableName.length() - (pos + 1));
                variableName = variableName.left(pos);
            }
        } else {
            int length = remainingInput.length();
            int referenceEndPosition = 1;

            for (; referenceEndPosition < length; ++referenceEndPosition) {
                if (!isValidVariableNameCharacter(remainingInput[referenceEndPosition]))
                    break;
            }

            variableName = remainingInput.substring(1, referenceEndPosition - 1);
            remainingInput.remove(0, referenceEndPosition);
        }

        isValid = isValidVariableName(variableName);
        if (!isValid)
            break;

        ASSERT(!variableName.isEmpty());

        String variableValue = pageState->getVariable(variableName);
        if (variableValue.isEmpty())
            continue;

        if (containsVariableReference(variableValue, isValid)) {
            if (!isValid)
                break;

            variableValue = substituteVariableReferences(variableValue, document, escapeMode);
            continue;
        }

        if (!conversionMode.isEmpty()) {
            // Override default escape mode, if desired
            WMLVariableEscapingMode specifiedEscapeMode = WMLVariableEscapingNone; 
            if (isValid = isValidVariableEscapingModeString(conversionMode, specifiedEscapeMode))
                escapeMode = specifiedEscapeMode;

            if (!isValid)
                break;
        }

        switch (escapeMode) {
        case WMLVariableEscapingNone:
            break;
        case WMLVariableEscapingEscape:
            variableValue = encodeWithURLEscapeSequences(variableValue); 
            break;
        case WMLVariableEscapingUnescape:
            variableValue = decodeURLEscapeSequences(variableValue);
            break;
        }

        result += variableValue;
        ASSERT(isValid);
    }

    if (!isValid) {
        reportWMLError(document, WMLErrorInvalidVariableReference);
        return reference;
    }

    return result;
}

}

#endif
