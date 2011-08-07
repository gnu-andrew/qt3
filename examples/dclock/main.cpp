/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "dclock.h"
#include <qapplication.h>


int main( int argc, char **argv )
{
    QApplication a( argc, argv );
    DigitalClock *clock = new DigitalClock;
    clock->resize( 170, 80 );
    a.setMainWidget( clock );
    clock->setCaption("Qt Example - Digital Clock");
    clock->show();
    return a.exec();
}
