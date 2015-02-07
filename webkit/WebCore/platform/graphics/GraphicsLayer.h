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
 * Copyright (C) 2009 Apple Inc. All rights reserved.
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
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#ifndef GraphicsLayer_h
#define GraphicsLayer_h

#if USE(ACCELERATED_COMPOSITING)

#include "Animation.h"
#include "Color.h"
#include "FloatPoint.h"
#include "FloatPoint3D.h"
#include "FloatSize.h"
#if ENABLE(3D_CANVAS)
#include "GraphicsContext3D.h"
#endif
#include "GraphicsLayerClient.h"
#include "IntRect.h"
#include "TransformationMatrix.h"
#include "TransformOperations.h"
#include <wtf/OwnPtr.h>
#include <wtf/PassOwnPtr.h>

#if PLATFORM(MAC)
#ifdef __OBJC__
@class WebLayer;
@class CALayer;
typedef CALayer PlatformLayer;
typedef CALayer* NativeLayer;
#else
typedef void* PlatformLayer;
typedef void* NativeLayer;
#endif
#elif PLATFORM(WIN)
namespace WebCore {
class WKCACFLayer;
typedef WKCACFLayer PlatformLayer;
typedef void* NativeLayer;
}
#elif PLATFORM(QT)
class QGraphicsItem;
typedef QGraphicsItem PlatformLayer;
typedef QGraphicsItem* NativeLayer;
#else
typedef void* PlatformLayer;
typedef void* NativeLayer;
#endif

namespace WebCore {

class FloatPoint3D;
class GraphicsContext;
class Image;
class TextStream;
class TimingFunction;

// Base class for animation values (also used for transitions). Here to
// represent values for properties being animated via the GraphicsLayer,
// without pulling in style-related data from outside of the platform directory.
class AnimationValue : public Noncopyable {
public:
    AnimationValue(float keyTime, const TimingFunction* timingFunction = 0)
        : m_keyTime(keyTime)
        , m_timingFunction(0)
    {
        if (timingFunction)
            m_timingFunction.set(new TimingFunction(*timingFunction));
    }
    
    virtual ~AnimationValue() { }

    float keyTime() const { return m_keyTime; }
    const TimingFunction* timingFunction() const { return m_timingFunction.get(); }

private:
    float m_keyTime;
    OwnPtr<TimingFunction> m_timingFunction;
};

// Used to store one float value of an animation.
class FloatAnimationValue : public AnimationValue {
public:
    FloatAnimationValue(float keyTime, float value, const TimingFunction* timingFunction = 0)
        : AnimationValue(keyTime, timingFunction)
        , m_value(value)
    {
    }

    float value() const { return m_value; }

private:
    float m_value;
};

// Used to store one transform value in a keyframe list.
class TransformAnimationValue : public AnimationValue {
public:
    TransformAnimationValue(float keyTime, const TransformOperations* value = 0, const TimingFunction* timingFunction = 0)
        : AnimationValue(keyTime, timingFunction)
    {
        if (value)
            m_value.set(new TransformOperations(*value));
    }

    const TransformOperations* value() const { return m_value.get(); }

private:
    OwnPtr<TransformOperations> m_value;
};

// Used to store a series of values in a keyframe list. Values will all be of the same type,
// which can be inferred from the property.
class KeyframeValueList : public Noncopyable {
public:

    KeyframeValueList(AnimatedPropertyID property)
        : m_property(property)
    {
    }
    
    ~KeyframeValueList()
    {
        deleteAllValues(m_values);
    }
    
    AnimatedPropertyID property() const { return m_property; }

    size_t size() const { return m_values.size(); }
    const AnimationValue* at(size_t i) const { return m_values.at(i); }
    
    // Insert, sorted by keyTime. Takes ownership of the pointer.
    void insert(const AnimationValue*);
    
protected:
    Vector<const AnimationValue*> m_values;
    AnimatedPropertyID m_property;
};



// GraphicsLayer is an abstraction for a rendering surface with backing store,
// which may have associated transformation and animations.

class GraphicsLayer {
public:

    static PassOwnPtr<GraphicsLayer> create(GraphicsLayerClient*);
    
    virtual ~GraphicsLayer();

    GraphicsLayerClient* client() const { return m_client; }

    // Layer name. Only used to identify layers in debug output
    const String& name() const { return m_name; }
    virtual void setName(const String& name) { m_name = name; }

    // For hosting this GraphicsLayer in a native layer hierarchy.
    virtual NativeLayer nativeLayer() const { return 0; }

    GraphicsLayer* parent() const { return m_parent; };
    void setParent(GraphicsLayer* layer) { m_parent = layer; } // Internal use only.
    
    // Returns true if the layer has the given layer as an ancestor (excluding self).
    bool hasAncestor(GraphicsLayer*) const;
    
    const Vector<GraphicsLayer*>& children() const { return m_children; }
    // Returns true if the child list changed.
    virtual bool setChildren(const Vector<GraphicsLayer*>&);

