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

#ifndef EDITFUNCTIONSIMPL_H
#define EDITFUNCTIONSIMPL_H

#include "editfunctions.h"
#include "hierarchyview.h"
#include "metadatabase.h"
#include <qmap.h>

class FormWindow;
class QListViewItem;

class EditFunctions : public EditFunctionsBase
{
    Q_OBJECT

public:
    EditFunctions( QWidget *parent, FormWindow *fw, bool showOnlySlots = FALSE );

    void setCurrentFunction( const QString &function );
    void functionAdd( const QString &access = QString::null,
		      const QString &type = QString::null  );
    void functionAdd() { functionAdd( "public" ); }

signals:
    void itemRenamed(const QString &);

protected slots:
    void okClicked();
    void functionRemove();
    void currentItemChanged( QListViewItem * );
    void currentTextChanged( const QString &txt );
    void currentSpecifierChanged( const QString &s );
    void currentAccessChanged( const QString &a );
    void currentReturnTypeChanged( const QString &type );
    void currentTypeChanged( const QString &type );
    void displaySlots( bool justSlots );
    void emitItemRenamed( QListViewItem *, int, const QString & );

private:
    enum Attribute { Name, Specifier, Access, ReturnType, Type };
    struct FunctItem {
	int id;
	QString oldName;
	QString newName;
	QString oldRetTyp;
	QString retTyp;
	QString spec;
	QString oldSpec;
	QString access;
	QString oldAccess;
	QString type;
	QString oldType;

	Q_DUMMY_COMPARISON_OPERATOR( FunctItem )
    };

    void changeItem( QListViewItem *item, Attribute a, const QString &nV );

    FormWindow *formWindow;
    QMap<QListViewItem*, int> functionIds;
    QStringList removedFunctions;
    QValueList<MetaDataBase::Function> itemList;
    QValueList<FunctItem> functList;
    int id;
    QString lastType;
};

#endif
