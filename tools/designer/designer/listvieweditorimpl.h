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

#ifndef LISTVIEWEDITORIMPL_H
#define LISTVIEWEDITORIMPL_H

#include "listvieweditor.h"

#include <qmap.h>
#include <qpixmap.h>
#include <qvaluelist.h>

class FormWindow;

class ListViewEditor : public ListViewEditorBase
{
    Q_OBJECT

public:
    ListViewEditor( QWidget *parent, QListView *lv, FormWindow *fw );

signals:
    void itemRenamed(const QString &);

protected slots:
    void applyClicked();
    void columnClickable(bool);
    void columnDownClicked();
    void columnPixmapChosen();
    void columnPixmapDeleted();
    void columnResizable(bool);
    void columnTextChanged(const QString &);
    void columnUpClicked();
    void currentColumnChanged(QListBoxItem*);
    void currentItemChanged(QListViewItem*);
    void deleteColumnClicked();
    void itemColChanged(int);
    void itemDeleteClicked();
    void itemDownClicked();
    void itemNewClicked();
    void itemNewSubClicked();
    void itemPixmapChoosen();
    void itemPixmapDeleted();
    void itemTextChanged(const QString &);
    void itemUpClicked();
    void itemLeftClicked();
    void itemRightClicked();
    void newColumnClicked();
    void okClicked();
    void initTabPage(const QString &page);
    void emitItemRenamed(QListViewItem*, int, const QString&); // signal relay

private:
    struct Column
    {
	QListBoxItem *item;
	QString text;
	QPixmap pixmap;
	bool clickable, resizable;
	Q_DUMMY_COMPARISON_OPERATOR( Column )
    };

private:
    void setupColumns();
    void setupItems();
    Column *findColumn( QListBoxItem *i );
    void transferItems( QListView *from, QListView *to );
    void displayItem( QListViewItem *i, int col );

private:
    QListView *listview;
    QValueList<Column> columns;
    int numColumns;
    FormWindow *formwindow;

};


#endif
