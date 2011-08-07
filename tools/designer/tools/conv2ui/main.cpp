/**********************************************************************
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt GUI Designer.
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

#include <qapplication.h>
#include <private/qpluginmanager_p.h>
#include <qstringlist.h>
#include <qdir.h>
#include <stdio.h>
#include "filterinterface.h"
#include <qfileinfo.h>

class Conv2ui {
public:
    Conv2ui();
    ~Conv2ui();
    bool reinit();
    bool convert( const QString & filename, const QDir & dest );
    QStringList featureList();
private:
    QString getTag( const QString & filename );
    QString absName( const QString & filename );
    QPluginManager<ImportFilterInterface> * importFiltersManager;
};

Conv2ui::Conv2ui() : importFiltersManager( 0 )
{
    reinit();
}

Conv2ui::~Conv2ui()
{
    if ( importFiltersManager )
	delete importFiltersManager;
}

bool Conv2ui::reinit()
{
    if ( importFiltersManager )
	delete importFiltersManager;
    importFiltersManager = new QPluginManager<ImportFilterInterface>( IID_ImportFilter,
						      QApplication::libraryPaths(),
						      "/designer");
    return !!importFiltersManager;
}

bool Conv2ui::convert( const QString & filename, const QDir & dest )
{
    if ( !importFiltersManager ) {
	qWarning( "Conv2ui: no QPluginManager was found" );
	return FALSE;
    }

    if ( !QFile::exists( absName( filename ) ) ) {
	qWarning( "Conv2ui: can not find file %s", filename.latin1() );
	return FALSE;
    }

    QString tag = getTag( filename );
    QStringList lst = importFiltersManager->featureList();

    for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it ) {

	ImportFilterInterface * interface = 0;
	importFiltersManager->queryInterface( *it, &interface );

	if ( !interface ) continue;

	if ( (*it).contains( tag, FALSE ) ) {
	    // Make sure we output the files in the destination dir
	    QString absfile = absName( filename );
	    QString current = QDir::currentDirPath();
	    QDir::setCurrent( dest.absPath() );
	    interface->import( *it, absfile ); // resulting files go into the current dir
	    QDir::setCurrent( current );
	}

	interface->release();

    }

    return TRUE;
}

QStringList Conv2ui::featureList()
{
    if ( !importFiltersManager ) {
	return 0;
    }
    return importFiltersManager->featureList();
}

QString Conv2ui::getTag( const QString & filename )
{
    QFileInfo info( filename );
    return info.extension();
}

QString Conv2ui::absName( const QString & filename )
{
    QString absFileName = QDir::isRelativePath( filename ) ?
			  QDir::currentDirPath() +
			  QDir::separator() +
			  filename :
			  filename;
    return absFileName;
}

void printHelpMessage( QStringList & formats )
{
    printf( "Usage: conv2ui [options] <file> <destination directory>\n\n" );
    printf( "Options:\n");
    printf( "\t-help\t\tDisplay this information\n" );
    printf( "\t-silent\t\tDon't write any messages\n");
    printf( "\t-version\tDisplay version of conv2ui\n");
    printf( "Supported file formats:\n" );
    for ( QStringList::Iterator it = formats.begin(); it != formats.end(); ++it )
	printf( "\t%s\n", (*it).latin1() );
}

void printVersion()
{
    printf( "Qt user interface file converter for Qt version %s\n", QT_VERSION_STR );
}

int main( int argc, char ** argv )
{
    Conv2ui conv;

    bool help = TRUE;
    bool silent = FALSE;
    bool version = FALSE;
    bool unrecognized = FALSE;

    // parse options
    int argi = 1;
    while ( argi < argc && argv[argi][0] == '-' ) {
	int i = 1;
	while ( argv[argi][i] != '\0' ) {

	    if ( argv[argi][i] == 's' ) {
		silent = TRUE;
		break;
	    } else if ( argv[argi][i] == 'h' ) {
		help = TRUE;
		break;
	    } else if ( argv[argi][i] == 'v' ) {
		version = TRUE;
		silent = TRUE;
		break;
	    } else {
		unrecognized = TRUE;
	    }
	    i++;
	}
	argi++;
    }

    // get source file
    QString src;
    if ( argv[argi] ) {
	src = argv[argi++];
    }

    // get destination directory
    QDir dst;
    if ( argv[argi] ) {
	dst.setPath( argv[argi++] );
	help = FALSE;
    }

    if ( !silent ) {
        printf( "Qt user interface file converter.\n" );
	if ( unrecognized ) {
	    printf( "conv2ui: Unrecognized option\n" );
	}
    }
    if ( version ) {
	printVersion();
	return 0;
    } else if ( help ) {
	QStringList formats = conv.featureList();
	printHelpMessage( formats );
	return 0;
    }

    // do the magic
    if ( conv.convert( src, dst ) )
	return 0;
    return -1;
}
