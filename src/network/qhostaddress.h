/****************************************************************************
**
** Definition of QHostAddress class.
**
** Created : 979899
**
** Copyright (C) 1997-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the network module of the Qt GUI Toolkit.
**
** This file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the files LICENSE.GPL2
** and LICENSE.GPL3 included in the packaging of this file.
** Alternatively you may (at your option) use any later version
** of the GNU General Public License if such license has been
** publicly approved by Trolltech ASA (or its successors, if any)
** and the KDE Free Qt Foundation.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/.
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** This file may be used under the terms of the Q Public License as
** defined by Trolltech ASA and appearing in the file LICENSE.QPL
** included in the packaging of this file.  Licensees holding valid Qt
** Commercial licenses may use this file in accordance with the Qt
** Commercial License Agreement provided with the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#ifndef QHOSTADDRESS_H
#define QHOSTADDRESS_H

#ifndef QT_H
#include "qstring.h"
#endif // QT_H

#if !defined( QT_MODULE_NETWORK ) || defined( QT_LICENSE_PROFESSIONAL ) || defined( QT_INTERNAL_NETWORK )
#define QM_EXPORT_NETWORK
#else
#define QM_EXPORT_NETWORK Q_EXPORT
#endif

#ifndef QT_NO_NETWORK
class QHostAddressPrivate;

typedef struct {
    Q_UINT8 c[16];
} Q_IPV6ADDR;

class QM_EXPORT_NETWORK QHostAddress
{
public:
    QHostAddress();
    QHostAddress( Q_UINT32 ip4Addr );
    QHostAddress( Q_UINT8 *ip6Addr );
    QHostAddress(const Q_IPV6ADDR &ip6Addr);
#ifndef QT_NO_STRINGLIST
    QHostAddress(const QString &address);
#endif
    QHostAddress( const QHostAddress & );
    virtual ~QHostAddress();

    QHostAddress & operator=( const QHostAddress & );

    void setAddress( Q_UINT32 ip4Addr );
    void setAddress( Q_UINT8 *ip6Addr );
#ifndef QT_NO_STRINGLIST
    bool setAddress( const QString& address );
#endif
    bool	 isIp4Addr()	 const; // obsolete
    Q_UINT32	 ip4Addr()	 const; // obsolete

    bool	 isIPv4Address() const;
    Q_UINT32	 toIPv4Address() const;
    bool	 isIPv6Address() const;
    Q_IPV6ADDR	 toIPv6Address() const;

#ifndef QT_NO_SPRINTF
    QString	 toString() const;
#endif

    bool	 operator==( const QHostAddress & ) const;
    bool	 isNull() const;

private:
    QHostAddressPrivate* d;
};

#endif //QT_NO_NETWORK
#endif
