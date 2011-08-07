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
    QApplication app( argc, argv, FALSE );

    if ( createConnections() ) {
    	QSqlCursor cur( "prices" ); 
	cur.select( "id=202" );
	if ( cur.next() ) {
	    QSqlRecord *buffer = cur.primeUpdate();
	    double price = buffer->value( "price" ).toDouble();
	    double newprice = price * 1.05;
	    buffer->setValue( "price", newprice );
	    cur.update();
	}
    }

    return 0;
}
