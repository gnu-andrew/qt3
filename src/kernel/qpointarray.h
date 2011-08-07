/****************************************************************************
**
** Definition of QPointArray class
**
** Created : 940213
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

#ifndef QPOINTARRAY_H
#define QPOINTARRAY_H

#ifndef QT_H
#include "qmemarray.h"
#include "qpoint.h"
#endif // QT_H


#if defined(Q_TEMPLATEDLL)
//Q_TEMPLATE_EXTERN template class Q_EXPORT QMemArray<QPoint>;
#endif

class Q_EXPORT QPointArray : public QMemArray<QPoint>
{
public:
    QPointArray() {}
    ~QPointArray() {}
    QPointArray( int size ) : QMemArray<QPoint>( size ) {}
    QPointArray( const QPointArray &a ) : QMemArray<QPoint>( a ) {}
    QPointArray( const QRect &r, bool closed=FALSE );
    QPointArray( int nPoints, const QCOORD *points );

    QPointArray	 &operator=( const QPointArray &a )
	{ return (QPointArray&)assign( a ); }

    QPointArray copy() const
	{ QPointArray tmp; return *((QPointArray*)&tmp.duplicate(*this)); }

    void    translate( int dx, int dy );
    QRect   boundingRect() const;

    void    point( uint i, int *x, int *y ) const;
    QPoint  point( uint i ) const;
    void    setPoint( uint i, int x, int y );
    void    setPoint( uint i, const QPoint &p );
    bool    setPoints( int nPoints, const QCOORD *points );
    bool    setPoints( int nPoints, int firstx, int firsty, ... );
    bool    putPoints( int index, int nPoints, const QCOORD *points );
    bool    putPoints( int index, int nPoints, int firstx, int firsty, ... );
    bool    putPoints( int index, int nPoints,
		       const QPointArray & from, int fromIndex=0 );

    void    makeArc( int x, int y, int w, int h, int a1, int a2 );
    void    makeEllipse( int x, int y, int w, int h );
    void    makeArc( int x, int y, int w, int h, int a1, int a2,
		     const QWMatrix& );
#ifndef QT_NO_BEZIER
    QPointArray cubicBezier() const;
#endif
    void*  shortPoints( int index = 0, int nPoints = -1 ) const;
    static void cleanBuffers();

protected:
    static uint splen;
    static void* sp;
};


/*****************************************************************************
  QPointArray stream functions
 *****************************************************************************/
#ifndef QT_NO_DATASTREAM
Q_EXPORT QDataStream &operator<<( QDataStream &, const QPointArray & );
Q_EXPORT QDataStream &operator>>( QDataStream &, QPointArray & );
#endif

/*****************************************************************************
  Misc. QPointArray functions
 *****************************************************************************/

inline void QPointArray::setPoint( uint i, const QPoint &p )
{
    setPoint( i, p.x(), p.y() );
}


#endif // QPOINTARRAY_H
