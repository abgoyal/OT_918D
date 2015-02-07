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
 * Copyright (C) 1999 Lars Knoll (knoll@kde.org)
 * Copyright (C) 2000 Frederik Holljen (frederik.holljen@hig.no)
 * Copyright (C) 2001 Peter Kelly (pmk@post.com)
 * Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
 * Copyright (C) 2004, 2008 Apple Inc. All rights reserved.
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
#include "NodeIterator.h"

#include "Document.h"
#include "ExceptionCode.h"
#include "NodeFilter.h"
#include "ScriptState.h"

namespace WebCore {

NodeIterator::NodePointer::NodePointer()
{
}

NodeIterator::NodePointer::NodePointer(PassRefPtr<Node> n, bool b)
    : node(n)
    , isPointerBeforeNode(b)
{
}

void NodeIterator::NodePointer::clear()
{
    node.clear();
}

bool NodeIterator::NodePointer::moveToNext(Node* root)
{
    if (!node)
        return false;
    if (isPointerBeforeNode) {
        isPointerBeforeNode = false;
        return true;
    }
    node = node->traverseNextNode(root);
    return node;
}

bool NodeIterator::NodePointer::moveToPrevious(Node* root)
{
    if (!node)
        return false;
    if (!isPointerBeforeNode) {
        isPointerBeforeNode = true;
        return true;
    }
    node = node->traversePreviousNode(root);
    return node;
}

NodeIterator::NodeIterator(PassRefPtr<Node> rootNode, unsigned whatToShow, PassRefPtr<NodeFilter> filter, bool expandEntityReferences)
    : Traversal(rootNode, whatToShow, filter, expandEntityReferences)
    , m_referenceNode(root(), true)
    , m_detached(false)
{
    root()->document()->attachNodeIterator(this);
}

NodeIterator::~NodeIterator()
{
    root()->document()->detachNodeIterator(this);
}

PassRefPtr<Node> NodeIterator::nextNode(ScriptState* state, ExceptionCode& ec)
{
    if (m_detached) {
        ec = INVALID_STATE_ERR;
        return 0;
    }

    RefPtr<Node> result;

    m_candidateNode = m_referenceNode;
    while (m_candidateNode.moveToNext(root())) {
        // NodeIterators treat the DOM tree as a flat list of nodes.
        // In other words, FILTER_REJECT does not pass over descendants
        // of the rejected node. Hence, FILTER_REJECT is the same as FILTER_SKIP.
        RefPtr<Node> provisionalResult = m_candidateNode.node;
        bool nodeWasAccepted = acceptNode(state, provisionalResult.get()) == NodeFilter::FILTER_ACCEPT;
        if (state && state->hadException())
            break;
        if (nodeWasAccepted) {
            m_referenceNode = m_candidateNode;
            result = provisionalResult.release();
            break;
        }
    }

    m_candidateNode.clear();
    return result.release();
}

PassRefPtr<Node> NodeIterator::previousNode(ScriptState* state, ExceptionCode& ec)
{
    if (m_detached) {
        ec = INVALID_STATE_ERR;
        return 0;
    }

    RefPtr<Node> result;

    m_candidateNode = m_referenceNode;
    while (m_candidateNode.moveToPrevious(root())) {
        // NodeIterators treat the DOM tree as a flat list of nodes.
        // In other words, FILTER_REJECT does not pass over descendants
        // of the rejected node. Hence, FILTER_REJECT is the same as FILTER_SKIP.
        RefPtr<Node> provisionalResult = m_candidateNode.node;
        bool nodeWasAccepted = acceptNode(state, provisionalResult.get()) == NodeFilter::FILTER_ACCEPT;
        if (state && state->hadException())
            break;
        if (nodeWasAccepted) {
            m_referenceNode = m_candidateNode;
            result = provisionalResult.release();
            break;
        }
    }

    m_candidateNode.clear();
    return result.release();
}

void NodeIterator::detach()
{
    root()->document()->detachNodeIterator(this);
    m_detached = true;
    m_referenceNode.node.clear();
}

void NodeIterator::nodeWillBeRemoved(Node* removedNode)
{
    updateForNodeRemoval(removedNode, m_candidateNode);
    updateForNodeRemoval(removedNode, m_referenceNode);
}

void NodeIterator::updateForNodeRemoval(Node* removedNode, NodePointer& referenceNode) const
{
    ASSERT(!m_detached);
    ASSERT(removedNode);
    ASSERT(root()->document() == removedNode->document());

    // Iterator is not affected if the removed node is the reference node and is the root.
    // or if removed node is not the reference node, or the ancestor of the reference node.
    if (!removedNode->isDescendantOf(root()))
        return;
    bool willRemoveReferenceNode = removedNode == referenceNode.node;
    bool willRemoveReferenceNodeAncestor = referenceNode.node && referenceNode.node->isDescendantOf(removedNode);
    if (!willRemoveReferenceNode && !willRemoveReferenceNodeAncestor)
        return;

    if (referenceNode.isPointerBeforeNode) {
        Node* node = removedNode->traverseNextNode(root());
        if (node) {
            // Move out from under the node being removed if the reference node is
            // a descendant of the node being removed.
            if (willRemoveReferenceNodeAncestor) {
                while (node && node->isDescendantOf(removedNode))
                    node = node->traverseNextNode(root());
            }
            if (node)
                referenceNode.node = node;
        } else {
            node = removedNode->traversePreviousNode(root());
            if (node) {
                // Move out from under the node being removed if the reference node is
                // a descendant of the node being removed.
                if (willRemoveReferenceNodeAncestor) {
                    while (node && node->isDescendantOf(removedNode))
                        node = node->traversePreviousNode(root());
                }
                if (node) {
                    // Removing last node.
                    // Need to move the pointer after the node preceding the 
                    // new reference node.
                    referenceNode.node = node;
                    referenceNode.isPointerBeforeNode = false;
                }
            }
        }
    } else {
        Node* node = removedNode->traversePreviousNode(root());
        if (node) {
            // Move out from under the node being removed if the reference node is
            // a descendant of the node being removed.
            if (willRemoveReferenceNodeAncestor) {
                while (node && node->isDescendantOf(removedNode))
                    node = node->traversePreviousNode(root());
            }
            if (node)
                referenceNode.node = node;
        } else {
            node = removedNode->traverseNextNode(root());
            // Move out from under the node being removed if the reference node is
            // a descendant of the node being removed.
            if (willRemoveReferenceNodeAncestor) {
                while (node && node->isDescendantOf(removedNode))
                    node = node->traversePreviousNode(root());
            }
            if (node)
                referenceNode.node = node;
        }
    }
}


} // namespace WebCore
