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

#include "parenmatcher.h"
#include "paragdata.h"

#include "qtextedit.h"
#include <private/qrichtext_p.h>
#include <qapplication.h>

ParenMatcher::ParenMatcher()
{
    enabled = TRUE;
}

bool ParenMatcher::match( QTextCursor *cursor )
{
    if ( !enabled )
	return FALSE;
    bool ret = FALSE;

    QChar c( cursor->paragraph()->at( cursor->index() )->c );
    bool ok1 = FALSE;
    bool ok2 = FALSE;
    if ( c == '{' || c == '(' || c == '[' ) {
	ok1 = checkOpenParen( cursor );
	ret = ok1 || ret;
    } else if ( cursor->index() > 0 ) {
	c = cursor->paragraph()->at( cursor->index() - 1 )->c;
	if ( c == '}' || c == ')' || c == ']' ) {
	    ok2 = checkClosedParen( cursor );
	    ret = ok2 || ret;
	}
    }

    return ret;
}

bool ParenMatcher::checkOpenParen( QTextCursor *cursor )
{
    if ( !cursor->paragraph()->extraData() )
	return FALSE;
    ParenList parenList = ( (ParagData*)cursor->paragraph()->extraData() )->parenList;

    Paren openParen, closedParen;
    QTextParagraph *closedParenParag = cursor->paragraph();

    int i = 0;
    int ignore = 0;
    bool foundOpen = FALSE;
    QChar c = cursor->paragraph()->at( cursor->index() )->c;
    for (;;) {
	if ( !foundOpen ) {
	    if ( i >= (int)parenList.count() )
		goto bye;
	    openParen = parenList[ i ];
	    if ( openParen.pos != cursor->index() ) {
		++i;
		continue;
	    } else {
		foundOpen = TRUE;
		++i;
	    }
	}
	
	if ( i >= (int)parenList.count() ) {
	    for (;;) {
		closedParenParag = closedParenParag->next();
		if ( !closedParenParag )
		    goto bye;
		if ( closedParenParag->extraData() &&
		     ( (ParagData*)closedParenParag->extraData() )->parenList.count() > 0 ) {
		    parenList = ( (ParagData*)closedParenParag->extraData() )->parenList;
		    break;
		}
	    }
	    i = 0;
	}
	
	closedParen = parenList[ i ];
	if ( closedParen.type == Paren::Open ) {
	    ignore++;
	    ++i;
	    continue;
	} else {
	    if ( ignore > 0 ) {
		ignore--;
		++i;
		continue;
	    }

	    int id = Match;
	    if ( c == '{' && closedParen.chr != '}' ||
		 c == '(' && closedParen.chr != ')' ||
		 c == '[' && closedParen.chr != ']' )
		id = Mismatch;
	    cursor->document()->setSelectionStart( id, *cursor );
	    int tidx = cursor->index();
	    QTextParagraph *tstring = cursor->paragraph();
	    cursor->setParagraph( closedParenParag );
	    cursor->setIndex( closedParen.pos + 1 );
	    cursor->document()->setSelectionEnd( id, *cursor );
	    cursor->setParagraph( tstring );
	    cursor->setIndex( tidx );
	    return TRUE;
	}
    }

 bye:
    return FALSE;
}

bool ParenMatcher::checkClosedParen( QTextCursor *cursor )
{
    if ( !cursor->paragraph()->extraData() )
	return FALSE;
    ParenList parenList = ( (ParagData*)cursor->paragraph()->extraData() )->parenList;

    Paren openParen, closedParen;
    QTextParagraph *openParenParag = cursor->paragraph();

    int i = (int)parenList.count() - 1;
    int ignore = 0;
    bool foundClosed = FALSE;
    QChar c = cursor->paragraph()->at( cursor->index() - 1 )->c;
    for (;;) {
	if ( !foundClosed ) {
	    if ( i < 0 )
		goto bye;
	    closedParen = parenList[ i ];
	    if ( closedParen.pos != cursor->index() - 1 ) {
		--i;
		continue;
	    } else {
		foundClosed = TRUE;
		--i;
	    }
	}
	
	if ( i < 0 ) {
	    for (;;) {
		openParenParag = openParenParag->prev();
		if ( !openParenParag )
		    goto bye;
		if ( openParenParag->extraData() &&
		     ( (ParagData*)openParenParag->extraData() )->parenList.count() > 0 ) {
		    parenList = ( (ParagData*)openParenParag->extraData() )->parenList;
		    break;
		}
	    }
	    i = (int)parenList.count() - 1;
	}
	
	openParen = parenList[ i ];
	if ( openParen.type == Paren::Closed ) {
	    ignore++;
	    --i;
	    continue;
	} else {
	    if ( ignore > 0 ) {
		ignore--;
		--i;
		continue;
	    }
	
	    int id = Match;
	    if ( c == '}' && openParen.chr != '{' ||
		 c == ')' && openParen.chr != '(' ||
		 c == ']' && openParen.chr != '[' )
		id = Mismatch;
	    cursor->document()->setSelectionStart( id, *cursor );
	    int tidx = cursor->index();
	    QTextParagraph *tstring = cursor->paragraph();
	    cursor->setParagraph( openParenParag );
	    cursor->setIndex( openParen.pos );
	    cursor->document()->setSelectionEnd( id, *cursor );
	    cursor->setParagraph( tstring );
	    cursor->setIndex( tidx );
	    return TRUE;
	}
    }

 bye:
    return FALSE;
}
