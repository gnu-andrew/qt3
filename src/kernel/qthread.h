/****************************************************************************
**
** Definition of QThread class
**
** Created : 931107
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the kernel module of the Qt GUI Toolkit.
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

#ifndef QTHREAD_H
#define QTHREAD_H

#if defined(QT_THREAD_SUPPORT)

#ifndef QT_H
#include "qwindowdefs.h"
#ifndef QT_NO_COMPAT
#include "qmutex.h"
#include "qsemaphore.h"
#include "qwaitcondition.h"
#endif // QT_NO_COMPAT
#endif // QT_H

#include <limits.h>

class QThreadInstance;

class Q_EXPORT QThread : public Qt
{
public:
    static Qt::HANDLE currentThread();

#ifndef QT_NO_COMPAT
    static void postEvent( QObject *,QEvent * );
#endif

    static void initialize();
    static void cleanup();

    static void exit();

#ifdef Q_QDOC
    QThread( unsigned int stackSize = 0 );
#else
    QThread( unsigned int stackSize );
    QThread();
#endif

    virtual ~QThread();

    // default argument causes thread to block indefinately
    bool wait( unsigned long time = ULONG_MAX );

    enum Priority {
	IdlePriority,

	LowestPriority,
	LowPriority,
	NormalPriority,
	HighPriority,
	HighestPriority,

	TimeCriticalPriority,

	InheritPriority
    };

#ifdef Q_QDOC
    void start( Priority = InheritPriority );
#else
    void start( Priority );
    void start();
#endif

    void terminate();

    bool finished() const;
    bool running() const;

protected:
    virtual void run() = 0;

    static void sleep( unsigned long );
    static void msleep( unsigned long );
    static void usleep( unsigned long );

private:
    QThreadInstance * d;
    friend class QThreadInstance;

#if defined(Q_DISABLE_COPY)
    QThread( const QThread & );
    QThread &operator=( const QThread & );
#endif // Q_DISABLE_COPY
};

#endif // QT_THREAD_SUPPORT

#endif // QTHREAD_H
