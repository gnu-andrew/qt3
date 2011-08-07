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

#include "proparser.h"

#include <qdir.h>
#include <qfile.h>
#include <qfileinfo.h>
#include <qregexp.h>
#include <qstringlist.h>
#include <qtextstream.h>

#ifdef Q_OS_UNIX
#include <unistd.h>
#endif

#ifdef Q_OS_WIN32
#define QT_POPEN _popen
#else
#define QT_POPEN popen
#endif

QString loadFile( const QString &fileName )
{
    QFile file( fileName );
    if ( !file.open(IO_ReadOnly) ) {
	fprintf( stderr, "error: Cannot load '%s': %s\n",
		 file.name().latin1(),
		 file.errorString().latin1() );
	return QString();
    }

    QTextStream in( &file );
    return in.read();
}

QMap<QString, QString> proFileTagMap( const QString& text )
{
    QString t = text;
    QMap<QString, QString> tagMap;
    bool stillProcess = true; // If include() has a $$tag then we need to reprocess
    
    while(stillProcess) {

	/* 
	    Strip any commments before we try to include.  We
	    still need to do it after we include to make sure the
	    included file does not have comments
	*/
	t.replace( QRegExp(QString("#[^\n]*\n")), QString(" ") );

	/*
	    Process include() commands.
	    $$PWD is a special case so we have to change it while 
	    we know where the included file is.
        */
	QRegExp callToInclude("include\\s*\\(\\s*([^()\\s]+)\\s*\\)");
	int i = 0;
	while ( (i = callToInclude.search(t, i)) != -1 ) {
	    bool doneWithVar = false;
	    QString fileName = callToInclude.cap(1);
	    QString after = fileName.replace("$$PWD", QDir::currentDirPath());
	    if (!tagMap.isEmpty() && after.contains("$$")) {
		QRegExp var( "\\$\\$[({]?([a-zA-Z0-9_]+)[)}]?" );
		int ii = 0;
		while ((ii = after.find(var, ii)) != -1) {
		    if (tagMap.contains(var.cap(1))) {
			after.replace(ii, var.cap(0).length(), tagMap[var.cap(1)]);
		    } else { // Couldn't find it
			doneWithVar = true;
			break;
		    }
		}
		
	    }
	    if (doneWithVar || !after.contains("$$")) {
		after = loadFile(after);
		QFileInfo fi(callToInclude.cap(1));
		after.replace("$$PWD", fi.dirPath());
		t.replace( i, callToInclude.matchedLength(), after );
	    }
	    i += after.length();
	}

	/*
	    Strip comments, merge lines ending with backslash, add
	    spaces around '=' and '+=', replace '\n' with ';', and
	    simplify white spaces.
	*/
	t.replace( QRegExp(QString("#[^\n]*\n")), QString(" ") );
	t.replace( QRegExp(QString("\\\\[^\n\\S]*\n")), QString(" ") );
	t.replace( "=", QString(" = ") );
	t.replace( "+ =", QString(" += ") );
	t.replace( "\n", QString(";") );
	t = t.simplifyWhiteSpace();
	
	/*
	    Populate tagMap with 'key = value' entries.
	*/
	QStringList lines = QStringList::split( QChar(';'), t );
	QStringList::Iterator line;
	for ( line = lines.begin(); line != lines.end(); ++line ) {
	    QStringList toks = QStringList::split( QChar(' '), *line );
	    
	    if ( toks.count() >= 3 && 
		(toks[1] == QString("=") || toks[1] == QString("+=")) ) {
		QString tag = toks.first();
		int k = tag.findRev( QChar(':') ); // as in 'unix:'
		if ( k != -1 )
		    tag = tag.mid( k + 1 );
		toks.remove( toks.begin() );
		
		QString action = toks.first();
		toks.remove( toks.begin() );
		
		if ( tagMap.contains(tag) ) {
		    if ( action == QString("=") )
			tagMap.replace( tag, toks.join(QChar(' ')) );
		    else
			tagMap[tag] += QChar( ' ' ) + toks.join( QChar(' ') );
		} else {
		    tagMap[tag] = toks.join( QChar(' ') );
		}
	    }
	}
	
	/*
	    Expand $$variables within the 'value' part of a 'key = value'
	    pair.
	*/
	QRegExp var( "\\$\\$[({]?([a-zA-Z0-9_]+)[)}]?" );
	QMap<QString, QString>::Iterator it;
	for ( it = tagMap.begin(); it != tagMap.end(); ++it ) {
	    int i = 0;
	    while ( (i = var.search((*it), i)) != -1 ) {
		int len = var.matchedLength();
		QString invocation = var.cap(1);
		QString after;
		
		if ( invocation == "system" ) {
		    // skip system(); it will be handled in the next pass
		    ++i;
		} else {
		    if ( tagMap.contains(invocation) )
			after = tagMap[invocation];
		    else if (invocation.lower() == "pwd")
			after = QDir::currentDirPath();
		    (*it).replace( i, len, after );
		    i += after.length();
		}
	    }
	}
    
	/*
	  Execute system() calls.
	*/
	QRegExp callToSystem( "\\$\\$system\\s*\\(([^()]*)\\)" );
	for ( it = tagMap.begin(); it != tagMap.end(); ++it ) {
	    int i = 0;
	    while ( (i = callToSystem.search((*it), i)) != -1 ) {
		/*
		  This code is stolen from qmake's project.cpp file.
		  Ideally we would use the same parser, so we wouldn't
		  have this code duplication.
		*/
		QString after;
		char buff[256];
		FILE *proc = QT_POPEN( callToSystem.cap(1).latin1(), "r" );
		while ( proc && !feof(proc) ) {
		    int read_in = (int)fread( buff, 1, 255, proc );
		    if ( !read_in )
			break;
		    for ( int i = 0; i < read_in; i++ ) {
			if ( buff[i] == '\n' || buff[i] == '\t' )
			    buff[i] = ' ';
		    }
		    buff[read_in] = '\0';
		    after += buff;
		}
		(*it).replace( i, callToSystem.matchedLength(), after );
		i += after.length();
	    }
	}
	stillProcess = callToInclude.search(t) != -1;
    }
    return tagMap;
}
