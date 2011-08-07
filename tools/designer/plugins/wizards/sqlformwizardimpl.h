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

#ifndef SQLFORMWIZARDIMPL_H
#define SQLFORMWIZARDIMPL_H

#include "sqlformwizard.h"
#include <templatewizardiface.h>
#include <designerinterface.h>
#include <qvaluelist.h>

class SqlFormWizard : public SqlFormWizardBase
{
    Q_OBJECT

public:
    SqlFormWizard( QUnknownInterface *aIface, QWidget *w, QWidget* parent = 0, DesignerFormWindow *fw = 0,
		   const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~SqlFormWizard();

    void accept() const;

protected slots:
    void connectionSelected( const QString & );
    void tableSelected( const QString & );
    void autoPopulate( bool populate );
    void fieldDown();
    void fieldUp();
    void removeField();
    void addField();
    void setupDatabaseConnections();
    void accept();
    void addSortField();
    void reSortSortField();
    void removeSortField();
    void sortFieldUp();
    void sortFieldDown();
    void nextPageClicked();

private:
    void setupPage1();

private:
    QWidget *widget;
    QUnknownInterface *appIface;
    DesignerFormWindow *formWindow;
    enum Mode {
	None,
	View,
	Browser,
	Table
    };
    Mode mode;

};

#endif // SQLFORMWIZARDIMPL_H
