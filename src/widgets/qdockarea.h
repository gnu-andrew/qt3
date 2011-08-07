/****************************************************************************
**
** Definition of the QDockArea class
**
** Created : 001010
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the workspace module of the Qt GUI Toolkit.
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

#ifndef QDOCKAREA_H
#define QDOCKAREA_H

#ifndef QT_H
#include "qwidget.h"
#include "qptrlist.h"
#include "qdockwindow.h"
#include "qlayout.h"
#include "qvaluelist.h"
#include "qguardedptr.h"
#include "qtextstream.h"
#endif // QT_H

#ifndef QT_NO_MAINWINDOW

class QSplitter;
class QBoxLayout;
class QDockAreaLayout;
class QMouseEvent;
class QDockWindowResizeHandle;
class QDockAreaPrivate;

class Q_EXPORT QDockAreaLayout : public QLayout
{
    Q_OBJECT
    friend class QDockArea;

public:
    QDockAreaLayout( QWidget* parent, Qt::Orientation o, QPtrList<QDockWindow> *wl, int space = -1, int margin = -1, const char *name = 0 )
	: QLayout( parent, space, margin, name ), orient( o ), dockWindows( wl ), parentWidget( parent ) { init(); }
    ~QDockAreaLayout() {}

    void addItem( QLayoutItem * ) {}
    bool hasHeightForWidth() const;
    int heightForWidth( int ) const;
    int widthForHeight( int ) const;
    QSize sizeHint() const;
    QSize minimumSize() const;
    QLayoutIterator iterator();
    QSizePolicy::ExpandData expanding() const { return QSizePolicy::NoDirection; }
    void invalidate();
    Qt::Orientation orientation() const { return orient; }
    QValueList<QRect> lineList() const { return lines; }
    QPtrList<QDockWindow> lineStarts() const { return ls; }

protected:
    void setGeometry( const QRect& );

private:
    void init();
    int layoutItems( const QRect&, bool testonly = FALSE );
    Qt::Orientation orient;
    bool dirty;
    int cached_width, cached_height;
    int cached_hfw, cached_wfh;
    QPtrList<QDockWindow> *dockWindows;
    QWidget *parentWidget;
    QValueList<QRect> lines;
    QPtrList<QDockWindow> ls;
#if defined(Q_DISABLE_COPY) // Disabled copy constructor and operator=
    QDockAreaLayout( const QDockAreaLayout & );
    QDockAreaLayout &operator=( const QDockAreaLayout & );
#endif
};

class Q_EXPORT QDockArea : public QWidget
{
    Q_OBJECT
    Q_ENUMS( HandlePosition )
    Q_PROPERTY( Orientation orientation READ orientation )
    Q_PROPERTY( int count READ count )
    Q_PROPERTY( bool empty READ isEmpty )
    Q_PROPERTY( HandlePosition handlePosition READ handlePosition )

    friend class QDockWindow;
    friend class QDockWindowResizeHandle;
    friend class QDockAreaLayout;

public:
    enum HandlePosition { Normal, Reverse };

    QDockArea( Orientation o, HandlePosition h = Normal, QWidget* parent=0, const char* name=0 );
    ~QDockArea();

    void moveDockWindow( QDockWindow *w, const QPoint &globalPos, const QRect &rect, bool swap );
    void removeDockWindow( QDockWindow *w, bool makeFloating, bool swap, bool fixNewLines = TRUE );
    void moveDockWindow( QDockWindow *w, int index = -1 );
    bool hasDockWindow( QDockWindow *w, int *index = 0 );

    void invalidNextOffset( QDockWindow *dw );

    Orientation orientation() const { return orient; }
    HandlePosition handlePosition() const { return hPos; }

    bool eventFilter( QObject *, QEvent * );
    bool isEmpty() const;
    int count() const;
    QPtrList<QDockWindow> dockWindowList() const;

    bool isDockWindowAccepted( QDockWindow *dw );
    void setAcceptDockWindow( QDockWindow *dw, bool accept );

public slots:
    void lineUp( bool keepNewLines );

private:
    struct DockWindowData
    {
	int index;
	int offset;
	int line;
	QSize fixedExtent;
	QGuardedPtr<QDockArea> area;
    };

    int findDockWindow( QDockWindow *w );
    int lineOf( int index );
    DockWindowData *dockWindowData( QDockWindow *w );
    void dockWindow( QDockWindow *dockWindow, DockWindowData *data );
    void updateLayout();
    void invalidateFixedSizes();
    int maxSpace( int hint, QDockWindow *dw );
    void setFixedExtent( int d, QDockWindow *dw );
    bool isLastDockWindow( QDockWindow *dw );

private:
    Orientation orient;
    QPtrList<QDockWindow> *dockWindows;
    QDockAreaLayout *layout;
    HandlePosition hPos;
    QPtrList<QDockWindow> forbiddenWidgets;
    QDockAreaPrivate *d;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QDockArea( const QDockArea & );
    QDockArea& operator=( const QDockArea & );
#endif

};

#ifndef QT_NO_TEXTSTREAM
Q_EXPORT QTextStream &operator<<( QTextStream &, const QDockArea & );
Q_EXPORT QTextStream &operator>>( QTextStream &, QDockArea & );
#endif

#define Q_DEFINED_QDOCKAREA
#include "qwinexport.h"
#endif

#endif //QT_NO_MAINWINDOW
