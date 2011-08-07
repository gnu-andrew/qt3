/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "main.h"
#include <qdatatable.h>

InvoiceItemCursor::InvoiceItemCursor() :
    QSqlCursor( "invoiceitem" )
{
    QSqlFieldInfo productName( "productname", QVariant::String );
    append( productName );
    setCalculated( productName.name(), TRUE );
}


QVariant InvoiceItemCursor::calculateField( const QString & name )
{
    if ( name == "productname" ) {
	QSqlQuery query( "SELECT name FROM prices WHERE id=" +
		     field( "pricesid" )->value().toString() );
	if ( query.next() )
	    return query.value( 0 );
    }

    return QVariant( QString::null );
}


int main( int argc, char *argv[] )
{
    QApplication app( argc, argv );

    if ( createConnections() ) {
	InvoiceItemCursor invoiceItemCursor;

	QDataTable *invoiceItemTable = new QDataTable( &invoiceItemCursor );

	app.setMainWidget( invoiceItemTable );

	invoiceItemTable->addColumn( "productname", "Product" );
	invoiceItemTable->addColumn( "quantity",    "Quantity" );
	invoiceItemTable->addColumn( "paiddate",    "Paid" );

	invoiceItemTable->refresh();
	invoiceItemTable->show();

	return app.exec();
    }

    return 1;
}
