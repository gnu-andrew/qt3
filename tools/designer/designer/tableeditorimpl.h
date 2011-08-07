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

#ifndef TABLEEDITORIMPL_H
#define TABLEEDITORIMPL_H

#include "tableeditor.h"
#include <qmap.h>

class QListBoxItem;
class QTable;
class FormWindow;

class TableEditor : public TableEditorBase
{
    Q_OBJECT

public:
    TableEditor( QWidget* parent = 0, QWidget *editWidget = 0, FormWindow *fw = 0,
		 const char* name = 0, bool modal = FALSE, WFlags fl = 0 );
    ~TableEditor();

protected slots:
    void columnDownClicked();
    void columnTextChanged( const QString & );
    void columnUpClicked();
    void currentColumnChanged( QListBoxItem * );
    void currentFieldChanged( const QString & );
    void currentRowChanged( QListBoxItem * );
    void deleteColumnClicked();
    void deleteRowClicked();
    void newColumnClicked();
    void newRowClicked();
    void okClicked();
    void rowDownClicked();
    void rowTextChanged( const QString & );
    void rowUpClicked();
    void applyClicked();
    void chooseRowPixmapClicked();
    void deleteRowPixmapClicked();
    void chooseColPixmapClicked();
    void deleteColPixmapClicked();

private:
    void readFromTable();
    void readColumns();
    void readRows();
    void saveFieldMap();
    void restoreFieldMap();

private:
    QTable *editTable;
    FormWindow *formWindow;
    QMap<int, QString> fieldMap;
    QMap<QListBoxItem*, QString> tmpFieldMap;

};

#endif // TABLEEDITOR_H
