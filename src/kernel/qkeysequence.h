/****************************************************************************
**
** Definition of QKeySequence class
**
** Created : 0108007
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

#ifndef QKEYSEQUENCE_H
#define QKEYSEQUENCE_H

#ifndef QT_H
#ifndef QT_H
#include "qnamespace.h"
#include "qstring.h"
#endif // QT_H
#endif

#ifndef QT_NO_ACCEL

/*****************************************************************************
  QKeySequence stream functions
 *****************************************************************************/
#ifndef QT_NO_DATASTREAM
class QKeySequence;
Q_EXPORT QDataStream &operator<<( QDataStream &, const QKeySequence & );
Q_EXPORT QDataStream &operator>>( QDataStream &, QKeySequence & );
#endif

class QKeySequencePrivate;

class Q_EXPORT QKeySequence : public Qt
{
public:
    QKeySequence();
    QKeySequence( const QString& key );
    QKeySequence( int key );
    QKeySequence( int k1, int k2, int k3 = 0, int k4 = 0 );
    QKeySequence( const QKeySequence & );
    ~QKeySequence();

    uint count() const;
    bool isEmpty() const;
    Qt::SequenceMatch matches( const QKeySequence & ) const;

    operator QString() const;
    operator int () const;
    int operator[]( uint ) const;
    QKeySequence &operator=( const QKeySequence & );
    bool operator==( const QKeySequence& ) const;
    bool operator!= ( const QKeySequence& ) const;

private:
    static int decodeString( const QString & );
    static QString encodeString( int );
    int assign( QString );
    void setKey( int key, int index );

    QKeySequencePrivate* d;

    friend Q_EXPORT QDataStream &operator<<( QDataStream &, const QKeySequence & );
    friend Q_EXPORT QDataStream &operator>>( QDataStream &, QKeySequence & );
    friend class QAccelManager;
};

#else

class Q_EXPORT QKeySequence : public Qt
{
public:
    QKeySequence() {}
    QKeySequence( int ) {}
};

#endif //QT_NO_ACCEL

#endif
