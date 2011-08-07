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

#include <metatranslator.h>

#include <qregexp.h>
#include <qstring.h>
#include <qtranslator.h>

typedef QValueList<QTranslatorMessage> TML;

static void printUsage()
{
    fprintf( stderr, "Usage:\n"
	     "    qm2ts [ options ] qm-files\n"
	     "Options:\n"
	     "    -help  Display this information and exit\n"
	     "    -verbose\n"
	     "           Explain what is being done\n"
	     "    -version\n"
	     "           Display the version of qm2ts and exit\n" );
}

int main( int argc, char **argv )
{
    bool verbose = FALSE;
    int numQmFiles = 0;

    for ( int i = 1; i < argc; i++ ) {
	if ( qstrcmp(argv[i], "-help") == 0 ) {
	    printUsage();
	    return 0;
	} else if ( qstrcmp(argv[i], "-verbose") == 0 ) {
	    verbose = TRUE;
	    continue;
	} else if ( qstrcmp(argv[i], "-version") == 0 ) {
	    fprintf( stderr, "qm2ts version %s\n", QT_VERSION_STR );
	    return 0;
	}

	numQmFiles++;
	QTranslator tor( 0 );
	if ( tor.load(argv[i], ".") ) {
	    QString g = argv[i];
	    g.replace( QRegExp(QString("\\.qm$")), QString::null );
	    g += QString( ".ts" );

	    if ( verbose )
		fprintf( stderr, "Generating '%s'...\n", g.latin1() );

	    MetaTranslator metator;
	    int ignored = 0;

	    TML all = tor.messages();
	    TML::Iterator it;
	    for ( it = all.begin(); it != all.end(); ++it ) {
		if ( (*it).sourceText() == 0 ) {
		    ignored++;
		} else {
		    QCString context = (*it).context();
		    if ( context.isEmpty() )
			context = "@default";
		    metator.insert( MetaTranslatorMessage(context,
				    (*it).sourceText(), (*it).comment(),
				    (*it).translation(), FALSE,
				    MetaTranslatorMessage::Finished) );
		}
	    }

	    if ( !metator.save(g) ) {
		fprintf( stderr,
			 "qm2ts warning: For some reason, I cannot save '%s'\n",
			 g.latin1() );
	    } else {
		if ( verbose ) {
		    int converted = (int) metator.messages().count();
		    fprintf( stderr, " %d message%s converted (%d ignored)\n",
			     converted, converted == 1 ? "" : "s", ignored );
		}
		if ( ignored > 0 )
		    fprintf( stderr,
			     "qm2ts warning: File '%s' is not a Qt 2.x .qm"
			     " file (some information is lost)\n",
			     argv[i] );
	    }
	} else {
	    fprintf( stderr,
		     "qm2ts warning: For some reason, I cannot load '%s'\n",
		     argv[i] );
	}
    }

    if ( numQmFiles == 0 ) {
	printUsage();
	return 1;
    }
    return 0;
}
