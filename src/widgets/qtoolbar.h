/****************************************************************************
**
** Definition of QToolBar class
**
** Created : 980306
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

#ifndef QTOOLBAR_H
#define QTOOLBAR_H

#ifndef QT_H
#include "qdockwindow.h"
#endif // QT_H

#ifndef QT_NO_TOOLBAR

class QMainWindow;
class QButton;
class QBoxLayout;
class QToolBarPrivate;

class Q_EXPORT QToolBar: public QDockWindow
{
    Q_OBJECT
    Q_PROPERTY( QString label READ label WRITE setLabel )

public:
    QToolBar( const QString &label,
	      QMainWindow *, ToolBarDock = DockTop,
	      bool newLine = FALSE, const char* name=0 );
    QToolBar( const QString &label, QMainWindow *, QWidget *,
	      bool newLine = FALSE, const char* name=0, WFlags f = 0 );
    QToolBar( QMainWindow* parent=0, const char* name=0 );
    ~QToolBar();

    void addSeparator();

    void show();
    void hide();

    QMainWindow * mainWindow() const;

    virtual void setStretchableWidget( QWidget * );

    bool event( QEvent * e );

    virtual void setLabel( const QString & );
    QString label() const;

    virtual void clear();

    QSize minimumSize() const;
    QSize minimumSizeHint() const;

    void setOrientation( Orientation o );
    void setMinimumSize( int minw, int minh );

protected:
    void resizeEvent( QResizeEvent *e );
    void styleChange( QStyle & );

private slots:
    void createPopup();

private:
    void init();
    void checkForExtension( const QSize &sz );
    QToolBarPrivate * d;
    QMainWindow * mw;
    QWidget * sw;
    QString l;

    friend class QMainWindow;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QToolBar( const QToolBar & );
    QToolBar& operator=( const QToolBar & );
#endif
};

#endif // QT_NO_TOOLBAR

#endif // QTOOLBAR_H
