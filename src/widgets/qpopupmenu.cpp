/****************************************************************************
**
** Implementation of QPopupMenu class
**
** Created : 941128
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

#include "qpopupmenu.h"
#ifndef QT_NO_POPUPMENU
#include "qmenubar.h"
#include "qaccel.h"
#include "qpainter.h"
#include "qdrawutil.h"
#include "qapplication.h"
#include "qpixmap.h"
#include "qpixmapcache.h"
#include "qtimer.h"
#include "qwhatsthis.h"
#include "qobjectlist.h"
#include "qguardedptr.h"
#include "qeffects_p.h"
#include "qcursor.h"
#include "qstyle.h"
#include "qtimer.h"
#include "qdatetime.h"
#if defined(QT_ACCESSIBILITY_SUPPORT)
#include "qaccessible.h"
#endif

//#define ANIMATED_POPUP
//#define BLEND_POPUP

// Motif style parameters

static const int motifArrowHMargin	= 6;	// arrow horizontal margin
static const int motifArrowVMargin	= 2;	// arrow vertical margin


/*

+-----------------------------
|      PopupFrame
|   +-------------------------
|   |	   ItemFrame
|   |	+---------------------
|   |	|
|   |	|			   \
|   |	|   ^	T E X T	  ^	    | ItemVMargin
|   |	|   |		  |	   /
|   |	      ItemHMargin
|

*/

#if 0
# define DEBUG_SLOPPY_SUBMENU
#endif

// used for internal communication
static QPopupMenu * syncMenu = 0;
static int syncMenuId = 0;

// Used to detect motion prior to mouse-release
static int motion;

// used to provide ONE single-shot timer
static QTimer * singleSingleShot = 0;

static bool supressAboutToShow = FALSE;

static void cleanup()
{
    delete singleSingleShot;
    singleSingleShot = 0;
}

static void popupSubMenuLater( int msec, QPopupMenu * receiver ) {
    if ( !singleSingleShot ) {
	singleSingleShot = new QTimer( qApp, "popup submenu timer" );
	qAddPostRoutine( cleanup );
    }

    singleSingleShot->disconnect( SIGNAL(timeout()) );
    QObject::connect( singleSingleShot, SIGNAL(timeout()),
		      receiver, SLOT(subMenuTimer()) );
    singleSingleShot->start( msec, TRUE );
}

static bool preventAnimation = FALSE;

#ifndef QT_NO_WHATSTHIS
extern void qWhatsThisBDH();
static QMenuItem* whatsThisItem = 0;
#endif

/*!
    \class QPopupMenu qpopupmenu.h
    \brief The QPopupMenu class provides a popup menu widget.

    \ingroup application
    \ingroup basic
    \mainclass

    A popup menu widget is a selection menu. It can be either a
    pull-down menu in a menu bar or a standalone context (popup) menu.
    Pull-down menus are shown by the menu bar when the user clicks on
    the respective item or presses the specified shortcut key. Use
    QMenuBar::insertItem() to insert a popup menu into a menu bar.
    Show a context menu either asynchronously with popup() or
    synchronously with exec().

    Technically, a popup menu consists of a list of menu items. You
    add items with insertItem(). An item is either a string, a pixmap
    or a custom item that provides its own drawing function (see
    QCustomMenuItem). In addition, items can have an optional icon
    drawn on the very left side and an accelerator key such as
    "Ctrl+X".

    There are three kinds of menu items: separators, menu items that
    perform an action and menu items that show a submenu. Separators
    are inserted with insertSeparator(). For submenus, you pass a
    pointer to a QPopupMenu in your call to insertItem(). All other
    items are considered action items.

    When inserting action items you usually specify a receiver and a
    slot. The receiver will be notifed whenever the item is selected.
    In addition, QPopupMenu provides two signals, activated() and
    highlighted(), which signal the identifier of the respective menu
    item. It is sometimes practical to connect several items to one
    slot. To distinguish between them, specify a slot that takes an
    integer argument and use setItemParameter() to associate a unique
    value with each item.

    You clear a popup menu with clear() and remove single items with
    removeItem() or removeItemAt().

    A popup menu can display check marks for certain items when
    enabled with setCheckable(TRUE). You check or uncheck items with
    setItemChecked().

    Items are either enabled or disabled. You toggle their state with
    setItemEnabled(). Just before a popup menu becomes visible, it
    emits the aboutToShow() signal. You can use this signal to set the
    correct enabled/disabled states of all menu items before the user
    sees it. The corresponding aboutToHide() signal is emitted when
    the menu hides again.

    You can provide What's This? help for single menu items with
    setWhatsThis(). See QWhatsThis for general information about this
    kind of lightweight online help.

    For ultimate flexibility, you can also add entire widgets as items
    into a popup menu (for example, a color selector).

    A QPopupMenu can also provide a tear-off menu. A tear-off menu is
    a top-level window that contains a copy of the menu. This makes it
    possible for the user to "tear off" frequently used menus and
    position them in a convenient place on the screen. If you want
    that functionality for a certain menu, insert a tear-off handle
    with insertTearOffHandle(). When using tear-off menus, bear in
    mind that the concept isn't typically used on Microsoft Windows so
    users may not be familiar with it. Consider using a QToolBar
    instead. Tear-off menus cannot contain custom widgets; if the
    original menu contains a custom widget item, this item is omitted.

    \link menu-example.html menu/menu.cpp\endlink is an example of
    QMenuBar and QPopupMenu use.

    \important insertItem removeItem removeItemAt clear text pixmap iconSet insertSeparator changeItem whatsThis setWhatsThis accel setAccel setItemEnabled isItemEnabled setItemVisible isItemVisible setItemChecked isItemChecked connectItem disconnectItem setItemParameter itemParameter

    <img src=qpopmenu-m.png> <img src=qpopmenu-w.png>

    \sa QMenuBar
    \link guibooks.html#fowler GUI Design Handbook: Menu, Drop-Down and
    Pop-Up\endlink
*/


/*!
    \fn void QPopupMenu::aboutToShow()

    This signal is emitted just before the popup menu is displayed.
    You can connect it to any slot that sets up the menu contents
    (e.g. to ensure that the right items are enabled).

    \sa aboutToHide(), setItemEnabled(), setItemChecked(), insertItem(), removeItem()
*/

/*!
    \fn void QPopupMenu::aboutToHide()

    This signal is emitted just before the popup menu is hidden after
    it has been displayed.

    \warning Do not open a widget in a slot connected to this signal.

    \sa aboutToShow(), setItemEnabled(), setItemChecked(), insertItem(), removeItem()
*/



/*****************************************************************************
  QPopupMenu member functions
 *****************************************************************************/

class QMenuDataData {
    // attention: also defined in qmenudata.cpp
public:
    QMenuDataData();
    QGuardedPtr<QWidget> aWidget;
    int aInt;
};

class QPopupMenuPrivate {
public:
    struct Scroll {
	enum { ScrollNone=0, ScrollUp=0x01, ScrollDown=0x02 };
	uint scrollable : 2;
	uint direction : 1;
	int topScrollableIndex, scrollableSize;
	QTime lastScroll;
	QTimer *scrolltimer;
    } scroll;
    QSize calcSize;
    QRegion mouseMoveBuffer;
};

static QPopupMenu* active_popup_menu = 0;

/*!
    Constructs a popup menu called \a name with parent \a parent.

    Although a popup menu is always a top-level widget, if a parent is
    passed the popup menu will be deleted when that parent is
    destroyed (as with any other QObject).
*/

QPopupMenu::QPopupMenu( QWidget *parent, const char *name )
    : QFrame( parent, name, WType_Popup  | WNoAutoErase )
{
    d = new QPopupMenuPrivate;
    d->scroll.scrollableSize = d->scroll.topScrollableIndex = 0;
    d->scroll.scrollable = QPopupMenuPrivate::Scroll::ScrollNone;
    d->scroll.scrolltimer = 0;
    isPopupMenu	  = TRUE;
#ifndef QT_NO_ACCEL
    autoaccel	  = 0;
    accelDisabled = FALSE;
#endif
    popupActive	  = -1;
    snapToMouse	  = TRUE;
    tab = 0;
    checkable = 0;
    tornOff = 0;
    pendingDelayedContentsChanges = 0;
    pendingDelayedStateChanges = 0;
    maxPMWidth = 0;

    tab = 0;
    ncols = 1;
    setFrameStyle( QFrame::PopupPanel | QFrame::Raised );
    setMouseTracking(style().styleHint(QStyle::SH_PopupMenu_MouseTracking, this));
    style().polishPopupMenu( this );
    setBackgroundMode( PaletteButton );
    connectModalRecursionSafety = 0;

    setFocusPolicy( StrongFocus );
}

/*!
    Destroys the popup menu.
*/

QPopupMenu::~QPopupMenu()
{
    if ( syncMenu == this && qApp ) {
	qApp->exit_loop();
	syncMenu = 0;
    }

    if(d->scroll.scrolltimer)
	delete d->scroll.scrolltimer;

    if ( isVisible() ) {
        parentMenu = 0;
        hidePopups();
    }

    delete (QWidget*) QMenuData::d->aWidget;  // tear-off menu

    preventAnimation = FALSE;
    delete d;
}


/*!
    Updates the item with identity \a id.
*/
void QPopupMenu::updateItem( int id )		// update popup menu item
{
    updateRow( indexOf(id) );
}


void QPopupMenu::setCheckable( bool enable )
{
    if ( isCheckable() != enable ) {
	checkable = enable;
	badSize = TRUE;
	if ( QMenuData::d->aWidget )
	    ( (QPopupMenu*)(QWidget*)QMenuData::d->aWidget)->setCheckable( enable );
    }
}

/*!
    \property QPopupMenu::checkable
    \brief whether the display of check marks on menu items is enabled

    When TRUE, the display of check marks on menu items is enabled.
    Checking is always enabled when in Windows-style.

    \sa QMenuData::setItemChecked()
*/

bool QPopupMenu::isCheckable() const
{
    return checkable;
}

void QPopupMenu::menuContentsChanged()
{
    // here the part that can't be delayed
    QMenuData::menuContentsChanged();
    badSize = TRUE;				// might change the size
#if defined(Q_WS_MAC) && !defined(QMAC_QMENUBAR_NO_NATIVE)
    mac_dirty_popup = 1;
#endif
    if( pendingDelayedContentsChanges )
        return;
    pendingDelayedContentsChanges = 1;
    if( !pendingDelayedStateChanges ) // if the timer hasn't been started yet
	QTimer::singleShot( 0, this, SLOT(performDelayedChanges()));
}

