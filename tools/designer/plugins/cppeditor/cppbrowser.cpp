/**********************************************************************
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
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

#include "cppbrowser.h"
#include <private/qrichtext_p.h>
#include <qprocess.h>
#include <qmainwindow.h>
#include <qstatusbar.h>
#include <editor.h>

CppEditorBrowser::CppEditorBrowser( Editor *e )
    : EditorBrowser( e )
{
}

void CppEditorBrowser::showHelp( const QString &w )
{
    QString word( w );
    if ( word[ 0 ] == 'Q' ) {
	if ( word[ (int)word.length() - 1 ] == '&' ||
	     word[ (int)word.length() - 1 ] == '*' )
	    word.remove( word.length() - 1, 1 );
	word = word.lower() + ".html";
	QStringList lst;
	lst << "assistant" << "-file" << word;
	QProcess proc( lst );
	proc.start();
	return;
    }

    if ( word.find( '(' ) != -1 ) {
	QString txt = "::" + word.left( word.find( '(' ) );
	QTextDocument *doc = curEditor->document();
	QTextParagraph *p = doc->firstParagraph();
	while ( p ) {
	    if ( p->string()->toString().find( txt ) != -1 ) {
		curEditor->setCursorPosition( p->paragId(), 0 );
		return;
	    }
	    p = p->next();
	}
    }

    QMainWindow *mw = ::qt_cast<QMainWindow*>(curEditor->topLevelWidget());
    if ( mw )
	mw->statusBar()->message( tr( "Nothing available for '%1'" ).arg( w ), 1500 );
}
