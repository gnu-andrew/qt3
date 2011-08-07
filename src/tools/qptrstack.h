/****************************************************************************
**
** Definition of QPtrStack pointer based template class
**
** Created : 920917
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

#ifndef QPTRSTACK_H
#define QPTRSTACK_H

#ifndef QT_H
#include "qglist.h"
#endif // QT_H

template<class type>
class QPtrStack : protected QGList
{
public:
    QPtrStack()				{ }
    QPtrStack( const QPtrStack<type> &s ) : QGList( s ) { }
    ~QPtrStack()			{ clear(); }
    QPtrStack<type> &operator=(const QPtrStack<type> &s)
			{ return (QPtrStack<type>&)QGList::operator=(s); }
    bool  autoDelete() const		{ return QPtrCollection::autoDelete(); }
    void  setAutoDelete( bool del )	{ QPtrCollection::setAutoDelete(del); }
    uint  count()   const		{ return QGList::count(); }
    bool  isEmpty() const		{ return QGList::count() == 0; }
    void  push( const type *d )		{ QGList::insertAt(0,Item(d)); }
    type *pop()				{ return (type *)QGList::takeFirst(); }
    bool  remove()			{ return QGList::removeFirst(); }
    void  clear()			{ QGList::clear(); }
    type *top()	    const		{ return (type *)QGList::cfirst(); }
	  operator type *() const	{ return (type *)QGList::cfirst(); }
    type *current() const		{ return (type *)QGList::cfirst(); }

#ifdef Q_QDOC
protected:
    virtual QDataStream& read( QDataStream&, QPtrCollection::Item& );
    virtual QDataStream& write( QDataStream&, QPtrCollection::Item ) const;
#endif

private:
    void  deleteItem( Item d );
};

#if !defined(Q_BROKEN_TEMPLATE_SPECIALIZATION)
template<> inline void QPtrStack<void>::deleteItem( QPtrCollection::Item )
{
}
#endif

template<class type> inline void QPtrStack<type>::deleteItem( QPtrCollection::Item d )
{
    if ( del_item ) delete (type *)d;
}

#ifndef QT_NO_COMPAT
#define QStack QPtrStack
#endif

#endif // QPTRSTACK_H
