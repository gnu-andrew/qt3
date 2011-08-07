/****************************************************************************
**
** Definition of QUrlOperator class
**
** Created : 950429
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the kernel module of the Qt GUI Toolkit.
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

#ifndef QURLOPERATOR_H
#define QURLOPERATOR_H

#ifndef QT_H
#include "qobject.h"
#include "qurl.h"
#include "qptrlist.h"
#include "qnetworkprotocol.h"
#include "qstringlist.h" // QString->QStringList conversion
#endif // QT_H

#ifndef QT_NO_NETWORKPROTOCOL

class QUrlInfo;
class QUrlOperatorPrivate;

class Q_EXPORT QUrlOperator : public QObject, public QUrl
{
    friend class QNetworkProtocol;

    Q_OBJECT

public:
    QUrlOperator();
    QUrlOperator( const QString &urL );
    QUrlOperator( const QUrlOperator& url );
    QUrlOperator( const QUrlOperator& url, const QString& relUrl, bool checkSlash = FALSE );
    virtual ~QUrlOperator();

    virtual void setPath( const QString& path );
    virtual bool cdUp();

    virtual const QNetworkOperation *listChildren();
    virtual const QNetworkOperation *mkdir( const QString &dirname );
    virtual const QNetworkOperation *remove( const QString &filename );
    virtual const QNetworkOperation *rename( const QString &oldname, const QString &newname );
    virtual const QNetworkOperation *get( const QString &location = QString::null );
    virtual const QNetworkOperation *put( const QByteArray &data, const QString &location = QString::null  );
    virtual QPtrList<QNetworkOperation> copy( const QString &from, const QString &to, bool move = FALSE, bool toPath = TRUE );
    virtual void copy( const QStringList &files, const QString &dest, bool move = FALSE );
    virtual bool isDir( bool *ok = 0 );

    virtual void setNameFilter( const QString &nameFilter );
    QString nameFilter() const;

    virtual QUrlInfo info( const QString &entry ) const;

    QUrlOperator& operator=( const QUrlOperator &url );
    QUrlOperator& operator=( const QString &url );

    virtual void stop();

signals:
    void newChildren( const QValueList<QUrlInfo> &, QNetworkOperation *res );
    void finished( QNetworkOperation *res );
    void start( QNetworkOperation *res );
    void createdDirectory( const QUrlInfo &, QNetworkOperation *res );
    void removed( QNetworkOperation *res );
    void itemChanged( QNetworkOperation *res );
    void data( const QByteArray &, QNetworkOperation *res );
    void dataTransferProgress( int bytesDone, int bytesTotal, QNetworkOperation *res );
    void startedNextCopy( const QPtrList<QNetworkOperation> &lst );
    void connectionStateChanged( int state, const QString &data );

protected:
    void reset();
    bool parse( const QString& url );
    virtual bool checkValid();
    virtual void clearEntries();
    void getNetworkProtocol();
    void deleteNetworkProtocol();

private slots:
    const QNetworkOperation *startOperation( QNetworkOperation *op );
    void copyGotData( const QByteArray &data, QNetworkOperation *op );
    void continueCopy( QNetworkOperation *op );
    void finishedCopy();
    void addEntry( const QValueList<QUrlInfo> &i );
    void slotItemChanged( QNetworkOperation *op );

private:
    void deleteOperation( QNetworkOperation *op );

    QUrlOperatorPrivate *d;
};

#endif // QT_NO_NETWORKPROTOCOL

#endif // QURLOPERATOR_H
