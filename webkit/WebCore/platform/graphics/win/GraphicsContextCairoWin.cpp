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
 * Copyright (C) 2008 Apple Inc. All rights reserved.
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
#include "GraphicsContext.h"

#include "AffineTransform.h"
#include "Path.h"

#include <cairo-win32.h>
#include "GraphicsContextPlatformPrivateCairo.h"

using namespace std;

namespace WebCore {

static cairo_t* createCairoContextWithHDC(HDC hdc, bool hasAlpha)
{
    // Put the HDC In advanced mode so it will honor affine transforms.
    SetGraphicsMode(hdc, GM_ADVANCED);

    cairo_surface_t* surface = 0;

    HBITMAP bitmap = static_cast<HBITMAP>(GetCurrentObject(hdc, OBJ_BITMAP));

    BITMAP info;
    if (!GetObject(bitmap, sizeof(info), &info))
        surface = cairo_win32_surface_create(hdc);
    else {
        ASSERT(info.bmBitsPixel == 32);

        surface = cairo_image_surface_create_for_data((unsigned char*)info.bmBits,
                                               CAIRO_FORMAT_ARGB32,
                                               info.bmWidth,
                                               info.bmHeight,
                                               info.bmWidthBytes);
    }

    cairo_t* context = cairo_create(surface);
    cairo_surface_destroy(surface);

    return context;
}

GraphicsContext::GraphicsContext(HDC dc, bool hasAlpha)
    : m_common(createGraphicsContextPrivate())
    , m_data(new GraphicsContextPlatformPrivate)
{
    if (dc) {
        m_data->cr = createCairoContextWithHDC(dc, hasAlpha);
        m_data->m_hdc = dc;
    } else {
        setPaintingDisabled(true);
        m_data->cr = 0;
        m_data->m_hdc = 0;
    }

    if (m_data->cr) {
        // Make sure the context starts in sync with our state.
        setPlatformFillColor(fillColor(), fillColorSpace());
        setPlatformStrokeColor(strokeColor(), strokeColorSpace());
    }
}

void GraphicsContext::releaseWindowsContext(HDC hdc, const IntRect& dstRect, bool supportAlphaBlend, bool mayCreateBitmap)
{
    if (!mayCreateBitmap || !hdc || !inTransparencyLayer()) {
        m_data->restore();
        return;
    }

    if (dstRect.isEmpty())
        return;

    HBITMAP bitmap = static_cast<HBITMAP>(GetCurrentObject(hdc, OBJ_BITMAP));

    BITMAP info;
    GetObject(bitmap, sizeof(info), &info);
    ASSERT(info.bmBitsPixel == 32);

    // Need to make a cairo_surface_t out of the bitmap's pixel buffer and then draw
    // it into our context.
    cairo_surface_t* image = cairo_image_surface_create_for_data((unsigned char*)info.bmBits,
                                            CAIRO_FORMAT_ARGB32,
                                            info.bmWidth,
                                            info.bmHeight,
                                            info.bmWidthBytes);

    // Scale the target surface to the new image size, and flip it
    // so that when we set the srcImage as the surface it will draw
    // right-side-up.
    cairo_translate(m_data->cr, 0, dstRect.height());
    cairo_scale(m_data->cr, dstRect.width(), -dstRect.height());
    cairo_set_source_surface (m_data->cr, image, dstRect.x(), dstRect.y());

    if (m_data->layers.size())
        cairo_paint_with_alpha(m_data->cr, m_data->layers.last());
    else
        cairo_paint(m_data->cr);
     
    // Delete all our junk.
    cairo_surface_destroy(image);
    ::DeleteDC(hdc);
    ::DeleteObject(bitmap);
}

void GraphicsContextPlatformPrivate::syncContext(PlatformGraphicsContext* cr)
{
    if (!cr)
       return;

    cairo_surface_t* surface = cairo_get_target(cr);
    m_hdc = cairo_win32_surface_get_dc(surface);   

    SetGraphicsMode(m_hdc, GM_ADVANCED); // We need this call for themes to honor world transforms.
}

void GraphicsContextPlatformPrivate::flush()
{
    cairo_surface_t* surface = cairo_win32_surface_create(m_hdc);
    cairo_surface_flush(surface);
    cairo_surface_destroy(surface);
}

}
