/**********************************************************************
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Designer.
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

#include "parser.h"
#include <qobject.h>
#include <qstringlist.h>

class NormalizeObject : public QObject
{
public:
    NormalizeObject() : QObject() {}
    static QCString normalizeSignalSlot( const char *signalSlot ) { return QObject::normalizeSignalSlot( signalSlot ); }
};

QString Parser::cleanArgs( const QString &func )
{
    QString slot( func );
    int begin = slot.find( "(" ) + 1;
    QString args = slot.mid( begin );
    args = args.left( args.find( ")" ) );
    QStringList lst = QStringList::split( ',', args );
    QString res = slot.left( begin );
    for ( QStringList::Iterator it = lst.begin(); it != lst.end(); ++it ) {
	if ( it != lst.begin() )
	    res += ",";
	QString arg = *it;
	int pos = 0;
	if ( ( pos = arg.find( "&" ) ) != -1 ) {
	    arg = arg.left( pos + 1 );
	} else if ( ( pos = arg.find( "*" ) ) != -1 ) {
	    arg = arg.left( pos + 1 );
	} else {
	    arg = arg.simplifyWhiteSpace();
	    if ( ( pos = arg.find( ':' ) ) != -1 )
		arg = arg.left( pos ).simplifyWhiteSpace() + ":" + arg.mid( pos + 1 ).simplifyWhiteSpace();
	    QStringList l = QStringList::split( ' ', arg );
	    if ( l.count() == 2 ) {
		if ( l[ 0 ] != "const" && l[ 0 ] != "unsigned" && l[ 0 ] != "var" )
		    arg = l[ 0 ];
	    } else if ( l.count() == 3 ) {
		arg = l[ 0 ] + " " + l[ 1 ];
	    }
	}
	res += arg;
    }	
    res += ")";

    return QString::fromLatin1( NormalizeObject::normalizeSignalSlot( res.latin1() ) );
}
