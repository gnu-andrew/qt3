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
#include <qsqlquery.h>
#include "../connection.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv, FALSE );

    if ( createConnections() ) {
	QSqlQuery query( "SELECT id, surname FROM staff" );
	if ( query.isActive() ) {
	    while ( query.next() ) {
		qDebug( query.value(0).toString() + ": " +
		        query.value(1).toString() );
	    }
	}
    }

    return 0;
}