void QPopupMenu::performDelayedContentsChanged()
{
    pendingDelayedContentsChanges = 0;
    // here the part the can be delayed
#ifndef QT_NO_ACCEL
    // if performDelayedStateChanged() will be called too,
    // it will call updateAccel() too, no need to do it twice
    if( !pendingDelayedStateChanges )
        updateAccel( 0 );
#endif
    if ( isVisible() ) {
	if ( tornOff )
	    return;
	updateSize(TRUE);
	update();
    }
    QPopupMenu* p = (QPopupMenu*)(QWidget*)QMenuData::d->aWidget;
    if ( p && p->isVisible() ) {
	p->updateSize(TRUE);
	p->update();
    }
#if defined(Q_WS_MAC) && !defined(QMAC_QMENUBAR_NO_NATIVE)
    mac_dirty_popup = 1;
#endif
}


void QPopupMenu::menuStateChanged()
{
    // here the part that can't be delayed
    if( pendingDelayedStateChanges )
	return;
    pendingDelayedStateChanges = 1;
    if( !pendingDelayedContentsChanges ) // if the timer hasn't been started yet
	QTimer::singleShot( 0, this, SLOT(performDelayedChanges()));
}

void QPopupMenu::performDelayedStateChanged()
{
    pendingDelayedStateChanges = 0;
    // here the part that can be delayed
#ifndef QT_NO_ACCEL
    updateAccel( 0 ); // ### when we have a good solution for the accel vs. focus widget problem, remove that. That is only a workaround
    // if you remove this, see performDelayedContentsChanged()
#endif
    update();
    if ( QMenuData::d->aWidget )
	QMenuData::d->aWidget->update();
}

void QPopupMenu::performDelayedChanges()
{
    if( pendingDelayedContentsChanges )
	performDelayedContentsChanged();
    if( pendingDelayedStateChanges )
	performDelayedStateChanged();
}

void QPopupMenu::menuInsPopup( QPopupMenu *popup )
{
    connect( popup, SIGNAL(activatedRedirect(int)),
	     SLOT(subActivated(int)) );
    connect( popup, SIGNAL(highlightedRedirect(int)),
	     SLOT(subHighlighted(int)) );
    connect( popup, SIGNAL(destroyed(QObject*)),
	     this, SLOT(popupDestroyed(QObject*)) );
}

void QPopupMenu::menuDelPopup( QPopupMenu *popup )
{
    popup->disconnect( SIGNAL(activatedRedirect(int)) );
    popup->disconnect( SIGNAL(highlightedRedirect(int)) );
    disconnect( popup, SIGNAL(destroyed(QObject*)),
		this, SLOT(popupDestroyed(QObject*)) );
}


void QPopupMenu::frameChanged()
{
    menuContentsChanged();
}

/*!
    Displays the popup menu so that the item number \a indexAtPoint
    will be at the specified \e global position \a pos. To translate a
    widget's local coordinates into global coordinates, use
    QWidget::mapToGlobal().

    When positioning a popup with exec() or popup(), bear in mind that
    you cannot rely on the popup menu's current size(). For
    performance reasons, the popup adapts its size only when
    necessary, so in many cases, the size before and after the show is
    different. Instead, use sizeHint(). It calculates the proper size
    depending on the menu's current contents.
*/

void QPopupMenu::popup( const QPoint &pos, int indexAtPoint )
{
    if ( !isPopup() && isVisible() )
	hide();

    //avoid circularity
    if ( isVisible() || !isEnabled() )
	return;

#if defined(Q_WS_MAC) && !defined(QMAC_QMENUBAR_NO_NATIVE)
    if( macPopupMenu(pos, indexAtPoint ))
	return;
#endif

#if (QT_VERSION-0 >= 0x040000)
#error "Fix this now"
    // #### should move to QWidget - anything might need this functionality,
    // #### since anything can have WType_Popup window flag.
    // #### This includes stuff in QPushButton and some stuff for setting
    // #### the geometry of QDialog.
    // QPopupMenu
    // ::exec()
    // ::popup()
    // QPushButton (shouldn't require QMenuPopup)
    // ::popupPressed
    // Some stuff in qwidget.cpp for dialogs... can't remember exactly.
    // Also the code here indicatets the parameter should be a rect, not a
    // point.
#endif

    if(d->scroll.scrollable) {
	d->scroll.scrollable = QPopupMenuPrivate::Scroll::ScrollNone;
	d->scroll.topScrollableIndex = d->scroll.scrollableSize = 0;
	badSize = TRUE;
    }
    updateSize();

    QPoint mouse = QCursor::pos();
    snapToMouse = pos == mouse;

    // have to emit here as a menu might be setup in a slot connected
    // to aboutToShow which will change the size of the menu
    bool s = supressAboutToShow;
    supressAboutToShow = TRUE;
    if ( !s) {
	emit aboutToShow();
	updateSize(TRUE);
    }

    int screen_num;
    if (QApplication::desktop()->isVirtualDesktop())
	screen_num =
	    QApplication::desktop()->screenNumber( QApplication::reverseLayout() ?
						   pos+QPoint(width(),0) : pos );
    else
	screen_num = QApplication::desktop()->screenNumber( this );
#ifdef Q_WS_MAC
    QRect screen = QApplication::desktop()->availableGeometry( screen_num );
#else
    QRect screen = QApplication::desktop()->screenGeometry( screen_num );
#endif
    int sw = screen.width();			// screen width
    int sh = screen.height();			// screen height
    int sx = screen.x();			// screen pos
    int sy = screen.y();
    int x  = pos.x();
    int y  = pos.y();
    if ( indexAtPoint >= 0 )			// don't subtract when < 0
	y -= itemGeometry( indexAtPoint ).y();		// (would subtract 2 pixels!)
    int w  = width();
    int h  = height();

    if ( snapToMouse ) {
	if ( qApp->reverseLayout() )
	    x -= w;
	if ( x+w > sx+sw )
	    x = mouse.x()-w;
	if ( y+h > sy+sh )
	    y = mouse.y()-h;
	if ( x < sx )
	    x = mouse.x();
	if ( y < sy )
	    y = sy;
    }

    if ( x+w > sx+sw )				// the complete widget must
	x = sx+sw - w;				//   be visible
    if ( y+h > sy+sh )
	y = sy+sh - h;
    if ( x < sx )
	x = sx;
    if ( y < sy )
	y = sy;

    if(style().styleHint(QStyle::SH_PopupMenu_Scrollable, this)) {
	int off_top = 0, off_bottom = 0;
	if(y+h > sy+sh)
	    off_bottom = (y+h) - (sy+sh);
	if(y < sy)
	    off_top = sy - y;
	if(off_bottom || off_top) {
	    int ch = updateSize().height(); //store the old height, before setting scrollable --Sam
	    const int vextra = style().pixelMetric(QStyle::PM_PopupMenuFrameVerticalExtra, this);
	    d->scroll.scrollableSize = h - off_top - off_bottom - 2*vextra;
	    if(off_top) {
		move( x, y = sy );
		d->scroll.scrollable = d->scroll.scrollable | QPopupMenuPrivate::Scroll::ScrollUp;
	    }
	    if( off_bottom )
		d->scroll.scrollable = d->scroll.scrollable | QPopupMenuPrivate::Scroll::ScrollDown;
	    if( off_top != off_bottom && indexAtPoint >= 0 ) {
		ch -= (vextra * 2);
		if(ch > sh) //no bigger than the screen!
		    ch = sh;
		if( ch > d->scroll.scrollableSize ) 
		    d->scroll.scrollableSize = ch;
	    }

	    updateSize(TRUE); //now set the size using the scrollable/scrollableSize as above
            w = width();
            h = height();
	    if(indexAtPoint >= 0) { 
		if(off_top) { //scroll to it
		    register QMenuItem *mi = NULL;
		    QMenuItemListIt it(*mitems);
		    for(int tmp_y = 0; tmp_y < off_top && (mi=it.current()); ) {
			QSize sz = style().sizeFromContents(QStyle::CT_PopupMenuItem, this,
							    QSize(0, itemHeight( mi )),
							    QStyleOption(mi,maxPMWidth,0));
			tmp_y += sz.height();
			d->scroll.topScrollableIndex++;
		    }
		} 
	    }
	}
    }
    move( x, y );
    motion=0;
    actItem = -1;

#ifndef QT_NO_EFFECTS
    int hGuess = qApp->reverseLayout() ? QEffects::LeftScroll : QEffects::RightScroll;
    int vGuess = QEffects::DownScroll;
    if ( qApp->reverseLayout() ) {
	if ( snapToMouse && ( x + w/2 > mouse.x() ) ||
	    ( parentMenu && parentMenu->isPopupMenu &&
	    ( x + w/2 > ((QPopupMenu*)parentMenu)->x() ) ) )
	    hGuess = QEffects::RightScroll;
    } else {
	if ( snapToMouse && ( x + w/2 < mouse.x() ) ||
	    ( parentMenu && parentMenu->isPopupMenu &&
	    ( x + w/2 < ((QPopupMenu*)parentMenu)->x() ) ) )
	    hGuess = QEffects::LeftScroll;
    }

#ifndef QT_NO_MENUBAR
    if ( snapToMouse && ( y + h/2 < mouse.y() ) ||
	( parentMenu && parentMenu->isMenuBar &&
	( y + h/2 < ((QMenuBar*)parentMenu)->mapToGlobal( ((QMenuBar*)parentMenu)->pos() ).y() ) ) )
	vGuess = QEffects::UpScroll;
#endif

    if ( QApplication::isEffectEnabled( UI_AnimateMenu ) &&
	 preventAnimation == FALSE ) {
	if ( QApplication::isEffectEnabled( UI_FadeMenu ) )
	    qFadeEffect( this );
	else if ( parentMenu )
	    qScrollEffect( this, parentMenu->isPopupMenu ? hGuess : vGuess );
	else
	    qScrollEffect( this, hGuess | vGuess );
    } else
#endif
    {
	show();
    }
#if defined(QT_ACCESSIBILITY_SUPPORT)
    QAccessible::updateAccessibility( this, 0, QAccessible::PopupMenuStart );
#endif
}

/*!
    \fn void QPopupMenu::activated( int id )

    This signal is emitted when a menu item is selected; \a id is the
    id of the selected item.

    Normally, you connect each menu item to a single slot using
    QMenuData::insertItem(), but sometimes you will want to connect
    several items to a single slot (most often if the user selects
    from an array). This signal is useful in such cases.

    \sa highlighted(), QMenuData::insertItem()
*/

/*!
    \fn void QPopupMenu::highlighted( int id )

    This signal is emitted when a menu item is highlighted; \a id is
    the id of the highlighted item.

    \sa activated(), QMenuData::insertItem()
*/

/*! \fn void QPopupMenu::highlightedRedirect( int id )
  \internal
  Used internally to connect submenus to their parents.
*/

/*! \fn void QPopupMenu::activatedRedirect( int id )
  \internal
  Used internally to connect submenus to their parents.
*/

void QPopupMenu::subActivated( int id )
{
    emit activatedRedirect( id );
}

void QPopupMenu::subHighlighted( int id )
{
    emit highlightedRedirect( id );
}

static bool fromAccel = FALSE;

