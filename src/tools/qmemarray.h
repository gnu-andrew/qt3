/****************************************************************************
**
** Definition of QMemArray template/macro class
**
** Created : 930906
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

#ifndef QMEMARRAY_H
#define QMEMARRAY_H

#ifndef QT_H
#include "qgarray.h"
#endif // QT_H


template<class type>
class QMemArray : public QGArray
{
public:
    typedef type* Iterator;
    typedef const type* ConstIterator;
    typedef type ValueType;

protected:
    QMemArray( int, int ) : QGArray( 0, 0 ) {}

public:
    QMemArray() {}
    QMemArray( int size ) : QGArray(size*sizeof(type)) {} // ### 4.0 Q_EXPLICIT
    QMemArray( const QMemArray<type> &a ) : QGArray(a) {}
   ~QMemArray() {}
    QMemArray<type> &operator=(const QMemArray<type> &a)
				{ return (QMemArray<type>&)QGArray::assign(a); }
    type *data()    const	{ return (type *)QGArray::data(); }
    uint  nrefs()   const	{ return QGArray::nrefs(); }
    uint  size()    const	{ return QGArray::size()/sizeof(type); }
    uint  count()   const	{ return size(); }
    bool  isEmpty() const	{ return QGArray::size() == 0; }
    bool  isNull()  const	{ return QGArray::data() == 0; }
    bool  resize( uint size )	{ return QGArray::resize(size*sizeof(type)); }
    bool  resize( uint size, Optimization optim ) { return QGArray::resize(size*sizeof(type), optim); }
    bool  truncate( uint pos )	{ return QGArray::resize(pos*sizeof(type)); }
    bool  fill( const type &d, int size = -1 )
	{ return QGArray::fill((char*)&d,size,sizeof(type) ); }
    void  detach()		{ QGArray::detach(); }
    QMemArray<type>   copy() const
	{ QMemArray<type> tmp; return tmp.duplicate(*this); }
    QMemArray<type>& assign( const QMemArray<type>& a )
	{ return (QMemArray<type>&)QGArray::assign(a); }
    QMemArray<type>& assign( const type *a, uint n )
	{ return (QMemArray<type>&)QGArray::assign((char*)a,n*sizeof(type)); }
    QMemArray<type>& duplicate( const QMemArray<type>& a )
	{ return (QMemArray<type>&)QGArray::duplicate(a); }
    QMemArray<type>& duplicate( const type *a, uint n )
	{ return (QMemArray<type>&)QGArray::duplicate((char*)a,n*sizeof(type)); }
    QMemArray<type>& setRawData( const type *a, uint n )
	{ return (QMemArray<type>&)QGArray::setRawData((char*)a,
						     n*sizeof(type)); }
    void resetRawData( const type *a, uint n )
	{ QGArray::resetRawData((char*)a,n*sizeof(type)); }
    int	 find( const type &d, uint i=0 ) const
	{ return QGArray::find((char*)&d,i,sizeof(type)); }
    int	 contains( const type &d ) const
	{ return QGArray::contains((char*)&d,sizeof(type)); }
    void sort() { QGArray::sort(sizeof(type)); }
    int  bsearch( const type &d ) const
	{ return QGArray::bsearch((const char*)&d,sizeof(type)); }
    // ### Qt 4.0: maybe provide uint overload as work-around for MSVC bug
    type& operator[]( int i ) const
	{ return (type &)(*(type *)QGArray::at(i*sizeof(type))); }
    type& at( uint i ) const
	{ return (type &)(*(type *)QGArray::at(i*sizeof(type))); }
	 operator const type*() const { return (const type *)QGArray::data(); }
    bool operator==( const QMemArray<type> &a ) const { return isEqual(a); }
    bool operator!=( const QMemArray<type> &a ) const { return !isEqual(a); }
    Iterator begin() { return data(); }
    Iterator end() { return data() + size(); }
    ConstIterator begin() const { return data(); }
    ConstIterator end() const { return data() + size(); }
};

#ifndef QT_NO_COMPAT
#define QArray QMemArray
#endif

#define Q_DEFINED_QMEMARRAY
#include "qwinexport.h"
#endif // QARRAY_H
