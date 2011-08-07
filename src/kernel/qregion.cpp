/****************************************************************************
**
** Implementation of QRegion class
**
** Created : 950726
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

#include "qregion.h"
#include "qpointarray.h"
#include "qbuffer.h"
#include "qdatastream.h"

// BEING REVISED: paul
/*!
    \class QRegion qregion.h
    \brief The QRegion class specifies a clip region for a painter.

    \ingroup images
    \ingroup graphics

    QRegion is used with QPainter::setClipRegion() to limit the paint
    area to what needs to be painted. There is also a
    QWidget::repaint() that takes a QRegion parameter. QRegion is the
    best tool for reducing flicker.

    A region can be created from a rectangle, an ellipse, a polygon or
    a bitmap. Complex regions may be created by combining simple
    regions using unite(), intersect(), subtract() or eor() (exclusive
    or). You can move a region using translate().

    You can test whether a region isNull(), isEmpty() or if it
    contains() a QPoint or QRect. The bounding rectangle is given by
    boundingRect().

    The function rects() gives a decomposition of the region into
    rectangles.

    Example of using complex regions:
    \code
	void MyWidget::paintEvent( QPaintEvent * )
	{
	    QPainter p;                         // our painter
	    QRegion r1( QRect(100,100,200,80),  // r1 = elliptic region
			QRegion::Ellipse );
	    QRegion r2( QRect(100,120,90,30) ); // r2 = rectangular region
	    QRegion r3 = r1.intersect( r2 );    // r3 = intersection
	    p.begin( this );                    // start painting widget
	    p.setClipRegion( r3 );              // set clip region
	    ...                                 // paint clipped graphics
	    p.end();                            // painting done
	}
    \endcode

    QRegion is an \link shclass.html implicitly shared\endlink class.

    \warning Due to window system limitations, the whole coordinate
    space for a region is limited to the points between -32767 and
    32767 on Mac OS X and Windows 95/98/ME.

    \sa QPainter::setClipRegion(), QPainter::setClipRect()
*/


/*!
    \enum QRegion::RegionType

    Specifies the shape of the region to be created.

    \value Rectangle  the region covers the entire rectangle.
    \value Ellipse  the region is an ellipse inside the rectangle.
*/

/*!
    \fn Region  QRegion::handle() const

    Returns the region's handle.
*/

/*****************************************************************************
  QRegion member functions
 *****************************************************************************/

/*!
    Constructs a rectangular or elliptic region.

    If \a t is \c Rectangle, the region is the filled rectangle (\a x,
    \a y, \a w, \a h). If \a t is \c Ellipse, the region is the filled
    ellipse with center at (\a x + \a w / 2, \a y + \a h / 2) and size
    (\a w ,\a h ).
*/
QRegion::QRegion( int x, int y, int w, int h, RegionType t )
{
    QRegion tmp(QRect(x,y,w,h),t);
    tmp.data->ref();
    data = tmp.data;
}

/*!
    Detaches from shared region data to make sure that this region is
    the only one referring to the data.

    \sa copy(), \link shclass.html shared classes\endlink
*/

void QRegion::detach()
{
    if ( data->count != 1 )
	*this = copy();
}

#ifndef QT_NO_DATASTREAM
/*
    Executes region commands in the internal buffer and rebuilds the
    original region.

    We do this when we read a region from the data stream.

    If \a ver is non-0, uses the format version \a ver on reading the
    byte array.
*/

void QRegion::exec( const QByteArray &buffer, int ver )
{
    QBuffer buf( buffer );
    QDataStream s( &buf );
    if ( ver )
	s.setVersion( ver );
    buf.open( IO_ReadOnly );
    QRegion rgn;
#if defined(QT_CHECK_STATE)
    int test_cnt = 0;
#endif
    while ( !s.eof() ) {
	Q_INT32 id;
	if ( s.version() == 1 ) {
	    int id_int;
	    s >> id_int;
	    id = id_int;
	} else {
	    s >> id;
	}
#if defined(QT_CHECK_STATE)
	if ( test_cnt > 0 && id != QRGN_TRANSLATE )
	    qWarning( "QRegion::exec: Internal error" );
	test_cnt++;
#endif
	if ( id == QRGN_SETRECT || id == QRGN_SETELLIPSE ) {
	    QRect r;
	    s >> r;
	    rgn = QRegion( r, id == QRGN_SETRECT ? Rectangle : Ellipse );
	} else if ( id == QRGN_SETPTARRAY_ALT || id == QRGN_SETPTARRAY_WIND ) {
	    QPointArray a;
	    s >> a;
	    rgn = QRegion( a, id == QRGN_SETPTARRAY_WIND );
	} else if ( id == QRGN_TRANSLATE ) {
	    QPoint p;
	    s >> p;
	    rgn.translate( p.x(), p.y() );
	} else if ( id >= QRGN_OR && id <= QRGN_XOR ) {
	    QByteArray bop1, bop2;
	    QRegion r1, r2;
	    s >> bop1;	r1.exec( bop1 );
	    s >> bop2;	r2.exec( bop2 );
	    switch ( id ) {
		case QRGN_OR:
		    rgn = r1.unite( r2 );
		    break;
		case QRGN_AND:
		    rgn = r1.intersect( r2 );
		    break;
		case QRGN_SUB:
		    rgn = r1.subtract( r2 );
		    break;
		case QRGN_XOR:
		    rgn = r1.eor( r2 );
		    break;
	    }
	} else if ( id == QRGN_RECTS ) {
	    // (This is the only form used in Qt 2.0)
	    Q_UINT32 n;
	    s >> n;
	    QRect r;
	    for ( int i=0; i<(int)n; i++ ) {
		s >> r;
		rgn = rgn.unite( QRegion(r) );
	    }
	}
    }
    buf.close();
    *this = rgn;
}