    // Add child layers. If the child is already parented, it will be removed from its old parent.
    virtual void addChild(GraphicsLayer*);
    virtual void addChildAtIndex(GraphicsLayer*, int index);
    virtual void addChildAbove(GraphicsLayer* layer, GraphicsLayer* sibling);
    virtual void addChildBelow(GraphicsLayer* layer, GraphicsLayer* sibling);
    virtual bool replaceChild(GraphicsLayer* oldChild, GraphicsLayer* newChild);

    void removeAllChildren();
    virtual void removeFromParent();

    GraphicsLayer* maskLayer() const { return m_maskLayer; }
    virtual void setMaskLayer(GraphicsLayer* layer) { m_maskLayer = layer; }
    
    // The given layer will replicate this layer and its children; the replica renders behind this layer.
    virtual void setReplicatedByLayer(GraphicsLayer*);
    // Whether this layer is being replicated by another layer.
    bool isReplicated() const { return m_replicaLayer; }
    // The layer that replicates this layer (if any).
    GraphicsLayer* replicaLayer() const { return m_replicaLayer; }

    const FloatPoint& replicatedLayerPosition() const { return m_replicatedLayerPosition; }
    void setReplicatedLayerPosition(const FloatPoint& p) { m_replicatedLayerPosition = p; }

    // Offset is origin of the renderer minus origin of the graphics layer (so either zero or negative).
    IntSize offsetFromRenderer() const { return m_offsetFromRenderer; }
    void setOffsetFromRenderer(const IntSize& offset) { m_offsetFromRenderer = offset; }

    // The position of the layer (the location of its top-left corner in its parent)
    const FloatPoint& position() const { return m_position; }
    virtual void setPosition(const FloatPoint& p) { m_position = p; }
    
    // Anchor point: (0, 0) is top left, (1, 1) is bottom right. The anchor point
    // affects the origin of the transforms.
    const FloatPoint3D& anchorPoint() const { return m_anchorPoint; }
    virtual void setAnchorPoint(const FloatPoint3D& p) { m_anchorPoint = p; }

    // The bounds of the layer
    const FloatSize& size() const { return m_size; }
    virtual void setSize(const FloatSize& size) { m_size = size; }

    const TransformationMatrix& transform() const { return m_transform; }
    virtual void setTransform(const TransformationMatrix& t) { m_transform = t; }

    const TransformationMatrix& childrenTransform() const { return m_childrenTransform; }
    virtual void setChildrenTransform(const TransformationMatrix& t) { m_childrenTransform = t; }

    bool preserves3D() const { return m_preserves3D; }
    virtual void setPreserves3D(bool b) { m_preserves3D = b; }
    
    bool masksToBounds() const { return m_masksToBounds; }
    virtual void setMasksToBounds(bool b) { m_masksToBounds = b; }
    
    bool drawsContent() const { return m_drawsContent; }
    virtual void setDrawsContent(bool b) { m_drawsContent = b; }

    // The color used to paint the layer backgrounds
    const Color& backgroundColor() const { return m_backgroundColor; }
    virtual void setBackgroundColor(const Color&);
    virtual void clearBackgroundColor();
    bool backgroundColorSet() const { return m_backgroundColorSet; }

    // opaque means that we know the layer contents have no alpha
    bool contentsOpaque() const { return m_contentsOpaque; }
    virtual void setContentsOpaque(bool b) { m_contentsOpaque = b; }

    bool backfaceVisibility() const { return m_backfaceVisibility; }
    virtual void setBackfaceVisibility(bool b) { m_backfaceVisibility = b; }

    float opacity() const { return m_opacity; }
    virtual void setOpacity(float opacity) { m_opacity = opacity; }

    // Some GraphicsLayers paint only the foreground or the background content
    GraphicsLayerPaintingPhase paintingPhase() const { return m_paintingPhase; }
    void setPaintingPhase(GraphicsLayerPaintingPhase phase) { m_paintingPhase = phase; }

    virtual void setNeedsDisplay() = 0;
    // mark the given rect (in layer coords) as needing dispay. Never goes deep.
    virtual void setNeedsDisplayInRect(const FloatRect&) = 0;

    // Set that the position/size of the contents (image or video).
    IntRect contentsRect() const { return m_contentsRect; }
    virtual void setContentsRect(const IntRect& r) { m_contentsRect = r; }
    
    // Return true if the animation is handled by the compositing system. If this returns
    // false, the animation will be run by AnimationController.
    virtual bool addAnimation(const KeyframeValueList&, const IntSize& /*boxSize*/, const Animation*, const String& /*keyframesName*/, double /*timeOffset*/) { return false; }
    virtual void removeAnimationsForProperty(AnimatedPropertyID) { }
    virtual void removeAnimationsForKeyframes(const String& /* keyframesName */) { }
    virtual void pauseAnimation(const String& /* keyframesName */, double /*timeOffset*/) { }
    
    virtual void suspendAnimations(double time);
    virtual void resumeAnimations();
    
