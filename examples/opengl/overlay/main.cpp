/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/
//
// Qt OpenGL example: Box
//
// A small example showing how a GLWidget can be used just as any Qt widget
// 
// File: main.cpp
//
// The main() function 
// 

#include "globjwin.h"
#include <qapplication.h>
#include <qgl.h>
#include <qmessagebox.h>

/*
  The main program is here. 
*/

int main( int argc, char **argv )
{
    QApplication::setColorSpec( QApplication::CustomColor );
    QApplication a(argc,argv);

    if ( !QGLFormat::hasOpenGL() ) {
	qWarning( "This system has no OpenGL support. Exiting." );
	return -1;
    }

    // Check for existence of overlays
    if ( !QGLFormat::hasOpenGLOverlays() ) {
	QMessageBox::critical( 0, qApp->argv()[0], 
			       "This system does not support OpenGL overlays",
			       "Exit" );
	return 1;
    }

    GLObjectWindow w;
    w.resize( 400, 350 );
    a.setMainWidget( &w );

    w.show();
    return a.exec();
}
