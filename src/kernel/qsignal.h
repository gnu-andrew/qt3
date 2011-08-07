/****************************************************************************
**
** Definition of QSignal class
**
** Created : 941201
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

#ifndef QSIGNAL_H
#define QSIGNAL_H

#ifndef QT_H
#include "qvariant.h"
#include "qobject.h"
#endif // QT_H


class Q_EXPORT QSignal : public QObject
{
    Q_OBJECT

public:
    QSignal( QObject *parent=0, const char *name=0 );
    ~QSignal();

    bool	connect( const QObject *receiver, const char *member );
    bool	disconnect( const QObject *receiver, const char *member=0 );

    void	activate();

#ifndef QT_NO_COMPAT
    bool	isBlocked()	 const		{ return QObject::signalsBlocked(); }
    void	block( bool b )		{ QObject::blockSignals( b ); }
#ifndef QT_NO_VARIANT
    void	setParameter( int value );
    int		parameter() const;
#endif
#endif

#ifndef QT_NO_VARIANT
    void	setValue( const QVariant &value );
    QVariant	value() const;
#endif
signals:
#ifndef QT_NO_VARIANT
    void signal( const QVariant& );
#endif
    void intSignal( int );

private:
#ifndef QT_NO_VARIANT
    QVariant val;
#endif
private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QSignal( const QSignal & );
    QSignal &operator=( const QSignal & );
#endif
};


#endif // QSIGNAL_H
