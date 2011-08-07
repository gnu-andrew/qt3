/****************************************************************************
**
** Definition of QMenuBar class
**
** Created : 941209
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

#ifndef QMENUBAR_H
#define QMENUBAR_H

#ifndef QT_H
#include "qpopupmenu.h" // ### remove or keep for users' convenience?
#include "qframe.h"
#include "qmenudata.h"
#endif // QT_H

#ifndef QT_NO_MENUBAR

class QPopupMenu;

class Q_EXPORT QMenuBar : public QFrame, public QMenuData
{
    Q_OBJECT
    Q_ENUMS( Separator )
    Q_PROPERTY( Separator separator READ separator WRITE setSeparator DESIGNABLE false )
    Q_PROPERTY( bool defaultUp READ isDefaultUp WRITE setDefaultUp )

public:
    QMenuBar( QWidget* parent=0, const char* name=0 );
    ~QMenuBar();

    void	updateItem( int id );

    void	show();				// reimplemented show
    void	hide();				// reimplemented hide

    bool	eventFilter( QObject *, QEvent * );

    int		heightForWidth(int) const;

    enum	Separator { Never=0, InWindowsStyle=1 };
    Separator 	separator() const;
    virtual void	setSeparator( Separator when );

    void	setDefaultUp( bool );
    bool	isDefaultUp() const;

    bool customWhatsThis() const;

    QSize sizeHint() const;
    QSize minimumSize() const;
    QSize minimumSizeHint() const;

    void activateItemAt( int index );

#if defined(Q_WS_MAC) && !defined(QMAC_QMENUBAR_NO_NATIVE)
    static void initialize();
    static void cleanup();
#endif

signals:
    void	activated( int itemId );
    void	highlighted( int itemId );

protected:
    void	drawContents( QPainter * );
    void	fontChange( const QFont & );
    void	mousePressEvent( QMouseEvent * );
    void	mouseReleaseEvent( QMouseEvent * );
    void	mouseMoveEvent( QMouseEvent * );
    void	keyPressEvent( QKeyEvent * );
    void	focusInEvent( QFocusEvent * );
    void	focusOutEvent( QFocusEvent * );
    void	resizeEvent( QResizeEvent * );
    void	leaveEvent( QEvent * );
    void	menuContentsChanged();
    void	menuStateChanged();
    void 	styleChange( QStyle& );
    int	itemAtPos( const QPoint & );
    void	hidePopups();
    QRect	itemRect( int item );

private slots:
    void	subActivated( int itemId );
    void	subHighlighted( int itemId );
#ifndef QT_NO_ACCEL
    void	accelActivated( int itemId );
    void	accelDestroyed();
#endif
    void	popupDestroyed( QObject* );
    void 	performDelayedChanges();

    void	languageChange();

private:
    void 	performDelayedContentsChanged();
    void 	performDelayedStateChanged();
    void	menuInsPopup( QPopupMenu * );
    void	menuDelPopup( QPopupMenu * );
    void	frameChanged();

    bool	tryMouseEvent( QPopupMenu *, QMouseEvent * );
    void	tryKeyEvent( QPopupMenu *, QKeyEvent * );
    void	goodbye( bool cancelled = FALSE );
    void	openActPopup();

    void setActiveItem( int index, bool show = TRUE, bool activate_first_item = TRUE );
    void setAltMode( bool );

    int		calculateRects( int max_width = -1 );

#ifndef QT_NO_ACCEL
    void	setupAccelerators();
    QAccel     *autoaccel;
#endif
    QRect      *irects;
    int		rightSide;

    uint	mseparator : 1;
    uint	waitforalt : 1;
    uint	popupvisible  : 1;
    uint	hasmouse : 1;
    uint 	defaultup : 1;
    uint 	toggleclose : 1;
    uint        pendingDelayedContentsChanges : 1;
    uint        pendingDelayedStateChanges : 1;

    friend class QPopupMenu;

#if defined(Q_WS_MAC) && !defined(QMAC_QMENUBAR_NO_NATIVE)
    friend class QWidget;
    friend class QApplication;
    friend void qt_mac_set_modal_state(bool, QMenuBar *);

    void macCreateNativeMenubar();
    void macRemoveNativeMenubar();
    void macDirtyNativeMenubar();

#if !defined(QMAC_QMENUBAR_NO_EVENT)
    static void qt_mac_install_menubar_event(MenuRef);
    static OSStatus qt_mac_menubar_event(EventHandlerCallRef, EventRef, void *);
#endif
    virtual void macWidgetChangedWindow();
    bool syncPopups(MenuRef ret, QPopupMenu *d);
    MenuRef createMacPopup(QPopupMenu *d, int id, bool =FALSE);
    bool updateMenuBar();
#if !defined(QMAC_QMENUBAR_NO_MERGE)
    uint isCommand(QMenuItem *, bool just_check=FALSE);
#endif

    uint mac_eaten_menubar : 1;
    class MacPrivate;
    MacPrivate *mac_d;
    static bool activate(MenuRef, short, bool highlight=FALSE, bool by_accel=FALSE);
    static bool activateCommand(uint cmd);
    static bool macUpdateMenuBar();
    static bool macUpdatePopupVisible(MenuRef, bool);
    static bool macUpdatePopup(MenuRef);
#endif

private:	// Disabled copy constructor and operator=

#if defined(Q_DISABLE_COPY)
    QMenuBar( const QMenuBar & );
    QMenuBar &operator=( const QMenuBar & );
#endif
};


#endif // QT_NO_MENUBAR

#endif // QMENUBAR_H
