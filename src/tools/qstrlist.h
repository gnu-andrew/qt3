/****************************************************************************
**
** Definition of QStrList, QStrIList and QStrListIterator classes
**
** Created : 920730
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

#ifndef QSTRLIST_H
#define QSTRLIST_H

#ifndef QT_H
#include "qstring.h"
#include "qptrlist.h"
#include "qdatastream.h"
#endif // QT_H

#if defined(Q_QDOC)
class QStrListIterator : public QPtrListIterator<char>
{
};
#else
typedef QPtrListIterator<char> QStrListIterator;
#endif

class Q_EXPORT QStrList : public QPtrList<char>
{
public:
    QStrList( bool deepCopies=TRUE ) { dc = deepCopies; del_item = deepCopies; }
    QStrList( const QStrList & );
    ~QStrList()			{ clear(); }
    QStrList& operator=( const QStrList & );

private:
    QPtrCollection::Item newItem( QPtrCollection::Item d ) { return dc ? qstrdup( (const char*)d ) : d; }
    void deleteItem( QPtrCollection::Item d ) { if ( del_item ) delete[] (char*)d; }
    int compareItems( QPtrCollection::Item s1, QPtrCollection::Item s2 ) { return qstrcmp((const char*)s1,
							 (const char*)s2); }
#ifndef QT_NO_DATASTREAM
    QDataStream &read( QDataStream &s, QPtrCollection::Item &d )
				{ s >> (char *&)d; return s; }
    QDataStream &write( QDataStream &s, QPtrCollection::Item d ) const
				{ return s << (const char *)d; }
#endif
    bool  dc;
};


class Q_EXPORT QStrIList : public QStrList	// case insensitive string list
{
public:
    QStrIList( bool deepCopies=TRUE ) : QStrList( deepCopies ) {}
    ~QStrIList()			{ clear(); }
private:
    int	  compareItems( QPtrCollection::Item s1, QPtrCollection::Item s2 )
				{ return qstricmp((const char*)s1,
						    (const char*)s2); }
};


inline QStrList & QStrList::operator=( const QStrList &strList )
{
    clear();
    dc = strList.dc;
    del_item = dc;
    QPtrList<char>::operator=( strList );
    return *this;
}

inline QStrList::QStrList( const QStrList &strList )
    : QPtrList<char>( strList )
{
    dc = FALSE;
    operator=( strList );
}

#endif // QSTRLIST_H
