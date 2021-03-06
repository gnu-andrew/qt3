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

#ifndef ACTIONLISTVIEW_H
#define ACTIONLISTVIEW_H

#include <qlistview.h>
#include "actiondnd.h"

class ActionItem : public QListViewItem
{
public:
    ActionItem( QListView *lv, bool group )
	: QListViewItem( lv ),
	  a( group ? 0 : new QDesignerAction( 0 ) ),
	  g( group ? new QDesignerActionGroup( 0 ) : 0 ) { setDragEnabled( TRUE ); }
    ActionItem( QListView *lv, QAction *ac );
    ActionItem( QListViewItem *i, QAction *ac );
    ActionItem( ActionItem *parent, bool group = FALSE )
	: QListViewItem( parent ),
	  a( group ? 0 : new QDesignerAction( parent->actionGroup() ) ),
	  g( group ? new QDesignerActionGroup( parent->actionGroup() ) : 0 ) { setDragEnabled( TRUE ); moveToEnd(); }

    QDesignerAction *action() const { return a; }
    QDesignerActionGroup *actionGroup() const { return g; }

private:
    void moveToEnd();

private:
    QDesignerAction *a;
    QDesignerActionGroup *g;

};

class ActionListView : public QListView
{
    Q_OBJECT

public:
    ActionListView( QWidget *parent = 0, const char *name = 0 );

protected:
    QDragObject *dragObject();

private slots:
    void rmbMenu( QListViewItem *i, const QPoint &p );

signals:
    void insertAction();
    void insertActionGroup();
    void insertDropDownActionGroup();
    void deleteAction();
    void connectAction();

};

#endif
