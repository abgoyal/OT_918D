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
 * Copyright (C) 2008 Apple Inc. All Rights Reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#include "config.h"

#if ENABLE(SVG_ANIMATION)
#include "SVGSMILElement.h"

#include "CSSPropertyNames.h"
#include "Document.h"
#include "Event.h"
#include "EventListener.h"
#include "FloatConversion.h"
#include "FrameView.h"
#include "HTMLNames.h"
#include "MappedAttribute.h"
#include "SMILTimeContainer.h"
#include "SVGNames.h"
#include "SVGParserUtilities.h"
#include "SVGSVGElement.h"
#include "SVGURIReference.h"
#include "XLinkNames.h"
#include <math.h>
#include <wtf/MathExtras.h>
#include <wtf/StdLibExtras.h>
#include <wtf/Vector.h>

using namespace std;

namespace WebCore {
    
// This is used for duration type time values that can't be negative.
static const double invalidCachedTime = -1.;
    
class ConditionEventListener : public EventListener {
public:
    static PassRefPtr<ConditionEventListener> create(SVGSMILElement* animation, Element* eventBase, SVGSMILElement::Condition* condition)
    {
        return adoptRef(new ConditionEventListener(animation, eventBase, condition));
    }

    static const ConditionEventListener* cast(const EventListener* listener)
    {
        return listener->type() == ConditionEventListenerType
            ? static_cast<const ConditionEventListener*>(listener)
            : 0;
    }

    virtual bool operator==(const EventListener& other);

    void unregister()
    {
        // If this has only one ref then the event base is dead already and we don't need to remove ourself.
        if (!hasOneRef())
            m_eventBase->removeEventListener(m_condition->m_name, this, false);
    }

private:
    ConditionEventListener(SVGSMILElement* animation, Element* eventBase, SVGSMILElement::Condition* condition) 
        : EventListener(ConditionEventListenerType)
        , m_animation(animation)
        , m_condition(condition) 
        , m_eventBase(eventBase)
    {
        m_eventBase->addEventListener(m_condition->m_name, this, false);
    }

    virtual void handleEvent(ScriptExecutionContext*, Event*);

