/**********************************************************************
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

#include <qlistview.h>
#include <qgroupbox.h>
#include <qlineedit.h>
#include <qcombobox.h>
#include <qmessagebox.h>

#include "metadatabase.h"
#include "formwindow.h"
#include "mainwindow.h"
#include "hierarchyview.h"
#include "command.h"
#include "variabledialogimpl.h"


VariableDialog::VariableDialog( FormWindow *fw, QWidget *parent )
    : VariableDialogBase( parent ), formWindow( fw )
{
    varView->setSorting( -1 );
    QValueList<MetaDataBase::Variable> varLst = MetaDataBase::variables( formWindow );
    QValueList<MetaDataBase::Variable>::Iterator it = varLst.begin();
    for ( ; it != varLst.end(); ++it ) {
	QListViewItem *i = new QListViewItem( varView );
	i->setText( 0, (*it).varName );
	i->setText( 1, (*it).varAccess );
    }

    if ( varView->firstChild() )
	varView->setCurrentItem( varView->firstChild() );
    else
	propBox->setEnabled( FALSE );
}

VariableDialog::~VariableDialog()
{
}

void VariableDialog::setCurrentItem( QString text )
{
    QListViewItem *i = varView->findItem( text, 0 );
    if ( i )
	varView->setCurrentItem( i );    
}

void VariableDialog::okClicked()
{
    QValueList<MetaDataBase::Variable> lst;

    QListViewItemIterator it( varView );
    while ( it.current() != 0 ) {
	MetaDataBase::Variable v;
	v.varName = it.current()->text( 0 ).simplifyWhiteSpace();
	if ( v.varName[ (int)v.varName.length() - 1 ] != ';' )
	    v.varName += ";";
	v.varAccess = it.current()->text( 1 );
	lst << v;
	++it;
    }

    if ( !lst.isEmpty() ) {
	QValueList<MetaDataBase::Variable> invalidLst;
	QValueList<MetaDataBase::Variable>::Iterator it1 = lst.begin();
	QValueList<MetaDataBase::Variable>::Iterator it2;
	for ( ; it1 != lst.end(); ++it1 ) {
	    it2 = it1;
	    ++it2;
	    for ( ; it2 != lst.end(); ++it2 ) {
		if ( MetaDataBase::extractVariableName( (*it1).varName ) ==
		     MetaDataBase::extractVariableName( (*it2).varName ) ) {
		    invalidLst << (*it1);
		    break;
		}
	    }
	}
	if ( !invalidLst.isEmpty() ) {
	    if ( QMessageBox::information( this, tr( "Edit Variables" ),
					   tr( "One variable has been declared twice.\n"
					   "Remove this variable?" ), tr( "&Yes" ), tr( "&No" ) ) == 0 ) {
		for ( it2 = invalidLst.begin(); it2 != invalidLst.end(); ++it2 ) {
		    it = varView->firstChild();
		    while ( it.current() != 0 ) {
			if ( MetaDataBase::extractVariableName( (*it)->text( 0 ).simplifyWhiteSpace() ) ==
			     MetaDataBase::extractVariableName( (*it2).varName ) ) {
			    delete (*it);
			    break;
			}
			++it;
		    }
		}
	    }
	    formWindow->mainWindow()->objectHierarchy()->updateFormDefinitionView();
	    return;
	}
    }
    Command *cmd = new SetVariablesCommand( "Edit variables", formWindow, lst );
    formWindow->commandHistory()->addCommand( cmd );
    cmd->execute();
    accept();
}

void VariableDialog::addVariable()
{
    QListViewItem *i = new QListViewItem( varView, varView->lastItem() );
    i->setText( 0, "int newVariable" );
    i->setText( 1, "protected" );
    varView->setCurrentItem( i );
    varView->setSelected( i, TRUE );
    varName->setFocus();
    varName->selectAll();
}

void VariableDialog::deleteVariable()
{
    QListViewItem *i = varView->selectedItem();
    if ( !i )
	return;
    delete i;
    i = 0;
    if ( varView->firstChild() )
	varView->setSelected( varView->firstChild(), TRUE );
}

void VariableDialog::currentItemChanged( QListViewItem *i )
{
    if ( !i ) {
	varName->clear();
	accessCombo->setCurrentItem( 1 );
	propBox->setEnabled( FALSE );
	return;
    }
    varName->setText( i->text( 0 ) );
    if ( i->text( 1 ) == "public" )
	accessCombo->setCurrentItem( 0 );
    else if ( i->text( 1 ) == "protected" )
	accessCombo->setCurrentItem( 1 );
    else
	accessCombo->setCurrentItem( 2 );
    propBox->setEnabled( TRUE );
}

void VariableDialog::nameChanged()
{
    if ( !varView->currentItem() )
	return;
    varView->currentItem()->setText( 0, varName->text() );
}

void VariableDialog::accessChanged()
{
    if ( !varView->currentItem() )
	return;
    varView->currentItem()->setText( 1, accessCombo->currentText() );
}
