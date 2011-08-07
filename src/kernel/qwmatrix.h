/****************************************************************************
**
** Definition of QWMatrix class
**
** Created : 941020
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

#ifndef QWMATRIX_H
#define QWMATRIX_H

#ifndef QT_H
#include "qwindowdefs.h"
#include "qpointarray.h"
#include "qrect.h"
#include "qregion.h"
#endif // QT_H

#ifndef QT_NO_WMATRIX


class Q_EXPORT QWMatrix					// 2D transform matrix
{
public:
    QWMatrix();
    QWMatrix( double m11, double m12, double m21, double m22,
	      double dx, double dy );

    void	setMatrix( double m11, double m12, double m21, double m22,
			   double dx,  double dy );

    double	m11() const { return _m11; }
    double	m12() const { return _m12; }
    double	m21() const { return _m21; }
    double	m22() const { return _m22; }
    double	dx()  const { return _dx; }
    double	dy()  const { return _dy; }

    void	map( int x, int y, int *tx, int *ty )	      const;
    void	map( double x, double y, double *tx, double *ty ) const;
    QRect	mapRect( const QRect & )	const;

    QPoint	map( const QPoint &p )	const { return operator *( p ); }
    QRect	map( const QRect &r )	const { return mapRect ( r ); }
    QPointArray map( const QPointArray &a ) const { return operator * ( a ); }
    QRegion     map( const QRegion &r ) const { return operator *( r ); }
    QRegion     mapToRegion( const QRect &r ) const { return operator *( r ); }
    QPointArray	mapToPolygon( const QRect &r )	const;

    void	reset();
    bool	isIdentity() const;

    QWMatrix   &translate( double dx, double dy );
    QWMatrix   &scale( double sx, double sy );
    QWMatrix   &shear( double sh, double sv );
    QWMatrix   &rotate( double a );

    bool isInvertible() const { return (_m11*_m22 - _m12*_m21) != 0; }
    double det() const { return _m11*_m22 - _m12*_m21; }

    QWMatrix	invert( bool * = 0 ) const;

    bool	operator==( const QWMatrix & ) const;
    bool	operator!=( const QWMatrix & ) const;
    QWMatrix   &operator*=( const QWMatrix & );

    /* we use matrix multiplication semantics here */
    QPoint operator * (const QPoint & ) const;
    QRegion operator * (const QRect & ) const;
    QRegion operator * (const QRegion & ) const;
    QPointArray operator *  ( const QPointArray &a ) const;

    enum TransformationMode {
	Points, Areas
    };
    static void setTransformationMode( QWMatrix::TransformationMode m );
    static TransformationMode transformationMode();
private:
    double	_m11, _m12;
    double	_m21, _m22;
    double	_dx,  _dy;
};

Q_EXPORT QWMatrix operator*( const QWMatrix &, const QWMatrix & );


/*****************************************************************************
  QWMatrix stream functions
 *****************************************************************************/

Q_EXPORT QDataStream &operator<<( QDataStream &, const QWMatrix & );
Q_EXPORT QDataStream &operator>>( QDataStream &, QWMatrix & );


#endif // QT_NO_WMATRIX

#endif // QWMATRIX_H
