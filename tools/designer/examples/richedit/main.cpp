/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/

#include <qapplication.h>
#include "richedit.h"


int main( int argc, char *argv[] ) 
{
    QApplication app( argc, argv );

    EditorForm richeditForm;
    app.setMainWidget( &richeditForm );
    richeditForm.show();

    return app.exec();
}


