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

#include "listdnd.h"
#include <qwidget.h>
#include <qpainter.h>

ListDnd::ListDnd( QScrollView * eventSource, const char * name )
    : QObject( eventSource, name ),
      dragInside( FALSE ), dragDelete( TRUE ), dropConfirmed( FALSE ), dMode( Both )
{
    src = eventSource;
    src->setAcceptDrops( TRUE );
    src->installEventFilter( this );
    src->viewport()->installEventFilter( this );

    line = new QWidget( src->viewport(), 0, Qt::WStyle_NoBorder | WStyle_StaysOnTop );
    line->setBackgroundColor( Qt::black );
    line->resize( src->viewport()->width(), 2 );
    line->hide();
}

void ListDnd::setDragMode( int mode )
{
    dMode = mode;
}

int ListDnd::dragMode() const
{
    return dMode;
}

bool ListDnd::eventFilter( QObject *, QEvent * event )
{
    switch ( event->type() ) {
    case QEvent::DragEnter:
	return dragEnterEvent( (QDragEnterEvent *) event );
    case QEvent::DragLeave:
	return dragLeaveEvent( (QDragLeaveEvent *) event );
    case QEvent::DragMove:
	return dragMoveEvent( (QDragMoveEvent *) event );
    case QEvent::Drop:
	return dropEvent( (QDropEvent *) event );
    case QEvent::MouseButtonPress:
	return mousePressEvent( (QMouseEvent *) event );
    case QEvent::MouseMove:
	return mouseMoveEvent( (QMouseEvent *) event );
    default:
	break;
    }
    return FALSE;
}


bool ListDnd::dragEnterEvent( QDragEnterEvent * event )
{
    if ( dMode == None ) {
	return TRUE;
    }

    bool ok = ( ( ( dMode & Both ) == Both ) ||
		( ( dMode & Internal ) && ( event->source() == src ) ) ||
		( ( dMode & External ) && ( event->source() != src ) ) );

    if ( ok && canDecode( event ) ) {
	event->accept();
	dragInside = TRUE;
	if ( !( dMode & NullDrop ) ) {
	    updateLine( event->pos() );
	    line->show();
	}
    }
    return TRUE;
}

bool ListDnd::dragLeaveEvent( QDragLeaveEvent * )
{
    dragInside = FALSE;
    line->hide();
    return TRUE;
}

bool ListDnd::dragMoveEvent( QDragMoveEvent * event )
{
    if ( dragInside && dMode && !( dMode & NullDrop ) ) {
	updateLine( event->pos() );
    }
    return TRUE;
}

bool ListDnd::dropEvent( QDropEvent * )
{
    // do nothing
    return FALSE;
}

bool ListDnd::mousePressEvent( QMouseEvent * event )
{
    if ( event->button() == LeftButton )
	mousePressPos = event->pos();
    return FALSE;
}

bool ListDnd::mouseMoveEvent( QMouseEvent * )
{
    // do nothing
    return FALSE;
}

void ListDnd::updateLine( const QPoint & )
{
    // do nothing
}

bool ListDnd::canDecode( QDragEnterEvent * )
{
    // do nothing
    return FALSE;
}