#ifndef QT_NO_ACCEL
void QPopupMenu::accelActivated( int id )
{
    QMenuItem *mi = findItem( id );
    if ( mi && mi->isEnabledAndVisible() ) {
	QGuardedPtr<QSignal> signal = mi->signal();
	fromAccel = TRUE;
	actSig( mi->id() );
	fromAccel = FALSE;
	if ( signal )
	    signal->activate();
    }
}

void QPopupMenu::accelDestroyed()		// accel about to be deleted
{
    autoaccel = 0;				// don't delete it twice!
}
#endif //QT_NO_ACCEL

void QPopupMenu::popupDestroyed( QObject *o )
{
    removePopup( (QPopupMenu*)o );
}

void QPopupMenu::actSig( int id, bool inwhatsthis )
{
    if ( !inwhatsthis ) {
	emit activated( id );
#if defined(QT_ACCESSIBILITY_SUPPORT)
	if ( !fromAccel )
	    QAccessible::updateAccessibility( this, indexOf(id)+1, QAccessible::MenuCommand );
#endif
    } else {
#ifndef QT_NO_WHATSTHIS
	QRect r( itemGeometry( indexOf( id ) ) );
	QPoint p( r.center().x(), r.bottom() );
	QString whatsThis = findItem( id )->whatsThis();
	if ( whatsThis.isNull() )
	    whatsThis = QWhatsThis::textFor( this, p );
	QWhatsThis::leaveWhatsThisMode( whatsThis, mapToGlobal( p ), this );
#endif
    }

    emit activatedRedirect( id );
}

void QPopupMenu::hilitSig( int id )
{
    emit highlighted( id );
    emit highlightedRedirect( id );

#if defined(QT_ACCESSIBILITY_SUPPORT)
    QAccessible::updateAccessibility( this, indexOf(id)+1, QAccessible::Focus );
    QAccessible::updateAccessibility( this, indexOf(id)+1, QAccessible::Selection );
#endif
}

void QPopupMenu::setFirstItemActive()
{
    QMenuItemListIt it(*mitems);
    register QMenuItem *mi;
    int ai = 0;
    if(d->scroll.scrollable)
	ai = d->scroll.topScrollableIndex;
    while ( (mi=it.current()) ) {
	++it;
	if ( !mi->isSeparator() && mi->id() != QMenuData::d->aInt &&
	     ( style().styleHint( QStyle::SH_PopupMenu_AllowActiveAndDisabled, this ) || mi->isEnabledAndVisible() )) {
	    setActiveItem( ai );
	    return;
	}
	ai++;
    }
    actItem = -1;
}

/*!
  \internal
  Hides all popup menus (in this menu tree) that are currently open.
*/

void QPopupMenu::hideAllPopups()
{
    register QMenuData *top = this;		// find top level popup
    if ( !preventAnimation )
	QTimer::singleShot( 10, this, SLOT(allowAnimation()) );
    preventAnimation = TRUE;

    if ( !isPopup() )
	return; // nothing to do

    while ( top->parentMenu && top->parentMenu->isPopupMenu
	    && ((QPopupMenu*)top->parentMenu)->isPopup() )
	top = top->parentMenu;
    ((QPopupMenu*)top)->hide();			// cascade from top level

#ifndef QT_NO_WHATSTHIS
    if (whatsThisItem) {
	qWhatsThisBDH();
	whatsThisItem = 0;
    }
#endif

}

/*!
  \internal
  Hides all popup sub-menus.
*/

void QPopupMenu::hidePopups()
{
    if ( !preventAnimation )
	QTimer::singleShot( 10, this, SLOT(allowAnimation()) );
    preventAnimation = TRUE;

    QMenuItemListIt it(*mitems);
    register QMenuItem *mi;
    while ( (mi=it.current()) ) {
	++it;
	if ( mi->popup() && mi->popup()->parentMenu == this ) //avoid circularity
	    mi->popup()->hide();
    }
    popupActive = -1;				// no active sub menu
    if(style().styleHint(QStyle::SH_PopupMenu_SubMenuPopupDelay, this))
	d->mouseMoveBuffer = QRegion();

    QRect mfrect = itemGeometry( actItem );
    setMicroFocusHint( mfrect.x(), mfrect.y(), mfrect.width(), mfrect.height(), FALSE );
}


/*!
  \internal
  Sends the event to the menu bar.
*/

bool QPopupMenu::tryMenuBar( QMouseEvent *e )
{
    register QMenuData *top = this;		// find top level
    while ( top->parentMenu )
	top = top->parentMenu;
#ifndef QT_NO_MENUBAR
    return top->isMenuBar ?
	((QMenuBar *)top)->tryMouseEvent( this, e ) :
			      ((QPopupMenu*)top)->tryMouseEvent(this, e );
#else
    return ((QPopupMenu*)top)->tryMouseEvent(this, e );
#endif
}


/*!
  \internal
*/
bool QPopupMenu::tryMouseEvent( QPopupMenu *p, QMouseEvent * e)
{
    if ( p == this )
	return FALSE;
    QPoint pos = mapFromGlobal( e->globalPos() );
    if ( !rect().contains( pos ) )		// outside
	return FALSE;
    QMouseEvent ee( e->type(), pos, e->globalPos(), e->button(), e->state() );
    event( &ee );
    return TRUE;
}

/*!
  \internal
  Tells the menu bar to go back to idle state.
*/

void QPopupMenu::byeMenuBar()
{
#ifndef QT_NO_MENUBAR
    register QMenuData *top = this;		// find top level
    while ( top->parentMenu )
	top = top->parentMenu;
#endif
    hideAllPopups();
#ifndef QT_NO_MENUBAR
    if ( top->isMenuBar )
	((QMenuBar *)top)->goodbye();
#endif
}


/*!
  \internal
  Return the item at \a pos, or -1 if there is no item there or if
  it is a separator item.
*/

int QPopupMenu::itemAtPos( const QPoint &pos, bool ignoreSeparator ) const
{
    if ( !contentsRect().contains(pos) )
	return -1;

    int row = 0;
    int x = contentsRect().x();
    int y = contentsRect().y();
    QMenuItem *mi;
    QMenuItemListIt it( *mitems );
    if(d->scroll.scrollable) {
	if(d->scroll.topScrollableIndex) {
	    for( ; (mi = it.current()) && row < d->scroll.topScrollableIndex; row++)
		++it;
	    if(!mi) {
		row = 0;
		it.toFirst();
	    }
	    y += style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
	}
    }
    int itemw = contentsRect().width() / ncols;
    QSize sz;
    while ( (mi=it.current()) ) {
	if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown &&
	   y >= contentsRect().height() - style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this))
	    return -1;
	++it;
	if ( !mi->isVisible() ) {
	    ++row;
	    continue;
	}
	int itemh = itemHeight( mi );

	sz = style().sizeFromContents(QStyle::CT_PopupMenuItem, this,
				      QSize(0, itemh),
				      QStyleOption(mi,maxPMWidth));
	sz = sz.expandedTo(QSize(itemw, sz.height()));
	itemw = sz.width();
	itemh = sz.height();

	if ( ncols > 1 && y + itemh > contentsRect().bottom() ) {
	    y = contentsRect().y();
	    x +=itemw;
	}
	if ( QRect( x, y, itemw, itemh ).contains( pos ) )
	    break;
	y += itemh;
	++row;
    }

    if ( mi && ( !ignoreSeparator || !mi->isSeparator() ) )
	return row;
    return -1;
}

/*!
  \internal
  Returns the geometry of item number \a index.
*/

QRect QPopupMenu::itemGeometry( int index )
{
    QMenuItem *mi;
    QSize sz;
    int row = 0, scrollh = 0;
    int x = contentsRect().x();
    int y = contentsRect().y();
    QMenuItemListIt it( *mitems );
    if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollUp) {
	scrollh = style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
	y += scrollh;
	if(d->scroll.topScrollableIndex) {
	    for( ; (mi = it.current()) && row < d->scroll.topScrollableIndex; row++)
		++it;
	    if(!mi) {
		row = 0;
		it.toFirst();
	    }
	}
    }
    int itemw = contentsRect().width() / ncols;
    while ( (mi=it.current()) ) {
	if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown &&
	   y >= contentsRect().height() - scrollh)
	    break;
	++it;
	if ( !mi->isVisible() ) {
	    ++row;
	    continue;
	}
	int itemh = itemHeight( mi );

	sz = style().sizeFromContents(QStyle::CT_PopupMenuItem, this,
				      QSize(0, itemh),
				      QStyleOption(mi,maxPMWidth));
	sz = sz.expandedTo(QSize(itemw, sz.height()));
	itemw = sz.width();
	itemh = sz.height();
	if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown &&
	   (y + itemh > contentsRect().height() - scrollh))
	    itemh -= (y + itemh) - (contentsRect().height() - scrollh);
	if ( ncols > 1 && y + itemh > contentsRect().bottom() ) {
	    y = contentsRect().y();
	    x +=itemw;
	}
	if ( row == index )
	    return QRect( x,y,itemw,itemh );
	y += itemh;
	++row;
    }

    return QRect(0,0,0,0);
}


/*!
  \internal
  Calculates and sets the size of the popup menu, based on the size
  of the items.
*/

