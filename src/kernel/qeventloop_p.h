/****************************************************************************
**
** Definition of QEventLoop class
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
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with
** the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#ifndef QEVENTLOOP_P_H
#define QEVENTLOOP_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//
//

#ifndef QT_H
#include "qplatformdefs.h"
#endif // QT_H

// SCO OpenServer redefines raise -> kill
#if defined(raise)
# undef raise
#endif

#include "qwindowdefs.h"

class QSocketNotifier;
#ifdef Q_OS_MAC
class QMacSockNotPrivate;
#endif

#if defined(Q_OS_UNIX) || defined (Q_WS_WIN)
#include "qptrlist.h"
#endif // Q_OS_UNIX || Q_WS_WIN

#if defined(Q_OS_UNIX)
struct QSockNot
{
    QSocketNotifier *obj;
    int fd;
    fd_set *queue;
};

class QSockNotType
{
public:
    QSockNotType();
    ~QSockNotType();

    QPtrList<QSockNot> *list;
    fd_set select_fds;
    fd_set enabled_fds;
    fd_set pending_fds;

};
#endif // Q_OS_UNIX

#if defined(Q_WS_WIN)
struct QSockNot {
    QSocketNotifier *obj;
    int fd;
};
#endif // Q_WS_WIN

class QEventLoopPrivate
{
public:
    QEventLoopPrivate()
    {
	reset();
    }

    void reset() {
	looplevel = 0;
	quitcode = 0;
	quitnow = FALSE;
	exitloop = FALSE;
	shortcut = FALSE;
    }

    int looplevel;
    int quitcode;
    unsigned int quitnow  : 1;
    unsigned int exitloop : 1;
    unsigned int shortcut : 1;

#if defined(Q_WS_MAC)
    uchar        next_select_timer;
    EventLoopTimerRef select_timer;
#endif

#if defined(Q_WS_X11)
    int xfd;
#endif // Q_WS_X11

#if defined(Q_OS_UNIX)
    int thread_pipe[2];

    // pending socket notifiers list
    QPtrList<QSockNot> sn_pending_list;
    // highest fd for all socket notifiers
    int sn_highest;
    // 3 socket notifier types - read, write and exception
    QSockNotType sn_vec[3];
#endif

#ifdef Q_WS_WIN
    // pending socket notifiers list
    QPtrList<QSockNot> sn_pending_list;
#endif // Q_WS_WIN

};

#endif // QEVENTLOOP_P_H
