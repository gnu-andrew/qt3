/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include "archivedialog.h"
#include <qapplication.h>

int main(int argc, char **argv)
{
    QApplication a( argc, argv );
    ArchiveDialog ad;
    ad.show();

    QObject::connect( &a, SIGNAL(lastWindowClosed()),
		      &a, SLOT(quit()) );
    
    return a.exec();
}
