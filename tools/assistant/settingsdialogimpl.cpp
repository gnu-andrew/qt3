/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Assistant.
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

#include "settingsdialogimpl.h"
#include "docuparser.h"
#include "config.h"

#include <qapplication.h>
#include <qpushbutton.h>
#include <qcheckbox.h>
#include <qcolordialog.h>
#include <qdir.h>
#include <qfiledialog.h>
#include <qfileinfo.h>
#include <qlineedit.h>
#include <qlistbox.h>
#include <qlistview.h>
#include <qmessagebox.h>
#include <qptrstack.h>
#include <qsettings.h>
#include <qtimer.h>
#include <qtoolbutton.h>
#include <qtabwidget.h>
#include <qmap.h>


SettingsDialog::SettingsDialog( QWidget *parent, const char* name )
    : SettingsDialogBase( parent, name )
{
    init();
}

void SettingsDialog::init()
{
    Config *config = Config::configuration();

    browserApp->setText( config->webBrowser() );
    homePage->setText( config->homePage() );
    pdfApp->setText( config->pdfReader() );
}

void SettingsDialog::selectColor()
{
    QColor c = QColorDialog::getColor( colorButton->paletteBackgroundColor(), this );
    colorButton->setPaletteBackgroundColor( c );
}

void SettingsDialog::browseWebApp()
{
    setFile( browserApp, tr( "Qt Assistant - Set Web Browser" ) );
}

void SettingsDialog::browsePDFApplication()
{
    setFile( pdfApp, tr( "Qt Assistant - Set PDF Browser" ) );
}

void SettingsDialog::browseHomepage()
{
    setFile( homePage, tr( "Qt Assistant - Set Homepage" ) );
}

void SettingsDialog::setFile( QLineEdit *le, const QString &caption )
{
    QFileDialog *fd = new QFileDialog( this );
    fd->setCaption( caption );
    fd->setMode( QFileDialog::AnyFile );
    fd->setDir( QDir::homeDirPath() );

    if ( fd->exec() == QDialog::Accepted ) {
	if ( !fd->selectedFile().isEmpty() )
	   le->setText( fd->selectedFile() );
    }
}

void SettingsDialog::accept()
{
    Config *config = Config::configuration();

    config->setWebBrowser( browserApp->text() );
    config->setHomePage( homePage->text() );
    config->setPdfReader( pdfApp->text() );

    hide();
    done( Accepted );
}

void SettingsDialog::reject()
{
    init();
    done( Rejected );
}
