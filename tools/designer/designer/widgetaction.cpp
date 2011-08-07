/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
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

#include "widgetaction.h"
#include <qwidget.h>
#include <qtoolbutton.h>
#include "mainwindow.h"
#include <qstylefactory.h>
#include <qstyle.h>

static QStyle *windowsStyle = 0;

void WidgetAction::addedTo( QWidget *w, QWidget *container )
{
    if ( ::qt_cast<QToolButton*>(w) && ::qt_cast<QScrollView*>(container->parent()->parent()) ) {
	if ( !windowsStyle )
	    windowsStyle = QStyleFactory::create( "windows" );
	w->setStyle( windowsStyle );
	( (QToolButton*)w )->setUsesTextLabel( TRUE );
	( (QToolButton*)w )->setTextPosition( QToolButton::Right );
	w->setBackgroundMode( container->backgroundMode() );
    }
}

WidgetAction::~WidgetAction()
{
    if ( MainWindow::self )
	MainWindow::self->toolActions.removeRef( this );
}

void WidgetAction::init( const QString &g )
{
    MainWindow::self->toolActions.append( this );
    grp = g;
}
