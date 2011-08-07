/****************************************************************************
**
** Implementation of QProcess class
**
** Created : 20000905
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

#ifndef QPROCESS_H
#define QPROCESS_H

#ifndef QT_H
#include "qobject.h"
#include "qstringlist.h"
#include "qdir.h"
#endif // QT_H

#ifndef QT_NO_PROCESS

class QProcessPrivate;
class QMembuf;


class Q_EXPORT QProcess : public QObject
{
    Q_OBJECT
public:
    QProcess( QObject *parent=0, const char *name=0 );
    QProcess( const QString& arg0, QObject *parent=0, const char *name=0 );
    QProcess( const QStringList& args, QObject *parent=0, const char *name=0 );
    ~QProcess();

    // set and get the arguments and working directory
    QStringList arguments() const;
    void clearArguments();
    virtual void setArguments( const QStringList& args );
    virtual void addArgument( const QString& arg );
#ifndef QT_NO_DIR
    QDir workingDirectory() const;
    virtual void setWorkingDirectory( const QDir& dir );
#endif

    // set and get the comms wanted
    enum Communication { Stdin=0x01, Stdout=0x02, Stderr=0x04, DupStderr=0x08 };
    void setCommunication( int c );
    int communication() const;

    // start the execution
    virtual bool start( QStringList *env=0 );
    virtual bool launch( const QString& buf, QStringList *env=0  );
    virtual bool launch( const QByteArray& buf, QStringList *env=0  );

    // inquire the status
    bool isRunning() const;
    bool normalExit() const;
    int exitStatus() const;

    // reading
    virtual QByteArray readStdout();
    virtual QByteArray readStderr();
    bool canReadLineStdout() const;
    bool canReadLineStderr() const;
    virtual QString readLineStdout();
    virtual QString readLineStderr();

    // get platform dependent process information
#if defined(Q_OS_WIN32)
    typedef void* PID;
#else
    typedef Q_LONG PID;
#endif
    PID processIdentifier();

    void flushStdin();

signals:
    void readyReadStdout();
    void readyReadStderr();
    void processExited();
    void wroteToStdin();
    void launchFinished();

public slots:
    // end the execution
    void tryTerminate() const;
    void kill() const;

    // input
    virtual void writeToStdin( const QByteArray& buf );
    virtual void writeToStdin( const QString& buf );
    virtual void closeStdin();

protected: // ### or private?
    void connectNotify( const char * signal );
    void disconnectNotify( const char * signal );
private:
    void setIoRedirection( bool value );
    void setNotifyOnExit( bool value );
    void setWroteStdinConnected( bool value );

    void init();
    void reset();
#if defined(Q_OS_WIN32)
    uint readStddev( HANDLE dev, char *buf, uint bytes );
#endif
    QMembuf* membufStdout();
    QMembuf* membufStderr();

private slots:
    void socketRead( int fd );
    void socketWrite( int fd );
    void timeout();
    void closeStdinLaunch();

private:
    QProcessPrivate *d;
#ifndef QT_NO_DIR
    QDir        workingDir;
#endif
    QStringList _arguments;

    int  exitStat; // exit status
    bool exitNormal; // normal exit?
    bool ioRedirection; // automatically set be (dis)connectNotify
    bool notifyOnExit; // automatically set be (dis)connectNotify
    bool wroteToStdinConnected; // automatically set be (dis)connectNotify

    bool readStdoutCalled;
    bool readStderrCalled;
    int comms;

    friend class QProcessPrivate;
#if defined(Q_OS_UNIX)
    friend class QProcessManager;
    friend class QProc;
#endif

#if defined(Q_DISABLE_COPY) // Disabled copy constructor and operator=
    QProcess( const QProcess & );
    QProcess &operator=( const QProcess & );
#endif
};

#endif // QT_NO_PROCESS

#endif // QPROCESS_H
