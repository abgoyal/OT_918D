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
 * Copyright (c) 2009, Google Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 *     * Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 * copyright notice, this list of conditions and the following disclaimer
 * in the documentation and/or other materials provided with the
 * distribution.
 *     * Neither the name of Google Inc. nor the names of its
 * contributors may be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

// This source file coalesces the V8 derived sources into a single object file to
// reduce bloat and allow us to link release builds on 32-bit Windows.

// Require explicit conversion to AtomicString. This helps catch cases where
// the generated bindings cause an expensive implicit conversion.
#define NO_IMPLICIT_ATOMICSTRING

#include "bindings/V8Attr.cpp"
#include "bindings/V8BarInfo.cpp"
#include "bindings/V8BeforeLoadEvent.cpp"
#include "bindings/V8Blob.cpp"
#include "bindings/V8WebGLActiveInfo.cpp"
#include "bindings/V8WebGLArray.cpp"
#include "bindings/V8WebGLArrayBuffer.cpp"
#include "bindings/V8WebGLBuffer.cpp"
#include "bindings/V8WebGLByteArray.cpp"
#include "bindings/V8WebGLContextAttributes.cpp"
#include "bindings/V8WebGLFloatArray.cpp"
#include "bindings/V8WebGLFramebuffer.cpp"
#include "bindings/V8CanvasGradient.cpp"
#include "bindings/V8WebGLIntArray.cpp"
#include "bindings/V8CanvasPattern.cpp"
#include "bindings/V8CanvasPixelArray.cpp"
#include "bindings/V8WebGLProgram.cpp"
#include "bindings/V8WebGLRenderbuffer.cpp"
#include "bindings/V8CanvasRenderingContext.cpp"
#include "bindings/V8CanvasRenderingContext2D.cpp"
#include "bindings/V8WebGLRenderingContext.cpp"
#include "bindings/V8WebGLShader.cpp"
#include "bindings/V8WebGLShortArray.cpp"
#include "bindings/V8WebGLTexture.cpp"
#include "bindings/V8WebGLUniformLocation.cpp"
#include "bindings/V8WebGLUnsignedByteArray.cpp"
#include "bindings/V8WebGLUnsignedIntArray.cpp"
#include "bindings/V8WebGLUnsignedShortArray.cpp"
#include "bindings/V8CDATASection.cpp"
#include "bindings/V8CharacterData.cpp"
#include "bindings/V8ClientRect.cpp"
#include "bindings/V8ClientRectList.cpp"
#include "bindings/V8Clipboard.cpp"
#include "bindings/V8Comment.cpp"
#include "bindings/V8CompositionEvent.cpp"
// ANDROID
// Temporary modification - will not be upstreamed
#include "bindings/V8Connection.cpp"
#include "bindings/V8Console.cpp"
#include "bindings/V8Coordinates.cpp"
#include "bindings/V8Counter.cpp"
#include "bindings/V8CSSCharsetRule.cpp"
#include "bindings/V8CSSFontFaceRule.cpp"
#include "bindings/V8CSSImportRule.cpp"
#include "bindings/V8CSSMediaRule.cpp"
#include "bindings/V8CSSPageRule.cpp"
#include "bindings/V8CSSPrimitiveValue.cpp"
#include "bindings/V8CSSRule.cpp"
#include "bindings/V8CSSRuleList.cpp"
#include "bindings/V8CSSStyleDeclaration.cpp"
#include "bindings/V8CSSStyleRule.cpp"
#include "bindings/V8CSSStyleSheet.cpp"
#include "bindings/V8CSSValue.cpp"
#include "bindings/V8CSSValueList.cpp"
#include "bindings/V8CSSVariablesDeclaration.cpp"
#include "bindings/V8CSSVariablesRule.cpp"
#include "bindings/V8DataGridColumn.cpp"
#include "bindings/V8DataGridColumnList.cpp"
#include "bindings/V8Document.cpp"
#include "bindings/V8DocumentFragment.cpp"
#include "bindings/V8DocumentType.cpp"
#include "bindings/V8DOMCoreException.cpp"
#include "bindings/V8DOMImplementation.cpp"
#include "bindings/V8DOMParser.cpp"
#include "bindings/V8DOMSelection.cpp"
#include "bindings/V8DOMWindow.cpp"
#include "bindings/V8Element.cpp"
#include "bindings/V8Entity.cpp"
#include "bindings/V8EntityReference.cpp"
#include "bindings/V8ErrorEvent.cpp"
#include "bindings/V8Event.cpp"
#include "bindings/V8EventException.cpp"
#include "bindings/V8File.cpp"
#include "bindings/V8FileList.cpp"
#include "bindings/V8Geolocation.cpp"
#include "bindings/V8Geoposition.cpp"
#include "bindings/V8History.cpp"
#include "bindings/V8HTMLAllCollection.cpp"
#include "bindings/V8HTMLAnchorElement.cpp"
#include "bindings/V8HTMLAppletElement.cpp"
#include "bindings/V8HTMLAreaElement.cpp"
#include "bindings/V8HTMLAudioElement.cpp"
#include "bindings/V8HTMLBaseElement.cpp"
#include "bindings/V8HTMLBaseFontElement.cpp"
#include "bindings/V8HTMLBlockquoteElement.cpp"
#include "bindings/V8HTMLBodyElement.cpp"
#include "bindings/V8HTMLBRElement.cpp"
#include "bindings/V8HTMLButtonElement.cpp"
#include "bindings/V8HTMLCanvasElement.cpp"
#include "bindings/V8HTMLCollection.cpp"
#include "bindings/V8HTMLDataGridCellElement.cpp"
#include "bindings/V8HTMLDataGridColElement.cpp"
#include "bindings/V8HTMLDataGridElement.cpp"
#include "bindings/V8HTMLDataGridRowElement.cpp"
#include "bindings/V8HTMLDirectoryElement.cpp"
#include "bindings/V8HTMLDivElement.cpp"
#include "bindings/V8HTMLDListElement.cpp"
#include "bindings/V8HTMLDocument.cpp"
#include "bindings/V8HTMLElement.cpp"
#include "bindings/V8HTMLEmbedElement.cpp"
#include "bindings/V8HTMLFieldSetElement.cpp"
#include "bindings/V8HTMLFontElement.cpp"
#include "bindings/V8HTMLFormElement.cpp"
#include "bindings/V8HTMLFrameElement.cpp"
#include "bindings/V8HTMLFrameSetElement.cpp"
#include "bindings/V8HTMLHeadElement.cpp"
#include "bindings/V8HTMLHeadingElement.cpp"
#include "bindings/V8HTMLHRElement.cpp"
#include "bindings/V8HTMLHtmlElement.cpp"
#include "bindings/V8HTMLIFrameElement.cpp"
#include "bindings/V8HTMLImageElement.cpp"
#include "bindings/V8HTMLInputElement.cpp"
#include "bindings/V8HTMLIsIndexElement.cpp"
#include "bindings/V8HTMLLabelElement.cpp"
#include "bindings/V8HTMLLegendElement.cpp"
#include "bindings/V8HTMLLIElement.cpp"
#include "bindings/V8HTMLLinkElement.cpp"
#include "bindings/V8HTMLMapElement.cpp"
#include "bindings/V8HTMLMarqueeElement.cpp"
#include "bindings/V8HTMLMediaElement.cpp"
#include "bindings/V8HTMLMenuElement.cpp"
#include "bindings/V8HTMLMetaElement.cpp"
#include "bindings/V8HTMLModElement.cpp"
#include "bindings/V8HTMLObjectElement.cpp"
#include "bindings/V8HTMLOListElement.cpp"
#include "bindings/V8HTMLOptGroupElement.cpp"
#include "bindings/V8HTMLOptionElement.cpp"
#include "bindings/V8HTMLOptionsCollection.cpp"
#include "bindings/V8HTMLParagraphElement.cpp"
#include "bindings/V8HTMLParamElement.cpp"
#include "bindings/V8HTMLPreElement.cpp"
#include "bindings/V8HTMLQuoteElement.cpp"
#include "bindings/V8HTMLScriptElement.cpp"
#include "bindings/V8HTMLSelectElement.cpp"
#include "bindings/V8HTMLSourceElement.cpp"
#include "bindings/V8HTMLStyleElement.cpp"
#include "bindings/V8HTMLTableCaptionElement.cpp"
#include "bindings/V8HTMLTableCellElement.cpp"
#include "bindings/V8HTMLTableColElement.cpp"
#include "bindings/V8HTMLTableElement.cpp"
#include "bindings/V8HTMLTableRowElement.cpp"
#include "bindings/V8HTMLTableSectionElement.cpp"
#include "bindings/V8HTMLTextAreaElement.cpp"
#include "bindings/V8HTMLTitleElement.cpp"
#include "bindings/V8HTMLUListElement.cpp"
#include "bindings/V8HTMLVideoElement.cpp"
#include "bindings/V8ImageData.cpp"
#include "bindings/V8KeyboardEvent.cpp"
#include "bindings/V8Location.cpp"
#include "bindings/V8Media.cpp"
#include "bindings/V8MediaError.cpp"
#include "bindings/V8MediaList.cpp"
#include "bindings/V8MessageChannel.cpp"
#include "bindings/V8MessageEvent.cpp"
#include "bindings/V8MessagePort.cpp"
#include "bindings/V8MimeType.cpp"
#include "bindings/V8MimeTypeArray.cpp"
#include "bindings/V8MouseEvent.cpp"
#include "bindings/V8MutationEvent.cpp"
#include "bindings/V8NamedNodeMap.cpp"
#include "bindings/V8Navigator.cpp"
#include "bindings/V8Node.cpp"
#include "bindings/V8NodeFilter.cpp"
#include "bindings/V8NodeIterator.cpp"
#include "bindings/V8NodeList.cpp"
#include "bindings/V8Notation.cpp"
#include "bindings/V8Notification.cpp"
#include "bindings/V8NotificationCenter.cpp"
#include "bindings/V8OverflowEvent.cpp"
#include "bindings/V8PageTransitionEvent.cpp"
#include "bindings/V8Plugin.cpp"
#include "bindings/V8PluginArray.cpp"
#include "bindings/V8PopStateEvent.cpp"
#include "bindings/V8PositionError.cpp"
#include "bindings/V8ProcessingInstruction.cpp"
#include "bindings/V8ProgressEvent.cpp"
#include "bindings/V8Range.cpp"
#include "bindings/V8RangeException.cpp"
#include "bindings/V8Rect.cpp"
#include "bindings/V8RGBColor.cpp"
#include "bindings/V8Screen.cpp"
#include "bindings/V8StyleSheet.cpp"
#include "bindings/V8StyleSheetList.cpp"
#include "bindings/V8Text.cpp"
#include "bindings/V8TextEvent.cpp"
#include "bindings/V8TextMetrics.cpp"
#include "bindings/V8TimeRanges.cpp"
#include "bindings/V8Touch.cpp"
#include "bindings/V8TouchEvent.cpp"
#include "bindings/V8TouchList.cpp"
#include "bindings/V8TreeWalker.cpp"
#include "bindings/V8UIEvent.cpp"
#include "bindings/V8ValidityState.cpp"
#include "bindings/V8WebKitAnimationEvent.cpp"
#include "bindings/V8WebKitCSSKeyframeRule.cpp"
#include "bindings/V8WebKitCSSKeyframesRule.cpp"
#include "bindings/V8WebKitCSSMatrix.cpp"
#include "bindings/V8WebKitCSSTransformValue.cpp"
#include "bindings/V8WebKitPoint.cpp"
#include "bindings/V8WebKitTransitionEvent.cpp"
#include "bindings/V8WheelEvent.cpp"
#include "bindings/V8XMLHttpRequest.cpp"
#include "bindings/V8XMLHttpRequestException.cpp"
#include "bindings/V8XMLHttpRequestProgressEvent.cpp"
#include "bindings/V8XMLHttpRequestUpload.cpp"
#include "bindings/V8XMLSerializer.cpp"

