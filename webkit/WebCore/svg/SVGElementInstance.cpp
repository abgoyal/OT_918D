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
    Copyright (C) 2007, 2008 Nikolas Zimmermann <zimmermann@kde.org>
    Copyright (C) Research In Motion Limited 2010. All rights reserved.

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/

#include "config.h"

#if ENABLE(SVG)
#include "SVGElementInstance.h"

#include "ContainerNodeAlgorithms.h"
#include "Event.h"
#include "EventException.h"
#include "EventListener.h"
#include "EventNames.h"
#include "FrameView.h"
#include "SVGElementInstanceList.h"
#include "SVGUseElement.h"

#include <wtf/RefCountedLeakCounter.h>

namespace WebCore {

#ifndef NDEBUG
static WTF::RefCountedLeakCounter instanceCounter("WebCoreSVGElementInstance");
#endif

SVGElementInstance::SVGElementInstance(SVGUseElement* useElement, PassRefPtr<SVGElement> originalElement)
    : m_useElement(useElement)
    , m_element(originalElement)
    , m_previousSibling(0)
    , m_nextSibling(0)
    , m_firstChild(0)
    , m_lastChild(0)
{
    ASSERT(m_useElement);
    ASSERT(m_element);

    // Register as instance for passed element.
    m_element->mapInstanceToElement(this);

#ifndef NDEBUG
    instanceCounter.increment();
#endif
}

SVGElementInstance::~SVGElementInstance()
{
#ifndef NDEBUG
    instanceCounter.decrement();
#endif

    // Deregister as instance for passed element.
    m_element->removeInstanceMapping(this);

    removeAllChildrenInContainer<SVGElementInstance, SVGElementInstance>(this);
}

PassRefPtr<SVGElementInstanceList> SVGElementInstance::childNodes()
{
    return SVGElementInstanceList::create(this);
}

void SVGElementInstance::setShadowTreeElement(SVGElement* element)
{
    ASSERT(element);
    m_shadowTreeElement = element;
}

void SVGElementInstance::appendChild(PassRefPtr<SVGElementInstance> child)
{
    appendChildToContainer<SVGElementInstance, SVGElementInstance>(child.get(), this);
}

void SVGElementInstance::invalidateAllInstancesOfElement(SVGElement* element)
{
    if (!element)
        return;

    if (element->isStyled() && static_cast<SVGStyledElement*>(element)->instanceUpdatesBlocked())
        return;

    const HashSet<SVGElementInstance*>& set = element->instancesForElement();
    if (set.isEmpty())
        return;

    // Mark all use elements referencing 'element' for rebuilding
    const HashSet<SVGElementInstance*>::const_iterator end = set.end();
    for (HashSet<SVGElementInstance*>::const_iterator it = set.begin(); it != end; ++it) {
        ASSERT((*it)->correspondingElement() == element);
        (*it)->correspondingUseElement()->invalidateShadowTree();
    }
}

ScriptExecutionContext* SVGElementInstance::scriptExecutionContext() const
{
    return m_element->document();
}

bool SVGElementInstance::addEventListener(const AtomicString& eventType, PassRefPtr<EventListener> listener, bool useCapture)
{
    return m_element->addEventListener(eventType, listener, useCapture);
}

bool SVGElementInstance::removeEventListener(const AtomicString& eventType, EventListener* listener, bool useCapture)
{
    return m_element->removeEventListener(eventType, listener, useCapture);
}

void SVGElementInstance::removeAllEventListeners()
{
    m_element->removeAllEventListeners();
}

bool SVGElementInstance::dispatchEvent(PassRefPtr<Event> prpEvent)
{
    RefPtr<EventTarget> protect = this;
    RefPtr<Event> event = prpEvent;

    event->setTarget(this);

    SVGElement* element = shadowTreeElement();
    if (!element)
        return false;

    RefPtr<FrameView> view = element->document()->view();
    return element->dispatchGenericEvent(event.release());
}

EventTargetData* SVGElementInstance::eventTargetData()
{
    return m_element->eventTargetData();
}

EventTargetData* SVGElementInstance::ensureEventTargetData()
{
    // Avoid crashing - return a default dummy value
    DEFINE_STATIC_LOCAL(EventTargetData, dummyEventTargetData, ());
    dummyEventTargetData.eventListenerMap.clear();
    return &dummyEventTargetData;
}

}

#endif