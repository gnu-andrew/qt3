/****************************************************************************
**
** Definition of QPtrVector pointer based template class
**
** Created : 930907
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

#ifndef QPTRVECTOR_H
#define QPTRVECTOR_H

#ifndef QT_H
#include "qgvector.h"
#endif // QT_H

template<class type>
class QPtrVector
#ifdef Q_QDOC
	: public QPtrCollection
#else
	: public QGVector
#endif
{
public:
    QPtrVector()				{ }
    QPtrVector( uint size ) : QGVector(size) { }
    QPtrVector( const QPtrVector<type> &v ) : QGVector( v ) { }
    ~QPtrVector()				{ clear(); }
    QPtrVector<type> &operator=(const QPtrVector<type> &v)
			{ return (QPtrVector<type>&)QGVector::operator=(v); }
    bool operator==( const QPtrVector<type> &v ) const { return QGVector::operator==(v); }
    type **data()   const		{ return (type **)QGVector::data(); }
    uint  size()    const		{ return QGVector::size(); }
    uint  count()   const		{ return QGVector::count(); }
    bool  isEmpty() const		{ return QGVector::count() == 0; }
    bool  isNull()  const		{ return QGVector::size() == 0; }
    bool  resize( uint size )		{ return QGVector::resize(size); }
    bool  insert( uint i, const type *d){ return QGVector::insert(i,(Item)d); }
    bool  remove( uint i )		{ return QGVector::remove(i); }
    type *take( uint i )		{ return (type *)QGVector::take(i); }
    void  clear()			{ QGVector::clear(); }
    bool  fill( const type *d, int size=-1 )
					{ return QGVector::fill((Item)d,size);}
    void  sort()			{ QGVector::sort(); }
    int	  bsearch( const type *d ) const{ return QGVector::bsearch((Item)d); }
    int	  findRef( const type *d, uint i=0 ) const
					{ return QGVector::findRef((Item)d,i);}
    int	  find( const type *d, uint i= 0 ) const
					{ return QGVector::find((Item)d,i); }
    uint  containsRef( const type *d ) const
				{ return QGVector::containsRef((Item)d); }
    uint  contains( const type *d ) const
					{ return QGVector::contains((Item)d); }
    type *operator[]( int i ) const	{ return (type *)QGVector::at(i); }
    type *at( uint i ) const		{ return (type *)QGVector::at(i); }
    void  toList( QGList *list ) const	{ QGVector::toList(list); }

#ifdef Q_QDOC
protected:
    virtual int compareItems( QPtrCollection::Item d1, QPtrCollection::Item d2 );
    virtual QDataStream& read( QDataStream &s, QPtrCollection::Item &d );
    virtual QDataStream& write( QDataStream &s, QPtrCollection::Item d ) const;
#endif

private:
    void  deleteItem( Item d );
};

#if !defined(Q_BROKEN_TEMPLATE_SPECIALIZATION)
template<> inline void QPtrVector<void>::deleteItem( QPtrCollection::Item )
{
}
#endif

template<class type> inline void QPtrVector<type>::deleteItem( QPtrCollection::Item d )
{
    if ( del_item ) delete (type *)d;
}

#ifndef QT_NO_COMPAT
#define QVector QPtrVector
#endif

#define Q_DEFINED_QPTRVECTOR
#include "qwinexport.h"
#endif // QVECTOR_H
