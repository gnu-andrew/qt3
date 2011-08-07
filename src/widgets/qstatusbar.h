/****************************************************************************
**
** Definition of QStatusBar class
**
** Created : 980316
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
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

#ifndef QSTATUSBAR_H
#define QSTATUSBAR_H

#ifndef QT_H
#include "qwidget.h"
#endif // QT_H

#ifndef QT_NO_STATUSBAR


class QStatusBarPrivate;


class Q_EXPORT QStatusBar: public QWidget
{
    Q_OBJECT
    Q_PROPERTY( bool sizeGripEnabled READ isSizeGripEnabled WRITE setSizeGripEnabled )

public:
    QStatusBar( QWidget* parent=0, const char* name=0 );
    virtual ~QStatusBar();

    virtual void addWidget( QWidget *, int stretch = 0, bool = FALSE );
    virtual void removeWidget( QWidget * );

    void setSizeGripEnabled(bool);
    bool isSizeGripEnabled() const;

public slots:
    void message( const QString &);
    void message( const QString &, int );
    void clear();

signals:
    void messageChanged( const QString &text );

protected:
    void paintEvent( QPaintEvent * );
    void resizeEvent( QResizeEvent * );

    void reformat();
    void hideOrShow();
    bool event( QEvent *);

private:
    QStatusBarPrivate * d;
private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QStatusBar( const QStatusBar & );
    QStatusBar& operator=( const QStatusBar & );
#endif
};

#endif // QT_NO_STATUSBAR

#endif // QSTATUSBAR_H
