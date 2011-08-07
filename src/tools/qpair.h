/****************************************************************************
**
** Definition of QPair class
**
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the tools module of the Qt GUI Toolkit.
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

#ifndef QPAIR_H
#define QPAIR_H

#ifndef QT_H
#include "qglobal.h"
#include "qdatastream.h"
#endif // QT_H

template <class T1, class T2>
struct QPair
{
    typedef T1 first_type;
    typedef T2 second_type;

    QPair()
	: first( T1() ), second( T2() )
    {}
    QPair( const T1& t1, const T2& t2 )
	: first( t1 ), second( t2 )
    {}

    QPair<T1, T2>& operator=(const QPair<T1, T2>& other)
    {
	if (this != &other) {
	    first = other.first;
	    second = other.second;
	}
	return *this;
    }

    T1 first;
    T2 second;
};

template <class T1, class T2>
Q_INLINE_TEMPLATES bool operator==( const QPair<T1, T2>& x, const QPair<T1, T2>& y )
{
    return x.first == y.first && x.second == y.second;
}

template <class T1, class T2>
Q_INLINE_TEMPLATES bool operator<( const QPair<T1, T2>& x, const QPair<T1, T2>& y )
{
    return x.first < y.first ||
	   ( !( y.first < x.first ) && x.second < y.second );
}

template <class T1, class T2>
Q_INLINE_TEMPLATES QPair<T1, T2> qMakePair( const T1& x, const T2& y )
{
    return QPair<T1, T2>( x, y );
}

#ifndef QT_NO_DATASTREAM
template <class T1, class T2>
inline QDataStream& operator>>( QDataStream& s, QPair<T1, T2>& p )
{
    s >> p.first >> p.second;
    return s;
}

template <class T1, class T2>
inline QDataStream& operator<<( QDataStream& s, const QPair<T1, T2>& p )
{
    s << p.first << p.second;
    return s;
}
#endif

#endif
