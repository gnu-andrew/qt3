/****************************************************************************
**
** Definition of QDict template class
**
** Created : 920821
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

#ifndef QDICT_H
#define QDICT_H

#ifndef QT_H
#include "qgdict.h"
#endif // QT_H

template<class type>
class QDict
#ifdef Q_QDOC
	: public QPtrCollection
#else
	: public QGDict
#endif
{
public:
    QDict( int size = 17, bool caseSensitive = TRUE )
	: QGDict( size, StringKey, caseSensitive, FALSE ) { }
    QDict( const QDict<type> &d ) : QGDict( d ) { }
    ~QDict()				{ clear(); }
    QDict<type> &operator=(const QDict<type> &d)
			{ return (QDict<type>&)QGDict::operator=(d); }
    uint  count()   const		{ return QGDict::count(); }
    uint  size()    const		{ return QGDict::size(); }
    bool  isEmpty() const		{ return QGDict::count() == 0; }

    void  insert( const QString &k, const type *d )
					{ QGDict::look_string(k,(Item)d,1); }
    void  replace( const QString &k, const type *d )
					{ QGDict::look_string(k,(Item)d,2); }
    bool  remove( const QString &k )	{ return QGDict::remove_string(k); }
    type *take( const QString &k )	{ return (type *)QGDict::take_string(k); }
    type *find( const QString &k ) const
		{ return (type *)((QGDict*)this)->QGDict::look_string(k,0,0); }
    type *operator[]( const QString &k ) const
		{ return (type *)((QGDict*)this)->QGDict::look_string(k,0,0); }

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
template<> inline void QDict<void>::deleteItem( Item )
{
}
#endif

template<class type> inline void QDict<type>::deleteItem( QPtrCollection::Item d )
{
    if ( del_item ) delete (type *)d;
}

template<class type>
class QDictIterator : public QGDictIterator
{
public:
    QDictIterator(const QDict<type> &d) : QGDictIterator((QGDict &)d) { }
    ~QDictIterator()	      {}
    uint  count()   const     { return dict->count(); }
    bool  isEmpty() const     { return dict->count() == 0; }
    type *toFirst()	      { return (type *)QGDictIterator::toFirst(); }
    operator type *() const   { return (type *)QGDictIterator::get(); }
    type *operator*()         { return (type *)QGDictIterator::get(); }
    type   *current() const   { return (type *)QGDictIterator::get(); }
    QString currentKey() const{ return QGDictIterator::getKeyString(); }
    type *operator()()	      { return (type *)QGDictIterator::operator()(); }
    type *operator++()	      { return (type *)QGDictIterator::operator++(); }
    type *operator+=(uint j)  { return (type *)QGDictIterator::operator+=(j); }
};

#define Q_DEFINED_QDICT
#include "qwinexport.h"
#endif // QDICT_H
