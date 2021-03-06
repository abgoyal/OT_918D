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
    Copyright (C) 2007 Eric Seidel <eric@webkit.org>
              (C) 2007 Rob Buis <buis@kde.org>
    Copyright (C) 2008 Apple Inc. All Rights Reserved.

    This file is part of the WebKit project

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

#if ENABLE(SVG) && ENABLE(SVG_ANIMATION)
#include "SVGAnimateMotionElement.h"

#include "MappedAttribute.h"
#include "RenderObject.h"
#include "SVGElementInstance.h"
#include "SVGMPathElement.h"
#include "SVGParserUtilities.h"
#include "SVGPathElement.h"
#include "SVGTransformList.h"
#include <math.h>
#include <wtf/StdLibExtras.h>

namespace WebCore {
    
using namespace SVGNames;

SVGAnimateMotionElement::SVGAnimateMotionElement(const QualifiedName& tagName, Document* doc)
    : SVGAnimationElement(tagName, doc)
    , m_baseIndexInTransformList(0)
    , m_angle(0)
{
}

SVGAnimateMotionElement::~SVGAnimateMotionElement()
{
}

bool SVGAnimateMotionElement::hasValidTarget() const
{
    if (!SVGAnimationElement::hasValidTarget())
        return false;
    SVGElement* targetElement = this->targetElement();
    if (!targetElement->isStyledTransformable() && !targetElement->hasTagName(SVGNames::textTag))
        return false;
    // Spec: SVG 1.1 section 19.2.15
    if (targetElement->hasTagName(gTag)
        || targetElement->hasTagName(defsTag)
        || targetElement->hasTagName(useTag)
        || targetElement->hasTagName(imageTag)
        || targetElement->hasTagName(switchTag)
        || targetElement->hasTagName(pathTag)
        || targetElement->hasTagName(rectTag)
        || targetElement->hasTagName(circleTag)
        || targetElement->hasTagName(ellipseTag)
        || targetElement->hasTagName(lineTag)
        || targetElement->hasTagName(polylineTag)
        || targetElement->hasTagName(polygonTag)
        || targetElement->hasTagName(textTag)
        || targetElement->hasTagName(clipPathTag)
        || targetElement->hasTagName(maskTag)
        || targetElement->hasTagName(aTag)
#if ENABLE(SVG_FOREIGN_OBJECT)
        || targetElement->hasTagName(foreignObjectTag)
#endif
        )
        return true;
    return false;
}

void SVGAnimateMotionElement::parseMappedAttribute(MappedAttribute* attr)
{
    if (attr->name() == SVGNames::pathAttr) {
        m_path = Path();
        pathFromSVGData(m_path, attr->value());
    } else
        SVGAnimationElement::parseMappedAttribute(attr);
}
    
SVGAnimateMotionElement::RotateMode SVGAnimateMotionElement::rotateMode() const
{
    DEFINE_STATIC_LOCAL(const AtomicString, autoVal, ("auto"));
    DEFINE_STATIC_LOCAL(const AtomicString, autoReverse, ("auto-reverse"));
    String rotate = getAttribute(SVGNames::rotateAttr);
    if (rotate == autoVal)
        return RotateAuto;
    if (rotate == autoReverse)
        return RotateAutoReverse;
    return RotateAngle;
}

Path SVGAnimateMotionElement::animationPath() const
{
    for (Node* child = firstChild(); child; child = child->nextSibling()) {
        if (child->hasTagName(SVGNames::mpathTag)) {
            SVGMPathElement* mPath = static_cast<SVGMPathElement*>(child);
            SVGPathElement* pathElement = mPath->pathElement();
            if (pathElement)
                return pathElement->toPathData();
            return Path();
        }
    }
    if (hasAttribute(SVGNames::pathAttr))
        return m_path;
    return Path();
}

static bool parsePoint(const String& s, FloatPoint& point)
{
    if (s.isEmpty())
        return false;
    const UChar* cur = s.characters();
    const UChar* end = cur + s.length();
    
    if (!skipOptionalSpaces(cur, end))
        return false;
    
    float x = 0.0f;
    if (!parseNumber(cur, end, x))
        return false;
    
    float y = 0.0f;
    if (!parseNumber(cur, end, y))
        return false;
    
    point = FloatPoint(x, y);
    
    // disallow anything except spaces at the end
    return !skipOptionalSpaces(cur, end);
}
    
void SVGAnimateMotionElement::resetToBaseValue(const String&)
{
    if (!hasValidTarget())
        return;
    SVGElement* target = targetElement();
    AffineTransform* transform = target->supplementalTransform();
    if (!transform)
        return;
    transform->makeIdentity();
}

bool SVGAnimateMotionElement::calculateFromAndToValues(const String& fromString, const String& toString)
{
    parsePoint(fromString, m_fromPoint);
    parsePoint(toString, m_toPoint);
    return true;
}
    
bool SVGAnimateMotionElement::calculateFromAndByValues(const String& fromString, const String& byString)
{
    parsePoint(fromString, m_fromPoint);
    FloatPoint byPoint;
    parsePoint(byString, byPoint);
    m_toPoint = FloatPoint(m_fromPoint.x() + byPoint.x(), m_fromPoint.y() + byPoint.y());
    return true;
}

void SVGAnimateMotionElement::calculateAnimatedValue(float percentage, unsigned, SVGSMILElement*)
{
    SVGElement* target = targetElement();
    if (!target)
        return;
    AffineTransform* transform = target->supplementalTransform();
    if (!transform)
        return;
    
    if (!isAdditive())
        transform->makeIdentity();
    
    // FIXME: Implement accumulate.
    
    if (animationMode() == PathAnimation) {
        ASSERT(!animationPath().isEmpty());
        Path path = animationPath();
        float positionOnPath = path.length() * percentage;
        bool ok;
        FloatPoint position = path.pointAtLength(positionOnPath, ok);
        if (ok) {
            transform->translate(position.x(), position.y());
            RotateMode rotateMode = this->rotateMode();
            if (rotateMode == RotateAuto || rotateMode == RotateAutoReverse) {
                float angle = path.normalAngleAtLength(positionOnPath, ok);
                if (rotateMode == RotateAutoReverse)
                    angle += 180.f;
                transform->rotate(angle);
            }
        }
        return;
    }
    FloatSize diff = m_toPoint - m_fromPoint;
    transform->translate(diff.width() * percentage + m_fromPoint.x(), diff.height() * percentage + m_fromPoint.y());
}
    
void SVGAnimateMotionElement::applyResultsToTarget()
{
    // We accumulate to the target element transform list so there is not much to do here.
    SVGElement* targetElement = this->targetElement();
    if (targetElement && targetElement->renderer())
        targetElement->renderer()->setNeedsLayout(true);
    
    // ...except in case where we have additional instances in <use> trees.
    const HashSet<SVGElementInstance*>& instances = targetElement->instancesForElement();
    const HashSet<SVGElementInstance*>::const_iterator end = instances.end();
    for (HashSet<SVGElementInstance*>::const_iterator it = instances.begin(); it != end; ++it) {
        SVGElement* shadowTreeElement = (*it)->shadowTreeElement();
        ASSERT(shadowTreeElement);
        AffineTransform* transform = shadowTreeElement->supplementalTransform();
        AffineTransform* t = targetElement->supplementalTransform();
        transform->setMatrix(t->a(), t->b(), t->c(), t->d(), t->e(), t->f());
        if (shadowTreeElement->renderer())
            shadowTreeElement->renderer()->setNeedsLayout(true);
    }
}

float SVGAnimateMotionElement::calculateDistance(const String& fromString, const String& toString)
{
    FloatPoint from;
    FloatPoint to;
    if (!parsePoint(fromString, from))
        return -1.f;
    if (!parsePoint(toString, to))
        return -1.f;
    FloatSize diff = to - from;
    return sqrtf(diff.width() * diff.width() + diff.height() * diff.height());
}

}

#endif // ENABLE(SVG)

// vim:ts=4:noet
