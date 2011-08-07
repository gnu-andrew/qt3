/**********************************************************************
**
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

#include "syntaxhighlighter_html.h"
#include "qstring.h"
#include "qmap.h"
#include "qapplication.h"


SyntaxHighlighter_HTML::SyntaxHighlighter_HTML()
    : QTextPreProcessor(), lastFormat( 0 ), lastFormatId( -1 )
{
    QFont f( qApp->font() );

    addFormat(Standard, new QTextFormat(f, qApp->palette().color(QPalette::Active, QColorGroup::Text)));
    addFormat(Keyword, new QTextFormat(f, qApp->palette().color(QPalette::Active, QColorGroup::Dark)));
    addFormat(Attribute, new QTextFormat(f, qApp->palette().color(QPalette::Active, QColorGroup::Link)));
    addFormat(AttribValue, new QTextFormat(f, qApp->palette().color(QPalette::Active, QColorGroup::LinkVisited)));
}

SyntaxHighlighter_HTML::~SyntaxHighlighter_HTML()
{
}

void SyntaxHighlighter_HTML::process( QTextDocument *doc, QTextParagraph *string, int, bool invalidate )
{

    QTextFormat *formatStandard = format( Standard );
    QTextFormat *formatKeyword = format( Keyword );
    QTextFormat *formatAttribute = format( Attribute );
    QTextFormat *formatAttribValue = format( AttribValue );

    const int StateStandard  = 0;
    const int StateTag       = 1;
    const int StateAttribute = 2;
    const int StateAttribVal = 3;

    QString buffer = "";

    int state = StateStandard;


    if ( string->prev() ) {
	if ( string->prev()->endState() == -1 )
	    process( doc, string->prev(), 0, FALSE );
	state = string->prev()->endState();
    }


    int i = 0;
    for ( ;; ) {
	QChar c = string->at( i )->c;

	if ( c == '<' ) {
	    if ( state != StateStandard  )
		string->setFormat( i - buffer.length(), buffer.length(), formatStandard, FALSE );
	    buffer = c;
	    state = StateTag;
	    string->setFormat( i, 1, formatKeyword, FALSE );
	}
	else if ( c == '>' && ( state != StateStandard ) ) {
	    string->setFormat( i, 1, formatKeyword, FALSE );
	    buffer = "";
	    state = StateStandard;
	}
	else if ( c == ' ' && state == StateTag ) {
	    buffer += c;
	    string->setFormat( i, 1, formatStandard, FALSE );
	    state = StateAttribute;
	}
	else if ( c == '=' && state == StateAttribute ) {
	    buffer += c;
	    string->setFormat( i, 1, formatStandard, FALSE );
	    state = StateAttribute;
	}
	else if ( c == '\"' && state == StateAttribute ) {
	    buffer += c;
	    string->setFormat( i, 1, formatStandard, FALSE );
	    state = StateAttribVal;
	}
	else if ( c == '\"' && state == StateAttribVal ) {
	    buffer += c;
	    string->setFormat( i, 1, formatStandard, FALSE );
	    state = StateAttribute;
	}
	else if ( state == StateAttribute ) {
	    buffer += c;
	    string->setFormat( i, 1, formatAttribute, FALSE );
	}
	else if ( state == StateAttribVal ) {
	    buffer += c;
	    string->setFormat( i, 1, formatAttribValue, FALSE );
	}
	else if ( state == StateTag ) {
	    string->setFormat( i, 1, formatKeyword, FALSE );
	    buffer += c;
	}
	else if ( state == StateStandard ) {
	    string->setFormat( i, 1, formatStandard, FALSE );
	}

	i++;
	if ( i >= string->length() )
	    break;
    }

    string->setEndState( state );
    string->setFirstPreProcess( FALSE );

    if ( invalidate && string->next() &&
	 !string->next()->firstPreProcess() && string->next()->endState() != -1 ) {
	QTextParagraph *p = string->next();
	while ( p ) {
	    if ( p->endState() == -1 )
		return;
	    p->setEndState( -1 );
	    p = p->next();
	}
    }
}

QTextFormat *SyntaxHighlighter_HTML::format( int id )
{
    if ( lastFormatId == id  && lastFormat )
	return lastFormat;

    QTextFormat *f = formats[ id ];
    lastFormat = f ? f : formats[ 0 ];
    lastFormatId = id;
    return lastFormat;
}

void SyntaxHighlighter_HTML::addFormat( int id, QTextFormat *f )
{
    formats.insert( id, f );
}
