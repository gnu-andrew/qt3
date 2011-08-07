/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Linguist.
**
** This file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free
** Software Foundation and appearing in the files LICENSE.GPL2
** and LICENSE.GPL3 included in the packaging of this file.
** Alternatively you may (at your option) use any later version
** of the GNU General Public License if such license has been
** publicly approved by Trolltech ASA (or its successors, if any)
** and the KDE Free Qt Foundation.
**
** Please review the following information to ensure GNU General
** Public Licensing requirements will be met:
** http://trolltech.com/products/qt/licenses/licensing/opensource/.
** If you are unsure which license is appropriate for your use, please
** review the following information:
** http://trolltech.com/products/qt/licenses/licensing/licensingoverview
** or contact the sales department at sales@trolltech.com.
**
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with
** the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#include "trwindow.h"

#include <qapplication.h>
#include <qlabel.h>
#include <qpixmap.h>
#include <qtimer.h>
#include <qtextcodec.h>
#include <qtranslator.h>
#include <qsettings.h>
#include <qsplashscreen.h>

int main( int argc, char **argv )
{
    QApplication app( argc, argv );
    QApplication::setOverrideCursor( Qt::waitCursor );


    QTranslator translator( 0 );
    translator.load( QString( "linguist_" ) + QTextCodec::locale(), "." );
    app.installTranslator( &translator );

    bool showSplash = TRUE;

    QString keybase("/Qt Linguist/3.1/");
    QSettings config;
    config.insertSearchPath( QSettings::Windows, "/Trolltech" );

    QRect r( QApplication::desktop()->screenGeometry() );
    r.setX( config.readNumEntry( keybase + "Geometry/MainwindowX", r.x() ) );
    r.setY( config.readNumEntry( keybase + "Geometry/MainwindowY", r.y() ) );
    r.setWidth( config.readNumEntry( keybase + "Geometry/MainwindowWidth", r.width() ) );
    r.setHeight( config.readNumEntry( keybase + "Geometry/MainwindowHeight", r.height() ) );

    QSplashScreen *splash = 0;
    if ( showSplash ) {
	splash = new QSplashScreen( QPixmap::fromMimeSource("splash.png"),
				    Qt::WDestructiveClose );
	splash->show();
    }

    TrWindow *tw = new TrWindow;
    app.setMainWidget( tw );

    if ( app.argc() > 1 )
	tw->openFile( QString(app.argv()[app.argc() - 1]) );

    if ( config.readBoolEntry( keybase + "Geometry/MainwindowMaximized", FALSE ) )
	tw->showMaximized();
    else
	tw->show();
    if ( splash )
	splash->finish( tw );
    QApplication::restoreOverrideCursor();

    return app.exec();
}