QSize QPopupMenu::updateSize(bool force_update, bool do_resize)
{
    polish();
    if ( count() == 0 ) {
	QSize ret = QSize( 50, 8 );
	if(do_resize)
	    setFixedSize( ret );
	badSize = TRUE;
	return ret;
    }

    int scrheight = 0;
    if(d->scroll.scrollableSize) {
	if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollUp)
	    scrheight += style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
	if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown)
	    scrheight += style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
    }

    if(badSize || force_update) {
#ifndef QT_NO_ACCEL
	updateAccel( 0 );
#endif
	int height = 0;
	int max_width = 0, max_height = 0;
	QFontMetrics fm = fontMetrics();
	register QMenuItem *mi;
	maxPMWidth = 0;
	int maxWidgetWidth = 0;
	tab = 0;

	for ( QMenuItemListIt it( *mitems ); it.current(); ++it ) {
	    mi = it.current();
	    QWidget *miw = mi->widget();
	    if (miw) {
		if ( miw->parentWidget() != this )
		    miw->reparent( this, QPoint(0,0), TRUE );
		// widget items musn't propgate mouse events
		((QPopupMenu*)miw)->setWFlags(WNoMousePropagation);
	    }
	    if ( mi->custom() )
		mi->custom()->setFont( font() );
	    if ( mi->iconSet() != 0)
		maxPMWidth = QMAX( maxPMWidth,
				   mi->iconSet()->pixmap( QIconSet::Small, QIconSet::Normal ).width() + 4 );
	}

	int dh = QApplication::desktop()->height();
	ncols = 1;

	for ( QMenuItemListIt it2( *mitems ); it2.current(); ++it2 ) {
	    mi = it2.current();
	    if ( !mi->isVisible() )
		continue;
	    int w = 0;
	    int itemHeight = QPopupMenu::itemHeight( mi );

	    if ( mi->widget() ) {
		QSize s( mi->widget()->sizeHint() );
		s = s.expandedTo( mi->widget()->minimumSize() );
		mi->widget()->resize( s );
		if ( s.width()  > maxWidgetWidth )
		    maxWidgetWidth = s.width();
		itemHeight = s.height();
	    } else {
		if( ! mi->isSeparator() ) {
		    if ( mi->custom() ) {
			if ( mi->custom()->fullSpan() ) {
			    maxWidgetWidth = QMAX( maxWidgetWidth,
						   mi->custom()->sizeHint().width() );
			} else {
			    QSize s ( mi->custom()->sizeHint() );
			    w += s.width();
			}
		    }

		    w += maxPMWidth;

		    if (! mi->text().isNull()) {
			QString s = mi->text();
			int t;
			if ( (t = s.find('\t')) >= 0 ) { // string contains tab
			    w += fm.width( s, t );
			    w -= s.contains('&') * fm.width('&');
			    w += s.contains("&&") * fm.width('&');
			    int tw = fm.width( s.mid(t + 1) );
			    if ( tw > tab)
				tab = tw;
			} else {
			    w += fm.width( s );
			    w -= s.contains('&') * fm.width('&');
			    w += s.contains("&&") * fm.width('&');
			}
		    } else if (mi->pixmap())
			w += mi->pixmap()->width();
		} else {
		    if ( mi->custom() ) {
			QSize s ( mi->custom()->sizeHint() );
			w += s.width();
		    } else {
			w = itemHeight = 2;
		    }
		}

		QSize sz = style().sizeFromContents(QStyle::CT_PopupMenuItem, this,
						    QSize(w, itemHeight),
						    QStyleOption(mi,maxPMWidth));

		w = sz.width();
		itemHeight = sz.height();

#if defined(QT_CHECK_NULL)
		if ( mi->text().isNull() && !mi->pixmap() && !mi->iconSet() &&
		     !mi->isSeparator() && !mi->widget() && !mi->custom() )
		    qWarning( "QPopupMenu: (%s) Popup has invalid menu item",
			      name( "unnamed" ) );
#endif
	    }
	    height += itemHeight;
	    if(style().styleHint(QStyle::SH_PopupMenu_Scrollable, this)) {
		if(scrheight && height >= d->scroll.scrollableSize - scrheight) {
                    height = d->scroll.scrollableSize - scrheight;
		    break;
                }
	    } else if( height + 2*frameWidth() >= dh ) {
		ncols++;
		max_height = QMAX(max_height, height - itemHeight);
		height = itemHeight;
	    }
	    if ( w > max_width )
		max_width = w;
	}
	if( ncols == 1 && !max_height )
	    max_height = height;

	if(style().styleHint(QStyle::SH_PopupMenu_Scrollable, this)) {
	    height += scrheight;
	    setMouseTracking(TRUE);
	}

	if ( tab )
	    tab -= fontMetrics().minRightBearing();
	else
	    max_width -= fontMetrics().minRightBearing();

	if ( max_width + tab < maxWidgetWidth )
	    max_width = maxWidgetWidth - tab;

	const int fw = frameWidth();
	int extra_width = (fw+style().pixelMetric(QStyle::PM_PopupMenuFrameHorizontalExtra, this)) * 2,
	   extra_height = (fw+style().pixelMetric(QStyle::PM_PopupMenuFrameVerticalExtra,   this)) * 2;
	if ( ncols == 1 )
	    d->calcSize = QSize( QMAX( minimumWidth(), max_width + tab + extra_width ),
			      QMAX( minimumHeight() , height + extra_height ) );
	else
	    d->calcSize = QSize( QMAX( minimumWidth(), (ncols*(max_width + tab)) + extra_width ),
			      QMAX( minimumHeight(), QMIN( max_height + extra_height + 1, dh ) ) );
	badSize = FALSE;
    }

    {
	// Position the widget items. It could be done in drawContents
	// but this way we get less flicker.
	QSize sz;
	int x = contentsRect().x();
	int y = contentsRect().y();
	int itemw = contentsRect().width() / ncols;
	for(QMenuItemListIt it(*mitems); it.current(); ++it) {
	    QMenuItem *mi = it.current();
	    if ( !mi->isVisible() )
		continue;

	    int itemh = itemHeight( mi );
	    
	    sz = style().sizeFromContents(QStyle::CT_PopupMenuItem, this,
					  QSize(0, itemh), QStyleOption(mi,maxPMWidth));
	    sz = sz.expandedTo(QSize(itemw, sz.height()));
	    itemw = sz.width();
	    itemh = sz.height();

	    if ( ncols > 1 && y + itemh > contentsRect().bottom() ) {
		y = contentsRect().y();
		x +=itemw;
	    }
	    if ( mi->widget() ) 
		mi->widget()->setGeometry( x, y, itemw, mi->widget()->height() );
	    y += itemh;
	}
    }

    if( do_resize && size() != d->calcSize ) {
	setMaximumSize( d->calcSize );
        d->calcSize = maximumSize(); //let the max size adjust it (virtual)
	resize( d->calcSize );
    }
    return d->calcSize;
}


#ifndef QT_NO_ACCEL
/*!
  \internal
  The \a parent is 0 when it is updated when a menu item has
  changed a state, or it is something else if called from the menu bar.
*/

void QPopupMenu::updateAccel( QWidget *parent )
{
    QMenuItemListIt it(*mitems);
    register QMenuItem *mi;

    if ( parent ) {
	delete autoaccel;
	autoaccel = 0;
    } else if ( !autoaccel ) {
	// we have no parent. Rather than ignoring any accelerators we try to find this popup's main window
	if ( tornOff ) {
	    parent = this;
	} else {
	    QWidget *w = (QWidget *) this;
	    parent = w->parentWidget();
	    while ( (!w->testWFlags(WType_TopLevel) || !w->testWFlags(WType_Popup)) && parent ) {
		w = parent;
		parent = parent->parentWidget();
	    }
	}
    }

    if ( parent == 0 && autoaccel == 0 )
 	return;

    if ( autoaccel )				// build it from scratch
	autoaccel->clear();
    else {
	// create an autoaccel in any case, even if we might not use
	// it immediately. Maybe the user needs it later.
	autoaccel = new QAccel( parent, this );
	connect( autoaccel, SIGNAL(activated(int)),
		 SLOT(accelActivated(int)) );
	connect( autoaccel, SIGNAL(activatedAmbiguously(int)),
		 SLOT(accelActivated(int)) );
	connect( autoaccel, SIGNAL(destroyed()),
		 SLOT(accelDestroyed()) );
	if ( accelDisabled )
	    autoaccel->setEnabled( FALSE );
    }
    while ( (mi=it.current()) ) {
	++it;
	QKeySequence k = mi->key();
	if ( (int)k ) {
	    int id = autoaccel->insertItem( k, mi->id() );
#ifndef QT_NO_WHATSTHIS
	    autoaccel->setWhatsThis( id, mi->whatsThis() );
#endif
	}
	if ( !mi->text().isNull() || mi->custom() ) {
	    QString s = mi->text();
	    int i = s.find('\t');

	    // Note: Only looking at the first key in the sequence!
	    if ( (int)k && (int)k != Key_unknown ) {
		QString t = (QString)mi->key();
		if ( i >= 0 )
		    s.replace( i+1, s.length()-i, t );
		else {
		    s += '\t';
		    s += t;
		}
	    } else if ( !k ) {
 		if ( i >= 0 )
 		    s.truncate( i );
	    }
	    if ( s != mi->text() ) {
		mi->setText( s );
		badSize = TRUE;
	    }
	}
	if ( mi->popup() && parent ) {		// call recursively
	    // reuse
	    QPopupMenu* popup = mi->popup();
	    if (!popup->avoid_circularity) {
		popup->avoid_circularity = 1;
		popup->updateAccel( parent );
		popup->avoid_circularity = 0;
	    }
	}
    }
}

/*!
  \internal
  It would be better to check in the slot.
*/

void QPopupMenu::enableAccel( bool enable )
{
    if ( autoaccel )
	autoaccel->setEnabled( enable );
    accelDisabled = !enable;		// rememeber when updateAccel
    QMenuItemListIt it(*mitems);
    register QMenuItem *mi;
    while ( (mi=it.current()) ) {		// do the same for sub popups
	++it;
	if ( mi->popup() )			// call recursively
	    mi->popup()->enableAccel( enable );
    }
}
#endif

/*!
    \reimp
*/
void QPopupMenu::setFont( const QFont &font )
{
    QWidget::setFont( font );
    badSize = TRUE;
    if ( isVisible() ) {
	updateSize();
	update();
    }
}

/*!
    \reimp
*/
void QPopupMenu::show()
{
    if ( !isPopup() && isVisible() )
	hide();

    if ( isVisible() ) {
	supressAboutToShow = FALSE;
	QWidget::show();
	return;
    }
    if (!supressAboutToShow)
	emit aboutToShow();
    else
	supressAboutToShow = FALSE;
    performDelayedChanges();
    updateSize(TRUE);
    QWidget::show();
    popupActive = -1;
    if(style().styleHint(QStyle::SH_PopupMenu_SubMenuPopupDelay, this))
	d->mouseMoveBuffer = QRegion();
}

/*!
    \reimp
*/

void QPopupMenu::hide()
{
    if ( syncMenu == this && qApp ) {
	qApp->exit_loop();
	syncMenu = 0;
    }

    if ( !isVisible() ) {
	QWidget::hide();
  	return;
    }
    emit aboutToHide();

    actItem = popupActive = -1;
    if(style().styleHint(QStyle::SH_PopupMenu_SubMenuPopupDelay, this))
	d->mouseMoveBuffer = QRegion();
    mouseBtDn = FALSE;				// mouse button up
#if defined(QT_ACCESSIBILITY_SUPPORT)
    QAccessible::updateAccessibility( this, 0, QAccessible::PopupMenuEnd );
#endif
    parentMenu = 0;
    hidePopups();
    QWidget::hide();
}


/*!
    Calculates the height in pixels of the item in row \a row.
*/
int QPopupMenu::itemHeight( int row ) const
{
    return itemHeight( mitems->at( row ) );
}

/*!
    \overload

    Calculates the height in pixels of the menu item \a mi.
*/
int QPopupMenu::itemHeight( QMenuItem *mi ) const
{
    if  ( mi->widget() )
	return mi->widget()->height();
    if ( mi->custom() && mi->custom()->fullSpan() )
	return mi->custom()->sizeHint().height();

    QFontMetrics fm(fontMetrics());
    int h = 0;
    if ( mi->isSeparator() ) // separator height
        h = 2;
    else if ( mi->pixmap() ) // pixmap height
        h = mi->pixmap()->height();
    else                     // text height
        h = fm.height();

    if ( !mi->isSeparator() && mi->iconSet() != 0 )
        h = QMAX(h, mi->iconSet()->pixmap( QIconSet::Small,
					   QIconSet::Normal ).height());
    if ( mi->custom() )
        h = QMAX(h, mi->custom()->sizeHint().height());

    return h;
}


