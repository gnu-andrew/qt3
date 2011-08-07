/****************************************************************************
**
** ...
**
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
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

#include "qmutexpool_p.h"

#ifdef QT_THREAD_SUPPORT

#include <qthread.h>

Q_EXPORT QMutexPool *qt_global_mutexpool = 0;


/*!
    \class QMutexPool qmutexpool_p.h
    \brief The QMutexPool class provides a pool of QMutex objects.

    \internal

    \ingroup thread

    QMutexPool is a convenience class that provides access to a fixed
    number of QMutex objects.

    Typical use of a QMutexPool is in situations where it is not
    possible or feasible to use one QMutex for every protected object.
    The mutex pool will return a mutex based on the address of the
    object that needs protection.

    For example, consider this simple class:

    \code
    class Number {
    public:
	Number( double n ) : num ( n ) { }

	void setNumber( double n ) { num = n; }
	double number() const { return num; }

    private:
	double num;
    };
    \endcode

    Adding a QMutex member to the Number class does not make sense,
    because it is so small. However, in order to ensure that access to
    each Number is protected, you need to use a mutex. In this case, a
    QMutexPool would be ideal.

    Code to calculate the square of a number would then look something
    like this:

    \code
    void calcSquare( Number *num )
    {
	QMutexLocker locker( mutexpool.get( num ) );
	num.setNumber( num.number() * num.number() );
    }
    \endcode

    This function will safely calculate the square of a number, since
    it uses a mutex from a QMutexPool. The mutex is locked and
    unlocked automatically by the QMutexLocker class. See the
    QMutexLocker documentation for more details.
*/

/*!
    Constructs  a QMutexPool, reserving space for \a size QMutexes. If
    \a recursive is TRUE, all QMutexes in the pool will be recursive
    mutexes; otherwise they will all be non-recursive (the default).

    The QMutexes are created when needed, and deleted when the
    QMutexPool is destructed.
*/
QMutexPool::QMutexPool( bool recursive, int size )
    : mutex( FALSE ), count( size ), recurs( recursive )
{
    mutexes = new QMutex*[count];
    for ( int index = 0; index < count; ++index ) {
	mutexes[index] = 0;
    }
}

/*!
    Destructs a QMutexPool. All QMutexes that were created by the pool
    are deleted.
*/
QMutexPool::~QMutexPool()
{
    QMutexLocker locker( &mutex );
    for ( int index = 0; index < count; ++index ) {
	delete mutexes[index];
	mutexes[index] = 0;
    }
    delete [] mutexes;
    mutexes = 0;
}

/*!
    Returns a QMutex from the pool. QMutexPool uses the value \a
    address to determine which mutex is retured from the pool.
*/
QMutex *QMutexPool::get( void *address )
{
    int index = (int) ( (unsigned long) address % count );

    if ( ! mutexes[index] ) {
	// mutex not created, create one

	QMutexLocker locker( &mutex );
	// we need to check once again that the mutex hasn't been created, since
	// 2 threads could be trying to create a mutex as the same index...
	if ( ! mutexes[index] ) {
	    mutexes[index] = new QMutex( recurs );
	}
    }

    return mutexes[index];
}

#endif
