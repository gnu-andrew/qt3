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

#include "projectsettingsimpl.h"
#include "project.h"
#include "formwindow.h"
#include "metadatabase.h"
#include "mainwindow.h"
#include "asciivalidator.h"
#include "mainwindow.h"
#include "sourcefile.h"
#include "workspace.h"

#include <qlineedit.h>
#include <qtextedit.h>
#include <qcheckbox.h>
#include <qfiledialog.h>
#include <qcombobox.h>
#include <qobjectlist.h>
#include <qheader.h>
#include <qpushbutton.h>
#include <qlabel.h>

/*
 *  Constructs a ProjectSettings which is a child of 'parent', with the
 *  name 'name' and widget flags set to 'f'
 *
 *  The dialog will by default be modeless, unless you set 'modal' to
 *  TRUE to construct a modal dialog.
 */
ProjectSettings::ProjectSettings( Project *pro, QWidget* parent,  const char* name, bool modal, WFlags fl )
    : ProjectSettingsBase( parent, name, modal, fl ), project( pro )
{
    connect( buttonHelp, SIGNAL( clicked() ), MainWindow::self, SLOT( showDialogHelp() ) );

    editProjectFile->setFocus();

    if ( project->isDummy() ) {
	editProjectFile->setEnabled( FALSE );
	editProjectFile->setText( project->projectName() );
    } else {
	if ( project->fileName().isEmpty() || project->fileName() == ".pro" ) {
	    editProjectFile->setText( tr( "unnamed.pro" ) );
	    editProjectFile->selectAll();
	} else {
	    editProjectFile->setText( project->fileName() );
	}
    }

    editDatabaseFile->setText( project->databaseDescription() );

    comboLanguage->insertStringList( MetaDataBase::languages() );
    for ( int j = 0; j < (int)comboLanguage->count(); ++j ) {
	if ( project->language() == comboLanguage->text( j ) ) {
	    comboLanguage->setCurrentItem( j );
	    break;
	}
    }
}

/*
 *  Destroys the object and frees any allocated resources
 */
ProjectSettings::~ProjectSettings()
{
}

void ProjectSettings::chooseDatabaseFile()
{
    QString fn = QFileDialog::getSaveFileName(editDatabaseFile->text(), tr("Database Files (*.db);;All Files (*)"), 
					      this);
    if ( fn.isEmpty() )
	return;
    editDatabaseFile->setText( fn );
}

void ProjectSettings::chooseProjectFile()
{
    QString fn = QFileDialog::getSaveFileName(editProjectFile->text(), tr("Project Files (*.pro);;All Files (*)"), 
					      this);
    if ( fn.isEmpty() )
	return;
    editProjectFile->setText( fn );
}

void ProjectSettings::helpClicked()
{
}

void ProjectSettings::okClicked()
{
    // ### check for validity
    project->setFileName( editProjectFile->text(), FALSE );
    project->setDatabaseDescription( editDatabaseFile->text() );
    project->setLanguage( comboLanguage->text( comboLanguage->currentItem() ) );
    project->setModified( TRUE );
    accept();
}

void ProjectSettings::languageChanged( const QString & )
{
}