/*!
    Draws menu item \a mi in the area \a x, \a y, \a w, \a h, using
    painter \a p. The item is drawn active if \a act is TRUE or drawn
    inactive if \a act is FALSE. The rightmost \a tab_ pixels are used
    for accelerator text.

    \sa QStyle::drawControl()
*/
void QPopupMenu::drawItem( QPainter* p, int tab_, QMenuItem* mi,
			   bool act, int x, int y, int w, int h)
{
    QStyle::SFlags flags = QStyle::Style_Default;
    if (isEnabled() && mi->isEnabledAndVisible() && (!mi->popup() || mi->popup()->isEnabled()) )
	flags |= QStyle::Style_Enabled;
    if (act)
	flags |= QStyle::Style_Active;
    if (mouseBtDn)
	flags |= QStyle::Style_Down;

    const QColorGroup &cg = ((flags&QStyle::Style_Enabled) ? colorGroup() : palette().disabled() );

    if ( mi->custom() && mi->custom()->fullSpan() ) {
	QMenuItem dummy;
	style().drawControl(QStyle::CE_PopupMenuItem, p, this, QRect(x, y, w, h), cg,
			    flags, QStyleOption(&dummy,maxPMWidth,tab_));
	mi->custom()->paint( p, cg, act, flags&QStyle::Style_Enabled, x, y, w, h );
    } else
	style().drawControl(QStyle::CE_PopupMenuItem, p, this, QRect(x, y, w, h), cg,
			    flags, QStyleOption(mi,maxPMWidth,tab_));
}


/*!
    Draws all menu items using painter \a p.
*/
void QPopupMenu::drawContents( QPainter* p )
{
    QMenuItemListIt it(*mitems);
    QMenuItem *mi = 0;
    int row = 0;
    int x = contentsRect().x();
    int y = contentsRect().y();
    if(d->scroll.scrollable) {
	if(d->scroll.topScrollableIndex) {
	    for( ; (mi = it.current()) && row < d->scroll.topScrollableIndex; row++)
		++it;
	    if(!mi)
		it.toFirst();
	}
	if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollUp) {
	    QRect rect(x, y, contentsRect().width(),
		       style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this));
	    if(!p->hasClipping() || p->clipRegion().contains(rect)) {
		QStyle::SFlags flags = QStyle::Style_Up;
		if (isEnabled())
		    flags |= QStyle::Style_Enabled;
		style().drawControl(QStyle::CE_PopupMenuScroller, p, this, rect,
				    colorGroup(), flags, QStyleOption(maxPMWidth));
	    }
	    y += rect.height();
	}
    }

    int itemw = contentsRect().width() / ncols;
    QSize sz;
    QStyle::SFlags flags;
    while ( (mi=it.current()) ) {
	if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown &&
	   y >= contentsRect().height() - style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this))
	    break;
	++it;
	if ( !mi->isVisible() ) {
	    ++row;
	    continue;
	}
	int itemh = itemHeight( mi );
	sz = style().sizeFromContents(QStyle::CT_PopupMenuItem, this,
				      QSize(0, itemh),
				      QStyleOption(mi,maxPMWidth,0)
				);
	sz = sz.expandedTo(QSize(itemw, sz.height()));
	itemw = sz.width();
	itemh = sz.height();

	if ( ncols > 1 && y + itemh > contentsRect().bottom() ) {
	    if ( y < contentsRect().bottom() ) {
		QRect rect(x, y, itemw, contentsRect().bottom() - y);
		if(!p->hasClipping() || p->clipRegion().contains(rect)) {
		    flags = QStyle::Style_Default;
		    if (isEnabled() && mi->isEnabledAndVisible())
			flags |= QStyle::Style_Enabled;
		    style().drawControl(QStyle::CE_PopupMenuItem, p, this, rect,
					colorGroup(), flags, QStyleOption((QMenuItem*)0,maxPMWidth));
		}
	    }
	    y = contentsRect().y();
	    x +=itemw;
	}
	if (!mi->widget() && (!p->hasClipping() || p->clipRegion().contains(QRect(x, y, itemw, itemh))))
	    drawItem( p, tab, mi, row == actItem, x, y, itemw, itemh );
	y += itemh;
	++row;
    }
    if ( y < contentsRect().bottom() ) {
	QRect rect(x, y, itemw, contentsRect().bottom() - y);
	if(!p->hasClipping() || p->clipRegion().contains(rect)) {
	    flags = QStyle::Style_Default;
	    if ( isEnabled() )
		flags |= QStyle::Style_Enabled;
	    style().drawControl(QStyle::CE_PopupMenuItem, p, this, rect,
				colorGroup(), flags, QStyleOption((QMenuItem*)0,maxPMWidth));
	}
    }
    if( d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown ) {
	int sh = style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
	QRect rect(x, contentsRect().height() - sh, contentsRect().width(), sh);
	if(!p->hasClipping() || p->clipRegion().contains(rect)) {
	    QStyle::SFlags flags = QStyle::Style_Down;
	    if (isEnabled())
		flags |= QStyle::Style_Enabled;
	    style().drawControl(QStyle::CE_PopupMenuScroller, p, this, rect,
				colorGroup(), flags, QStyleOption(maxPMWidth));
	}
    }
#if defined( DEBUG_SLOPPY_SUBMENU )
    if ( style().styleHint(QStyle::SH_PopupMenu_SloppySubMenus, this )) {
	p->setClipRegion( d->mouseMoveBuffer );
	p->fillRect( d->mouseMoveBuffer.boundingRect(), colorGroup().brush( QColorGroup::Highlight ) );
    }
#endif
}


/*****************************************************************************
  Event handlers
 *****************************************************************************/

/*!
    \reimp
*/

void QPopupMenu::paintEvent( QPaintEvent *e )
{
    QFrame::paintEvent( e );
}

/*!
    \reimp
*/

void QPopupMenu::closeEvent( QCloseEvent * e) {
    e->accept();
    byeMenuBar();
}


/*!
    \reimp
*/

void QPopupMenu::mousePressEvent( QMouseEvent *e )
{
    int sh = style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
    if (rect().contains(e->pos()) &&
	((d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollUp && e->pos().y() <= sh) || //up
	 (d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown &&
	     e->pos().y() >= contentsRect().height() - sh))) //down
	return;

    mouseBtDn = TRUE;				// mouse button down
    int item = itemAtPos( e->pos() );
    if ( item == -1 ) {
	if ( !rect().contains(e->pos()) && !tryMenuBar(e) ) {
	    byeMenuBar();
	}
	return;
    }
    register QMenuItem *mi = mitems->at(item);
    if ( item != actItem )			// new item activated
	setActiveItem( item );

    QPopupMenu *popup = mi->popup();
    if ( popup ) {
	if ( popup->isVisible() ) {		// sub menu already open
	    int pactItem = popup->actItem;
	    popup->actItem = -1;
	    popup->hidePopups();
	    popup->updateRow( pactItem );
	} else {				// open sub menu
	    hidePopups();
	    popupSubMenuLater( 20, this );
	}
    } else {
	hidePopups();
    }
}

/*!
    \reimp
*/

void QPopupMenu::mouseReleaseEvent( QMouseEvent *e )
{
    // do not hide a standalone context menu on press-release, unless
    // the user moved the mouse significantly
    if ( !parentMenu && !mouseBtDn && actItem < 0 && motion < 6 )
	return;

    mouseBtDn = FALSE;

    // if the user released the mouse outside the menu, pass control
    // to the menubar or our parent menu
    int sh = style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
    if ( !rect().contains( e->pos() ) && tryMenuBar(e) )
	return;
    else if((d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollUp && e->pos().y() <= sh) || //up
	    (d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown &&
	     e->pos().y() >= contentsRect().height() - sh)) //down
	return;

    if ( actItem < 0 ) { // we do not have an active item
	// if the release is inside without motion (happens with
	// oversized popup menus on small screens), ignore it
	if ( rect().contains( e->pos() ) && motion < 6 )
	    return;
	else
	    byeMenuBar();
    } else {	// selected menu item!
	register QMenuItem *mi = mitems->at(actItem);
	if ( mi ->widget() ) {
	    QWidget* widgetAt = QApplication::widgetAt( e->globalPos(), TRUE );
	    if ( widgetAt && widgetAt != this ) {
		QMouseEvent me( e->type(), widgetAt->mapFromGlobal( e->globalPos() ),
				e->globalPos(), e->button(), e->state() );
		QApplication::sendEvent( widgetAt, &me );
	    }
	}
	QPopupMenu *popup = mi->popup();
#ifndef QT_NO_WHATSTHIS
	    bool b = QWhatsThis::inWhatsThisMode();
#else
	    const bool b = FALSE;
#endif
	if ( !mi->isEnabledAndVisible() ) {
#ifndef QT_NO_WHATSTHIS
	    if ( b ) {
		actItem = -1;
		updateItem( mi->id() );
		byeMenuBar();
		actSig( mi->id(), b);
	    }
#endif
	} else 	if ( popup ) {
	    popup->setFirstItemActive();
	} else {				// normal menu item
	    byeMenuBar();			// deactivate menu bar
	    if ( mi->isEnabledAndVisible() ) {
		actItem = -1;
		updateItem( mi->id() );
		active_popup_menu = this;
		QGuardedPtr<QSignal> signal = mi->signal();
		actSig( mi->id(), b );
		if ( signal && !b )
		    signal->activate();
		active_popup_menu = 0;
	    }
	}
    }
}

/*!
    \reimp
*/

