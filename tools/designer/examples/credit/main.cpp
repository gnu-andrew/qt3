/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <qapplication.h>
#include "creditform.h"


int main( int argc, char *argv[] ) 
{
    QApplication app( argc, argv );

    CreditForm creditForm;
    app.setMainWidget( &creditForm );
    creditForm.show();

    return app.exec();
}


