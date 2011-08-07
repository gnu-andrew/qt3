/****************************************************************************
**
** Definition of QGList and QGListIterator classes
**
** Created : 920624
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

#ifndef QGLIST_H
#define QGLIST_H

#ifndef QT_H
#include "qptrcollection.h"
#endif // QT_H

class Q_EXPORT QLNode
{
friend class QGList;
friend class QGListIterator;
friend class QGListStdIterator;
public:
    QPtrCollection::Item getData()	{ return data; }
private:
    QPtrCollection::Item data;
    QLNode *prev;
    QLNode *next;
    QLNode( QPtrCollection::Item d ) { data = d; }
};

class QGListIteratorList; // internal helper class

class Q_EXPORT QGList : public QPtrCollection	// doubly linked generic list
{
friend class QGListIterator;
friend class QGListIteratorList;
friend class QGVector;				// needed by QGVector::toList
public:
    uint  count() const;			// return number of nodes

#ifndef QT_NO_DATASTREAM
    QDataStream &read( QDataStream & );		// read list from stream
    QDataStream &write( QDataStream & ) const;	// write list to stream
#endif
protected:
    QGList();					// create empty list
    QGList( const QGList & );			// make copy of other list
    virtual ~QGList();

    QGList &operator=( const QGList & );	// assign from other list
    bool operator==( const QGList& ) const;

    void inSort( QPtrCollection::Item );		// add item sorted in list
    void append( QPtrCollection::Item );		// add item at end of list
    bool insertAt( uint index, QPtrCollection::Item ); // add item at i'th position
    void relinkNode( QLNode * );		// relink as first item
    bool removeNode( QLNode * );		// remove node
    bool remove( QPtrCollection::Item = 0 );	// remove item (0=current)
    bool removeRef( QPtrCollection::Item = 0 );	// remove item (0=current)
    bool removeFirst();				// remove first item
    bool removeLast();				// remove last item
    bool removeAt( uint );			// remove item at i'th position
    bool replaceAt( uint, QPtrCollection::Item ); // replace item at position i with item
    QPtrCollection::Item takeNode( QLNode * );	// take out node
    QPtrCollection::Item take();		// take out current item
    QPtrCollection::Item takeAt( uint index );	// take out item at i'th pos
    QPtrCollection::Item takeFirst();		// take out first item
    QPtrCollection::Item takeLast();		// take out last item

    void sort();                        	// sort all items;
    void clear();				// remove all items

    int	 findRef( QPtrCollection::Item, bool = TRUE ); // find exact item in list
    int	 find( QPtrCollection::Item, bool = TRUE ); // find equal item in list

    uint containsRef( QPtrCollection::Item ) const;	// get number of exact matches
    uint contains( QPtrCollection::Item ) const;	// get number of equal matches

    QPtrCollection::Item at( uint index );	// access item at i'th pos
    int	  at() const;				// get current index
    QLNode *currentNode() const;		// get current node

    QPtrCollection::Item get() const;		// get current item

    QPtrCollection::Item cfirst() const;	// get ptr to first list item
    QPtrCollection::Item clast()  const;	// get ptr to last list item
    QPtrCollection::Item first();		// set first item in list curr
    QPtrCollection::Item last();		// set last item in list curr
    QPtrCollection::Item next();		// set next item in list curr
    QPtrCollection::Item prev();		// set prev item in list curr

    void  toVector( QGVector * ) const;		// put items in vector

    virtual int compareItems( QPtrCollection::Item, QPtrCollection::Item );

#ifndef QT_NO_DATASTREAM
    virtual QDataStream &read( QDataStream &, QPtrCollection::Item & );
    virtual QDataStream &write( QDataStream &, QPtrCollection::Item ) const;
#endif

    QLNode* begin() const { return firstNode; }
    QLNode* end() const { return 0; }
    QLNode* erase( QLNode* it );

private:
    void  prepend( QPtrCollection::Item );	// add item at start of list

    void heapSortPushDown( QPtrCollection::Item* heap, int first, int last );

    QLNode *firstNode;				// first node
    QLNode *lastNode;				// last node
    QLNode *curNode;				// current node
    int curIndex;				// current index
    uint numNodes;				// number of nodes
    QGListIteratorList *iterators; 		// list of iterators

    QLNode *locate( uint );			// get node at i'th pos
    QLNode *unlink();				// unlink node
};


inline uint QGList::count() const
{
    return numNodes;
}

inline bool QGList::removeFirst()
{
    first();
    return remove();
}

inline bool QGList::removeLast()
{
    last();
    return remove();
}

inline int QGList::at() const
{
    return curIndex;
}

inline QPtrCollection::Item QGList::at( uint index )
{
    QLNode *n = locate( index );
    return n ? n->data : 0;
}

inline QLNode *QGList::currentNode() const
{
    return curNode;
}

inline QPtrCollection::Item QGList::get() const
{
    return curNode ? curNode->data : 0;
}

inline QPtrCollection::Item QGList::cfirst() const
{
    return firstNode ? firstNode->data : 0;
}

inline QPtrCollection::Item QGList::clast() const
{
    return lastNode ? lastNode->data : 0;
}


/*****************************************************************************
  QGList stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
Q_EXPORT QDataStream &operator>>( QDataStream &, QGList & );
Q_EXPORT QDataStream &operator<<( QDataStream &, const QGList & );
#endif

/*****************************************************************************
  QGListIterator class
 *****************************************************************************/

class Q_EXPORT QGListIterator			// QGList iterator
{
friend class QGList;
friend class QGListIteratorList;
protected:
    QGListIterator( const QGList & );
    QGListIterator( const QGListIterator & );
    QGListIterator &operator=( const QGListIterator & );
   ~QGListIterator();

    bool  atFirst() const;			// test if at first item
    bool  atLast()  const;			// test if at last item
    QPtrCollection::Item	  toFirst();				// move to first item
    QPtrCollection::Item	  toLast();				// move to last item

    QPtrCollection::Item	  get() const;				// get current item
    QPtrCollection::Item	  operator()();				// get current and move to next
    QPtrCollection::Item	  operator++();				// move to next item (prefix)
    QPtrCollection::Item	  operator+=(uint);			// move n positions forward
    QPtrCollection::Item	  operator--();				// move to prev item (prefix)
    QPtrCollection::Item	  operator-=(uint);			// move n positions backward

protected:
    QGList *list;				// reference to list

private:
    QLNode  *curNode;				// current node in list
};


inline bool QGListIterator::atFirst() const
{
    return curNode == list->firstNode;
}

inline bool QGListIterator::atLast() const
{
    return curNode == list->lastNode;
}

inline QPtrCollection::Item QGListIterator::get() const
{
    return curNode ? curNode->data : 0;
}

class Q_EXPORT QGListStdIterator
{
public:
    inline QGListStdIterator( QLNode* n ) : node( n ){}
    inline operator QLNode* () { return node; }
protected:
    inline QLNode *next() { return node->next; }
    QLNode *node;
};


#endif	// QGLIST_H
