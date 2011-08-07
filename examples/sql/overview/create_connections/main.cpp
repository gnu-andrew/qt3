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
#include "../connection.h"

int main( int argc, char *argv[] )
{
    QApplication app( argc, argv, FALSE );

    if ( createConnections() ) {
	// Databases successfully opened; get pointers to them:
	QSqlDatabase *oracledb = QSqlDatabase::database( "ORACLE" );
	// Now we can now issue SQL commands to the oracle connection
	// or to the default connection
    }

    return 0;
}
