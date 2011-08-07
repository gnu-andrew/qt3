/****************************************************************************
**
** Definition of QMutex class
**
** Created : 931107
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

#ifndef QMUTEX_H
#define QMUTEX_H

#ifndef QT_H
#include "qglobal.h"
#endif // QT_H

#if defined(QT_THREAD_SUPPORT)

class QMutexPrivate;

const int Q_MUTEX_NORMAL = 0;
const int Q_MUTEX_RECURSIVE = 1;

class Q_EXPORT QMutex
{
    friend class QThread;
    friend class QWaitCondition;
    friend class QWaitConditionPrivate;

public:
    QMutex(bool recursive = FALSE);
    virtual ~QMutex();

    void lock();
    void unlock();
    bool locked();
    bool tryLock();

private:
    QMutexPrivate * d;

#if defined(Q_DISABLE_COPY)
    QMutex( const QMutex & );
    QMutex &operator=( const QMutex & );
#endif
};

class Q_EXPORT QMutexLocker
{
public:
    QMutexLocker( QMutex * );
    ~QMutexLocker();

    QMutex *mutex() const;

private:
    QMutex *mtx;

#if defined(Q_DISABLE_COPY)
    QMutexLocker( const QMutexLocker & );
    QMutexLocker &operator=( const QMutexLocker & );
#endif
};

inline QMutexLocker::QMutexLocker( QMutex *m )
    : mtx( m )
{
    if ( mtx ) mtx->lock();
}

inline QMutexLocker::~QMutexLocker()
{
    if ( mtx ) mtx->unlock();
}

inline QMutex *QMutexLocker::mutex() const
{
    return mtx;
}

#endif

#endif
