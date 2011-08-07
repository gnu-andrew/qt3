/****************************************************************************
**
** Definition of QDesktopWidget class.
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

#ifndef QDESKTOPWIDGET_H
#define QDESKTOPWIDGET_H

#ifndef QT_H
#include "qwidget.h"
#endif // QT_H

class QApplication;
class QDesktopWidgetPrivate; /* Don't touch! */

class Q_EXPORT QDesktopWidget : public QWidget
{
    Q_OBJECT
public:
    QDesktopWidget();
    ~QDesktopWidget();

    bool isVirtualDesktop() const;

    int numScreens() const;
    int primaryScreen() const;

    int screenNumber( QWidget *widget = 0 ) const; // ### 4.0: const QWidget*
    int screenNumber( const QPoint & ) const;

    QWidget *screen( int screen = -1 );

    const QRect& screenGeometry( int screen = -1 ) const;
    const QRect& screenGeometry( QWidget *widget ) const
    { return screenGeometry( screenNumber( widget ) ); }
    const QRect& screenGeometry( const QPoint &point ) const
    { return screenGeometry( screenNumber( point ) ); }

    const QRect& availableGeometry( int screen = -1 ) const;
    const QRect& availableGeometry( QWidget *widget ) const
    { return availableGeometry( screenNumber( widget ) ); }
    const QRect& availableGeometry( const QPoint &point ) const
    { return availableGeometry( screenNumber( point ) ); }

    void insertChild( QObject * );

signals:
    void resized( int );
    void workAreaResized( int );

protected:
    void resizeEvent( QResizeEvent *e );

private:
    QDesktopWidgetPrivate *d;

#if defined(Q_DISABLE_COPY) // Disabled copy constructor and operator=
    QDesktopWidget( const QDesktopWidget & );
    QDesktopWidget &operator=( const QDesktopWidget & );
#endif

    friend class QApplication;
#ifdef Q_WS_QWS
    friend class QWSDisplay;
#endif
};

#endif //QDESKTOPWIDGET_H
