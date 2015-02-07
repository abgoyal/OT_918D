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

// This all-in-one cpp file cuts down on template bloat to allow us to build our Windows release build.

#include "JSAbstractWorker.cpp"
#include "JSAttr.cpp"
#include "JSBarInfo.cpp"
#include "JSBeforeLoadEvent.cpp"
#include "JSBlob.cpp"
#include "JSCanvasGradient.cpp"
#include "JSCanvasPattern.cpp"
#include "JSCanvasRenderingContext.cpp"
#include "JSCanvasRenderingContext2D.cpp"
#if ENABLE(3D_CANVAS)    
#include "JSWebGLRenderingContext.cpp"
#endif
#include "JSCDATASection.cpp"
#include "JSCharacterData.cpp"
#include "JSClientRect.cpp"
#include "JSClientRectList.cpp"
#include "JSClipboard.cpp"
#include "JSComment.cpp"
#include "JSCompositionEvent.cpp"
#include "JSConsole.cpp"
#include "JSCoordinates.cpp"
#include "JSCounter.cpp"
#include "JSCSSCharsetRule.cpp"
#include "JSCSSFontFaceRule.cpp"
#include "JSCSSImportRule.cpp"
#include "JSCSSMediaRule.cpp"
#include "JSCSSPageRule.cpp"
#include "JSCSSPrimitiveValue.cpp"
#include "JSCSSRule.cpp"
#include "JSCSSRuleList.cpp"
#include "JSCSSStyleDeclaration.cpp"
#include "JSCSSStyleRule.cpp"
#include "JSCSSStyleSheet.cpp"
#include "JSCSSValue.cpp"
#include "JSCSSValueList.cpp"
#include "JSCSSVariablesDeclaration.cpp"
#include "JSCSSVariablesRule.cpp"
#include "JSDatabase.cpp"
#include "JSDataGridColumn.cpp"
#include "JSDataGridColumnList.cpp"
#include "JSDedicatedWorkerContext.cpp"
#include "JSDocument.cpp"
#include "JSDocumentFragment.cpp"
#include "JSDocumentType.cpp"
#include "JSDOMApplicationCache.cpp"
#include "JSDOMCoreException.cpp"
#include "JSDOMImplementation.cpp"
#include "JSDOMParser.cpp"
#include "JSDOMSelection.cpp"
#include "JSDOMWindow.cpp"
#include "JSElement.cpp"
#include "JSEntity.cpp"
#include "JSEntityReference.cpp"
#include "JSErrorEvent.cpp"
#include "JSEvent.cpp"
#include "JSEventException.cpp"
#include "JSEventSource.cpp"
#include "JSFile.cpp"
#include "JSFileList.cpp"
#include "JSGeolocation.cpp"
#include "JSGeoposition.cpp"
#include "JSHistory.cpp"
#include "JSHTMLAllCollection.cpp"
#include "JSHTMLAnchorElement.cpp"
#include "JSHTMLAppletElement.cpp"
#include "JSHTMLAreaElement.cpp"
#include "JSHTMLAudioElement.cpp"
#include "JSHTMLBaseElement.cpp"
#include "JSHTMLBaseFontElement.cpp"
#include "JSHTMLBlockquoteElement.cpp"
#include "JSHTMLBodyElement.cpp"
#include "JSHTMLBRElement.cpp"
#include "JSHTMLButtonElement.cpp"
#include "JSHTMLCanvasElement.cpp"
#include "JSHTMLCollection.cpp"
#include "JSHTMLDataGridElement.cpp"
#include "JSHTMLDataGridCellElement.cpp"
#include "JSHTMLDataGridColElement.cpp"
#include "JSHTMLDataGridRowElement.cpp"
#include "JSHTMLDataListElement.cpp"
#include "JSHTMLDirectoryElement.cpp"
#include "JSHTMLDivElement.cpp"
#include "JSHTMLDListElement.cpp"
#include "JSHTMLDocument.cpp"
#include "JSHTMLElement.cpp"
#include "JSHTMLElementWrapperFactory.cpp"
#include "JSHTMLEmbedElement.cpp"
#include "JSHTMLFieldSetElement.cpp"
#include "JSHTMLFontElement.cpp"
#include "JSHTMLFormElement.cpp"
#include "JSHTMLFrameElement.cpp"
#include "JSHTMLFrameSetElement.cpp"
#include "JSHTMLHeadElement.cpp"
#include "JSHTMLHeadingElement.cpp"
#include "JSHTMLHRElement.cpp"
#include "JSHTMLHtmlElement.cpp"
#include "JSHTMLIFrameElement.cpp"
#include "JSHTMLImageElement.cpp"
#include "JSHTMLInputElement.cpp"
#include "JSHTMLIsIndexElement.cpp"
#include "JSHTMLLabelElement.cpp"
#include "JSHTMLLegendElement.cpp"
#include "JSHTMLLIElement.cpp"
#include "JSHTMLLinkElement.cpp"
#include "JSHTMLMapElement.cpp"
#include "JSHTMLMarqueeElement.cpp"
#include "JSHTMLMediaElement.cpp"
#include "JSHTMLMenuElement.cpp"
#include "JSHTMLMetaElement.cpp"
#include "JSHTMLModElement.cpp"
#include "JSHTMLObjectElement.cpp"
#include "JSHTMLOListElement.cpp"
#include "JSHTMLOptGroupElement.cpp"
#include "JSHTMLOptionElement.cpp"
#include "JSHTMLOptionsCollection.cpp"
#include "JSHTMLParagraphElement.cpp"
#include "JSHTMLParamElement.cpp"
#include "JSHTMLPreElement.cpp"
#include "JSHTMLQuoteElement.cpp"
#include "JSHTMLScriptElement.cpp"
#include "JSHTMLSelectElement.cpp"
#include "JSHTMLSourceElement.cpp"
#include "JSHTMLStyleElement.cpp"
#include "JSHTMLTableCaptionElement.cpp"
#include "JSHTMLTableCellElement.cpp"
#include "JSHTMLTableColElement.cpp"
#include "JSHTMLTableElement.cpp"
#include "JSHTMLTableRowElement.cpp"
#include "JSHTMLTableSectionElement.cpp"
#include "JSHTMLTextAreaElement.cpp"
#include "JSHTMLTitleElement.cpp"
#include "JSHTMLUListElement.cpp"
#include "JSHTMLVideoElement.cpp"
#include "JSImageData.cpp"
#include "JSJavaScriptCallFrame.cpp"
#include "JSKeyboardEvent.cpp"
#include "JSLocation.cpp"
#include "JSMedia.cpp"
#include "JSMediaError.cpp"
#include "JSMediaList.cpp"
#include "JSMessageChannel.cpp"
#include "JSMessageEvent.cpp"
#include "JSMessagePort.cpp"
#include "JSMimeType.cpp"
#include "JSMimeTypeArray.cpp"
#include "JSMouseEvent.cpp"
#include "JSMutationEvent.cpp"
#include "JSNamedNodeMap.cpp"
#include "JSNavigator.cpp"
#include "JSNode.cpp"
#include "JSNodeFilter.cpp"
#include "JSNodeIterator.cpp"
#include "JSNodeList.cpp"
#include "JSNotation.cpp"
#include "JSNotificationCenter.cpp"
#include "JSNotification.cpp"
#include "JSOverflowEvent.cpp"
#include "JSPageTransitionEvent.cpp"
#include "JSPlugin.cpp"
#include "JSPluginArray.cpp"
#include "JSPopStateEvent.cpp"
#include "JSPositionError.cpp"
#include "JSProcessingInstruction.cpp"
#include "JSProgressEvent.cpp"
#include "JSRange.cpp"
#include "JSRangeException.cpp"
#include "JSRect.cpp"
#include "JSRGBColor.cpp"
#include "JSScreen.cpp"
#include "JSSharedWorker.cpp"
#include "JSSharedWorkerContext.cpp"
#include "JSSQLError.cpp"
#include "JSSQLResultSet.cpp"
#include "JSSQLResultSetRowList.cpp"
#include "JSSQLTransaction.cpp"
#include "JSStorage.cpp"
#include "JSStorageEvent.cpp"
#include "JSStyleSheet.cpp"
#include "JSStyleSheetList.cpp"
#include "JSSVGAElement.cpp"
#include "JSSVGAltGlyphElement.cpp"
#include "JSSVGAngle.cpp"
#include "JSSVGAnimateColorElement.cpp"
#include "JSSVGAnimatedAngle.cpp"
#include "JSSVGAnimatedBoolean.cpp"
#include "JSSVGAnimatedEnumeration.cpp"
#include "JSSVGAnimatedInteger.cpp"
#include "JSSVGAnimatedLength.cpp"
#include "JSSVGAnimatedLengthList.cpp"
#include "JSSVGAnimatedNumber.cpp"
#include "JSSVGAnimatedNumberList.cpp"
#include "JSSVGAnimatedPreserveAspectRatio.cpp"
#include "JSSVGAnimatedRect.cpp"
#include "JSSVGAnimatedString.cpp"
#include "JSSVGAnimatedTransformList.cpp"
#include "JSSVGAnimateElement.cpp"
#include "JSSVGAnimateTransformElement.cpp"
#include "JSSVGAnimationElement.cpp"
#include "JSSVGCircleElement.cpp"
#include "JSSVGClipPathElement.cpp"
#include "JSSVGColor.cpp"
#include "JSSVGComponentTransferFunctionElement.cpp"
#include "JSSVGCursorElement.cpp"
#include "JSSVGDefsElement.cpp"
#include "JSSVGDescElement.cpp"
#include "JSSVGDocument.cpp"
#include "JSSVGElement.cpp"
#include "JSSVGElementInstance.cpp"
#include "JSSVGElementInstanceList.cpp"
#include "JSSVGEllipseElement.cpp"
#include "JSSVGException.cpp"
#include "JSSVGFEBlendElement.cpp"
#include "JSSVGFEColorMatrixElement.cpp"
#include "JSSVGFEComponentTransferElement.cpp"
#include "JSSVGFECompositeElement.cpp"
#include "JSSVGFEDiffuseLightingElement.cpp"
#include "JSSVGFEDisplacementMapElement.cpp"
#include "JSSVGFEDistantLightElement.cpp"
#include "JSSVGFEFloodElement.cpp"
#include "JSSVGFEFuncAElement.cpp"
#include "JSSVGFEFuncBElement.cpp"
#include "JSSVGFEFuncGElement.cpp"
#include "JSSVGFEFuncRElement.cpp"
#include "JSSVGFEGaussianBlurElement.cpp"
#include "JSSVGFEImageElement.cpp"
#include "JSSVGFEMergeElement.cpp"
#include "JSSVGFEMergeNodeElement.cpp"
#include "JSSVGFEMorphologyElement.cpp"
#include "JSSVGFEOffsetElement.cpp"
#include "JSSVGFEPointLightElement.cpp"
#include "JSSVGFESpecularLightingElement.cpp"
#include "JSSVGFESpotLightElement.cpp"
#include "JSSVGFETileElement.cpp"
#include "JSSVGFETurbulenceElement.cpp"
#include "JSSVGFilterElement.cpp"
#include "JSSVGFontElement.cpp"
#include "JSSVGFontFaceElement.cpp"
#include "JSSVGFontFaceFormatElement.cpp"
#include "JSSVGFontFaceNameElement.cpp"
#include "JSSVGFontFaceSrcElement.cpp"
#include "JSSVGFontFaceUriElement.cpp"
#include "JSSVGForeignObjectElement.cpp"
#include "JSSVGGElement.cpp"
#include "JSSVGGlyphElement.cpp"
#include "JSSVGGradientElement.cpp"
#include "JSSVGImageElement.cpp"
#include "JSSVGLength.cpp"
#include "JSSVGLengthList.cpp"
#include "JSSVGLinearGradientElement.cpp"
#include "JSSVGLineElement.cpp"
#include "JSSVGMarkerElement.cpp"
#include "JSSVGMaskElement.cpp"
#include "JSSVGMatrix.cpp"
#include "JSSVGMetadataElement.cpp"
#include "JSSVGMissingGlyphElement.cpp"
#include "JSSVGNumber.cpp"
#include "JSSVGNumberList.cpp"
#include "JSSVGPaint.cpp"
#include "JSSVGPathElement.cpp"
#include "JSSVGPathSeg.cpp"
#include "JSSVGPathSegArcAbs.cpp"
#include "JSSVGPathSegArcRel.cpp"
#include "JSSVGPathSegClosePath.cpp"
#include "JSSVGPathSegCurvetoCubicAbs.cpp"
#include "JSSVGPathSegCurvetoCubicRel.cpp"
#include "JSSVGPathSegCurvetoCubicSmoothAbs.cpp"
#include "JSSVGPathSegCurvetoCubicSmoothRel.cpp"
#include "JSSVGPathSegCurvetoQuadraticAbs.cpp"
#include "JSSVGPathSegCurvetoQuadraticRel.cpp"
#include "JSSVGPathSegCurvetoQuadraticSmoothAbs.cpp"
#include "JSSVGPathSegCurvetoQuadraticSmoothRel.cpp"
#include "JSSVGPathSegLinetoAbs.cpp"
#include "JSSVGPathSegLinetoHorizontalAbs.cpp"
#include "JSSVGPathSegLinetoHorizontalRel.cpp"
#include "JSSVGPathSegLinetoRel.cpp"
#include "JSSVGPathSegLinetoVerticalAbs.cpp"
#include "JSSVGPathSegLinetoVerticalRel.cpp"
#include "JSSVGPathSegList.cpp"
#include "JSSVGPathSegMovetoAbs.cpp"
#include "JSSVGPathSegMovetoRel.cpp"
#include "JSSVGPatternElement.cpp"
#include "JSSVGPoint.cpp"
#include "JSSVGPointList.cpp"
#include "JSSVGPolygonElement.cpp"
#include "JSSVGPolylineElement.cpp"
#include "JSSVGPreserveAspectRatio.cpp"
#include "JSSVGRadialGradientElement.cpp"
#include "JSSVGRect.cpp"
#include "JSSVGRectElement.cpp"
#include "JSSVGRenderingIntent.cpp"
#include "JSSVGScriptElement.cpp"
#include "JSSVGSetElement.cpp"
#include "JSSVGStopElement.cpp"
#include "JSSVGStringList.cpp"
#include "JSSVGStyleElement.cpp"
#include "JSSVGSVGElement.cpp"
#include "JSSVGSwitchElement.cpp"
#include "JSSVGSymbolElement.cpp"
#include "JSSVGTextContentElement.cpp"
#include "JSSVGTextElement.cpp"
#include "JSSVGTextPathElement.cpp"
#include "JSSVGTextPositioningElement.cpp"
#include "JSSVGTitleElement.cpp"
#include "JSSVGTransform.cpp"
#include "JSSVGTransformList.cpp"
#include "JSSVGTRefElement.cpp"
#include "JSSVGTSpanElement.cpp"
#include "JSSVGUnitTypes.cpp"
#include "JSSVGUseElement.cpp"
#include "JSSVGViewElement.cpp"
#include "JSSVGZoomEvent.cpp"
#include "JSText.cpp"
#include "JSTextEvent.cpp"
#include "JSTextMetrics.cpp"
#include "JSTimeRanges.cpp"
#include "JSTreeWalker.cpp"
#include "JSUIEvent.cpp"
#include "JSValidityState.cpp"
#include "JSWebKitAnimationEvent.cpp"
#include "JSWebKitCSSKeyframeRule.cpp"
#include "JSWebKitCSSKeyframesRule.cpp"
#include "JSWebKitCSSTransformValue.cpp"
#include "JSWebKitCSSMatrix.cpp"
#include "JSWebKitPoint.cpp"
#include "JSWebKitTransitionEvent.cpp"
#include "JSWebSocket.cpp"
#include "JSWheelEvent.cpp"
#include "JSWorker.cpp"
#include "JSWorkerContext.cpp"
#include "JSWorkerLocation.cpp"
#include "JSWorkerNavigator.cpp"
#include "JSXMLHttpRequest.cpp"
#include "JSXMLHttpRequestException.cpp"
#include "JSXMLHttpRequestProgressEvent.cpp"
#include "JSXMLHttpRequestUpload.cpp"
#include "JSXMLSerializer.cpp"
#include "JSXPathEvaluator.cpp"
#include "JSXPathException.cpp"
#include "JSXPathExpression.cpp"
#include "JSXPathNSResolver.cpp"
#include "JSXPathResult.cpp"
#include "JSXSLTProcessor.cpp"
#include "UserAgentStyleSheetsData.cpp"

// On MSVC, including StaticConstructors.h causes all global objects not to be
// automatically initialized by the C runtime. This is useful in some specific
// cases (e.g., the *Names.cpp files), but can be dangerous in others. We don't
// want StaticConstructors.h to "pollute" all the source files we #include here
// accidentally, so we'll throw an error whenever any file includes it.
#ifdef StaticConstructors_h
#error Do not include any file in DerivedSources.cpp that includes StaticConstructors.h
#endif