void QPopupMenu::mouseMoveEvent( QMouseEvent *e )
{
    motion++;

    if ( parentMenu && parentMenu->isPopupMenu ) {
	QPopupMenu* p = (QPopupMenu*)parentMenu;
	int myIndex;

	p->findPopup( this, &myIndex );
	QPoint pPos = p->mapFromParent( e->globalPos() );
	if ( p->actItem != myIndex && !p->rect().contains( pPos ) )
	    p->setActiveItem( myIndex );

	if ( style().styleHint(QStyle::SH_PopupMenu_SloppySubMenus, this )) {
	    p->d->mouseMoveBuffer = QRegion();
#ifdef DEBUG_SLOPPY_SUBMENU
	    p->repaint();
#endif
	}
    }

    if ( (e->state() & Qt::MouseButtonMask) == 0 &&
	 !hasMouseTracking() )
	return;

    if(d->scroll.scrollable && e->pos().x() >= rect().x() && e->pos().x() <= rect().width()) {
        int sh = style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
        if((d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollUp && e->pos().y() <= sh) || 
           (d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown && e->pos().y() >= height()-sh)) {
            if(!d->scroll.scrolltimer) {
                d->scroll.scrolltimer = new QTimer(this, "popup scroll timer");
                QObject::connect( d->scroll.scrolltimer, SIGNAL(timeout()),
                                  this, SLOT(subScrollTimer()) );
            }
            if(!d->scroll.scrolltimer->isActive())
                d->scroll.scrolltimer->start(40);
            return;
        }
    }

    int	 item = itemAtPos( e->pos() );
    if ( item == -1 ) {				// no valid item
	int lastActItem = actItem;
	actItem = -1;
	if ( lastActItem >= 0 )
	    updateRow( lastActItem );
        if ( lastActItem > 0 ||
		    ( !rect().contains( e->pos() ) && !tryMenuBar( e ) ) ) {
	    popupSubMenuLater(style().styleHint(QStyle::SH_PopupMenu_SubMenuPopupDelay,
						this), this);
	}
    } else {					// mouse on valid item
	// but did not register mouse press
	if ( (e->state() & Qt::MouseButtonMask) && !mouseBtDn )
	    mouseBtDn = TRUE; // so mouseReleaseEvent will pop down

	register QMenuItem *mi = mitems->at( item );

	if ( mi->widget() ) {
	    QWidget* widgetAt = QApplication::widgetAt( e->globalPos(), TRUE );
	    if ( widgetAt && widgetAt != this ) {
		QMouseEvent me( e->type(), widgetAt->mapFromGlobal( e->globalPos() ),
				e->globalPos(), e->button(), e->state() );
		QApplication::sendEvent( widgetAt, &me );
	    }
	}

	if ( actItem == item )
	    return;

	if ( style().styleHint(QStyle::SH_PopupMenu_SloppySubMenus, this) &&
	     d->mouseMoveBuffer.contains( e->pos() ) ) {
	    actItem = item;
	    popupSubMenuLater( style().styleHint(QStyle::SH_PopupMenu_SubMenuPopupDelay, this) * 6,
			       this );
	    return;
	}

	if ( mi->popup() || ( popupActive >= 0 && popupActive != item ))
	    popupSubMenuLater( style().styleHint(QStyle::SH_PopupMenu_SubMenuPopupDelay, this),
			       this );
	else if ( singleSingleShot )
	    singleSingleShot->stop();

	if ( item != actItem )
	    setActiveItem( item );
    }
}


/*!
    \reimp
*/

void QPopupMenu::keyPressEvent( QKeyEvent *e )
{
    /*
      I get nothing but complaints about this.  -Brad

      - if (mouseBtDn && actItem >= 0) {
      -	if (e->key() == Key_Shift ||
      -	    e->key() == Key_Control ||
      -	    e->key() == Key_Alt)
      -	    return;
      -
      -	QMenuItem *mi = mitems->at(actItem);
      -	int modifier = (((e->state() & ShiftButton) ? SHIFT : 0) |
      -			((e->state() & ControlButton) ? CTRL : 0) |
      -			((e->state() & AltButton) ? ALT : 0));
      -
      - #ifndef QT_NO_ACCEL
      -	if (mi)
      -	    setAccel(modifier + e->key(), mi->id());
      - #endif
      - return;
      - }
    */

    QMenuItem  *mi = 0;
    QPopupMenu *popup;
    int dy = 0;
    bool ok_key = TRUE;

    int key = e->key();
    if ( QApplication::reverseLayout() ) {
	// in reverse mode opening and closing keys for submenues are reversed
	if ( key == Key_Left )
	    key = Key_Right;
	else if ( key == Key_Right )
	    key = Key_Left;
    }

    switch ( key ) {
    case Key_Tab:
	// ignore tab, otherwise it will be passed to the menubar
	break;

    case Key_Up:
	dy = -1;
	break;

    case Key_Down:
	dy = 1;
	break;

    case Key_Alt:
	if ( style().styleHint(QStyle::SH_MenuBar_AltKeyNavigation, this) )
	    byeMenuBar();
	break;

    case Key_Escape:
	if ( tornOff ) {
	    close();
	    return;
	}
	// just hide one
	{
	    QMenuData* p = parentMenu;
	    hide();
#ifndef QT_NO_MENUBAR
	    if ( p && p->isMenuBar )
		((QMenuBar*) p)->goodbye( TRUE );
#endif
	}
	break;

    case Key_Left:
	if ( ncols > 1 && actItem >= 0 ) {
	    QRect r( itemGeometry( actItem ) );
	    int newActItem = itemAtPos( QPoint( r.left() - 1, r.center().y() ) );
	    if ( newActItem >= 0 ) {
		setActiveItem( newActItem );
		break;
	    }
	}
	if ( parentMenu && parentMenu->isPopupMenu ) {
	    ((QPopupMenu *)parentMenu)->hidePopups();
	    if ( singleSingleShot )
		singleSingleShot->stop();
	    break;
	}

	ok_key = FALSE;
    	break;

    case Key_Right:
	if ( actItem >= 0 && ( mi=mitems->at(actItem) )->isEnabledAndVisible() && (popup=mi->popup()) ) {
	    hidePopups();
	    if ( singleSingleShot )
		singleSingleShot->stop();
	    // ### The next two lines were switched to fix the problem with the first item of the
	    // submenu not being highlighted...any reason why they should have been the other way??
	    subMenuTimer();
	    popup->setFirstItemActive();
	    break;
	} else if ( actItem == -1 && ( parentMenu && !parentMenu->isMenuBar )) {
	    dy = 1;
	    break;
	}
	if ( ncols > 1 && actItem >= 0 ) {
	    QRect r( itemGeometry( actItem ) );
	    int newActItem = itemAtPos( QPoint( r.right() + 1, r.center().y() ) );
	    if ( newActItem >= 0 ) {
		setActiveItem( newActItem );
		break;
	    }
	}
	ok_key = FALSE;
	break;

    case Key_Space:
	if (! style().styleHint(QStyle::SH_PopupMenu_SpaceActivatesItem, this))
	    break;
	// for motif, fall through

    case Key_Return:
    case Key_Enter:
	{
	    if ( actItem < 0 )
		break;
#ifndef QT_NO_WHATSTHIS
	    bool b = QWhatsThis::inWhatsThisMode();
#else
	    const bool b = FALSE;
#endif
	    mi = mitems->at( actItem );
	    if ( !mi->isEnabled() && !b )
		break;
	    popup = mi->popup();
	    if ( popup ) {
		hidePopups();
		popupSubMenuLater( 20, this );
		popup->setFirstItemActive();
	    } else {
		actItem = -1;
		updateItem( mi->id() );
		byeMenuBar();
		if ( mi->isEnabledAndVisible() || b ) {
		    active_popup_menu = this;
		    QGuardedPtr<QSignal> signal = mi->signal();
		    actSig( mi->id(), b );
		    if ( signal && !b )
			signal->activate();
		    active_popup_menu = 0;
		}
	    }
	}
	break;
#ifndef QT_NO_WHATSTHIS
    case Key_F1:
	if ( actItem < 0 || e->state() != ShiftButton)
	    break;
	mi = mitems->at( actItem );
	if ( !mi->whatsThis().isNull() ){
	    if ( !QWhatsThis::inWhatsThisMode() )
		QWhatsThis::enterWhatsThisMode();
	    QRect r( itemGeometry( actItem) );
	    QWhatsThis::leaveWhatsThisMode( mi->whatsThis(), mapToGlobal( r.bottomLeft()) );
	}
	//fall-through!
#endif
    default:
	ok_key = FALSE;

    }
    if ( !ok_key &&
	 ( !e->state() || e->state() == AltButton || e->state() == ShiftButton ) &&
	 e->text().length()==1 ) {
	QChar c = e->text()[0].upper();

	QMenuItemListIt it(*mitems);
	QMenuItem* first = 0;
	QMenuItem* currentSelected = 0;
	QMenuItem* firstAfterCurrent = 0;

	register QMenuItem *m;
	mi = 0;
	int indx = 0;
	int clashCount = 0;
	while ( (m=it.current()) ) {
	    ++it;
	    QString s = m->text();
	    if ( !s.isEmpty() ) {
		int i = s.find( '&' );
		while ( i >= 0 && i < (int)s.length() - 1 ) {
		    if ( s[i+1].upper() == c ) {
			ok_key = TRUE;
			clashCount++;
			if ( !first )
			    first = m;
			if ( indx == actItem )
			    currentSelected = m;
			else if ( !firstAfterCurrent && currentSelected )
			    firstAfterCurrent = m;
			break;
		    } else if ( s[i+1] == '&' ) {
			i = s.find( '&', i+2 );
		    } else {
			break;
		    }
		}
	    }
	    if ( mi )
		break;
	    indx++;
	}

	if ( 1 == clashCount ) { // No clashes, continue with selection
	    mi = first;
	    popup = mi->popup();
	    if ( popup ) {
		setActiveItem( indexOf(mi->id()) );
		hidePopups();
		popupSubMenuLater( 20, this );
		popup->setFirstItemActive();
	    } else {
		byeMenuBar();
#ifndef QT_NO_WHATSTHIS
		bool b = QWhatsThis::inWhatsThisMode();
#else
		const bool b = FALSE;
#endif
		if ( mi->isEnabledAndVisible() || b ) {
		    active_popup_menu = this;
		    QGuardedPtr<QSignal> signal = mi->signal();
		    actSig( mi->id(), b );
		    if ( signal && !b  )
			signal->activate();
		    active_popup_menu = 0;
		}
	    }
	} else if ( clashCount > 1 ) { // Clashes, highlight next...
	    // If there's clashes and no one is selected, use first one
	    // or if there is no clashes _after_ current, use first one
	    if ( !currentSelected || (currentSelected && !firstAfterCurrent))
		dy = indexOf( first->id() ) - actItem;
	    else
		dy = indexOf( firstAfterCurrent->id() ) - actItem;
	}
    }
#ifndef QT_NO_MENUBAR
    if ( !ok_key ) {				// send to menu bar
	register QMenuData *top = this;		// find top level
	while ( top->parentMenu )
	    top = top->parentMenu;
	if ( top->isMenuBar ) {
	    int beforeId = top->actItem;
	    ((QMenuBar*)top)->tryKeyEvent( this, e );
	    if ( beforeId != top->actItem )
		ok_key = TRUE;
	}
    }
#endif
    if ( actItem < 0 ) {
	if ( dy > 0 ) {
	    setFirstItemActive();
	} else if ( dy < 0 ) {
	    QMenuItemListIt it(*mitems);
	    it.toLast();
	    register QMenuItem *mi;
	    int ai = count() - 1;
	    while ( (mi=it.current()) ) {
		--it;
		if ( !mi->isSeparator() && mi->id() != QMenuData::d->aInt ) {
		    setActiveItem( ai );
		    return;
		}
		ai--;
	    }
	    actItem = -1;
	}
	return;
    }

    if ( dy ) {				// highlight next/prev
	register int i = actItem;
	int c = mitems->count();
	for(int n = c; n; n--) {
	    i = i + dy;
	    if(d->scroll.scrollable) {
		if(d->scroll.scrolltimer)
		    d->scroll.scrolltimer->stop();
		if(i < 0)
		    i = 0;
		else if(i >= c)
		    i  = c - 1;
	    } else {
		if ( i == c )
		    i = 0;
		else if ( i < 0 )
		    i = c - 1;
	    }
	    mi = mitems->at( i );
	    if ( !mi || !mi->isVisible() )
		continue;

	    if ( !mi->isSeparator() &&
		 ( style().styleHint(QStyle::SH_PopupMenu_AllowActiveAndDisabled, this)
		   || mi->isEnabledAndVisible() ) )
		break;
	}
	if ( i != actItem )
	    setActiveItem( i );
	if(d->scroll.scrollable) { //need to scroll to make it visible?
	    QRect r = itemGeometry(actItem);
	    if(r.isNull() || r.height() < itemHeight(mitems->at(actItem))) {
		bool refresh = FALSE;
		if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollUp && dy == -1) { //up
		    if(d->scroll.topScrollableIndex >= 0) {
			d->scroll.topScrollableIndex--;
			refresh = TRUE;
		    }
		} else if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown) { //down
		    QMenuItemListIt it(*mitems);
		    int sh = style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
		    for(int i = 0, y = ((d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollUp) ? sh : 0); it.current(); i++, ++it) {
			if(i >= d->scroll.topScrollableIndex) {
			    int itemh = itemHeight(it.current());
			    QSize sz = style().sizeFromContents(QStyle::CT_PopupMenuItem, this,
								QSize(0, itemh),
								QStyleOption(it.current(),maxPMWidth,0));
			    y += sz.height();
			    if(y > (contentsRect().height()-sh)) {
				if(sz.height() > sh || !it.atLast())
				    d->scroll.topScrollableIndex++;
				refresh = TRUE;
				break;
			    }
			}
		    }
		}
		if(refresh) {
		    updateScrollerState();
		    update();
		}
	    }
	}
    }

