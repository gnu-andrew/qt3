/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <qapplication.h>
#include <qsqldatabase.h>
#include <qsqlcursor.h>
#include <qdatatable.h>
#include "../connection.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    if ( createConnections() ) {
	QSqlCursor staffCursor( "staff" );
	QDataTable *staffTable = new QDataTable( &staffCursor, TRUE );
	app.setMainWidget( staffTable );
	staffTable->refresh();
	staffTable->show();

	return app.exec();
    }

    return 0;
}