#if ENABLE(OFFLINE_WEB_APPLICATIONS)
#include "bindings/V8DOMApplicationCache.cpp"
#endif

#if ENABLE(DOM_STORAGE)
#include "bindings/V8Storage.cpp"
#include "bindings/V8StorageEvent.cpp"
#endif

#if ENABLE(WEB_SOCKETS)
#include "bindings/V8WebSocket.cpp"
#endif

#if ENABLE(DATABASE)
#include "bindings/V8Database.cpp"
#include "bindings/V8SQLError.cpp"
#include "bindings/V8SQLResultSet.cpp"
#include "bindings/V8SQLResultSetRowList.cpp"
#include "bindings/V8SQLTransaction.cpp"
#endif

#if ENABLE(INDEXED_DATABASE)
#include "bindings/V8IDBDatabaseError.cpp"
#include "bindings/V8IDBDatabaseException.cpp"
#include "bindings/V8IDBRequest.cpp"
#include "bindings/V8IndexedDatabaseRequest.cpp"
#endif

#if ENABLE(WORKERS)
#include "bindings/V8AbstractWorker.cpp"
#include "bindings/V8DedicatedWorkerContext.cpp"
#include "bindings/V8Worker.cpp"
#include "bindings/V8WorkerContext.cpp"
#include "bindings/V8WorkerLocation.cpp"
#include "bindings/V8WorkerNavigator.cpp"
#endif

