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

#include <qapplication.h>

void FindDialog::init()
{
    lastBrowser = 0;
    onceFound = FALSE;
    findExpr = "";
    sb = new QStatusBar( this );
    FindDialogLayout->addWidget( sb );
    sb->message( tr( "Enter the text you are looking for." ) );
}

void FindDialog::destroy()
{
}

void FindDialog::doFind()
{
    doFind(radioForward->isChecked());
}

void FindDialog::doFind(bool forward)
{
    QTextBrowser *browser = (QTextBrowser*) mainWindow()->browsers()->currentBrowser();
    sb->clear();

    if (comboFind->currentText() != findExpr || lastBrowser != browser)
	onceFound = false;
    findExpr = comboFind->currentText();

    bool found;
    if (browser->hasSelectedText()) { // Search either forward or backward from cursor.
	found = browser->find(findExpr, checkCase->isChecked(), checkWords->isChecked(),
			      forward);
    } else {
	int para = forward ? 0 : INT_MAX;
	int index = forward ? 0 : INT_MAX;
	found = browser->find(findExpr, checkCase->isChecked(), checkWords->isChecked(),
			      forward, &para, &index);
    }

    if (!found) {
	if (onceFound) {
	    if (forward)
		statusMessage(tr("Search reached end of the document"));
	    else
		statusMessage(tr("Search reached start of the document"));
	} else {
	    statusMessage(tr( "Text not found" ));
	}
    }
    onceFound |= found;
    lastBrowser = browser;
}



MainWindow* FindDialog::mainWindow()
{
    return (MainWindow*) parent();
}

bool FindDialog::hasFindExpression()
{
    return !findExpr.isEmpty();
}

void FindDialog::statusMessage(const QString &message)
{
    if (isVisible())
	sb->message(message);
    else
	((MainWindow*) parent())->statusBar()->message(message, 2000);

}
