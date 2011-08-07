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

#include "cppeditor.h"
#include "syntaxhighliter_cpp.h"
#include <cindent.h>
#include "cppcompletion.h"
#include "cppbrowser.h"
#include <parenmatcher.h>
#include <qsettings.h>
#include <qpopupmenu.h>
#include <qinputdialog.h>
#include <designerinterface.h>

CppEditor::CppEditor( const QString &fn, QWidget *parent, const char *name, DesignerInterface *i )
    : Editor( fn, parent, name ), dIface( i )
{
    if ( dIface )
	dIface->addRef();
    document()->setPreProcessor( new SyntaxHighlighter_CPP );
    document()->setIndent( (indent = new CIndent) );
    completion = new CppEditorCompletion( this );
    browser = new CppEditorBrowser( this );
    int j = 0;
    while ( SyntaxHighlighter_CPP::keywords[ j ] != QString::null )
	    completion->addCompletionEntry( SyntaxHighlighter_CPP::keywords[ j++ ], 0, FALSE );
    configChanged();
}

CppEditor::~CppEditor()
{
    delete completion;
    if ( dIface )
	dIface->release();
}

void CppEditor::configChanged()
{
    QString path = "/Trolltech/CppEditor/";
    QMap<QString, ConfigStyle> styles = Config::readStyles( path );
    config()->styles = styles;
    ( (SyntaxHighlighter_CPP*)document()->preProcessor() )->updateStyles( config()->styles );

    completion->setEnabled( Config::completion( path ) );
    parenMatcher->setEnabled( Config::parenMatching( path ) );
    if ( Config::wordWrap( path ) ) {
	if ( hScrollBarMode() != AlwaysOff ) {
	    document()->setFormatter( new QTextFormatterBreakInWords );
	    setHScrollBarMode( AlwaysOff );
	}
    } else {
	if ( hScrollBarMode() != AlwaysOn ) {
	    QTextFormatterBreakWords *f = new QTextFormatterBreakWords;
	    f->setWrapEnabled( FALSE );
	    document()->setFormatter( f );
	    setHScrollBarMode( AlwaysOn );
	}
    }

    setFont( ( (SyntaxHighlighter_CPP*)document()->preProcessor() )->format( 0 )->font() );

    indent->setTabSize( Config::indentTabSize( path ) );
    indent->setIndentSize( Config::indentIndentSize( path ) );
    indent->setKeepTabs( Config::indentKeepTabs( path ) );
    indent->setAutoIndent( Config::indentAutoIndent( path ) );
    if ( !Config::indentAutoIndent( path ) )
	document()->setIndent( 0 );
    else
	document()->setIndent( indent );

    document()->setTabStops( ( (SyntaxHighlighter_CPP*)document()->preProcessor() )->format( QTextPreProcessor::Standard )->width( 'x' ) * Config::indentTabSize( path ) );

    Editor::configChanged();
}

QPopupMenu *CppEditor::createPopupMenu( const QPoint &p )
{
    QPopupMenu *m = Editor::createPopupMenu( p );
    m->insertSeparator();
    int adddeclid = m->insertItem( tr( "Add Include File (in Declaration)..." ), this, SLOT( addInclDecl() ) );
    int addimplid = m->insertItem( tr( "Add Include File (in Implementation)..." ), this, SLOT( addInclImpl() ) );
    int addforid = m->insertItem( tr( "Add Forward Declaration..." ), this, SLOT( addForward() ) );
    if ( !dIface->currentForm() ) {
	m->setItemEnabled( adddeclid, FALSE );
	m->setItemEnabled( addimplid, FALSE );
	m->setItemEnabled( addforid, FALSE );
    }
    return m;
}

void CppEditor::addInclDecl()
{
    if ( !dIface )
	return;
    QString s = QInputDialog::getText( tr( "Add Include File (In Declaration)" ),
				       tr( "Input this using the format <b>&lt;include.h&gt;</b> or <b>\"include.h\"</b>" ) );
    if ( s.isEmpty() )
	return;
    DesignerFormWindow *form = dIface->currentForm();
    QStringList lst = form->declarationIncludes();
    lst << s;
    form->setDeclarationIncludes( lst );
}

void CppEditor::addInclImpl()
{
    if ( !dIface )
	return;
    QString s = QInputDialog::getText( tr( "Add Include File (In Implementation)" ),
				       tr( "Input this using the format <b>&lt;include.h&gt;</b> or <b>\"include.h\"</b>" ) );
    if ( s.isEmpty() )
	return;
    DesignerFormWindow *form = dIface->currentForm();
    QStringList lst = form->implementationIncludes();
    lst << s;
    form->setImplementationIncludes( lst );
}

void CppEditor::addForward()
{
    if ( !dIface )
	return;
    QString s = QInputDialog::getText( tr( "Add Forward Declaration" ),
				       tr( "Input this using the format <b>ClassName;</b>" ) );
    if ( s.isEmpty() )
	return;
    DesignerFormWindow *form = dIface->currentForm();
    QStringList lst = form->forwardDeclarations();
    lst << s;
    form->setForwardDeclarations( lst );
}

void CppEditor::paste()
{
    Editor::paste();
    emit intervalChanged();
}
