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
    Copyright (C) 1998 Lars Knoll (knoll@mpi-hd.mpg.de)
    Copyright (C) 2001 Dirk Mueller (mueller@kde.org)
    Copyright (C) 2002 Waldo Bastian (bastian@kde.org)
    Copyright (C) 2006 Samuel Weinig (sam.weinig@gmail.com)
    Copyright (C) 2004, 2005, 2006, 2007 Apple Inc. All rights reserved.

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
#include "CachedImage.h"

#include "BitmapImage.h"
#include "Cache.h"
#include "CachedResourceClient.h"
#include "CachedResourceClientWalker.h"
#include "DocLoader.h"
#include "Frame.h"
#include "FrameLoaderTypes.h"
#include "FrameView.h"
#include "Request.h"
#include "Settings.h"
#include <wtf/CurrentTime.h>
#include <wtf/StdLibExtras.h>
#include <wtf/Vector.h>

#if PLATFORM(CG)
#include "PDFDocumentImage.h"
#endif

#if ENABLE(SVG_AS_IMAGE)
#include "SVGImage.h"
#endif

using std::max;

namespace WebCore {

CachedImage::CachedImage(const String& url)
    : CachedResource(url, ImageResource)
    , m_image(0)
    , m_decodedDataDeletionTimer(this, &CachedImage::decodedDataDeletionTimerFired)
    , m_httpStatusCodeErrorOccurred(false)
{
    m_status = Unknown;
}

CachedImage::CachedImage(Image* image)
    : CachedResource(String(), ImageResource)
    , m_image(image)
    , m_decodedDataDeletionTimer(this, &CachedImage::decodedDataDeletionTimerFired)
    , m_httpStatusCodeErrorOccurred(false)
{
    m_status = Cached;
    m_loading = false;
}

CachedImage::~CachedImage()
{
}

void CachedImage::decodedDataDeletionTimerFired(Timer<CachedImage>*)
{
    ASSERT(!hasClients());
    destroyDecodedData();
}

void CachedImage::load(DocLoader* docLoader)
{
#ifdef ANDROID_BLOCK_NETWORK_IMAGE
    if (!docLoader || (docLoader->autoLoadImages() && !docLoader->shouldBlockNetworkImage(m_url)))
#else
    if (!docLoader || docLoader->autoLoadImages())
#endif
        CachedResource::load(docLoader, true, DoSecurityCheck, true);
    else
        m_loading = false;
}

void CachedImage::didAddClient(CachedResourceClient* c)
{
    if (m_decodedDataDeletionTimer.isActive())
        m_decodedDataDeletionTimer.stop();
    
    if (m_data && !m_image && !m_errorOccurred) {
        createImage();
        m_image->setData(m_data, true);
    }

    if (m_image && !m_image->isNull())
        c->imageChanged(this);

    if (!m_loading)
        c->notifyFinished(this);
}

void CachedImage::allClientsRemoved()
{
    if (m_image && !m_errorOccurred)
        m_image->resetAnimation();
    if (double interval = cache()->deadDecodedDataDeletionInterval())
        m_decodedDataDeletionTimer.startOneShot(interval);
}

static Image* brokenImage()
{
    DEFINE_STATIC_LOCAL(RefPtr<Image>, brokenImage, (Image::loadPlatformResource("missingImage")));
    return brokenImage.get();
}

static Image* nullImage()
{
    DEFINE_STATIC_LOCAL(RefPtr<BitmapImage>, nullImage, (BitmapImage::create()));
    return nullImage.get();
}

Image* CachedImage::image() const
{
    ASSERT(!isPurgeable());

    if (m_errorOccurred)
        return brokenImage();

    if (m_image)
        return m_image.get();

    return nullImage();
}

void CachedImage::setImageContainerSize(const IntSize& containerSize)
{
    if (m_image)
        m_image->setContainerSize(containerSize);
}

bool CachedImage::usesImageContainerSize() const
{
    if (m_image)
        return m_image->usesContainerSize();

    return false;
}

bool CachedImage::imageHasRelativeWidth() const
{
    if (m_image)
        return m_image->hasRelativeWidth();

    return false;
}

bool CachedImage::imageHasRelativeHeight() const
{
    if (m_image)
        return m_image->hasRelativeHeight();

    return false;
}

IntSize CachedImage::imageSize(float multiplier) const
{
    ASSERT(!isPurgeable());

    if (!m_image)
        return IntSize();
    if (multiplier == 1.0f)
        return m_image->size();
        
    // Don't let images that have a width/height >= 1 shrink below 1 when zoomed.
    bool hasWidth = m_image->size().width() > 0;
    bool hasHeight = m_image->size().height() > 0;
    int width = m_image->size().width() * (m_image->hasRelativeWidth() ? 1.0f : multiplier);
    int height = m_image->size().height() * (m_image->hasRelativeHeight() ? 1.0f : multiplier);
    if (hasWidth)
        width = max(1, width);
    if (hasHeight)
        height = max(1, height);
    return IntSize(width, height);
}

IntRect CachedImage::imageRect(float multiplier) const
{
    ASSERT(!isPurgeable());

    if (!m_image)
        return IntRect();
    if (multiplier == 1.0f || (!m_image->hasRelativeWidth() && !m_image->hasRelativeHeight()))
        return m_image->rect();

    float widthMultiplier = (m_image->hasRelativeWidth() ? 1.0f : multiplier);
    float heightMultiplier = (m_image->hasRelativeHeight() ? 1.0f : multiplier);

    // Don't let images that have a width/height >= 1 shrink below 1 when zoomed.
    bool hasWidth = m_image->rect().width() > 0;
    bool hasHeight = m_image->rect().height() > 0;

    int width = static_cast<int>(m_image->rect().width() * widthMultiplier);
    int height = static_cast<int>(m_image->rect().height() * heightMultiplier);
    if (hasWidth)
        width = max(1, width);
    if (hasHeight)
        height = max(1, height);

    int x = static_cast<int>(m_image->rect().x() * widthMultiplier);
    int y = static_cast<int>(m_image->rect().y() * heightMultiplier);

    return IntRect(x, y, width, height);
}

void CachedImage::notifyObservers(const IntRect* changeRect)
{
    CachedResourceClientWalker w(m_clients);
    while (CachedResourceClient* c = w.next())
        c->imageChanged(this, changeRect);
}

void CachedImage::clear()
{
    destroyDecodedData();
    m_image = 0;
    setEncodedSize(0);
}

inline void CachedImage::createImage()
{
    // Create the image if it doesn't yet exist.
    if (m_image)
        return;
#if PLATFORM(CG)
    if (m_response.mimeType() == "application/pdf") {
        m_image = PDFDocumentImage::create();
        return;
    }
#endif
#if ENABLE(SVG_AS_IMAGE)
    if (m_response.mimeType() == "image/svg+xml") {
        m_image = SVGImage::create(this);
        return;
    }
#endif
    m_image = BitmapImage::create(this);
#if PLATFORM(ANDROID)
    m_image->setURL(url());
#endif
}

size_t CachedImage::maximumDecodedImageSize()
{
    Frame* frame = m_request ? m_request->docLoader()->frame() : 0;
    if (!frame)
        return 0;
    Settings* settings = frame->settings();
    return settings ? settings->maximumDecodedImageSize() : 0;
}

void CachedImage::data(PassRefPtr<SharedBuffer> data, bool allDataReceived)
{
    m_data = data;

    createImage();

    bool sizeAvailable = false;

    // Have the image update its data from its internal buffer.
    // It will not do anything now, but will delay decoding until 
    // queried for info (like size or specific image frames).
    sizeAvailable = m_image->setData(m_data, allDataReceived);

    // Go ahead and tell our observers to try to draw if we have either
    // received all the data or the size is known.  Each chunk from the
    // network causes observers to repaint, which will force that chunk
    // to decode.
    if (sizeAvailable || allDataReceived) {
        size_t maxDecodedImageSize = maximumDecodedImageSize();
        IntSize s = imageSize(1.0f);
        size_t estimatedDecodedImageSize = s.width() * s.height() * 4; // no overflow check
        if (m_image->isNull() || (maxDecodedImageSize > 0 && estimatedDecodedImageSize > maxDecodedImageSize)) {
            error();
            if (inCache())
                cache()->remove(this);
            return;
        }
        
        // It would be nice to only redraw the decoded band of the image, but with the current design
        // (decoding delayed until painting) that seems hard.
        notifyObservers();

        if (m_image)
            setEncodedSize(m_image->data() ? m_image->data()->size() : 0);
    }
    
    if (allDataReceived) {
        m_loading = false;
        checkNotify();
    }
}

void CachedImage::error()
{
    clear();
    m_errorOccurred = true;
    m_data.clear();
    notifyObservers();
    m_loading = false;
    checkNotify();
}

void CachedImage::checkNotify()
{
    if (m_loading)
        return;

    CachedResourceClientWalker w(m_clients);
    while (CachedResourceClient* c = w.next())
        c->notifyFinished(this);
}

void CachedImage::destroyDecodedData()
{
    bool canDeleteImage = !m_image || (m_image->hasOneRef() && m_image->isBitmapImage());
    if (isSafeToMakePurgeable() && canDeleteImage && !m_loading) {
        // Image refs the data buffer so we should not make it purgeable while the image is alive. 
        // Invoking addClient() will reconstruct the image object.
        m_image = 0;
        setDecodedSize(0);
        makePurgeable(true);
    } else if (m_image && !m_errorOccurred)
        m_image->destroyDecodedData();
}

void CachedImage::decodedSizeChanged(const Image* image, int delta)
{
    if (image != m_image)
        return;
    
    setDecodedSize(decodedSize() + delta);
}

void CachedImage::didDraw(const Image* image)
{
    if (image != m_image)
        return;
    
    double timeStamp = FrameView::currentPaintTimeStamp();
    if (!timeStamp) // If didDraw is called outside of a Frame paint.
        timeStamp = currentTime();
    
    CachedResource::didAccessDecodedData(timeStamp);
}

bool CachedImage::shouldPauseAnimation(const Image* image)
{
    if (image != m_image)
        return false;
    
    CachedResourceClientWalker w(m_clients);
    while (CachedResourceClient* c = w.next()) {
        if (c->willRenderImage(this))
            return false;
    }

    return true;
}

void CachedImage::animationAdvanced(const Image* image)
{
    if (image == m_image)
        notifyObservers();
}

void CachedImage::changedInRect(const Image* image, const IntRect& rect)
{
    if (image == m_image)
        notifyObservers(&rect);
}

} //namespace WebCore