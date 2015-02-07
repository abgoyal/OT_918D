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
 * Copyright (C) 2005, 2006 Apple Computer, Inc.  All rights reserved.
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
#include "InsertLineBreakCommand.h"

#include "CSSMutableStyleDeclaration.h"
#include "Document.h"
#include "Frame.h"
#include "HTMLElement.h"
#include "HTMLNames.h"
#include "Range.h"
#include "RenderObject.h"
#include "Text.h"
#include "VisiblePosition.h"
#include "htmlediting.h"
#include "visible_units.h"

namespace WebCore {

using namespace HTMLNames;

InsertLineBreakCommand::InsertLineBreakCommand(Document* document) 
    : CompositeEditCommand(document)
{
}

bool InsertLineBreakCommand::preservesTypingStyle() const
{
    return true;
}

void InsertLineBreakCommand::insertNodeAfterPosition(Node* node, const Position& pos)
{
    // Insert the BR after the caret position. In the case the
    // position is a block, do an append. We don't want to insert
    // the BR *after* the block.
    Element* cb = pos.node()->enclosingBlockFlowElement();
    if (cb == pos.node())
        appendNode(node, cb);
    else
        insertNodeAfter(node, pos.node());
}

void InsertLineBreakCommand::insertNodeBeforePosition(Node* node, const Position& pos)
{
    // Insert the BR after the caret position. In the case the
    // position is a block, do an append. We don't want to insert
    // the BR *before* the block.
    Element* cb = pos.node()->enclosingBlockFlowElement();
    if (cb == pos.node())
        appendNode(node, cb);
    else
        insertNodeBefore(node, pos.node());
}

// Whether we should insert a break element or a '\n'.
bool InsertLineBreakCommand::shouldUseBreakElement(const Position& insertionPos)
{
    // An editing position like [input, 0] actually refers to the position before
    // the input element, and in that case we need to check the input element's
    // parent's renderer.
    Position p(rangeCompliantEquivalent(insertionPos));
    return p.node()->renderer() && !p.node()->renderer()->style()->preserveNewline();
}

void InsertLineBreakCommand::doApply()
{
    deleteSelection();
    VisibleSelection selection = endingSelection();
    if (selection.isNone())
        return;
    
    VisiblePosition caret(selection.visibleStart());
    Position pos(caret.deepEquivalent());

    pos = positionAvoidingSpecialElementBoundary(pos);
    
    pos = positionOutsideTabSpan(pos);

    RefPtr<Node> nodeToInsert;
    if (shouldUseBreakElement(pos))
        nodeToInsert = createBreakElement(document());
    else
        nodeToInsert = document()->createTextNode("\n");
    
    // FIXME: Need to merge text nodes when inserting just after or before text.
    
    if (isEndOfParagraph(caret) && !lineBreakExistsAtVisiblePosition(caret)) {
        bool needExtraLineBreak = !pos.node()->hasTagName(hrTag) && !pos.node()->hasTagName(tableTag);
        
        insertNodeAt(nodeToInsert.get(), pos);
        
        if (needExtraLineBreak)
            insertNodeBefore(nodeToInsert->cloneNode(false), nodeToInsert);
        
        VisiblePosition endingPosition(Position(nodeToInsert.get(), 0));
        setEndingSelection(VisibleSelection(endingPosition));
    } else if (pos.deprecatedEditingOffset() <= caretMinOffset(pos.node())) {
        insertNodeAt(nodeToInsert.get(), pos);
        
        // Insert an extra br or '\n' if the just inserted one collapsed.
        if (!isStartOfParagraph(VisiblePosition(Position(nodeToInsert.get(), 0))))
            insertNodeBefore(nodeToInsert->cloneNode(false).get(), nodeToInsert.get());
        
        setEndingSelection(VisibleSelection(positionInParentAfterNode(nodeToInsert.get()), DOWNSTREAM));
    // If we're inserting after all of the rendered text in a text node, or into a non-text node,
    // a simple insertion is sufficient.
    } else if (pos.deprecatedEditingOffset() >= caretMaxOffset(pos.node()) || !pos.node()->isTextNode()) {
        insertNodeAt(nodeToInsert.get(), pos);
        setEndingSelection(VisibleSelection(positionInParentAfterNode(nodeToInsert.get()), DOWNSTREAM));
    } else if (pos.node()->isTextNode()) {
        // Split a text node
        Text* textNode = static_cast<Text*>(pos.node());
        splitTextNode(textNode, pos.deprecatedEditingOffset());
        insertNodeBefore(nodeToInsert, textNode);
        Position endingPosition = Position(textNode, 0);
        
        // Handle whitespace that occurs after the split
        updateLayout();
        if (!endingPosition.isRenderedCharacter()) {
            Position positionBeforeTextNode(positionInParentBeforeNode(textNode));
            // Clear out all whitespace and insert one non-breaking space
            deleteInsignificantTextDownstream(endingPosition);
            ASSERT(!textNode->renderer() || textNode->renderer()->style()->collapseWhiteSpace());
            // Deleting insignificant whitespace will remove textNode if it contains nothing but insignificant whitespace.
            if (textNode->inDocument())
                insertTextIntoNode(textNode, 0, nonBreakingSpaceString());
            else {
                RefPtr<Text> nbspNode = document()->createTextNode(nonBreakingSpaceString());
                insertNodeAt(nbspNode.get(), positionBeforeTextNode);
                endingPosition = Position(nbspNode.get(), 0);
            }
        }
        
        setEndingSelection(VisibleSelection(endingPosition, DOWNSTREAM));
    }

    // Handle the case where there is a typing style.
    
    CSSMutableStyleDeclaration* typingStyle = document()->frame()->typingStyle();
    
    if (typingStyle && typingStyle->length() > 0) {
        // Apply the typing style to the inserted line break, so that if the selection
        // leaves and then comes back, new input will have the right style.
        // FIXME: We shouldn't always apply the typing style to the line break here,
        // see <rdar://problem/5794462>.
        applyStyle(typingStyle, firstDeepEditingPositionForNode(nodeToInsert.get()), lastDeepEditingPositionForNode(nodeToInsert.get()));
        // Even though this applyStyle operates on a Range, it still sets an endingSelection().
        // It tries to set a VisibleSelection around the content it operated on. So, that VisibleSelection
        // will either (a) select the line break we inserted, or it will (b) be a caret just 
        // before the line break (if the line break is at the end of a block it isn't selectable).
        // So, this next call sets the endingSelection() to a caret just after the line break 
        // that we inserted, or just before it if it's at the end of a block.
        setEndingSelection(endingSelection().visibleEnd());
    }

    rebalanceWhitespace();
}

}
