/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the Qt Assistant.
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
#include "profile.h"
#include <qxml.h>
#include <qtextcodec.h>
#include <qfileinfo.h>
#include <qregexp.h>
#include <qdir.h>

#define QT_TITLE "Qt Reference Documentation"
#define DESIGNER_TITLE "Qt Designer Manual"
#define ASSISTANT_TITLE  "Qt Assistant Manual"
#define LINGUIST_TITLE "Guide to the Qt Translation Tools"
#define QMAKE_TITLE "qmake User Guide"

Profile *Profile::createDefaultProfile( const QString &docPath )
{
    QString path = qInstallPathDocs();
    if ( !docPath.isEmpty() )
        path = docPath;        
    path = path + "/html/";
    Profile *profile = new Profile;
    profile->valid = TRUE;
    profile->type = DefaultProfile;
    profile->props["name"] = "default";
    profile->props["applicationicon"] = "appicon.png";
    profile->props["aboutmenutext"] = "About Qt";
    profile->props["abouturl"] = "about_qt";
    profile->props["title"] = "Qt Assistant";
    profile->props["basepath"] = path;
    profile->props["startpage"] = path + "index.html";

    profile->addDCFTitle( path + "qt.dcf", QT_TITLE );
    profile->addDCFTitle( path + "designer.dcf", DESIGNER_TITLE );
    profile->addDCFTitle( path + "assistant.dcf", ASSISTANT_TITLE );
    profile->addDCFTitle( path + "linguist.dcf", LINGUIST_TITLE );
    profile->addDCFTitle( path + "qmake.dcf", QMAKE_TITLE );

    profile->addDCFIcon( QT_TITLE, "qt.png" );
    profile->addDCFIcon( DESIGNER_TITLE, "designer.png" );
    profile->addDCFIcon( ASSISTANT_TITLE, "assistant.png" );
    profile->addDCFIcon( LINGUIST_TITLE, "linguist.png" );

    profile->addDCFIndexPage( QT_TITLE, path + "index.html" );
    profile->addDCFIndexPage( DESIGNER_TITLE, path + "designer-manual.html" );
    profile->addDCFIndexPage( ASSISTANT_TITLE, path + "assistant.html" );
    profile->addDCFIndexPage( LINGUIST_TITLE, path + "linguist-manual.html" );
    profile->addDCFIndexPage( QMAKE_TITLE, path + "qmake-manual.html" );

    profile->addDCFImageDir( QT_TITLE, "../../gif/" );
    profile->addDCFImageDir( DESIGNER_TITLE, "../../gif/" );
    profile->addDCFImageDir( ASSISTANT_TITLE, "../../gif/" );
    profile->addDCFImageDir( LINGUIST_TITLE, "../../gif/" );
    profile->addDCFImageDir( QMAKE_TITLE, "../../gif/" );

    return profile;
}


Profile::Profile()
    : valid( TRUE ), dparser( 0 )
{
}


void Profile::removeDocFileEntry( const QString &docfile )
{
    docs.remove( docfile );

    QStringList titles;    
    
    for( QMap<QString,QString>::Iterator it = dcfTitles.begin();
	 it != dcfTitles.end(); ++it ) {
	if( (*it) == docfile ) {
	    indexPages.remove( *it );
	    icons.remove( *it );
	    imageDirs.remove( *it );
	    titles << it.key();
	} 
    }

    for( QStringList::ConstIterator title = titles.begin();
	 title != titles.end(); ++title ) {
	
	dcfTitles.remove( *title );
    }

#ifdef ASSISTANT_DEBUG   
    qDebug( "docs:\n  - " + docs.join( "\n  - " ) );
    qDebug( "titles:\n  - " + titles.join( "\n  - " ) );    
    qDebug( "keys:\n  - " + ( (QStringList*) &(dcfTitles.keys()) )->join( "\n  - " ) );
    qDebug( "values:\n  - " + ( (QStringList*) &(dcfTitles.values()) )->join( "\n  - " ) );
#endif
}
