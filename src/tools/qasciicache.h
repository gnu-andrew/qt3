/****************************************************************************
**
** Definition of QAsciiCache template/macro class
**
** Created : 950209
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

#ifndef QASCIICACHE_H
#define QASCIICACHE_H

#ifndef QT_H
#include "qgcache.h"
#endif // QT_H


template<class type> 
class QAsciiCache
#ifdef Q_QDOC
	: public QPtrCollection
#else
	: public QGCache
#endif
{
public:
    QAsciiCache( const QAsciiCache<type> &c ) : QGCache(c) {}
    QAsciiCache( int maxCost=100, int size=17, bool caseSensitive=TRUE,
		 bool copyKeys=TRUE )
	: QGCache( maxCost, size, AsciiKey, caseSensitive, copyKeys ) {}
   ~QAsciiCache()			{ clear(); }
    QAsciiCache<type> &operator=( const QAsciiCache<type> &c )
			{ return (QAsciiCache<type>&)QGCache::operator=(c); }
    int	  maxCost()   const		{ return QGCache::maxCost(); }
    int	  totalCost() const		{ return QGCache::totalCost(); }
    void  setMaxCost( int m )		{ QGCache::setMaxCost(m); }
    uint  count()     const		{ return QGCache::count(); }
    uint  size()      const		{ return QGCache::size(); }
    bool  isEmpty()   const		{ return QGCache::count() == 0; }
    void  clear()			{ QGCache::clear(); }
    bool  insert( const char *k, const type *d, int c=1, int p=0 )
			{ return QGCache::insert_other(k,(Item)d,c,p);}
    bool  remove( const char *k )
			{ return QGCache::remove_other(k); }
    type *take( const char *k )
			{ return (type *)QGCache::take_other(k); }
    type *find( const char *k, bool ref=TRUE ) const
			{ return (type *)QGCache::find_other(k,ref);}
    type *operator[]( const char *k ) const
			{ return (type *)QGCache::find_other(k);}
    void  statistics() const	      { QGCache::statistics(); }
private:
    void  deleteItem( Item d );
};

#if !defined(Q_BROKEN_TEMPLATE_SPECIALIZATION)
template<> inline void QAsciiCache<void>::deleteItem( QPtrCollection::Item )
{
}
#endif

template<class type> inline void QAsciiCache<type>::deleteItem( QPtrCollection::Item d )
{
    if ( del_item ) delete (type *)d;
}


template<class type> 
class QAsciiCacheIterator : public QGCacheIterator
{
public:
    QAsciiCacheIterator( const QAsciiCache<type> &c ):QGCacheIterator((QGCache &)c) {}
    QAsciiCacheIterator( const QAsciiCacheIterator<type> &ci)
				: QGCacheIterator( (QGCacheIterator &)ci ) {}
    QAsciiCacheIterator<type> &operator=(const QAsciiCacheIterator<type>&ci)
	{ return ( QAsciiCacheIterator<type>&)QGCacheIterator::operator=( ci ); }
    uint  count()   const     { return QGCacheIterator::count(); }
    bool  isEmpty() const     { return QGCacheIterator::count() == 0; }
    bool  atFirst() const     { return QGCacheIterator::atFirst(); }
    bool  atLast()  const     { return QGCacheIterator::atLast(); }
    type *toFirst()	      { return (type *)QGCacheIterator::toFirst(); }
    type *toLast()	      { return (type *)QGCacheIterator::toLast(); }
    operator type *() const   { return (type *)QGCacheIterator::get(); }
    type *current()   const   { return (type *)QGCacheIterator::get(); }
    const char *currentKey() const { return QGCacheIterator::getKeyAscii(); }
    type *operator()()	      { return (type *)QGCacheIterator::operator()();}
    type *operator++()	      { return (type *)QGCacheIterator::operator++(); }
    type *operator+=(uint j)  { return (type *)QGCacheIterator::operator+=(j);}
    type *operator--()	      { return (type *)QGCacheIterator::operator--(); }
    type *operator-=(uint j)  { return (type *)QGCacheIterator::operator-=(j);}
};


#endif // QASCIICACHE_H
