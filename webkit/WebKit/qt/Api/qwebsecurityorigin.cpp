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
    Copyright (C) 2008 Nokia Corporation and/or its subsidiary(-ies)

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
#include "qwebsecurityorigin.h"
#include "qwebsecurityorigin_p.h"
#include "qwebdatabase.h"
#include "qwebdatabase_p.h"

#include "DatabaseTracker.h"
#include "KURL.h"
#include "SecurityOrigin.h"
#include <QStringList>

using namespace WebCore;

void QWEBKIT_EXPORT qt_drt_whiteListAccessFromOrigin(const QString& sourceOrigin, const QString& destinationProtocol, const QString& destinationHost, bool allowDestinationSubdomains)
{
    SecurityOrigin::whiteListAccessFromOrigin(*SecurityOrigin::createFromString(sourceOrigin), destinationProtocol, destinationHost, allowDestinationSubdomains);
}

void QWEBKIT_EXPORT qt_drt_resetOriginAccessWhiteLists()
{
    SecurityOrigin::resetOriginAccessWhiteLists();
}

void QWEBKIT_EXPORT qt_drt_setDomainRelaxationForbiddenForURLScheme(bool forbidden, const QString& scheme)
{
    SecurityOrigin::setDomainRelaxationForbiddenForURLScheme(forbidden, scheme);
}

/*!
    \class QWebSecurityOrigin
    \since 4.5
    \brief The QWebSecurityOrigin class defines a security boundary for web sites.

    \inmodule QtWebKit

    QWebSecurityOrigin provides access to the security domains defined by web sites.
    An origin consists of a host name, a scheme, and a port number. Web sites
    with the same security origin can access each other's resources for client-side
    scripting or databases.

    For example the site \c{http://www.example.com/my/page.html} is allowed to share the same
    database as \c{http://www.example.com/my/overview.html}, or access each other's
    documents when used in HTML frame sets and JavaScript. At the same time it prevents
    \c{http://www.malicious.com/evil.html} from accessing \c{http://www.example.com/}'s resources,
    because they are of a different security origin.

    Call QWebFrame::securityOrigin() to get the QWebSecurityOrigin for a frame in a
    web page, and use host(), scheme() and port() to identify the security origin.

    Use databases() to access the databases defined within a security origin. The
    disk usage of the origin's databases can be limited with setDatabaseQuota().
    databaseQuota() and databaseUsage() report the current limit as well as the
    current usage.

    For more information refer to the
    \l{http://en.wikipedia.org/wiki/Same_origin_policy}{"Same origin policy" Wikipedia Article}.

    \sa QWebFrame::securityOrigin()
*/

/*!
    Constructs a security origin from \a other.
*/
QWebSecurityOrigin::QWebSecurityOrigin(const QWebSecurityOrigin& other) : d(other.d)
{
}

/*!
    Assigns the \a other security origin to this.
*/
QWebSecurityOrigin& QWebSecurityOrigin::operator=(const QWebSecurityOrigin& other)
{
    d = other.d;
    return *this;
}

/*!
    Returns the scheme defining the security origin.
*/
QString QWebSecurityOrigin::scheme() const
{
    return d->origin->protocol();
}

/*!
    Returns the host name defining the security origin.
*/
QString QWebSecurityOrigin::host() const
{
    return d->origin->host();
}

/*!
    Returns the port number defining the security origin.
*/
int QWebSecurityOrigin::port() const
{
    return d->origin->port();
}

/*!
    Returns the number of bytes all databases in the security origin
    use on the disk.
*/
qint64 QWebSecurityOrigin::databaseUsage() const
{
#if ENABLE(DATABASE)
    return DatabaseTracker::tracker().usageForOrigin(d->origin.get());
#else
    return 0;
#endif
}

/*!
    Returns the quota for the databases in the security origin.
*/
qint64 QWebSecurityOrigin::databaseQuota() const
{
#if ENABLE(DATABASE)
    return DatabaseTracker::tracker().quotaForOrigin(d->origin.get());
#else
    return 0;
#endif
}

/*!
    Sets the quota for the databases in the security origin to \a quota bytes.

    If the quota is set to a value less than the current usage, the quota will remain
    and no data will be purged to meet the new quota. However, no new data can be added
    to databases in this origin.
*/
void QWebSecurityOrigin::setDatabaseQuota(qint64 quota)
{
#if ENABLE(DATABASE)
    DatabaseTracker::tracker().setQuota(d->origin.get(), quota);
#endif
}

/*!
    Destroys the security origin.
*/
QWebSecurityOrigin::~QWebSecurityOrigin()
{
}

/*!
    \internal
*/
QWebSecurityOrigin::QWebSecurityOrigin(QWebSecurityOriginPrivate* priv)
{
    d = priv;
}

/*!
    Returns a list of all security origins with a database quota defined.
*/
QList<QWebSecurityOrigin> QWebSecurityOrigin::allOrigins()
{
    QList<QWebSecurityOrigin> webOrigins;

#if ENABLE(DATABASE)
    Vector<RefPtr<SecurityOrigin> > coreOrigins;
    DatabaseTracker::tracker().origins(coreOrigins);

    for (unsigned i = 0; i < coreOrigins.size(); ++i) {
        QWebSecurityOriginPrivate* priv = new QWebSecurityOriginPrivate(coreOrigins[i].get());
        webOrigins.append(priv);
    }
#endif

    return webOrigins;
}

/*!
    Returns a list of all databases defined in the security origin.
*/
QList<QWebDatabase> QWebSecurityOrigin::databases() const
{
    QList<QWebDatabase> databases;

#if ENABLE(DATABASE)
    Vector<String> nameVector;

    if (!DatabaseTracker::tracker().databaseNamesForOrigin(d->origin.get(), nameVector))
        return databases;
    for (unsigned i = 0; i < nameVector.size(); ++i) {
        QWebDatabasePrivate* priv = new QWebDatabasePrivate();
        priv->name = nameVector[i];
        priv->origin = this->d->origin;
        QWebDatabase webDatabase(priv);
        databases.append(webDatabase);
    }
#endif

    return databases;
}

/*!
    \since 4.6

    Adds the given \a scheme to the list of schemes that are considered equivalent
    to the \c file: scheme. They are not subject to cross domain restrictions.
*/
void QWebSecurityOrigin::addLocalScheme(const QString& scheme)
{
    SecurityOrigin::registerURLSchemeAsLocal(scheme);
}

/*!
    \since 4.6

    Removes the given \a scheme from the list of local schemes.

    \sa addLocalScheme()
*/
void QWebSecurityOrigin::removeLocalScheme(const QString& scheme)
{
    SecurityOrigin::removeURLSchemeRegisteredAsLocal(scheme);
}

/*!
    \since 4.6
    Returns a list of all the schemes that were set by the application as local schemes,
    \sa addLocalScheme(), removeLocalScheme()
*/
QStringList QWebSecurityOrigin::localSchemes()
{
    QStringList list;
    const URLSchemesMap& map = SecurityOrigin::localURLSchemes();
    URLSchemesMap::const_iterator end = map.end();
    for (URLSchemesMap::const_iterator i = map.begin(); i != end; ++i) {
        const QString scheme = *i;
        list.append(scheme);
    }
    return list;
}
