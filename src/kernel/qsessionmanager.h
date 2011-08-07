/****************************************************************************
**
** Definition of QSessionManager class
**
** Created : 990510
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

#ifndef QSESSIONMANAGER_H
#define QSESSIONMANAGER_H

#ifndef QT_H
#include "qobject.h"
#include "qwindowdefs.h"
#include "qstring.h"
#include "qstringlist.h"
#endif // QT_H
#ifndef QT_NO_SESSIONMANAGER

class QSessionManagerData;

class Q_EXPORT  QSessionManager : public QObject
{
    Q_OBJECT
    QSessionManager( QApplication *app, QString &id, QString &key );
    ~QSessionManager();
public:
    QString sessionId() const;
    QString sessionKey() const;
#if defined(Q_WS_X11) || defined(Q_WS_MAC)
    void* handle() const;
#endif

    bool allowsInteraction();
    bool allowsErrorInteraction();
    void release();

    void cancel();

    enum RestartHint {
	RestartIfRunning,
	RestartAnyway,
	RestartImmediately,
	RestartNever
    };
    void setRestartHint( RestartHint );
    RestartHint restartHint() const;

    void setRestartCommand( const QStringList& );
    QStringList restartCommand() const;
    void setDiscardCommand( const QStringList& );
    QStringList discardCommand() const;

    void setManagerProperty( const QString& name, const QString& value );
    void setManagerProperty( const QString& name, const QStringList& value );

    bool isPhase2() const;
    void requestPhase2();

private:
    friend class QApplication;
    friend class QBaseApplication;
    QSessionManagerData* d;
};

#endif // QT_NO_SESSIONMANAGER
#endif // QSESSIONMANAGER_H
