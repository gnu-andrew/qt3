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

#include "dbconnectionimpl.h"
#include "dbconnection.h"
#include "project.h"
#include "asciivalidator.h"
#include <qgroupbox.h>
#include <qcombobox.h>
#include <qspinbox.h>
#include <qlineedit.h>
#include <qlayout.h>
#include <qsqldatabase.h>

DatabaseConnectionEditor::DatabaseConnectionEditor( DatabaseConnection* connection, QWidget* parent,
						    const char* name, bool modal, WFlags fl )
    : DatabaseConnectionEditorBase( parent, name, modal, fl ), conn( connection )
{
    connectionWidget = new DatabaseConnectionWidget( grp );
    grpLayout->addWidget( connectionWidget, 0, 0 );
    init();
}

DatabaseConnectionEditor::~DatabaseConnectionEditor()
{
}

void DatabaseConnectionEditor::accept()
{
#ifndef QT_NO_SQL
    conn->setUsername( connectionWidget->editUsername->text() );
    conn->setPassword( connectionWidget->editPassword->text() );
    conn->setHostname( connectionWidget->editHostname->text() );
#endif
    DatabaseConnectionEditorBase::accept();
}

void DatabaseConnectionEditor::init()
{
    connectionWidget->editName->setEnabled( FALSE );
    connectionWidget->editName->setValidator( new AsciiValidator( connectionWidget->editName ) );
    connectionWidget->editName->setText( conn->name() );
    connectionWidget->comboDriver->setEnabled( FALSE );
    connectionWidget->comboDriver->lineEdit()->setText( conn->driver() );
    connectionWidget->editDatabase->setEnabled( FALSE );
    connectionWidget->editDatabase->setText( conn->database() );
    connectionWidget->editUsername->setEnabled( TRUE );
    connectionWidget->editUsername->setText( conn->username() );
    connectionWidget->editPassword->setEnabled( TRUE );
    connectionWidget->editPassword->setText( "" );
    connectionWidget->editHostname->setEnabled( TRUE );
    connectionWidget->editHostname->setText( conn->hostname() );
    connectionWidget->editPort->setEnabled( TRUE );
    connectionWidget->editPort->setValue( conn->port() );
    connectionWidget->editUsername->setFocus();
    connectionWidget->editUsername->selectAll();
}
