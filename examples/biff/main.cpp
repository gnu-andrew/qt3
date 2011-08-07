/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <qapplication.h>
#include "biff.h"


int main( int argc, char ** argv )
{
    QApplication a( argc, argv );
    Biff b;
    a.setMainWidget( &b );
    b.show();
    return a.exec();
}
