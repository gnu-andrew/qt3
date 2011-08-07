/****************************************************************************
**
** Definition of QSound class and QAuServer internal class
**
** Created : 000117
**
** Copyright (C) 1999-2008 Trolltech ASA.  All rights reserved.
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
#ifndef QSOUND_H
#define QSOUND_H

#ifndef QT_H
#include "qobject.h"
#endif // QT_H

#ifndef QT_NO_SOUND

class QSoundData;

class Q_EXPORT QSound : public QObject {
    Q_OBJECT
public:
    static bool isAvailable();
    static void play(const QString& filename);

    QSound(const QString& filename, QObject* parent=0, const char* name=0);
    ~QSound();

    /* Coming soon...
	?
    QSound(int hertz, Type type=Mono);
    int play(const ushort* data, int samples);
    bool full();
    signal void notFull();
	?
    */

#ifndef QT_NO_COMPAT
    static bool available() { return isAvailable(); }
#endif

    int loops() const;
    int loopsRemaining() const;
    void setLoops(int);
    QString fileName() const;

    bool isFinished() const;

public slots:
    void play();
    void stop();

private:
    QSoundData* d;
    friend class QAuServer;
};


/*
  QAuServer is an INTERNAL class.  If you wish to provide support for
  additional audio servers, you can make a subclass of QAuServer to do
  so, HOWEVER, your class may need to be re-engineered to some degree
  with each new Qt release, including minor releases.

  QAuBucket is whatever you want.
*/

class QAuBucket {
public:
    virtual ~QAuBucket();
};

class QAuServer : public QObject {
    Q_OBJECT

public:
    QAuServer(QObject* parent, const char* name);
    ~QAuServer();

    virtual void init(QSound*);
    virtual void play(const QString& filename);
    virtual void play(QSound*)=0;
    virtual void stop(QSound*)=0;
    virtual bool okay()=0;

protected:
    void setBucket(QSound*, QAuBucket*);
    QAuBucket* bucket(QSound*);
    int decLoop(QSound*);
};

#endif // QT_NO_SOUND

#endif
