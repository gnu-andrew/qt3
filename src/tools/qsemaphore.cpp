/****************************************************************************
**
** QSemaphore class for Unix
**
** Created : 20010725
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

#if defined(QT_THREAD_SUPPORT)

#include "qsemaphore.h"
#include "qmutex.h"
#include "qwaitcondition.h"


/*!
    \class QSemaphore qsemaphore.h
    \threadsafe
    \brief The QSemaphore class provides a robust integer semaphore.

    \ingroup thread
    \ingroup environment

    A QSemaphore can be used to serialize thread execution, in a
    similar way to a QMutex. A semaphore differs from a mutex, in
    that a semaphore can be accessed by more than one thread at a
    time.

    For example, suppose we have an application that stores data in a
    large tree structure. The application creates 10 threads
    (commonly called a thread pool) to perform searches on the tree.
    When the application searches the tree for some piece of data, it
    uses one thread per base node to do the searching. A semaphore
    could be used to make sure that two threads don't try to search
    the same branch of the tree at the same time.

    A non-computing example of a semaphore would be dining at a
    restuarant. A semaphore is initialized to have a maximum count
    equal to the number of chairs in the restuarant. As people
    arrive, they want a seat. As seats are filled, the semaphore is
    accessed, once per person. As people leave, the access is
    released, allowing more people to enter. If a party of 10 people
    want to be seated, but there are only 9 seats, those 10 people
    will wait, but a party of 4 people would be seated (taking the
    available seats to 5, making the party of 10 people wait longer).

    When a semaphore is created it is given a number which is the
    maximum number of concurrent accesses it will permit. Accesses to
    the sempahore are gained using operator++() or operator+=(), and
    released with operator--() or operator-=(). The number of
    accesses allowed is retrieved with available(), and the total
    number with total(). Note that the incrementing functions will
    block if there aren't enough available accesses. Use tryAccess()
    if you want to acquire accesses without blocking.
*/


class QSemaphorePrivate {
public:
    QSemaphorePrivate(int);

    QMutex mutex;
    QWaitCondition cond;

    int value, max;
};


QSemaphorePrivate::QSemaphorePrivate(int m)
    : mutex(FALSE), value(0), max(m)
{
}


/*!
    Creates a new semaphore. The semaphore can be concurrently
    accessed at most \a maxcount times.
*/
QSemaphore::QSemaphore(int maxcount)
{
    d = new QSemaphorePrivate(maxcount);
}


/*!
    Destroys the semaphore.

    \warning If you destroy a semaphore that has accesses in use the
    resultant behavior is undefined.
*/
QSemaphore::~QSemaphore()
{
    delete d;
}


/*!
    Postfix ++ operator.

    Try to get access to the semaphore. If \l available() == 0, this
    call will block until it can get access, i.e. until available() \>
    0.
*/
int QSemaphore::operator++(int)
{
    QMutexLocker locker(&d->mutex);
    while (d->value >= d->max)
	d->cond.wait(locker.mutex());

    ++d->value;
    if (d->value > d->max)
	d->value = d->max;

    return d->value;
}


/*!
    Postfix -- operator.

    Release access of the semaphore. This wakes all threads waiting
    for access to the semaphore.
*/
int QSemaphore::operator--(int)
{
    QMutexLocker locker(&d->mutex);

    --d->value;
    if (d->value < 0)
	d->value = 0;

    d->cond.wakeAll();

    return d->value;
}


/*!
    Try to get access to the semaphore. If \l available() \< \a n, this
    call will block until it can get all the accesses it wants, i.e.
    until available() \>= \a n.
*/
int QSemaphore::operator+=(int n)
{
    QMutexLocker locker(&d->mutex);

    if ( n < 0 || n > d->max ) {
#ifdef QT_CHECK_RANGE
	qWarning( "QSemaphore::operator+=: paramter %d out of range", n );
#endif // QT_CHECK_RANGE
	n = n < 0 ? 0 : d->max;
    }

    while (d->value + n > d->max)
	d->cond.wait(locker.mutex());

    d->value += n;

    return d->value;
}


/*!
    Release \a n accesses to the semaphore.
*/
int QSemaphore::operator-=(int n)
{
    QMutexLocker locker(&d->mutex);

    if ( n < 0 || n > d->value ) {
#ifdef QT_CHECK_RANGE
	qWarning( "QSemaphore::operator-=: paramter %d out of range", n );
#endif // QT_CHECK_RANGE
	n = n < 0 ? 0 : d->value;
    }

    d->value -= n;
    d->cond.wakeAll();

    return d->value;
}


/*!
    Returns the number of accesses currently available to the
    semaphore.
*/
int QSemaphore::available() const
{
    QMutexLocker locker(&d->mutex);
    return d->max - d->value;
}


/*!
    Returns the total number of accesses to the semaphore.
*/
int QSemaphore::total() const
{
    QMutexLocker locker(&d->mutex);
    return d->max;
}


/*!
    Try to get access to the semaphore. If \l available() \< \a n, this
    function will return FALSE immediately. If \l available() \>= \a n,
    this function will take \a n accesses and return TRUE. This
    function does \e not block.
*/
bool QSemaphore::tryAccess(int n)
{
    QMutexLocker locker(&d->mutex);

    if (d->value + n > d->max)
	return FALSE;

    d->value += n;

    return TRUE;
}

#endif // QT_THREAD_SUPPORT
