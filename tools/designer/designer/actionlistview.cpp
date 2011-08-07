/**********************************************************************
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Designer.
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
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with
** the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#include "actiondnd.h"
#include "actionlistview.h"
#include <qheader.h>

ActionListView::ActionListView( QWidget *parent, const char *name )
    : QListView( parent, name )
{
    setShowSortIndicator( TRUE );
    setResizeMode( LastColumn );
    setRootIsDecorated( TRUE );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ),
	     this, SLOT( rmbMenu( QListViewItem *, const QPoint & ) ) );
}

ActionItem::ActionItem( QListView *lv, QAction *ac )
    : QListViewItem( lv ), a( 0 ), g( 0 )
{
    g = ::qt_cast<QDesignerActionGroup*>(ac);
    if ( !g )
	a = ::qt_cast<QDesignerAction*>(ac);
    setDragEnabled( TRUE );
}

ActionItem::ActionItem( QListViewItem *i, QAction *ac )
    : QListViewItem( i ), a( 0 ), g( 0 )
{
    g = ::qt_cast<QDesignerActionGroup*>(ac);
    if ( !g )
	a = ::qt_cast<QDesignerAction*>(ac);
    setDragEnabled( TRUE );
    moveToEnd();
}

void ActionItem::moveToEnd()
{
    QListViewItem *i = this;
    while ( i->nextSibling() )
	i = i->nextSibling();
    if ( i != this )
	moveItem( i );
}

QDragObject *ActionListView::dragObject()
{
    ActionItem *i = (ActionItem*)currentItem();
    if ( !i )
	return 0;
    QStoredDrag *drag = 0;
    if ( i->action() ) {
	drag = new ActionDrag( i->action(), viewport() );
	drag->setPixmap( i->action()->iconSet().pixmap() );
    } else {
	drag = new ActionDrag( i->actionGroup(), viewport() );
	drag->setPixmap( i->actionGroup()->iconSet().pixmap() );
    }
    return drag;
}

void ActionListView::rmbMenu( QListViewItem *i, const QPoint &p )
{
    QPopupMenu *popup = new QPopupMenu( this );
    popup->insertItem( tr( "New &Action" ), 0 );
    popup->insertItem( tr( "New Action &Group" ), 1 );
    popup->insertItem( tr( "New &Dropdown Action Group" ), 2 );
    if ( i ) {
	popup->insertSeparator();
	popup->insertItem( tr( "&Connect Action..." ), 3 );
	popup->insertSeparator();
	popup->insertItem( tr( "Delete Action" ), 4 );
    }
    int res = popup->exec( p );
    if ( res == 0 )
	emit insertAction();
    else if ( res == 1 )
	emit insertActionGroup();
    else if ( res == 2 )
	emit insertDropDownActionGroup();
    else if ( res == 3 )
	emit connectAction();
    else if ( res == 4 )
	emit deleteAction();
}