#ifdef Q_OS_WIN32
    if ( !ok_key &&
	!( e->key() == Key_Control || e->key() == Key_Shift || e->key() == Key_Meta ) )
	qApp->beep();
#endif // Q_OS_WIN32
}


/*!
    \reimp
*/

void QPopupMenu::timerEvent( QTimerEvent *e )
{
    QFrame::timerEvent( e );
}

/*!
    \reimp
*/
void QPopupMenu::leaveEvent( QEvent * )
{
    if ( testWFlags( WStyle_Tool ) && style().styleHint(QStyle::SH_PopupMenu_MouseTracking, this) ) {
	int lastActItem = actItem;
	actItem = -1;
	if ( lastActItem >= 0 )
	    updateRow( lastActItem );
    }
}

/*!
    \reimp
*/
void QPopupMenu::styleChange( QStyle& old )
{
    QFrame::styleChange( old );
    setMouseTracking(style().styleHint(QStyle::SH_PopupMenu_MouseTracking, this));
    style().polishPopupMenu( this );
    updateSize(TRUE);
}

/*!\reimp
 */
void QPopupMenu::enabledChange( bool )
{
    if ( QMenuData::d->aWidget ) // torn-off menu
	QMenuData::d->aWidget->setEnabled( isEnabled() );
}


/*!
    If a popup menu does not fit on the screen it lays itself out so
    that it does fit. It is style dependent what layout means (for
    example, on Windows it will use multiple columns).

    This functions returns the number of columns necessary.

    \sa sizeHint()
*/
int QPopupMenu::columns() const
{
    return ncols;
}

/* This private slot handles the scrolling popupmenu */
void QPopupMenu::subScrollTimer() {
    QPoint pos = QCursor::pos();
    if(!d->scroll.scrollable || !isVisible()) {
	if(d->scroll.scrolltimer)
	    d->scroll.scrolltimer->stop();
	return;
    } else if(pos.x() > x() + width() || pos.x() < x()) {
	return;
    }
    int sh = style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
    if(!d->scroll.lastScroll.isValid()) {
	d->scroll.lastScroll = QTime::currentTime();
    } else {
	int factor=0;
	if(pos.y() < y())
	    factor = y() - pos.y();
	else if(pos.y() > y() + height())
	    factor = pos.y() - (y() + height());
	int msecs = 250 - ((factor / 10) * 40);
	if(d->scroll.lastScroll.msecsTo(QTime::currentTime()) < QMAX(0, msecs))
	    return;
	d->scroll.lastScroll = QTime::currentTime();
    }
    if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollUp && pos.y() <= y() + sh) { //up
	if(d->scroll.topScrollableIndex > 0) {
	    d->scroll.topScrollableIndex--;
	    updateScrollerState();
	    update(contentsRect());
	}
    } else if(d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollDown &&
	      pos.y() >= (y() + contentsRect().height()) - sh) { //down
	QMenuItemListIt it(*mitems);
	for(int i = 0, y = contentsRect().y() + sh; it.current(); i++, ++it) {
	    if(i >= d->scroll.topScrollableIndex) {
		int itemh = itemHeight(it.current());
		QSize sz = style().sizeFromContents(QStyle::CT_PopupMenuItem, this, QSize(0, itemh),
						    QStyleOption(it.current(),maxPMWidth,0));
		y += sz.height();
		if(y > contentsRect().height() - sh) {
		    d->scroll.topScrollableIndex++;
		    updateScrollerState();
		    update(contentsRect());
		    break;
		}
	    }
	}
    }
}

/* This private slot handles the delayed submenu effects */

void QPopupMenu::subMenuTimer() {

    if ( !isVisible() || (actItem < 0 && popupActive < 0) || actItem == popupActive )
	return;

    if ( popupActive >= 0 ) {
	hidePopups();
	popupActive = -1;
    }

    // hidePopups() may change actItem etc.
    if ( !isVisible() || actItem < 0 || actItem == popupActive )
	return;

    QMenuItem *mi = mitems->at(actItem);
    if ( !mi || !mi->isEnabledAndVisible() )
	return;

    QPopupMenu *popup = mi->popup();
    if ( !popup || !popup->isEnabled() )
	return;

    //avoid circularity
    if ( popup->isVisible() )
	return;

    Q_ASSERT( popup->parentMenu == 0 );
    popup->parentMenu = this;			// set parent menu

    emit popup->aboutToShow();
    supressAboutToShow = TRUE;


    QRect r( itemGeometry( actItem ) );
    QPoint p;
    QSize ps = popup->sizeHint();
    if( QApplication::reverseLayout() ) {
	p = QPoint( r.left() + motifArrowHMargin - ps.width(), r.top() + motifArrowVMargin );
	p = mapToGlobal( p );

	bool right = FALSE;
	if ( ( parentMenu && parentMenu->isPopupMenu &&
	       ((QPopupMenu*)parentMenu)->geometry().x() < geometry().x() ) ||
	     p.x() < 0 )
	    right = TRUE;
	if ( right && (ps.width() > QApplication::desktop()->width() - mapToGlobal( r.topRight() ).x() ) )
	    right = FALSE;
	if ( right )
	    p.setX( mapToGlobal( r.topRight() ).x() );
    } else {
	p = QPoint( r.right() - motifArrowHMargin, r.top() + motifArrowVMargin );
	p = mapToGlobal( p );

	bool left = FALSE;
	if ( ( parentMenu && parentMenu->isPopupMenu &&
	       ((QPopupMenu*)parentMenu)->geometry().x() > geometry().x() ) ||
	     p.x() + ps.width() > QApplication::desktop()->width() )
	    left = TRUE;
	if ( left && (ps.width() > mapToGlobal( r.topLeft() ).x() ) )
	    left = FALSE;
	if ( left )
	    p.setX( mapToGlobal( r.topLeft() ).x() - ps.width() );
    }
    QRect pr = popup->itemGeometry(popup->count() - 1);
    if (p.y() + ps.height() > QApplication::desktop()->height() &&
	p.y() - ps.height() + (QCOORD) pr.height() >= 0)
	p.setY( p.y() - ps.height() + (QCOORD) pr.height());

    if ( style().styleHint(QStyle::SH_PopupMenu_SloppySubMenus, this )) {
	 QPoint cur = QCursor::pos();
	 if ( r.contains( mapFromGlobal( cur ) ) ) {
	     QPoint pts[4];
	     pts[0] = QPoint( cur.x(), cur.y() - 2 );
	     pts[3] = QPoint( cur.x(), cur.y() + 2 );
	     if ( p.x() >= cur.x() )	{
		 pts[1] = QPoint( geometry().right(), p.y() );
		 pts[2] = QPoint( geometry().right(), p.y() + ps.height() );
	     } else {
		 pts[1] = QPoint( p.x() + ps.width(), p.y() );
		 pts[2] = QPoint( p.x() + ps.width(), p.y() + ps.height() );
	     }
	     QPointArray points( 4 );
	     for( int i = 0; i < 4; i++ )
		 points.setPoint( i, mapFromGlobal( pts[i] ) );
	     d->mouseMoveBuffer = QRegion( points );
	     repaint();
	 }
    }

    popupActive = actItem;
    popup->popup( p );
}

void QPopupMenu::allowAnimation()
{
    preventAnimation = FALSE;
}

void QPopupMenu::updateRow( int row )
{
    if ( !isVisible() )
	return;

    if ( badSize ) {
	updateSize();
	update();
	return;
    }
    updateSize();
    QRect r = itemGeometry( row );
    if ( !r.isNull() ) // can happen via the scroller
	repaint( r );
}


/*!
    \overload

    Executes this popup synchronously.

    Opens the popup menu so that the item number \a indexAtPoint will
    be at the specified \e global position \a pos. To translate a
    widget's local coordinates into global coordinates, use
    QWidget::mapToGlobal().

    The return code is the id of the selected item in either the popup
    menu or one of its submenus, or -1 if no item is selected
    (normally because the user pressed Esc).

    Note that all signals are emitted as usual. If you connect a menu
    item to a slot and call the menu's exec(), you get the result both
    via the signal-slot connection and in the return value of exec().

    Common usage is to position the popup at the current mouse
    position:
    \code
	exec( QCursor::pos() );
    \endcode
    or aligned to a widget:
    \code
	exec( somewidget.mapToGlobal(QPoint(0, 0)) );
    \endcode

    When positioning a popup with exec() or popup(), bear in mind that
    you cannot rely on the popup menu's current size(). For
    performance reasons, the popup adapts its size only when
    necessary. So in many cases, the size before and after the show is
    different. Instead, use sizeHint(). It calculates the proper size
    depending on the menu's current contents.

    \sa popup(), sizeHint()
*/

int QPopupMenu::exec( const QPoint & pos, int indexAtPoint )
{
    snapToMouse = TRUE;
    if ( !qApp )
	return -1;

    QPopupMenu* priorSyncMenu = syncMenu;

    syncMenu = this;
    syncMenuId = -1;

    QGuardedPtr<QPopupMenu> that = this;
    connectModal( that, TRUE );
    popup( pos, indexAtPoint );
    qApp->enter_loop();
    connectModal( that, FALSE );

    syncMenu = priorSyncMenu;
    return syncMenuId;
}



/*
  Connect the popup and all its submenus to modalActivation() if
  \a doConnect is true, otherwise disconnect.
 */
