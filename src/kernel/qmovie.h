/****************************************************************************
**
** Definition of movie classes
**
** Created : 970617
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

#ifndef QMOVIE_H
#define QMOVIE_H

#ifndef QT_H
#include "qpixmap.h" // ### remove or keep for users' convenience?
#endif // QT_H

#ifndef QT_NO_MOVIE

class QDataSource;
class QObject;
class QMoviePrivate;

class Q_EXPORT QMovie {
public:
    QMovie();
    QMovie(int bufsize);
    QMovie(QDataSource*, int bufsize=1024);
    QMovie(const QString &fileName, int bufsize=1024);
    QMovie(QByteArray data, int bufsize=1024);
    QMovie(const QMovie&);
    ~QMovie();

    QMovie& operator=(const QMovie&);

    int pushSpace() const;
    void pushData(const uchar* data, int length);

    const QColor& backgroundColor() const;
    void setBackgroundColor(const QColor&);

    const QRect& getValidRect() const;
    const QPixmap& framePixmap() const;
    const QImage& frameImage() const;

    bool isNull() const;

    int  frameNumber() const;
    int  steps() const;
    bool paused() const;
    bool finished() const;
    bool running() const;

    void unpause();
    void pause();
    void step();
    void step(int);
    void restart();

    int  speed() const;
    void setSpeed(int);

    void connectResize(QObject* receiver, const char *member);
    void disconnectResize(QObject* receiver, const char *member=0);

    void connectUpdate(QObject* receiver, const char *member);
    void disconnectUpdate(QObject* receiver, const char *member=0);

#ifdef Q_WS_QWS
    // Temporary hack
    void setDisplayWidget(QWidget * w);
#endif

    enum Status { SourceEmpty=-2,
		  UnrecognizedFormat=-1,
		  Paused=1,
		  EndOfFrame=2,
		  EndOfLoop=3,
		  EndOfMovie=4,
		  SpeedChanged=5 };
    void connectStatus(QObject* receiver, const char *member);
    void disconnectStatus(QObject* receiver, const char *member=0);

private:
    QMoviePrivate *d;
};

#endif	// QT_NO_MOVIE

#endif
