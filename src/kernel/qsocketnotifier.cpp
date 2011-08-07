/****************************************************************************
**
** Implementation of QSocketNotifier class
**
** Created : 951114
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

#include "qsocketnotifier.h"
#include "qapplication.h"
#include "qevent.h"
#include "qeventloop.h"
#include "qplatformdefs.h"

#if defined(Q_OS_UNIX)
#include <sys/types.h>
#endif


/*!
    \class QSocketNotifier qsocketnotifier.h
    \brief The QSocketNotifier class provides support for socket callbacks.

    \ingroup io

    This class makes it possible to write asynchronous socket-based
    code in Qt. Using synchronous socket operations blocks the
    program, which is clearly not acceptable for an event-driven GUI
    program.

    Once you have opened a non-blocking socket (whether for TCP, UDP,
    a UNIX-domain socket, or any other protocol family your operating
    system supports), you can create a socket notifier to monitor the
    socket. Then you connect the activated() signal to the slot you
    want to be called when a socket event occurs.

    Note for Windows users: the socket passed to QSocketNotifier will
    become non-blocking, even if it was created as a blocking socket.

    There are three types of socket notifiers (read, write and
    exception); you must specify one of these in the constructor.

    The type specifies when the activated() signal is to be emitted:
    \list 1
    \i QSocketNotifier::Read - There is data to be read (socket read event).
    \i QSocketNotifier::Write - Data can be written (socket write event).
    \i QSocketNofifier::Exception - An exception has occurred (socket
    exception event). We recommend against using this.
    \endlist

    For example, if you need to monitor both reads and writes for the
    same socket you must create two socket notifiers.

    For read notifiers it makes little sense to connect the
    activated() signal to more than one slot because the data can be
    read from the socket only once.

    Also observe that if you do not read all the available data when
    the read notifier fires, it fires again and again.

    For write notifiers, immediately disable the notifier after the
    activated() signal has been received and you have sent the data to
    be written on the socket. When you have more data to be written,
    enable it again to get a new activated() signal. The exception is
    if the socket data writing operation (send() or equivalent) fails
    with a "would block" error, which means that some buffer is full
    and you must wait before sending more data. In that case you do
    not need to disable and re-enable the write notifier; it will fire
    again as soon as the system allows more data to be sent.

    The behavior of a write notifier that is left in enabled state
    after having emitting the first activated() signal (and no "would
    block" error has occurred) is undefined. Depending on the
    operating system, it may fire on every pass of the event loop or
    not at all.

    If you need a time-out for your sockets you can use either \link
    QObject::startTimer() timer events\endlink or the QTimer class.

    Socket action is detected in the \link QApplication::exec() main
    event loop\endlink of Qt. The X11 version of Qt has a single UNIX
    select() call that incorporates all socket notifiers and the X
    socket.

    Note that on XFree86 for OS/2, select() works only in the thread
    in which main() is running; you should therefore use that thread
    for GUI operations.

    \sa QSocket, QServerSocket, QSocketDevice, QFile::handle()
*/

/*!
    \enum QSocketNotifier::Type

    \value Read
    \value Write
    \value Exception
*/


/*!
    Constructs a socket notifier called \a name, with the parent, \a
    parent. It watches \a socket for \a type events, and enables it.

    It is generally advisable to explicitly enable or disable the
    socket notifier, especially for write notifiers.

    \sa setEnabled(), isEnabled()
*/

QSocketNotifier::QSocketNotifier( int socket, Type type, QObject *parent,
				  const char *name )
    : QObject( parent, name )
{
#if defined(QT_CHECK_RANGE)
    if ( socket < 0 )
	qWarning( "QSocketNotifier: Invalid socket specified" );
#  if defined(Q_OS_UNIX)
    if ( socket >= FD_SETSIZE )
	qWarning( "QSocketNotifier: Socket descriptor too large for select()" );
#  endif
#endif
    sockfd = socket;
    sntype = type;
    snenabled = TRUE;
    QApplication::eventLoop()->registerSocketNotifier( this );
}

/*!
    Destroys the socket notifier.
*/

QSocketNotifier::~QSocketNotifier()
{
    setEnabled( FALSE );
}


/*!
    \fn void QSocketNotifier::activated( int socket )

    This signal is emitted under certain conditions specified by the
    notifier type():
    \list 1
    \i QSocketNotifier::Read - There is data to be read (socket read event).
    \i QSocketNotifier::Write - Data can be written (socket write event).
    \i QSocketNofifier::Exception - An exception has occurred (socket
    exception event).
    \endlist

    The \a socket argument is the \link socket() socket\endlink identifier.

    \sa type(), socket()
*/


/*!
    \fn int QSocketNotifier::socket() const

    Returns the socket identifier specified to the constructor.

    \sa type()
*/

/*!
    \fn Type QSocketNotifier::type() const

    Returns the socket event type specified to the constructor: \c
    QSocketNotifier::Read, \c QSocketNotifier::Write, or \c
    QSocketNotifier::Exception.

    \sa socket()
*/


/*!
    \fn bool QSocketNotifier::isEnabled() const

    Returns TRUE if the notifier is enabled; otherwise returns FALSE.

    \sa setEnabled()
*/

/*!
    Enables the notifier if \a enable is TRUE or disables it if \a
    enable is FALSE.

    The notifier is enabled by default.

    If the notifier is enabled, it emits the activated() signal
    whenever a socket event corresponding to its \link type()
    type\endlink occurs. If it is disabled, it ignores socket events
    (the same effect as not creating the socket notifier).

    Write notifiers should normally be disabled immediately after the
    activated() signal has been emitted; see discussion of write
    notifiers in the \link #details class description\endlink above.

    \sa isEnabled(), activated()
*/

void QSocketNotifier::setEnabled( bool enable )
{
    if ( sockfd < 0 )
	return;
    if ( snenabled == enable )			// no change
	return;
    snenabled = enable;

    QEventLoop *eventloop = QApplication::eventLoop();
    if ( ! eventloop ) // perhaps application is shutting down
	return;

    if ( snenabled )
	eventloop->registerSocketNotifier( this );
    else
	eventloop->unregisterSocketNotifier( this );
}


/*!\reimp
*/
bool QSocketNotifier::event( QEvent *e )
{
    // Emits the activated() signal when a \c QEvent::SockAct is
    // received.
    QObject::event( e );			// will activate filters
    if ( e->type() == QEvent::SockAct ) {
	emit activated( sockfd );
	return TRUE;
    }
    return FALSE;
}
