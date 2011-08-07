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

#if defined(DESIGNER)
#include "database.h"
#else
#include "database2.h"
#endif

#ifndef QT_NO_SQL

#if defined(DESIGNER)
#include "formwindow.h"
#include "mainwindow.h"
#endif

#include <qsqldatabase.h>
#include <qsqlform.h>
#include <qsqlcursor.h>
#include <qsqlrecord.h>

DatabaseSupport::DatabaseSupport()
{
    con = 0;
    frm = 0;
    parent = 0;
}

void DatabaseSupport::initPreview( const QString &connection, const QString &table, QObject *o,
				   const QMap<QString, QString> &databaseControls )
{
    tbl = table;
    dbControls = databaseControls;
    parent = o;

    if ( connection != "(default)" )
	con = QSqlDatabase::database( connection );
    else
	con = QSqlDatabase::database();
    frm = new QSqlForm( o, table );
    for ( QMap<QString, QString>::Iterator it = dbControls.begin(); it != dbControls.end(); ++it ) {
	QObject *chld = parent->child( it.key(), "QWidget" );
	if ( !chld )
	    continue;
	frm->insert( (QWidget*)chld, *it );
    }
}

QDesignerDataBrowser::QDesignerDataBrowser( QWidget *parent, const char *name )
    : QDataBrowser( parent, name )
{
}

bool QDesignerDataBrowser::event( QEvent* e )
{
    bool b = QDataBrowser::event( e );
#if defined(DESIGNER)
    if ( MainWindow::self->isPreviewing() ) {
#endif
	if ( e->type() == QEvent::Show ) {
	    if ( con ) {
		QSqlCursor* cursor = new QSqlCursor( tbl, TRUE, con );
		setSqlCursor( cursor, TRUE );
		setForm( frm );
		refresh();
		first();
	    }
	    return TRUE;
	}
#if defined(DESIGNER)
    }
#endif
    return b;
}

QDesignerDataView::QDesignerDataView( QWidget *parent, const char *name )
    : QDataView( parent, name )
{
}

bool QDesignerDataView::event( QEvent* e )
{
    bool b = QDataView::event( e );
#if defined(DESIGNER)
    if ( MainWindow::self->isPreviewing() ) {
#endif
	if ( e->type() == QEvent::Show ) {
	    setForm( frm );
	    readFields();
	    return TRUE;
	}
#if defined(DESIGNER)
    }
#endif
    return b;
}


#endif
