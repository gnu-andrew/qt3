/****************************************************************************
**
** Definition of QAsciiDict template class
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

#ifndef QASCIIDICT_H
#define QASCIIDICT_H

#ifndef QT_H
#include "qgdict.h"
#endif // QT_H

template<class type>
class QAsciiDict
#ifdef Q_QDOC
	: public QPtrCollection
#else
	: public QGDict
#endif
{
public:
    QAsciiDict(int size=17, bool caseSensitive=TRUE, bool copyKeys=TRUE )
	: QGDict(size,AsciiKey,caseSensitive,copyKeys) {}
    QAsciiDict( const QAsciiDict<type> &d ) : QGDict(d) {}
   ~QAsciiDict()			{ clear(); }
    QAsciiDict<type> &operator=(const QAsciiDict<type> &d)
			{ return (QAsciiDict<type>&)QGDict::operator=(d); }
    uint  count()   const		{ return QGDict::count(); }
    uint  size()    const		{ return QGDict::size(); }
    bool  isEmpty() const		{ return QGDict::count() == 0; }

    void  insert( const char *k, const type *d )
					{ QGDict::look_ascii(k,(Item)d,1); }
    void  replace( const char *k, const type *d )
					{ QGDict::look_ascii(k,(Item)d,2); }
    bool  remove( const char *k )	{ return QGDict::remove_ascii(k); }
    type *take( const char *k )		{ return (type *)QGDict::take_ascii(k); }
    type *find( const char *k ) const
		{ return (type *)((QGDict*)this)->QGDict::look_ascii(k,0,0); }
    type *operator[]( const char *k ) const
		{ return (type *)((QGDict*)this)->QGDict::look_ascii(k,0,0); }

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
template<> inline void QAsciiDict<void>::deleteItem( QPtrCollection::Item )
{
}
#endif

template<class type> inline void QAsciiDict<type>::deleteItem( QPtrCollection::Item d )
{
    if ( del_item ) delete (type *)d;
}

template<class type>
class QAsciiDictIterator : public QGDictIterator
{
public:
    QAsciiDictIterator(const QAsciiDict<type> &d)
	: QGDictIterator((QGDict &)d) {}
   ~QAsciiDictIterator()      {}
    uint  count()   const     { return dict->count(); }
    bool  isEmpty() const     { return dict->count() == 0; }
    type *toFirst()	      { return (type *)QGDictIterator::toFirst(); }
    operator type *() const   { return (type *)QGDictIterator::get(); }
    type   *current() const   { return (type *)QGDictIterator::get(); }
    const char *currentKey() const { return QGDictIterator::getKeyAscii(); }
    type *operator()()	      { return (type *)QGDictIterator::operator()(); }
    type *operator++()	      { return (type *)QGDictIterator::operator++(); }
    type *operator+=(uint j)  { return (type *)QGDictIterator::operator+=(j);}
};

#define Q_DEFINED_QASCIIDICT
#include "qwinexport.h"
#endif // QASCIIDICT_H
