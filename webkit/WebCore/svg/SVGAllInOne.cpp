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
 * Copyright (C) 2009 Apple Inc. All Rights Reserved.
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

// This all-in-one cpp file cuts down on template bloat to allow us to build our Windows release build.

#include "ColorDistance.cpp"
#include "SVGAElement.cpp"
#include "SVGAltGlyphElement.cpp"
#include "SVGAngle.cpp"
#include "SVGAnimateColorElement.cpp"
#include "SVGAnimatedPathData.cpp"
#include "SVGAnimatedPoints.cpp"
#include "SVGAnimateElement.cpp"
#include "SVGAnimateMotionElement.cpp"
#include "SVGAnimateTransformElement.cpp"
#include "SVGAnimationElement.cpp"
#include "SVGCircleElement.cpp"
#include "SVGClipPathElement.cpp"
#include "SVGColor.cpp"
#include "SVGComponentTransferFunctionElement.cpp"
#include "SVGCursorElement.cpp"
#include "SVGDefsElement.cpp"
#include "SVGDescElement.cpp"
#include "SVGDocument.cpp"
#include "SVGDocumentExtensions.cpp"
#include "SVGElement.cpp"
#include "SVGElementInstance.cpp"
#include "SVGElementInstanceList.cpp"
#include "SVGEllipseElement.cpp"
#include "SVGExternalResourcesRequired.cpp"
#include "SVGFEBlendElement.cpp"
#include "SVGFEColorMatrixElement.cpp"
#include "SVGFEComponentTransferElement.cpp"
#include "SVGFECompositeElement.cpp"
#include "SVGFEDiffuseLightingElement.cpp"
#include "SVGFEDisplacementMapElement.cpp"
#include "SVGFEDistantLightElement.cpp"
#include "SVGFEFloodElement.cpp"
#include "SVGFEFuncAElement.cpp"
#include "SVGFEFuncBElement.cpp"
#include "SVGFEFuncGElement.cpp"
#include "SVGFEFuncRElement.cpp"
#include "SVGFEGaussianBlurElement.cpp"
#include "SVGFEImageElement.cpp"
#include "SVGFELightElement.cpp"
#include "SVGFEMergeElement.cpp"
#include "SVGFEMergeNodeElement.cpp"
#include "SVGFEMorphologyElement.cpp"
#include "SVGFEOffsetElement.cpp"
#include "SVGFEPointLightElement.cpp"
#include "SVGFESpecularLightingElement.cpp"
#include "SVGFESpotLightElement.cpp"
#include "SVGFETileElement.cpp"
#include "SVGFETurbulenceElement.cpp"
#include "SVGFilterElement.cpp"
#include "SVGFilterPrimitiveStandardAttributes.cpp"
#include "SVGFitToViewBox.cpp"
#include "SVGFont.cpp"
#include "SVGFontData.cpp"
#include "SVGFontElement.cpp"
#include "SVGFontFaceElement.cpp"
#include "SVGFontFaceFormatElement.cpp"
#include "SVGFontFaceNameElement.cpp"
#include "SVGFontFaceSrcElement.cpp"
#include "SVGFontFaceUriElement.cpp"
#include "SVGForeignObjectElement.cpp"
#include "SVGGElement.cpp"
#include "SVGGlyphElement.cpp"
#include "SVGGradientElement.cpp"
#include "SVGHKernElement.cpp"
#include "SVGImageElement.cpp"
#include "SVGImageLoader.cpp"
#include "SVGLangSpace.cpp"
#include "SVGLength.cpp"
#include "SVGLengthList.cpp"
#include "SVGLineElement.cpp"
#include "SVGLinearGradientElement.cpp"
#include "SVGLocatable.cpp"
#include "SVGMPathElement.cpp"
#include "SVGMarkerElement.cpp"
#include "SVGMaskElement.cpp"
#include "SVGMetadataElement.cpp"
#include "SVGMissingGlyphElement.cpp"
#include "SVGNumberList.cpp"
#include "SVGPaint.cpp"
#include "SVGParserUtilities.cpp"
#include "SVGPathElement.cpp"
#include "SVGPathSegArc.cpp"
#include "SVGPathSegClosePath.cpp"
#include "SVGPathSegCurvetoCubic.cpp"
#include "SVGPathSegCurvetoCubicSmooth.cpp"
#include "SVGPathSegCurvetoQuadratic.cpp"
#include "SVGPathSegCurvetoQuadraticSmooth.cpp"
#include "SVGPathSegLineto.cpp"
#include "SVGPathSegLinetoHorizontal.cpp"
#include "SVGPathSegLinetoVertical.cpp"
#include "SVGPathSegList.cpp"
#include "SVGPathSegMoveto.cpp"
#include "SVGPatternElement.cpp"
#include "SVGPointList.cpp"
#include "SVGPolyElement.cpp"
#include "SVGPolygonElement.cpp"
#include "SVGPolylineElement.cpp"
#include "SVGPreserveAspectRatio.cpp"
#include "SVGRadialGradientElement.cpp"
#include "SVGRectElement.cpp"
#include "SVGSVGElement.cpp"
#include "SVGScriptElement.cpp"
#include "SVGSetElement.cpp"
#include "SVGStopElement.cpp"
#include "SVGStringList.cpp"
#include "SVGStylable.cpp"
#include "SVGStyleElement.cpp"
#include "SVGStyledElement.cpp"
#include "SVGStyledLocatableElement.cpp"
#include "SVGStyledTransformableElement.cpp"
#include "SVGSwitchElement.cpp"
#include "SVGSymbolElement.cpp"
#include "SVGTRefElement.cpp"
#include "SVGTSpanElement.cpp"
#include "SVGTests.cpp"
#include "SVGTextContentElement.cpp"
#include "SVGTextElement.cpp"
#include "SVGTextPathElement.cpp"
#include "SVGTextPositioningElement.cpp"
#include "SVGTitleElement.cpp"
#include "SVGTransform.cpp"
#include "SVGTransformable.cpp"
#include "SVGTransformDistance.cpp"
#include "SVGTransformList.cpp"
#include "SVGURIReference.cpp"
#include "SVGUseElement.cpp"
#include "SVGViewElement.cpp"
#include "SVGViewSpec.cpp"
#include "SVGZoomAndPan.cpp"
#include "SVGZoomEvent.cpp"
