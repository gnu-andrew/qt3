/****************************************************************************
**
** Definition of QGCache and QGCacheIterator classes
**
** Created : 950208
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

#ifndef QGCACHE_H
#define QGCACHE_H

#ifndef QT_H
#include "qptrcollection.h"
#include "qglist.h"
#include "qgdict.h"
#endif // QT_H


class QCList;					// internal classes
class QCListIt;
class QCDict;


class Q_EXPORT QGCache : public QPtrCollection	// generic LRU cache
{
friend class QGCacheIterator;
protected:
    enum KeyType { StringKey, AsciiKey, IntKey, PtrKey };
      // identical to QGDict's, but PtrKey is not used at the moment

    QGCache( int maxCost, uint size, KeyType kt, bool caseSensitive,
	     bool copyKeys );
    QGCache( const QGCache & );			// not allowed, calls fatal()
   ~QGCache();
    QGCache &operator=( const QGCache & );	// not allowed, calls fatal()

    uint    count()	const;
    uint    size()	const;
    int	    maxCost()	const	{ return mCost; }
    int	    totalCost() const	{ return tCost; }
    void    setMaxCost( int maxCost );
    void    clear();

    bool    insert_string( const QString &key, QPtrCollection::Item,
			   int cost, int priority );
    bool    insert_other( const char *key, QPtrCollection::Item,
			  int cost, int priority );
    bool    remove_string( const QString &key );
    bool    remove_other( const char *key );
    QPtrCollection::Item take_string( const QString &key );
    QPtrCollection::Item take_other( const char *key );

    QPtrCollection::Item find_string( const QString &key, bool ref=TRUE ) const;
    QPtrCollection::Item find_other( const char *key, bool ref=TRUE ) const;

    void    statistics() const;

private:
    bool    makeRoomFor( int cost, int priority = -1 );
    KeyType keytype;
    QCList *lruList;
    QCDict *dict;
    int	    mCost;
    int	    tCost;
    bool    copyk;
};


class Q_EXPORT QGCacheIterator			// generic cache iterator
{
protected:
    QGCacheIterator( const QGCache & );
    QGCacheIterator( const QGCacheIterator & );
   ~QGCacheIterator();
    QGCacheIterator &operator=( const QGCacheIterator & );

    uint	      count()   const;
    bool	      atFirst() const;
    bool	      atLast()  const;
    QPtrCollection::Item toFirst();
    QPtrCollection::Item toLast();

    QPtrCollection::Item get() const;
    QString	      getKeyString() const;
    const char       *getKeyAscii()  const;
    long	      getKeyInt()    const;

    QPtrCollection::Item operator()();
    QPtrCollection::Item operator++();
    QPtrCollection::Item operator+=( uint );
    QPtrCollection::Item operator--();
    QPtrCollection::Item operator-=( uint );

protected:
    QCListIt *it;				// iterator on cache list
};


#endif // QGCACHE_H