#if ENABLE(SHARED_WORKERS)
#include "bindings/V8SharedWorker.cpp"
#include "bindings/V8SharedWorkerContext.cpp"
#endif

#if ENABLE(SVG)
#include "bindings/V8SVGAElement.cpp"
#include "bindings/V8SVGAltGlyphElement.cpp"
#include "bindings/V8SVGAngle.cpp"
#include "bindings/V8SVGAnimateColorElement.cpp"
#include "bindings/V8SVGAnimatedAngle.cpp"
#include "bindings/V8SVGAnimatedBoolean.cpp"
#include "bindings/V8SVGAnimatedEnumeration.cpp"
#include "bindings/V8SVGAnimatedInteger.cpp"
#include "bindings/V8SVGAnimatedLength.cpp"
#include "bindings/V8SVGAnimatedLengthList.cpp"
#include "bindings/V8SVGAnimatedNumber.cpp"
#include "bindings/V8SVGAnimatedNumberList.cpp"
#include "bindings/V8SVGAnimatedPoints.cpp"
#include "bindings/V8SVGAnimatedPreserveAspectRatio.cpp"
#include "bindings/V8SVGAnimatedRect.cpp"
#include "bindings/V8SVGAnimatedString.cpp"
#include "bindings/V8SVGAnimatedTransformList.cpp"
#include "bindings/V8SVGAnimateElement.cpp"
#include "bindings/V8SVGAnimateTransformElement.cpp"
#include "bindings/V8SVGAnimationElement.cpp"
#include "bindings/V8SVGCircleElement.cpp"
#include "bindings/V8SVGClipPathElement.cpp"
#include "bindings/V8SVGColor.cpp"
#include "bindings/V8SVGComponentTransferFunctionElement.cpp"
#include "bindings/V8SVGCursorElement.cpp"
#include "bindings/V8SVGDefsElement.cpp"
#include "bindings/V8SVGDescElement.cpp"
#include "bindings/V8SVGDocument.cpp"
#include "bindings/V8SVGElement.cpp"
#include "bindings/V8SVGElementInstance.cpp"
#include "bindings/V8SVGElementInstanceList.cpp"
#include "bindings/V8SVGEllipseElement.cpp"
#include "bindings/V8SVGException.cpp"
#include "bindings/V8SVGFEBlendElement.cpp"
#include "bindings/V8SVGFEColorMatrixElement.cpp"
#include "bindings/V8SVGFEComponentTransferElement.cpp"
#include "bindings/V8SVGFECompositeElement.cpp"
#include "bindings/V8SVGFEDiffuseLightingElement.cpp"
#include "bindings/V8SVGFEDisplacementMapElement.cpp"
#include "bindings/V8SVGFEDistantLightElement.cpp"
#include "bindings/V8SVGFEFloodElement.cpp"
#include "bindings/V8SVGFEFuncAElement.cpp"
#include "bindings/V8SVGFEFuncBElement.cpp"
#include "bindings/V8SVGFEFuncGElement.cpp"
#include "bindings/V8SVGFEFuncRElement.cpp"
#include "bindings/V8SVGFEGaussianBlurElement.cpp"
#include "bindings/V8SVGFEImageElement.cpp"
#include "bindings/V8SVGFEMergeElement.cpp"
#include "bindings/V8SVGFEMergeNodeElement.cpp"
#include "bindings/V8SVGFEMorphologyElement.cpp"
#include "bindings/V8SVGFEOffsetElement.cpp"
#include "bindings/V8SVGFEPointLightElement.cpp"
#include "bindings/V8SVGFESpecularLightingElement.cpp"
#include "bindings/V8SVGFESpotLightElement.cpp"
#include "bindings/V8SVGFETileElement.cpp"
#include "bindings/V8SVGFETurbulenceElement.cpp"
#include "bindings/V8SVGFilterElement.cpp"
#include "bindings/V8SVGFontElement.cpp"
#include "bindings/V8SVGFontFaceElement.cpp"
#include "bindings/V8SVGFontFaceFormatElement.cpp"
#include "bindings/V8SVGFontFaceNameElement.cpp"
#include "bindings/V8SVGFontFaceSrcElement.cpp"
#include "bindings/V8SVGFontFaceUriElement.cpp"
#include "bindings/V8SVGForeignObjectElement.cpp"
#include "bindings/V8SVGGElement.cpp"
#include "bindings/V8SVGGlyphElement.cpp"
#include "bindings/V8SVGGradientElement.cpp"
#include "bindings/V8SVGImageElement.cpp"
#include "bindings/V8SVGLength.cpp"
#include "bindings/V8SVGLengthList.cpp"
#include "bindings/V8SVGLinearGradientElement.cpp"
#include "bindings/V8SVGLineElement.cpp"
#include "bindings/V8SVGMarkerElement.cpp"
#include "bindings/V8SVGMaskElement.cpp"
#include "bindings/V8SVGMatrix.cpp"
#include "bindings/V8SVGMetadataElement.cpp"
#include "bindings/V8SVGMissingGlyphElement.cpp"
#include "bindings/V8SVGNumber.cpp"
#include "bindings/V8SVGNumberList.cpp"
#include "bindings/V8SVGPaint.cpp"
#include "bindings/V8SVGPathElement.cpp"
#include "bindings/V8SVGPathSeg.cpp"
#include "bindings/V8SVGPathSegArcAbs.cpp"
#include "bindings/V8SVGPathSegArcRel.cpp"
#include "bindings/V8SVGPathSegClosePath.cpp"
#include "bindings/V8SVGPathSegCurvetoCubicAbs.cpp"
#include "bindings/V8SVGPathSegCurvetoCubicRel.cpp"
#include "bindings/V8SVGPathSegCurvetoCubicSmoothAbs.cpp"
#include "bindings/V8SVGPathSegCurvetoCubicSmoothRel.cpp"
#include "bindings/V8SVGPathSegCurvetoQuadraticAbs.cpp"
#include "bindings/V8SVGPathSegCurvetoQuadraticRel.cpp"
#include "bindings/V8SVGPathSegCurvetoQuadraticSmoothAbs.cpp"
#include "bindings/V8SVGPathSegCurvetoQuadraticSmoothRel.cpp"
#include "bindings/V8SVGPathSegLinetoAbs.cpp"
#include "bindings/V8SVGPathSegLinetoHorizontalAbs.cpp"
#include "bindings/V8SVGPathSegLinetoHorizontalRel.cpp"
#include "bindings/V8SVGPathSegLinetoRel.cpp"
#include "bindings/V8SVGPathSegLinetoVerticalAbs.cpp"
#include "bindings/V8SVGPathSegLinetoVerticalRel.cpp"
#include "bindings/V8SVGPathSegList.cpp"
#include "bindings/V8SVGPathSegMovetoAbs.cpp"
#include "bindings/V8SVGPathSegMovetoRel.cpp"
#include "bindings/V8SVGPatternElement.cpp"
#include "bindings/V8SVGPoint.cpp"
#include "bindings/V8SVGPointList.cpp"
#include "bindings/V8SVGPolygonElement.cpp"
#include "bindings/V8SVGPolylineElement.cpp"
#include "bindings/V8SVGPreserveAspectRatio.cpp"
#include "bindings/V8SVGRadialGradientElement.cpp"
#include "bindings/V8SVGRect.cpp"
#include "bindings/V8SVGRectElement.cpp"
#include "bindings/V8SVGRenderingIntent.cpp"
#include "bindings/V8SVGScriptElement.cpp"
#include "bindings/V8SVGSetElement.cpp"
#include "bindings/V8SVGStopElement.cpp"
#include "bindings/V8SVGStringList.cpp"
#include "bindings/V8SVGStyleElement.cpp"
#include "bindings/V8SVGSVGElement.cpp"
#include "bindings/V8SVGSwitchElement.cpp"
#include "bindings/V8SVGSymbolElement.cpp"
#include "bindings/V8SVGTextContentElement.cpp"
#include "bindings/V8SVGTextElement.cpp"
#include "bindings/V8SVGTextPathElement.cpp"
#include "bindings/V8SVGTextPositioningElement.cpp"
#include "bindings/V8SVGTitleElement.cpp"
#include "bindings/V8SVGTransform.cpp"
#include "bindings/V8SVGTransformList.cpp"
#include "bindings/V8SVGTRefElement.cpp"
#include "bindings/V8SVGTSpanElement.cpp"
#include "bindings/V8SVGUnitTypes.cpp"
#include "bindings/V8SVGUseElement.cpp"
#include "bindings/V8SVGViewElement.cpp"
#include "bindings/V8SVGZoomEvent.cpp"
#endif

#if ENABLE(XPATH)
#include "bindings/V8XPathEvaluator.cpp"
#include "bindings/V8XPathException.cpp"
#include "bindings/V8XPathExpression.cpp"
#include "bindings/V8XPathNSResolver.cpp"
#include "bindings/V8XPathResult.cpp"
#endif

#if ENABLE(XSLT)
#include "bindings/V8XSLTProcessor.cpp"
#endif

#if ENABLE(INSPECTOR)
#include "bindings/V8InjectedScriptHost.cpp"
#include "bindings/V8InspectorBackend.cpp"
#include "bindings/V8InspectorFrontendHost.cpp"
#endif

#if ENABLE(EVENTSOURCE)
#include "bindings/V8EventSource.cpp"
#endif