/*****************************************************************************
  QRegion stream functions
 *****************************************************************************/

/*!
    \relates QRegion

    Writes the region \a r to the stream \a s and returns a reference
    to the stream.

    \sa \link datastreamformat.html Format of the QDataStream operators \endlink
*/

QDataStream &operator<<( QDataStream &s, const QRegion &r )
{
    QMemArray<QRect> a = r.rects();
    if ( a.isEmpty() ) {
	s << (Q_UINT32)0;
    } else {
	if ( s.version() == 1 ) {
	    int i;
	    for ( i=(int)a.size()-1; i>0; i-- ) {
		s << (Q_UINT32)(12+i*24);
		s << (int)QRGN_OR;
	    }
	    for ( i=0; i<(int)a.size(); i++ ) {
		s << (Q_UINT32)(4+8) << (int)QRGN_SETRECT << a[i];
	    }
	}
	else {
	    s << (Q_UINT32)(4+4+16*a.size()); // 16: storage size of QRect
	    s << (Q_INT32)QRGN_RECTS;
	    s << (Q_UINT32)a.size();
	    for ( int i=0; i<(int)a.size(); i++ )
		s << a[i];
	}
    }
    return s;
}

/*!
    \relates QRegion

    Reads a region from the stream \a s into \a r and returns a
    reference to the stream.

    \sa \link datastreamformat.html Format of the QDataStream operators \endlink
*/

QDataStream &operator>>( QDataStream &s, QRegion &r )
{
    QByteArray b;
    s >> b;
    r.exec( b, s.version() );
    return s;
}
#endif //QT_NO_DATASTREAM

// These are not inline - they can be implemented better on some platforms
//  (eg. Windows at least provides 3-variable operations).  For now, simple.


/*!
    Applies the unite() function to this region and \a r. \c r1|r2 is
    equivalent to \c r1.unite(r2)

    \sa unite(), operator+()
*/
const QRegion QRegion::operator|( const QRegion &r ) const
    { return unite(r); }

/*!
    Applies the unite() function to this region and \a r. \c r1+r2 is
    equivalent to \c r1.unite(r2)

    \sa unite(), operator|()
*/
const QRegion QRegion::operator+( const QRegion &r ) const
    { return unite(r); }

/*!
    Applies the intersect() function to this region and \a r. \c r1&r2
    is equivalent to \c r1.intersect(r2)

    \sa intersect()
*/
const QRegion QRegion::operator&( const QRegion &r ) const
    { return intersect(r); }

/*!
    Applies the subtract() function to this region and \a r. \c r1-r2
    is equivalent to \c r1.subtract(r2)

    \sa subtract()
*/
const QRegion QRegion::operator-( const QRegion &r ) const
    { return subtract(r); }

/*!
    Applies the eor() function to this region and \a r. \c r1^r2 is
    equivalent to \c r1.eor(r2)

    \sa eor()
*/
const QRegion QRegion::operator^( const QRegion &r ) const
    { return eor(r); }

/*!
    Applies the unite() function to this region and \a r and assigns
    the result to this region. \c r1|=r2 is equivalent to \c
    r1=r1.unite(r2)

    \sa unite()
*/
QRegion& QRegion::operator|=( const QRegion &r )
    { return *this = *this | r; }

/*!
    Applies the unite() function to this region and \a r and assigns
    the result to this region. \c r1+=r2 is equivalent to \c
    r1=r1.unite(r2)

    \sa intersect()
*/
QRegion& QRegion::operator+=( const QRegion &r )
    { return *this = *this + r; }

/*!
    Applies the intersect() function to this region and \a r and
    assigns the result to this region. \c r1&=r2 is equivalent to \c
    r1=r1.intersect(r2)

    \sa intersect()
*/
QRegion& QRegion::operator&=( const QRegion &r )
    { return *this = *this & r; }

/*!
    Applies the subtract() function to this region and \a r and
    assigns the result to this region. \c r1-=r2 is equivalent to \c
    r1=r1.subtract(r2)

    \sa subtract()
*/
QRegion& QRegion::operator-=( const QRegion &r )
    { return *this = *this - r; }

/*!
    Applies the eor() function to this region and \a r and
    assigns the result to this region. \c r1^=r2 is equivalent to \c
    r1=r1.eor(r2)

    \sa eor()
*/
QRegion& QRegion::operator^=( const QRegion &r )
    { return *this = *this ^ r; }

