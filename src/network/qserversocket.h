/****************************************************************************
**
** Definition of QServerSocketClass.
**
** Created : 970521
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QSERVERSOCKET_H
#define QSERVERSOCKET_H

#ifndef QT_H
#include "qobject.h"
#include "qhostaddress.h"
#include "qsocketdevice.h" // ### remove or keep for users' convenience?
#endif // QT_H
#ifndef QT_NO_NETWORK

#if !defined( QT_MODULE_NETWORK ) || defined( QT_LICENSE_PROFESSIONAL ) || defined( QT_INTERNAL_NETWORK )
#define QM_EXPORT_NETWORK
#else
#define QM_EXPORT_NETWORK Q_EXPORT
#endif

class QServerSocketPrivate;


class QM_EXPORT_NETWORK QServerSocket : public QObject
{
    Q_OBJECT
public:
    QServerSocket( Q_UINT16 port, int backlog = 1,
		   QObject *parent=0, const char *name=0 );
    QServerSocket( const QHostAddress & address, Q_UINT16 port, int backlog = 1,
		   QObject *parent=0, const char *name=0 );
    QServerSocket( QObject *parent=0, const char *name=0 );
    virtual ~QServerSocket();

    bool ok() const;

    Q_UINT16 port() const ;

    int socket() const ;
    virtual void setSocket( int socket );

    QHostAddress address() const ;

    virtual void newConnection( int socket ) = 0;

protected:
    QSocketDevice *socketDevice();

private slots:
    void incomingConnection( int socket );

private:
    QServerSocketPrivate *d;
    void init( const QHostAddress & address, Q_UINT16 port, int backlog );
};

#endif // QT_NO_NETWORK
#endif // QSERVERSOCKET_H