void QPopupMenu::connectModal( QPopupMenu* receiver, bool doConnect )
{
    if ( !receiver )
	return;

    connectModalRecursionSafety = doConnect;

    if ( doConnect )
	connect( this, SIGNAL(activated(int)),
		 receiver, SLOT(modalActivation(int)) );
    else
	disconnect( this, SIGNAL(activated(int)),
		    receiver, SLOT(modalActivation(int)) );

    QMenuItemListIt it(*mitems);
    register QMenuItem *mi;
    while ( (mi=it.current()) ) {
	++it;
	if ( mi->popup() && mi->popup() != receiver
	     && (bool)(mi->popup()->connectModalRecursionSafety) != doConnect )
	    mi->popup()->connectModal( receiver, doConnect ); //avoid circular
    }
}


/*!
    Executes this popup synchronously.

    This is equivalent to \c{exec(mapToGlobal(QPoint(0,0)))}. In most
    situations you'll want to specify the position yourself, for
    example at the current mouse position:
    \code
	exec(QCursor::pos());
    \endcode
    or aligned to a widget:
    \code
	exec(somewidget.mapToGlobal(QPoint(0,0)));
    \endcode
*/

int QPopupMenu::exec()
{
    return exec(mapToGlobal(QPoint(0,0)));
}


/*  Internal slot used for exec(). */

void QPopupMenu::modalActivation( int id )
{
    syncMenuId = id;
}


/*!
    Sets the currently active item to index \a i and repaints as necessary.
*/

void QPopupMenu::setActiveItem( int i )
{
    int lastActItem = actItem;
    actItem = i;
    if ( lastActItem >= 0 )
	updateRow( lastActItem );
    if ( i >= 0 && i != lastActItem )
	updateRow( i );
    QMenuItem *mi = mitems->at( actItem );
    if ( !mi )
	return;

    if ( mi->widget() && mi->widget()->isFocusEnabled() ) {
	mi->widget()->setFocus();
    } else {
	setFocus();
	QRect mfrect = itemGeometry( actItem );
	setMicroFocusHint( mfrect.x(), mfrect.y(), mfrect.width(), mfrect.height(), FALSE );
    }
    if ( mi->id() != -1 )
	hilitSig( mi->id() );
#ifndef QT_NO_WHATSTHIS
    if (whatsThisItem && whatsThisItem != mi) {
	qWhatsThisBDH();
    }
    whatsThisItem = mi;
#endif
}


/*!
    \reimp
*/
QSize QPopupMenu::sizeHint() const
{
    constPolish();
    QPopupMenu* that = (QPopupMenu*) this;
    //We do not need a resize here, just the sizeHint..
    return that->updateSize(FALSE, FALSE).expandedTo( QApplication::globalStrut() );
}


/*!
    \overload

    Returns the id of the item at \a pos, or -1 if there is no item
    there or if it is a separator.
*/
int QPopupMenu::idAt( const QPoint& pos ) const
{
    return idAt( itemAtPos( pos ) );
}


/*!
    \fn int QPopupMenu::idAt( int index ) const

    Returns the identifier of the menu item at position \a index in
    the internal list, or -1 if \a index is out of range.

    \sa QMenuData::setId(), QMenuData::indexOf()
*/


/*!
    \reimp
 */
bool QPopupMenu::customWhatsThis() const
{
    return TRUE;
}


/*!
    \reimp
 */
bool QPopupMenu::focusNextPrevChild( bool next )
{
    register QMenuItem *mi;
    int dy = next? 1 : -1;
    if ( dy && actItem < 0 ) {
	setFirstItemActive();
    } else if ( dy ) {				// highlight next/prev
	register int i = actItem;
	int c = mitems->count();
	int n = c;
	while ( n-- ) {
	    i = i + dy;
	    if ( i == c )
		i = 0;
	    else if ( i < 0 )
		i = c - 1;
	    mi = mitems->at( i );
	    if ( mi && !mi->isSeparator() &&
		 ( ( style().styleHint(QStyle::SH_PopupMenu_AllowActiveAndDisabled, this)
		     && mi->isVisible() )
		   || mi->isEnabledAndVisible() ) )
		break;
	}
	if ( i != actItem )
	    setActiveItem( i );
    }
    return TRUE;
}


/*!
    \reimp
 */
void QPopupMenu::focusInEvent( QFocusEvent * )
{
}

/*!
    \reimp
 */
void QPopupMenu::focusOutEvent( QFocusEvent * )
{
}


class QTearOffMenuItem : public QCustomMenuItem
{
public:
    QTearOffMenuItem()
    {
    }
    ~QTearOffMenuItem()
    {
    }
    void paint( QPainter* p, const QColorGroup& cg, bool /* act*/,
		bool /*enabled*/, int x, int y, int w, int h )
    {
	p->setPen( QPen( cg.dark(), 1, DashLine ) );
	p->drawLine( x+2, y+h/2-1, x+w-4, y+h/2-1 );
	p->setPen( QPen( cg.light(), 1, DashLine ) );
	p->drawLine( x+2, y+h/2, x+w-4, y+h/2 );
    }
    bool fullSpan() const
    {
	return TRUE;
    }

    QSize sizeHint()
    {
	return QSize( 20, 6 );
    }
};



/*!
    Inserts a tear-off handle into the menu. A tear-off handle is a
    special menu item that creates a copy of the menu when the menu is
    selected. This "torn-off" copy lives in a separate window. It
    contains the same menu items as the original menu, with the
    exception of the tear-off handle.

    The handle item is assigned the identifier \a id or an
    automatically generated identifier if \a id is < 0. The generated
    identifiers (negative integers) are guaranteed to be unique within
    the entire application.

    The \a index specifies the position in the menu. The tear-off
    handle is appended at the end of the list if \a index is negative.
*/
int QPopupMenu::insertTearOffHandle( int id, int index )
{
    int myid = insertItem( new QTearOffMenuItem, id, index );
    connectItem( myid, this, SLOT( toggleTearOff() ) );
    QMenuData::d->aInt = myid;
    return myid;
}


/*!\internal

  implements tear-off menus
 */
void QPopupMenu::toggleTearOff()
{
    if ( active_popup_menu && active_popup_menu->tornOff ) {
	active_popup_menu->close();
    } else  if (QMenuData::d->aWidget ) {
	delete (QWidget*) QMenuData::d->aWidget; // delete the old one
    } else {
	// create a tear off menu
	QPopupMenu* p = new QPopupMenu( parentWidget(), "tear off menu" );
	connect( p, SIGNAL( activated(int) ), this, SIGNAL( activated(int) ) );
        connect( p, SIGNAL( highlighted(int) ), this, SIGNAL( highlighted(int) ) );
#ifndef QT_NO_WIDGET_TOPEXTRA
	p->setCaption( caption() );
#endif
	p->setCheckable( isCheckable() );
	p->reparent( parentWidget(), WType_TopLevel | WStyle_Tool |
		     WNoAutoErase | WDestructiveClose,
		     geometry().topLeft(), FALSE );
	p->mitems->setAutoDelete( FALSE );
	p->tornOff = TRUE;
	for ( QMenuItemListIt it( *mitems ); it.current(); ++it ) {
	    if ( it.current()->id() != QMenuData::d->aInt && !it.current()->widget() )
		p->mitems->append( it.current() );
	}
	p->show();
	QMenuData::d->aWidget = p;
    }
}

/*!
    \reimp
 */
void QPopupMenu::activateItemAt( int index )
{
    if ( index >= 0 && index < (int) mitems->count() ) {
	QMenuItem *mi = mitems->at( index );
	if ( index != actItem )			// new item activated
	    setActiveItem( index );
	QPopupMenu *popup = mi->popup();
	if ( popup ) {
	    if ( popup->isVisible() ) {		// sub menu already open
		int pactItem = popup->actItem;
		popup->actItem = -1;
		popup->hidePopups();
		popup->updateRow( pactItem );
	    } else {				// open sub menu
		hidePopups();
		actItem = index;
		subMenuTimer();
		popup->setFirstItemActive();
	    }
	} else {
	    byeMenuBar();			// deactivate menu bar

#ifndef QT_NO_WHATSTHIS
	    bool b = QWhatsThis::inWhatsThisMode();
#else
	    const bool b = FALSE;
#endif
	    if ( !mi->isEnabledAndVisible() ) {
#ifndef QT_NO_WHATSTHIS
		if ( b ) {
		    actItem = -1;
		    updateItem( mi->id() );
		    byeMenuBar();
		    actSig( mi->id(), b);
		}
#endif
	    } else {
		byeMenuBar();			// deactivate menu bar
		if ( mi->isEnabledAndVisible() ) {
		    actItem = -1;
		    updateItem( mi->id() );
		    active_popup_menu = this;
		    QGuardedPtr<QSignal> signal = mi->signal();
		    actSig( mi->id(), b );
		    if ( signal && !b )
			signal->activate();
		    active_popup_menu = 0;
		}
	    }
	}
    } else {
	if ( tornOff ) {
	    close();
	} else {
	    QMenuData* p = parentMenu;
	    hide();
#ifndef QT_NO_MENUBAR
	    if ( p && p->isMenuBar )
		((QMenuBar*) p)->goodbye( TRUE );
#endif
	}
    }

}

/*! \internal
  This private function is to update the scroll states in styles that support scrolling. */
void
QPopupMenu::updateScrollerState()
{
    uint old_scrollable = d->scroll.scrollable;
    d->scroll.scrollable = QPopupMenuPrivate::Scroll::ScrollNone;
    if(!style().styleHint(QStyle::SH_PopupMenu_Scrollable, this))
	return;

    QMenuItem *mi;
    QMenuItemListIt it( *mitems );
    if(d->scroll.topScrollableIndex) {
	for(int row = 0; (mi = it.current()) && row < d->scroll.topScrollableIndex; row++)
	    ++it;
	if(!mi)
	    it.toFirst();
    }
    int y = 0, sh = style().pixelMetric(QStyle::PM_PopupMenuScrollerHeight, this);
    if(!it.atFirst()) {
	// can't use |= because of a bug/feature in IBM xlC 5.0.2
	d->scroll.scrollable = d->scroll.scrollable | QPopupMenuPrivate::Scroll::ScrollUp;
	y += sh;
    }
    while ( (mi=it.current()) ) {
	++it;
	int myheight = contentsRect().height();
	QSize sz = style().sizeFromContents(QStyle::CT_PopupMenuItem, this,
					    QSize(0, itemHeight( mi )),
					    QStyleOption(mi,maxPMWidth));
	if(y + sz.height() >= myheight) {
	    d->scroll.scrollable = d->scroll.scrollable | QPopupMenuPrivate::Scroll::ScrollDown;
	    break;
	}
	y += sz.height();
    }
    if((d->scroll.scrollable & QPopupMenuPrivate::Scroll::ScrollUp) &&
       !(old_scrollable & QPopupMenuPrivate::Scroll::ScrollUp))
	d->scroll.topScrollableIndex++;
}

#endif // QT_NO_POPUPMENU