    // Layer contents
    virtual void setContentsToImage(Image*) { }
    virtual void setContentsToMedia(PlatformLayer*) { } // video or plug-in
    virtual void setContentsBackgroundColor(const Color&) { }
    
#if ENABLE(3D_CANVAS)
    virtual void setContentsToGraphicsContext3D(const GraphicsContext3D*) { }
    virtual void setGraphicsContext3DNeedsDisplay() { }
#endif
    // Callback from the underlying graphics system to draw layer contents.
    void paintGraphicsLayerContents(GraphicsContext&, const IntRect& clip);
    // Callback from the underlying graphics system when the layer has been displayed
    virtual void didDisplay(PlatformLayer*) { }
    
    virtual PlatformLayer* platformLayer() const { return 0; }
    
    void dumpLayer(TextStream&, int indent = 0) const;

    int repaintCount() const { return m_repaintCount; }
    int incrementRepaintCount() { return ++m_repaintCount; }

    // Report whether the underlying compositing system uses a top-down
    // or a bottom-up coordinate system.
    enum CompositingCoordinatesOrientation { CompositingCoordinatesTopDown, CompositingCoordinatesBottomUp };
    static CompositingCoordinatesOrientation compositingCoordinatesOrientation();

    // Set the geometry orientation (top-down, or bottom-up) for this layer, which also controls sublayer geometry.
    virtual void setGeometryOrientation(CompositingCoordinatesOrientation orientation) { m_geometryOrientation = orientation; }
    CompositingCoordinatesOrientation geometryOrientation() const { return m_geometryOrientation; }

    // Flippedness of the contents of this layer. Does not affect sublayer geometry.
    virtual void setContentsOrientation(CompositingCoordinatesOrientation orientation) { m_contentsOrientation = orientation; }
    CompositingCoordinatesOrientation contentsOrientation() const { return m_contentsOrientation; }

    bool showDebugBorders() { return m_client ? m_client->showDebugBorders() : false; }
    bool showRepaintCounter() { return m_client ? m_client->showRepaintCounter() : false; }
    
    void updateDebugIndicators();
    
    virtual void setDebugBackgroundColor(const Color&) { }
    virtual void setDebugBorder(const Color&, float /*borderWidth*/) { }
    // z-position is the z-equivalent of position(). It's only used for debugging purposes.
    virtual float zPosition() const { return m_zPosition; }
    virtual void setZPosition(float);

    virtual void distributeOpacity(float);
    virtual float accumulatedOpacity() const;

    // Some compositing systems may do internal batching to synchronize compositing updates
    // with updates drawn into the window. This is a signal to flush any internal batched state.
    virtual void syncCompositingState() { }

protected:

    typedef Vector<TransformOperation::OperationType> TransformOperationList;
    // Given a list of TransformAnimationValues, return an array of transform operations.
    // On return, if hasBigRotation is true, functions contain rotations of >= 180 degrees
    static void fetchTransformOperationList(const KeyframeValueList&, TransformOperationList&, bool& isValid, bool& hasBigRotation);

    virtual void setOpacityInternal(float) { }
    
    // The layer being replicated.
    GraphicsLayer* replicatedLayer() const { return m_replicatedLayer; }
    virtual void setReplicatedLayer(GraphicsLayer* layer) { m_replicatedLayer = layer; }

    GraphicsLayer(GraphicsLayerClient*);

    void dumpProperties(TextStream&, int indent) const;

    GraphicsLayerClient* m_client;
    String m_name;
    
    // Offset from the owning renderer
    IntSize m_offsetFromRenderer;
    
    // Position is relative to the parent GraphicsLayer
    FloatPoint m_position;
    FloatPoint3D m_anchorPoint;
    FloatSize m_size;
    TransformationMatrix m_transform;
    TransformationMatrix m_childrenTransform;

    Color m_backgroundColor;
    float m_opacity;
    float m_zPosition;

    bool m_backgroundColorSet : 1;
    bool m_contentsOpaque : 1;
    bool m_preserves3D: 1;
    bool m_backfaceVisibility : 1;
    bool m_usingTiledLayer : 1;
    bool m_masksToBounds : 1;
    bool m_drawsContent : 1;

    GraphicsLayerPaintingPhase m_paintingPhase;
    CompositingCoordinatesOrientation m_geometryOrientation;    // affects geometry of layer positions
    CompositingCoordinatesOrientation m_contentsOrientation;    // affects orientation of layer contents

    Vector<GraphicsLayer*> m_children;
    GraphicsLayer* m_parent;

    GraphicsLayer* m_maskLayer; // Reference to mask layer. We don't own this.

    GraphicsLayer* m_replicaLayer; // A layer that replicates this layer. We only allow one, for now.
                                   // The replica is not parented; this is the primary reference to it.
    GraphicsLayer* m_replicatedLayer; // For a replica layer, a reference to the original layer.
    FloatPoint m_replicatedLayerPosition; // For a replica layer, the position of the replica.

    IntRect m_contentsRect;

    int m_repaintCount;
};


} // namespace WebCore

#endif // USE(ACCELERATED_COMPOSITING)

#endif // GraphicsLayer_h

