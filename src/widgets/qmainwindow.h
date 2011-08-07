/****************************************************************************
**
** Definition of QMainWindow class
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

#ifndef QMAINWINDOW_H
#define QMAINWINDOW_H

#ifndef QT_H
#include "qwidget.h"
#include "qtoolbar.h"
#include "qptrlist.h"
#include "qtextstream.h"
#endif // QT_H

#ifndef QT_NO_MAINWINDOW

class QMenuBar;
class QStatusBar;
class QToolTipGroup;
class QMainWindowPrivate;
class QMainWindowLayout;
class QPopupMenu;

class Q_EXPORT QMainWindow: public QWidget
{
    Q_OBJECT
    Q_PROPERTY( bool rightJustification READ rightJustification WRITE setRightJustification DESIGNABLE false )
    Q_PROPERTY( bool usesBigPixmaps READ usesBigPixmaps WRITE setUsesBigPixmaps )
    Q_PROPERTY( bool usesTextLabel READ usesTextLabel WRITE setUsesTextLabel )
    Q_PROPERTY( bool dockWindowsMovable READ dockWindowsMovable WRITE setDockWindowsMovable )
    Q_PROPERTY( bool opaqueMoving READ opaqueMoving WRITE setOpaqueMoving )

public:
    QMainWindow( QWidget* parent=0, const char* name=0, WFlags f = WType_TopLevel );
    ~QMainWindow();

#ifndef QT_NO_MENUBAR
    QMenuBar * menuBar() const;
#endif
    QStatusBar * statusBar() const;
#ifndef QT_NO_TOOLTIP
    QToolTipGroup * toolTipGroup() const;
#endif

    virtual void setCentralWidget( QWidget * );
    QWidget * centralWidget() const;

    virtual void setDockEnabled( Dock dock, bool enable );
    bool isDockEnabled( Dock dock ) const;
    bool isDockEnabled( QDockArea *area ) const;
    virtual void setDockEnabled( QDockWindow *tb, Dock dock, bool enable );
    bool isDockEnabled( QDockWindow *tb, Dock dock ) const;
    bool isDockEnabled( QDockWindow *tb, QDockArea *area ) const;

    virtual void addDockWindow( QDockWindow *, Dock = DockTop, bool newLine = FALSE );
    virtual void addDockWindow( QDockWindow *, const QString &label,
				Dock = DockTop, bool newLine = FALSE );
    virtual void moveDockWindow( QDockWindow *, Dock = DockTop );
    virtual void moveDockWindow( QDockWindow *, Dock, bool nl, int index, int extraOffset = -1 );
    virtual void removeDockWindow( QDockWindow * );

    void show();
    void hide();
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    bool rightJustification() const;
    bool usesBigPixmaps() const;
    bool usesTextLabel() const;
    bool dockWindowsMovable() const;
    bool opaqueMoving() const;

    bool eventFilter( QObject*, QEvent* );

    bool getLocation( QDockWindow *tb, Dock &dock, int &index, bool &nl, int &extraOffset ) const;

    QPtrList<QDockWindow> dockWindows( Dock dock ) const;
    QPtrList<QDockWindow> dockWindows() const;
    void lineUpDockWindows( bool keepNewLines = FALSE );

    bool isDockMenuEnabled() const;

    // compatibility stuff
    bool hasDockWindow( QDockWindow *dw );
#ifndef QT_NO_TOOLBAR
    void addToolBar( QDockWindow *, Dock = DockTop, bool newLine = FALSE );
    void addToolBar( QDockWindow *, const QString &label,
		     Dock = DockTop, bool newLine = FALSE );
    void moveToolBar( QDockWindow *, Dock = DockTop );
    void moveToolBar( QDockWindow *, Dock, bool nl, int index, int extraOffset = -1 );
    void removeToolBar( QDockWindow * );

    bool toolBarsMovable() const;
    QPtrList<QToolBar> toolBars( Dock dock ) const;
    void lineUpToolBars( bool keepNewLines = FALSE );
#endif

    virtual QDockArea *dockingArea( const QPoint &p );
    QDockArea *leftDock() const;
    QDockArea *rightDock() const;
    QDockArea *topDock() const;
    QDockArea *bottomDock() const;

    virtual bool isCustomizable() const;

    bool appropriate( QDockWindow *dw ) const;

    enum DockWindows { OnlyToolBars, NoToolBars, AllDockWindows };
    QPopupMenu *createDockWindowMenu( DockWindows dockWindows = AllDockWindows ) const;

public slots:
    virtual void setRightJustification( bool );
    virtual void setUsesBigPixmaps( bool );
    virtual void setUsesTextLabel( bool );
    virtual void setDockWindowsMovable( bool );
    virtual void setOpaqueMoving( bool );
    virtual void setDockMenuEnabled( bool );
    virtual void whatsThis();
    virtual void setAppropriate( QDockWindow *dw, bool a );
    virtual void customize();

    // compatibility stuff
    void setToolBarsMovable( bool );

signals:
    void pixmapSizeChanged( bool );
    void usesTextLabelChanged( bool );
    void dockWindowPositionChanged( QDockWindow * );

#ifndef QT_NO_TOOLBAR
    // compatibility stuff
    void toolBarPositionChanged( QToolBar * );
#endif

protected slots:
    virtual void setUpLayout();
    virtual bool showDockMenu( const QPoint &globalPos );
    void menuAboutToShow();

protected:
    void paintEvent( QPaintEvent * );
    void childEvent( QChildEvent * );
    bool event( QEvent * );
    void styleChange( QStyle& );

private slots:
    void slotPlaceChanged();
    void doLineUp() { lineUpDockWindows( TRUE ); }

private:
    QMainWindowPrivate * d;
    void triggerLayout( bool deleteLayout = TRUE);
    bool dockMainWindow( QObject *dock );

#ifndef QT_NO_MENUBAR
    virtual void setMenuBar( QMenuBar * );
#endif
    virtual void setStatusBar( QStatusBar * );
#ifndef QT_NO_TOOLTIP
    virtual void setToolTipGroup( QToolTipGroup * );
#endif

    friend class QDockWindow;
    friend class QMenuBar;
    friend class QHideDock;
    friend class QToolBar;
    friend class QMainWindowLayout;
private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QMainWindow( const QMainWindow & );
    QMainWindow& operator=( const QMainWindow & );
#endif
};

#ifndef QT_NO_TOOLBAR
inline void QMainWindow::addToolBar( QDockWindow *w, ToolBarDock dock, bool newLine )
{
    addDockWindow( w, dock, newLine );
}

inline void QMainWindow::addToolBar( QDockWindow *w, const QString &label,
			      ToolBarDock dock, bool newLine )
{
    addDockWindow( w, label, dock, newLine );
}

inline void QMainWindow::moveToolBar( QDockWindow *w, ToolBarDock dock )
{
    moveDockWindow( w, dock );
}

inline void QMainWindow::moveToolBar( QDockWindow *w, ToolBarDock dock, bool nl, int index, int extraOffset )
{
    moveDockWindow( w, dock, nl, index, extraOffset );
}

inline void QMainWindow::removeToolBar( QDockWindow *w )
{
    removeDockWindow( w );
}

inline bool QMainWindow::toolBarsMovable() const
{
    return dockWindowsMovable();
}

inline void QMainWindow::lineUpToolBars( bool keepNewLines )
{
    lineUpDockWindows( keepNewLines );
}

inline void QMainWindow::setToolBarsMovable( bool b )
{
    setDockWindowsMovable( b );
}
#endif

#ifndef QT_NO_TEXTSTREAM
Q_EXPORT QTextStream &operator<<( QTextStream &, const QMainWindow & );
Q_EXPORT QTextStream &operator>>( QTextStream &, QMainWindow & );
#endif

#endif // QT_NO_MAINWINDOW

#endif // QMAINWINDOW_H
