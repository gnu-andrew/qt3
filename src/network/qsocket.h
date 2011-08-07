/****************************************************************************
**
** Definition of QSocket class.
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

#ifndef QSOCKET_H
#define QSOCKET_H

#ifndef QT_H
#include "qobject.h"
#include "qiodevice.h"
#include "qhostaddress.h" // int->QHostAddress conversion
#endif // QT_H

#if !defined( QT_MODULE_NETWORK ) || defined( QT_LICENSE_PROFESSIONAL ) || defined( QT_INTERNAL_NETWORK )
#define QM_EXPORT_NETWORK
#else
#define QM_EXPORT_NETWORK Q_EXPORT
#endif

#ifndef QT_NO_NETWORK
class QSocketPrivate;
class QSocketDevice;


class QM_EXPORT_NETWORK QSocket : public QObject, public QIODevice
{
    Q_OBJECT
public:
    enum Error {
	ErrConnectionRefused,
	ErrHostNotFound,
	ErrSocketRead
    };

    QSocket( QObject *parent=0, const char *name=0 );
    virtual ~QSocket();

    enum State { Idle, HostLookup, Connecting,
		 Connected, Closing,
		 Connection=Connected };
    State	 state() const;

    int		 socket() const;
    virtual void setSocket( int );

    QSocketDevice *socketDevice();
    virtual void setSocketDevice( QSocketDevice * );

#ifndef QT_NO_DNS
    virtual void connectToHost( const QString &host, Q_UINT16 port );
#endif
    QString	 peerName() const;

    // Implementation of QIODevice abstract virtual functions
    bool	 open( int mode );
    void	 close();
    void	 flush();
    Offset	 size() const;
    Offset	 at() const;
    bool	 at( Offset );
    bool	 atEnd() const;

    Q_ULONG	 bytesAvailable() const; // ### QIODevice::Offset instead?
    Q_ULONG	 waitForMore( int msecs, bool *timeout  ) const;
    Q_ULONG	 waitForMore( int msecs ) const; // ### Qt 4.0: merge the two overloads
    Q_ULONG	 bytesToWrite() const;
    void	 clearPendingData();

    Q_LONG	 readBlock( char *data, Q_ULONG maxlen );
    Q_LONG	 writeBlock( const char *data, Q_ULONG len );
    Q_LONG	 readLine( char *data, Q_ULONG maxlen );

    int		 getch();
    int		 putch( int );
    int		 ungetch(int);

    bool	 canReadLine() const;
    virtual	 QString readLine();

    Q_UINT16	 port() const;
    Q_UINT16	 peerPort() const;
    QHostAddress address() const;
    QHostAddress peerAddress() const;

    void	 setReadBufferSize( Q_ULONG );
    Q_ULONG	 readBufferSize() const;

signals:
    void	 hostFound();
    void	 connected();
    void	 connectionClosed();
    void	 delayedCloseFinished();
    void	 readyRead();
    void	 bytesWritten( int nbytes );
    void	 error( int );

protected slots:
    virtual void sn_read( bool force=FALSE );
    virtual void sn_write();

private slots:
    void	tryConnecting();
    void	emitErrorConnectionRefused();

private:
    QSocketPrivate *d;

    bool	 consumeWriteBuf( Q_ULONG nbytes );
    void	 tryConnection();
    void         setSocketIntern( int socket );

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QSocket( const QSocket & );
    QSocket &operator=( const QSocket & );
#endif
};

#endif //QT_NO_NETWORK
#endif // QSOCKET_H
