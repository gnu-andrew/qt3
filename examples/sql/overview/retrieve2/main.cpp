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
#include "../connection.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    if ( createConnections() ) {
	QSqlCursor cur( "staff" ); // Specify the table/view name
	cur.select(); // We'll retrieve every record
	while ( cur.next() ) {
	    qDebug( cur.value( "id" ).toString() + ": " +
		    cur.value( "surname" ).toString() + " " +
		    cur.value( "salary" ).toString() );
	}
    }

    return 0;
}
