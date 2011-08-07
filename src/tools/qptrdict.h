/****************************************************************************
**
** Definition of QPtrDict template class
**
** Created : 970415
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

#ifndef QPTRDICT_H
#define QPTRDICT_H

#ifndef QT_H
#include "qgdict.h"
#endif // QT_H

template<class type>
class QPtrDict
#ifdef Q_QDOC
	: public QPtrCollection
#else
	: public QGDict
#endif
{
public:
    QPtrDict(int size=17) : QGDict(size,PtrKey,0,0) {}
    QPtrDict( const QPtrDict<type> &d ) : QGDict(d) {}
   ~QPtrDict()				{ clear(); }
    QPtrDict<type> &operator=(const QPtrDict<type> &d)
			{ return (QPtrDict<type>&)QGDict::operator=(d); }
    uint  count()   const		{ return QGDict::count(); }
    uint  size()    const		{ return QGDict::size(); }
    bool  isEmpty() const		{ return QGDict::count() == 0; }
    void  insert( void *k, const type *d )
					{ QGDict::look_ptr(k,(Item)d,1); }
    void  replace( void *k, const type *d )
					{ QGDict::look_ptr(k,(Item)d,2); }
    bool  remove( void *k )		{ return QGDict::remove_ptr(k); }
    type *take( void *k )		{ return (type*)QGDict::take_ptr(k); }
    type *find( void *k ) const
		{ return (type *)((QGDict*)this)->QGDict::look_ptr(k,0,0); }
    type *operator[]( void *k ) const
		{ return (type *)((QGDict*)this)->QGDict::look_ptr(k,0,0); }
    void  clear()			{ QGDict::clear(); }
    void  resize( uint n )		{ QGDict::resize(n); }
    void  statistics() const		{ QGDict::statistics(); }

#ifdef Q_QDOC
protected:
    virtual QDataStream& read( QDataStream &, QPtrCollection::Item & );
    virtual QDataStream& write( QDataStream &, QPtrCollection::Item ) const;
#endif

private:
    void  deleteItem( Item d );
};

#if !defined(Q_BROKEN_TEMPLATE_SPECIALIZATION)
template<> inline void QPtrDict<void>::deleteItem( QPtrCollection::Item )
{
}
#endif

template<class type>
inline void QPtrDict<type>::deleteItem( QPtrCollection::Item d )
{
    if ( del_item ) delete (type *)d;
}

template<class type>
class QPtrDictIterator : public QGDictIterator
{
public:
    QPtrDictIterator(const QPtrDict<type> &d) :QGDictIterator((QGDict &)d) {}
   ~QPtrDictIterator()	      {}
    uint  count()   const     { return dict->count(); }
    bool  isEmpty() const     { return dict->count() == 0; }
    type *toFirst()	      { return (type *)QGDictIterator::toFirst(); }
    operator type *()  const  { return (type *)QGDictIterator::get(); }
    type *current()    const  { return (type *)QGDictIterator::get(); }
    void *currentKey() const  { return QGDictIterator::getKeyPtr(); }
    type *operator()()	      { return (type *)QGDictIterator::operator()(); }
    type *operator++()	      { return (type *)QGDictIterator::operator++(); }
    type *operator+=(uint j)  { return (type *)QGDictIterator::operator+=(j);}
};

#endif // QPTRDICT_H
