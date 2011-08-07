/****************************************************************************
**
** ...
**
** Copyright (C) 2001-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QCLEANUPHANDLER_H
#define QCLEANUPHANDLER_H

#ifndef QT_H
#include "qptrlist.h"
#endif // QT_H

template<class Type>
class QCleanupHandler
{
public:
    QCleanupHandler() : cleanupObjects( 0 ) {}
    ~QCleanupHandler() { clear(); }

    Type* add( Type **object ) {
	if ( !cleanupObjects )
	    cleanupObjects = new QPtrList<Type*>;
	cleanupObjects->insert( 0, object );
	return *object;
    }

    void remove( Type **object ) {
	if ( !cleanupObjects )
	    return;
	if ( cleanupObjects->findRef( object ) >= 0 )
	    (void) cleanupObjects->take();
    }

    bool isEmpty() const {
	return cleanupObjects ? cleanupObjects->isEmpty() : TRUE;
    }

    void clear() {
	if ( !cleanupObjects )
	    return;
	QPtrListIterator<Type*> it( *cleanupObjects );
	Type **object;
	while ( ( object = it.current() ) ) {
	    delete *object;
	    *object = 0;
	    cleanupObjects->remove( object );
	}
	delete cleanupObjects;
	cleanupObjects = 0;
    }

private:
    QPtrList<Type*> *cleanupObjects;
};

template<class Type>
class QSingleCleanupHandler
{
public:
    QSingleCleanupHandler() : object( 0 ) {}
    ~QSingleCleanupHandler() {
	if ( object ) {
	    delete *object;
	    *object = 0;
	}
    }
    Type* set( Type **o ) {
	object = o;
	return *object;
    }
    void reset() { object = 0; }
private:
    Type **object;
};

template<class Type>
class QSharedCleanupHandler
{
public:
    QSharedCleanupHandler() : object( 0 ) {}
    ~QSharedCleanupHandler() {
	if ( object ) {
	    if ( (*object)->deref() )
		delete *object;
	    *object = 0;
	}
    }
    Type* set( Type **o ) {
	object = o;
	return *object;
    }
    void reset() { object = 0; }
private:
    Type **object;
};

#endif //QCLEANUPHANDLER_H