    SVGSMILElement* m_animation;
    SVGSMILElement::Condition* m_condition;
    Element* m_eventBase;
};

bool ConditionEventListener::operator==(const EventListener& listener)
{
    if (const ConditionEventListener* conditionEventListener = ConditionEventListener::cast(&listener))
        return m_animation == conditionEventListener->m_animation
               && m_condition == conditionEventListener->m_condition
               && m_eventBase == conditionEventListener->m_eventBase;
    return false;
}

void ConditionEventListener::handleEvent(ScriptExecutionContext*, Event* event) 
{
    m_animation->handleConditionEvent(event, m_condition);
}

SVGSMILElement::Condition::Condition(Type type, BeginOrEnd beginOrEnd, const String& baseID, const String& name, SMILTime offset, int repeats)
    : m_type(type)
    , m_beginOrEnd(beginOrEnd)
    , m_baseID(baseID)
    , m_name(name)
    , m_offset(offset)
    , m_repeats(repeats) 
{
}
    
SVGSMILElement::SVGSMILElement(const QualifiedName& tagName, Document* doc)
    : SVGElement(tagName, doc)
    , m_conditionsConnected(false)
    , m_hasEndEventConditions(false)
    , m_intervalBegin(SMILTime::unresolved())
    , m_intervalEnd(SMILTime::unresolved())
    , m_previousIntervalBegin(SMILTime::unresolved())
    , m_isWaitingForFirstInterval(true)
    , m_activeState(Inactive)
    , m_lastPercent(0)
    , m_lastRepeat(0)
    , m_nextProgressTime(0)
    , m_documentOrderIndex(0)
    , m_cachedDur(invalidCachedTime)
    , m_cachedRepeatDur(invalidCachedTime)
    , m_cachedRepeatCount(invalidCachedTime)
    , m_cachedMin(invalidCachedTime)
    , m_cachedMax(invalidCachedTime)
{
}

SVGSMILElement::~SVGSMILElement()
{
    disconnectConditions();
    if (m_timeContainer)
        m_timeContainer->unschedule(this);
}
    
void SVGSMILElement::insertedIntoDocument()
{
    SVGElement::insertedIntoDocument();
#ifndef NDEBUG
    // Verify we are not in <use> instance tree.
    for (Node* n = this; n; n = n->parent())
        ASSERT(!n->isShadowNode());
#endif
    SVGSVGElement* owner = ownerSVGElement();
    if (!owner)
        return;
    m_timeContainer = owner->timeContainer();
    ASSERT(m_timeContainer);
    m_timeContainer->setDocumentOrderIndexesDirty();
    reschedule();
}

void SVGSMILElement::removedFromDocument()
{
    if (m_timeContainer) {
        m_timeContainer->unschedule(this);
        m_timeContainer = 0;
    }
    // Calling disconnectConditions() may kill us if there are syncbase conditions.
    // OK, but we don't want to die inside the call.
    RefPtr<SVGSMILElement> keepAlive(this);
    disconnectConditions();
    SVGElement::removedFromDocument();
}
   
void SVGSMILElement::finishParsingChildren()
{
    SVGElement::finishParsingChildren();

    // "If no attribute is present, the default begin value (an offset-value of 0) must be evaluated."
    if (!hasAttribute(SVGNames::beginAttr))
        m_beginTimes.append(0);

    if (m_isWaitingForFirstInterval) {
        resolveFirstInterval();
        reschedule();
    }
}

SMILTime SVGSMILElement::parseOffsetValue(const String& data)
{
    bool ok;
    double result = 0;
    String parse = data.stripWhiteSpace();
    if (parse.endsWith("h"))
        result = parse.left(parse.length() - 1).toDouble(&ok) * 60 * 60;
    else if (parse.endsWith("min"))
        result = parse.left(parse.length() - 3).toDouble(&ok) * 60;
    else if (parse.endsWith("ms"))
        result = parse.left(parse.length() - 2).toDouble(&ok) / 1000;
    else if (parse.endsWith("s"))
        result = parse.left(parse.length() - 1).toDouble(&ok);
    else
        result = parse.toDouble(&ok);
    if (!ok)
        return SMILTime::unresolved();
    return result;
}
    
SMILTime SVGSMILElement::parseClockValue(const String& data)
{
    if (data.isNull())
        return SMILTime::unresolved();
    
    String parse = data.stripWhiteSpace();

    DEFINE_STATIC_LOCAL(const AtomicString, indefiniteValue, ("indefinite"));
    if (parse == indefiniteValue)
        return SMILTime::indefinite();

    double result = 0;
    bool ok;
    int doublePointOne = parse.find(':');
    int doublePointTwo = parse.find(':', doublePointOne + 1);
    if (doublePointOne == 2 && doublePointTwo == 5 && parse.length() >= 8) { 
        result += parse.substring(0, 2).toUIntStrict(&ok) * 60 * 60;
        if (!ok)
            return SMILTime::unresolved();
        result += parse.substring(3, 2).toUIntStrict(&ok) * 60;
        if (!ok)
            return SMILTime::unresolved();
        result += parse.substring(6).toDouble(&ok);
    } else if (doublePointOne == 2 && doublePointTwo == -1 && parse.length() >= 5) { 
        result += parse.substring(0, 2).toUIntStrict(&ok) * 60;
        if (!ok)
            return SMILTime::unresolved();
        result += parse.substring(3).toDouble(&ok);
    } else
        return parseOffsetValue(parse);

    if (!ok)
        return SMILTime::unresolved();
    return result;
}
    
static void sortTimeList(Vector<SMILTime>& timeList)
{
    std::sort(timeList.begin(), timeList.end());
}
    
bool SVGSMILElement::parseCondition(const String& value, BeginOrEnd beginOrEnd)
{
    String parseString = value.stripWhiteSpace();
    
    double sign = 1.;
    bool ok;
    int pos = parseString.find('+');
    if (pos == -1) {
        pos = parseString.find('-');
        if (pos != -1)
            sign = -1.;
    }
    String conditionString;
    SMILTime offset = 0;
    if (pos == -1)
        conditionString = parseString;
    else {
        conditionString = parseString.left(pos).stripWhiteSpace();
        String offsetString = parseString.substring(pos + 1).stripWhiteSpace();
        offset = parseOffsetValue(offsetString);
        if (offset.isUnresolved())
            return false;
        offset = offset * sign;
    }
    if (conditionString.isEmpty())
        return false;
    pos = conditionString.find('.');
    
    String baseID;
    String nameString;
    if (pos == -1)
        nameString = conditionString;
    else {
        baseID = conditionString.left(pos);
        nameString = conditionString.substring(pos + 1);
    }
    if (nameString.isEmpty())
        return false;

    Condition::Type type;
    int repeats = -1;
    if (nameString.startsWith("repeat(") && nameString.endsWith(")")) {
        // FIXME: For repeat events we just need to add the data carrying TimeEvent class and 
        // fire the events at appropiate times.
        repeats = nameString.substring(7, nameString.length() - 8).toUIntStrict(&ok);
        if (!ok)
            return false;
        nameString = "repeat";
        type = Condition::EventBase;
    } else if (nameString == "begin" || nameString == "end") {
        if (baseID.isEmpty())
            return false;
        type = Condition::Syncbase;
    } else if (nameString.startsWith("accesskey(")) {
        // FIXME: accesskey() support.
        type = Condition::AccessKey;
    } else
        type = Condition::EventBase;
    
    m_conditions.append(Condition(type, beginOrEnd, baseID, nameString, offset, repeats));

    if (type == Condition::EventBase && beginOrEnd == End)
        m_hasEndEventConditions = true;

    return true;
}

bool SVGSMILElement::isSMILElement(Node* node)
{
    if (!node)
        return false;
    return node->hasTagName(SVGNames::setTag) || node->hasTagName(SVGNames::animateTag) || node->hasTagName(SVGNames::animateMotionTag)
            || node->hasTagName(SVGNames::animateTransformTag) || node->hasTagName(SVGNames::animateColorTag);
}
    
void SVGSMILElement::parseBeginOrEnd(const String& parseString, BeginOrEnd beginOrEnd)
{
    Vector<SMILTime>& timeList = beginOrEnd == Begin ? m_beginTimes : m_endTimes;
    if (beginOrEnd == End)
        m_hasEndEventConditions = false;
    HashSet<double> existing;
    for (unsigned n = 0; n < timeList.size(); ++n)
        existing.add(timeList[n].value());
    Vector<String> splitString;
    parseString.split(';', splitString);
    for (unsigned n = 0; n < splitString.size(); ++n) {
        SMILTime value = parseClockValue(splitString[n]);
        if (value.isUnresolved())
            parseCondition(splitString[n], beginOrEnd);
        else if (!existing.contains(value.value()))
            timeList.append(value);
    }
    sortTimeList(timeList);
}

void SVGSMILElement::parseMappedAttribute(MappedAttribute* attr)
{
    if (attr->name() == SVGNames::beginAttr) {
        if (!m_conditions.isEmpty()) {
            disconnectConditions();
            m_conditions.clear();
            parseBeginOrEnd(getAttribute(SVGNames::endAttr), End);
        }
        parseBeginOrEnd(attr->value().string(), Begin);
        if (inDocument())
            connectConditions();
    } else if (attr->name() == SVGNames::endAttr) {
        if (!m_conditions.isEmpty()) {
            disconnectConditions();
            m_conditions.clear();
            parseBeginOrEnd(getAttribute(SVGNames::beginAttr), Begin);
        }
        parseBeginOrEnd(attr->value().string(), End);
        if (inDocument())
            connectConditions();
    } else
        SVGElement::parseMappedAttribute(attr);
}

void SVGSMILElement::attributeChanged(Attribute* attr, bool preserveDecls)
{
    SVGElement::attributeChanged(attr, preserveDecls);
    
    const QualifiedName& attrName = attr->name();
    if (attrName == SVGNames::durAttr)
        m_cachedDur = invalidCachedTime;
    else if (attrName == SVGNames::repeatDurAttr)
        m_cachedRepeatDur = invalidCachedTime;
    else if (attrName == SVGNames::repeatCountAttr)
        m_cachedRepeatCount = invalidCachedTime;
    else if (attrName == SVGNames::minAttr)
        m_cachedMin = invalidCachedTime;
    else if (attrName == SVGNames::maxAttr)
        m_cachedMax = invalidCachedTime;
    
    if (inDocument()) {
        if (attrName == SVGNames::beginAttr)
            beginListChanged();
        else if (attrName == SVGNames::endAttr)
            endListChanged();
    }
}

void SVGSMILElement::connectConditions()
{
    if (m_conditionsConnected)
        disconnectConditions();
    m_conditionsConnected = true;
    for (unsigned n = 0; n < m_conditions.size(); ++n) {
        Condition& condition = m_conditions[n];
        if (condition.m_type == Condition::EventBase) {
            ASSERT(!condition.m_syncbase);
            Element* eventBase = condition.m_baseID.isEmpty() ? targetElement() : document()->getElementById(condition.m_baseID);
            if (!eventBase)
                continue;
            ASSERT(!condition.m_eventListener);
            condition.m_eventListener = ConditionEventListener::create(this, eventBase, &condition);
        } else if (condition.m_type == Condition::Syncbase) {
            ASSERT(!condition.m_baseID.isEmpty());
            condition.m_syncbase = document()->getElementById(condition.m_baseID);
            if (!isSMILElement(condition.m_syncbase.get())) {
                condition.m_syncbase = 0;
                continue;
            }
            SVGSMILElement* syncbase = static_cast<SVGSMILElement*>(condition.m_syncbase.get());
            syncbase->addTimeDependent(this);
        }
    }
}

void SVGSMILElement::disconnectConditions()
{
    if (!m_conditionsConnected)
        return;
    m_conditionsConnected = false;
    for (unsigned n = 0; n < m_conditions.size(); ++n) {
        Condition& condition = m_conditions[n];
        if (condition.m_type == Condition::EventBase) {
            ASSERT(!condition.m_syncbase);
            if (condition.m_eventListener) {
                condition.m_eventListener->unregister();
                condition.m_eventListener = 0;
            }
        } else if (condition.m_type == Condition::Syncbase) {
            if (condition.m_syncbase) {
                ASSERT(isSMILElement(condition.m_syncbase.get()));
                static_cast<SVGSMILElement*>(condition.m_syncbase.get())->removeTimeDependent(this);
            }
        }
        condition.m_syncbase = 0;
    }
}

void SVGSMILElement::reschedule()
{
    if (m_timeContainer)
        m_timeContainer->schedule(this);
}

SVGElement* SVGSMILElement::targetElement() const
{
    String href = xlinkHref();
    Node* target = href.isEmpty() ? parentNode() : document()->getElementById(SVGURIReference::getTarget(href));
    if (target && target->isSVGElement())
        return static_cast<SVGElement*>(target);
    return 0;
}
    
String SVGSMILElement::attributeName() const
{    
    return getAttribute(SVGNames::attributeNameAttr).string().stripWhiteSpace();
}
    
SMILTime SVGSMILElement::elapsed() const 
{
    return m_timeContainer ? m_timeContainer->elapsed() : 0;
} 
    
bool SVGSMILElement::isInactive() const
{
     return m_activeState == Inactive;
}

bool SVGSMILElement::isFrozen() const
{
    return m_activeState == Frozen;
}
    
SVGSMILElement::Restart SVGSMILElement::restart() const
{    
    DEFINE_STATIC_LOCAL(const AtomicString, never, ("never"));
    DEFINE_STATIC_LOCAL(const AtomicString, whenNotActive, ("whenNotActive"));
    const AtomicString& value = getAttribute(SVGNames::restartAttr);
    if (value == never)
        return RestartNever;
    if (value == whenNotActive)
        return RestartWhenNotActive;
    return RestartAlways;
}
    
SVGSMILElement::FillMode SVGSMILElement::fill() const
{   
    DEFINE_STATIC_LOCAL(const AtomicString, freeze, ("freeze"));
    const AtomicString& value = getAttribute(SVGNames::fillAttr);
    return value == freeze ? FillFreeze : FillRemove;
}
    
String SVGSMILElement::xlinkHref() const
{    
    return getAttribute(XLinkNames::hrefAttr);
}
    
SMILTime SVGSMILElement::dur() const
{   
    if (m_cachedDur != invalidCachedTime)
        return m_cachedDur;
    const AtomicString& value = getAttribute(SVGNames::durAttr);
    SMILTime clockValue = parseClockValue(value);
    return m_cachedDur = clockValue <= 0 ? SMILTime::unresolved() : clockValue;
}

SMILTime SVGSMILElement::repeatDur() const
{    
    if (m_cachedRepeatDur != invalidCachedTime)
        return m_cachedRepeatDur;
    const AtomicString& value = getAttribute(SVGNames::repeatDurAttr);
    SMILTime clockValue = parseClockValue(value);
    return m_cachedRepeatDur = clockValue < 0 ? SMILTime::unresolved() : clockValue;
}
    
// So a count is not really a time but let just all pretend we did not notice.
SMILTime SVGSMILElement::repeatCount() const
{    
    if (m_cachedRepeatCount != invalidCachedTime)
        return m_cachedRepeatCount;
    const AtomicString& value = getAttribute(SVGNames::repeatCountAttr);
    if (value.isNull())
        return SMILTime::unresolved();

    DEFINE_STATIC_LOCAL(const AtomicString, indefiniteValue, ("indefinite"));
    if (value == indefiniteValue)
        return SMILTime::indefinite();
    bool ok;
    double result = value.string().toDouble(&ok);
    return m_cachedRepeatCount = ok && result > 0 ? result : SMILTime::unresolved();
}

SMILTime SVGSMILElement::maxValue() const
{    
    if (m_cachedMax != invalidCachedTime)
        return m_cachedMax;
    const AtomicString& value = getAttribute(SVGNames::maxAttr);
    SMILTime result = parseClockValue(value);
    return m_cachedMax = (result.isUnresolved() || result < 0) ? SMILTime::indefinite() : result;
}
    
SMILTime SVGSMILElement::minValue() const
{    
    if (m_cachedMin != invalidCachedTime)
        return m_cachedMin;
    const AtomicString& value = getAttribute(SVGNames::minAttr);
    SMILTime result = parseClockValue(value);
    return m_cachedMin = (result.isUnresolved() || result < 0) ? 0 : result;
}
                 
SMILTime SVGSMILElement::simpleDuration() const
{
    return min(dur(), SMILTime::indefinite());
}
    
void SVGSMILElement::addBeginTime(SMILTime time)
{
    m_beginTimes.append(time);
    sortTimeList(m_beginTimes);
    beginListChanged();
}
    
void SVGSMILElement::addEndTime(SMILTime time)
{
    m_endTimes.append(time);
    sortTimeList(m_endTimes);
    endListChanged();
}
    
SMILTime SVGSMILElement::findInstanceTime(BeginOrEnd beginOrEnd, SMILTime minimumTime, bool equalsMinimumOK) const
{
    // FIXME: This searches from the beginning which is inefficient. The list is usually not long
    // (one entry in common cases) but you can construct a case where it does grow.
    const Vector<SMILTime>& list = beginOrEnd == Begin ? m_beginTimes : m_endTimes;
    for (unsigned n = 0; n < list.size(); ++n) {
        SMILTime time = list[n];
        ASSERT(!time.isUnresolved());
        if (time.isIndefinite() && beginOrEnd == Begin) {
            // "The special value "indefinite" does not yield an instance time in the begin list."
            continue;
        }
        if (equalsMinimumOK) {
            if (time >= minimumTime)
                return time;
        } else if (time > minimumTime)
            return time;
    }
    return SMILTime::unresolved();
}

SMILTime SVGSMILElement::repeatingDuration() const
{
    // Computing the active duration
    // http://www.w3.org/TR/SMIL2/smil-timing.html#Timing-ComputingActiveDur
    SMILTime repeatCount = this->repeatCount();
    SMILTime repeatDur = this->repeatDur();
    SMILTime simpleDuration = this->simpleDuration();
    if (simpleDuration == 0 || (repeatDur.isUnresolved() && repeatCount.isUnresolved()))
        return simpleDuration;
    SMILTime repeatCountDuration = simpleDuration * repeatCount;
    return min(repeatCountDuration, min(repeatDur, SMILTime::indefinite()));
}

SMILTime SVGSMILElement::resolveActiveEnd(SMILTime resolvedBegin, SMILTime resolvedEnd) const
{
    // Computing the active duration
    // http://www.w3.org/TR/SMIL2/smil-timing.html#Timing-ComputingActiveDur
    SMILTime preliminaryActiveDuration;
    if (!resolvedEnd.isUnresolved() && dur().isUnresolved() && repeatDur().isUnresolved() && repeatCount().isUnresolved())
        preliminaryActiveDuration = resolvedEnd - resolvedBegin;
    else if (!resolvedEnd.isFinite())
        preliminaryActiveDuration = repeatingDuration();
    else
        preliminaryActiveDuration = min(repeatingDuration(), resolvedEnd - resolvedBegin);
    
    SMILTime minValue = this->minValue();
    SMILTime maxValue = this->maxValue();
    if (minValue > maxValue) {
        // Ignore both.
        // http://www.w3.org/TR/2001/REC-smil-animation-20010904/#MinMax
        minValue = 0;
        maxValue = SMILTime::indefinite();
    }
    return resolvedBegin + min(maxValue, max(minValue, preliminaryActiveDuration));
}

void SVGSMILElement::resolveInterval(bool first, SMILTime& beginResult, SMILTime& endResult) const
{
    // See the pseudocode in
    // http://www.w3.org/TR/2001/REC-smil-animation-20010904/#Timing-BeginEnd-LifeCycle
    SMILTime beginAfter = first ? -numeric_limits<double>::infinity() : m_intervalEnd;
    SMILTime lastIntervalTempEnd = numeric_limits<double>::infinity();
    while (true) {
        SMILTime tempBegin = findInstanceTime(Begin, beginAfter, true);
        if (tempBegin.isUnresolved())
            break;
        SMILTime tempEnd;
        if (m_endTimes.isEmpty())
            tempEnd = resolveActiveEnd(tempBegin, SMILTime::indefinite());
        else {
            tempEnd = findInstanceTime(End, tempBegin, true);
            if ((first && tempBegin == tempEnd && tempEnd == lastIntervalTempEnd) || (!first && tempEnd == m_intervalEnd)) 
                tempEnd = findInstanceTime(End, tempBegin, false);    
            if (tempEnd.isUnresolved()) {
                if (!m_endTimes.isEmpty() && !m_hasEndEventConditions)
                    break;
            }
            tempEnd = resolveActiveEnd(tempBegin, tempEnd);
        }
        if (tempEnd > 0 || !first) {
            beginResult = tempBegin;
            endResult = tempEnd;
            return;
        } else if (restart() == RestartNever)
            break;
        else
            beginAfter = tempEnd;
        lastIntervalTempEnd = tempEnd;
    }
    beginResult = SMILTime::unresolved();
    endResult = SMILTime::unresolved();
}
    
void SVGSMILElement::resolveFirstInterval()
{
    SMILTime begin;
    SMILTime end;
    resolveInterval(true, begin, end);
    ASSERT(!begin.isIndefinite());

    if (!begin.isUnresolved() && (begin != m_intervalBegin || end != m_intervalEnd)) {   
        bool wasUnresolved = m_intervalBegin.isUnresolved();
        m_intervalBegin = begin;
        m_intervalEnd = end;
        notifyDependentsIntervalChanged(wasUnresolved ? NewInterval : ExistingInterval);
        m_nextProgressTime = min(m_nextProgressTime, m_intervalBegin);
        reschedule();
    }
}

void SVGSMILElement::resolveNextInterval()
{
    SMILTime begin;
    SMILTime end;
    resolveInterval(false, begin, end);
    ASSERT(!begin.isIndefinite());
    
    if (!begin.isUnresolved() && begin != m_intervalBegin) {
        m_intervalBegin = begin;
        m_intervalEnd = end;
        notifyDependentsIntervalChanged(NewInterval);
        m_nextProgressTime = min(m_nextProgressTime, m_intervalBegin);
    }
}

SMILTime SVGSMILElement::nextProgressTime() const
{
    return m_nextProgressTime;
}
    
void SVGSMILElement::beginListChanged()
{
    SMILTime elapsed = this->elapsed();
    if (m_isWaitingForFirstInterval)
        resolveFirstInterval();
    else if (elapsed < m_intervalBegin) {
        SMILTime newBegin = findInstanceTime(Begin, elapsed, false);
        if (newBegin < m_intervalBegin) {
            // Begin time changed, re-resolve the interval.
            SMILTime oldBegin = m_intervalBegin;
            m_intervalBegin = elapsed;
            resolveInterval(false, m_intervalBegin, m_intervalEnd);  
            ASSERT(!m_intervalBegin.isUnresolved());
            if (m_intervalBegin != oldBegin)
                notifyDependentsIntervalChanged(ExistingInterval);
        }
    }
    m_nextProgressTime = elapsed;
    reschedule();
}

void SVGSMILElement::endListChanged()
{
    SMILTime elapsed = this->elapsed();
    if (m_isWaitingForFirstInterval)
        resolveFirstInterval();
    else if (elapsed < m_intervalEnd && m_intervalBegin.isFinite()) {
        SMILTime newEnd = findInstanceTime(End, m_intervalBegin, false);
        if (newEnd < m_intervalEnd) {
            newEnd = resolveActiveEnd(m_intervalBegin, newEnd);
            if (newEnd != m_intervalEnd) {
                m_intervalEnd = newEnd;
                notifyDependentsIntervalChanged(ExistingInterval);
            }
        }
    }
    m_nextProgressTime = elapsed;
    reschedule();
}
    
void SVGSMILElement::checkRestart(SMILTime elapsed)
{
    ASSERT(!m_isWaitingForFirstInterval);
    ASSERT(elapsed >= m_intervalBegin);
    
    Restart restart = this->restart();
    if (restart == RestartNever)
        return;
    
    if (elapsed < m_intervalEnd) {
        if (restart != RestartAlways)
            return;
        SMILTime nextBegin = findInstanceTime(Begin, m_intervalBegin, false);
        if (nextBegin < m_intervalEnd) { 
            m_intervalEnd = nextBegin;
            notifyDependentsIntervalChanged(ExistingInterval);
        }
    } 
    if (elapsed >= m_intervalEnd)
        resolveNextInterval();
}
    
float SVGSMILElement::calculateAnimationPercentAndRepeat(SMILTime elapsed, unsigned& repeat) const
{
    SMILTime simpleDuration = this->simpleDuration();
    repeat = 0;
    if (simpleDuration.isIndefinite()) {
        repeat = 0;
        return 0.f;
    }
    if (simpleDuration == 0) {
        repeat = 0;
        return 1.f;
    }
    ASSERT(m_intervalBegin.isFinite());
    ASSERT(simpleDuration.isFinite());
    SMILTime activeTime = elapsed - m_intervalBegin;
    SMILTime repeatingDuration = this->repeatingDuration();
    if (elapsed >= m_intervalEnd || activeTime > repeatingDuration) {
        repeat = static_cast<unsigned>(repeatingDuration.value() / simpleDuration.value());
        if (fmod(repeatingDuration.value(), simpleDuration.value() == 0.))
            repeat--;
        return 1.f;
    }
    repeat = static_cast<unsigned>(activeTime.value() / simpleDuration.value());
    SMILTime simpleTime = fmod(activeTime.value(), simpleDuration.value());
    return narrowPrecisionToFloat(simpleTime.value() / simpleDuration.value());
}
    
SMILTime SVGSMILElement::calculateNextProgressTime(SMILTime elapsed) const
{
    if (m_activeState == Active) {
        // If duration is indefinite the value does not actually change over time. Same is true for <set>.
        SMILTime simpleDuration = this->simpleDuration();
        if (simpleDuration.isIndefinite() || hasTagName(SVGNames::setTag)) {
            SMILTime repeatCount = this->repeatCount();
            SMILTime repeatingDurationEnd = m_intervalBegin + repeatingDuration();
            // We are supposed to do freeze semantics when repeating ends, even if the element is still active. 
            // Take care that we get a timer callback at that point.
            if (elapsed < repeatingDurationEnd && repeatingDurationEnd < m_intervalEnd && repeatingDurationEnd.isFinite())
                return repeatingDurationEnd;
            return m_intervalEnd;
        } 
        return elapsed + 0.025;
    }
    return m_intervalBegin >= elapsed ? m_intervalBegin : SMILTime::unresolved();
}
    
SVGSMILElement::ActiveState SVGSMILElement::determineActiveState(SMILTime elapsed) const
{
    if (elapsed >= m_intervalBegin && elapsed < m_intervalEnd)
        return Active;

    if (m_activeState == Active)
        return fill() == FillFreeze ? Frozen : Inactive;

    return m_activeState;
}
    
bool SVGSMILElement::isContributing(SMILTime elapsed) const 
{ 
    // Animation does not contribute during the active time if it is past its repeating duration and has fill=remove.
    return (m_activeState == Active && (fill() == FillFreeze || elapsed <= m_intervalBegin + repeatingDuration())) || m_activeState == Frozen;
}
    
void SVGSMILElement::progress(SMILTime elapsed, SVGSMILElement* resultElement)
{
    ASSERT(m_timeContainer);
    ASSERT(m_isWaitingForFirstInterval || m_intervalBegin.isFinite());
    
    if (!m_conditionsConnected)
        connectConditions();
    
    if (!m_intervalBegin.isFinite()) {
        ASSERT(m_activeState == Inactive);
        m_nextProgressTime = SMILTime::unresolved();
        return;
    }
    
    if (elapsed < m_intervalBegin) {
        ASSERT(m_activeState != Active);
        if (m_activeState == Frozen && resultElement)
            updateAnimation(m_lastPercent, m_lastRepeat, resultElement);
        m_nextProgressTime = m_intervalBegin;
        return;
    }
    
    m_previousIntervalBegin = m_intervalBegin;
    
    if (m_activeState == Inactive) {
        m_isWaitingForFirstInterval = false;
        m_activeState = Active;
        startedActiveInterval();
    }
    
    unsigned repeat;
    float percent = calculateAnimationPercentAndRepeat(elapsed, repeat);

    checkRestart(elapsed);

    ActiveState oldActiveState = m_activeState;
    m_activeState = determineActiveState(elapsed);
    
    if (isContributing(elapsed)) {
        if (resultElement)
            updateAnimation(percent, repeat, resultElement);
        m_lastPercent = percent;
        m_lastRepeat = repeat;
    }

    if (oldActiveState == Active && m_activeState != Active)
        endedActiveInterval();

    m_nextProgressTime = calculateNextProgressTime(elapsed);
}
    
void SVGSMILElement::notifyDependentsIntervalChanged(NewOrExistingInterval newOrExisting)
{
    ASSERT(m_intervalBegin.isFinite());
    DEFINE_STATIC_LOCAL(HashSet<SVGSMILElement*>, loopBreaker, ());
    if (loopBreaker.contains(this))
        return;
    loopBreaker.add(this);
    
    TimeDependentSet::iterator end = m_timeDependents.end();
    for (TimeDependentSet::iterator it = m_timeDependents.begin(); it != end; ++it) {
        SVGSMILElement* dependent = *it;
        dependent->createInstanceTimesFromSyncbase(this, newOrExisting);
    }

    loopBreaker.remove(this);
}
    
void SVGSMILElement::createInstanceTimesFromSyncbase(SVGSMILElement* syncbase, NewOrExistingInterval)
{
    // FIXME: To be really correct, this should handle updating exising interval by changing 
    // the associated times instead of creating new ones.
    for (unsigned n = 0; n < m_conditions.size(); ++n) {
        Condition& condition = m_conditions[n];
        if (condition.m_type == Condition::Syncbase && condition.m_syncbase == syncbase) {
            ASSERT(condition.m_name == "begin" || condition.m_name == "end");
            // No nested time containers in SVG, no need for crazy time space conversions. Phew!
            SMILTime time = 0;
            if (condition.m_name == "begin")
                time = syncbase->m_intervalBegin + condition.m_offset;
            else
                time = syncbase->m_intervalEnd + condition.m_offset;
            ASSERT(time.isFinite());
            if (condition.m_beginOrEnd == Begin)
                addBeginTime(time);
            else
                addEndTime(time);
        }
    }
}
    
void SVGSMILElement::addTimeDependent(SVGSMILElement* animation)
{
    m_timeDependents.add(animation);
    if (m_intervalBegin.isFinite())
        animation->createInstanceTimesFromSyncbase(this, NewInterval);
}
    
void SVGSMILElement::removeTimeDependent(SVGSMILElement* animation)
{
    m_timeDependents.remove(animation);
}
    
void SVGSMILElement::handleConditionEvent(Event*, Condition* condition)
{
    if (condition->m_beginOrEnd == Begin)
        addBeginTime(elapsed() + condition->m_offset);
    else
        addEndTime(elapsed() + condition->m_offset);
}

void SVGSMILElement::beginByLinkActivation()
{
    addBeginTime(elapsed());
}
    
}

#endif

