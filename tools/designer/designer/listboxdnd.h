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

#ifndef LISTBOXDND_H
#define LISTBOXDND_H

#include <qptrlist.h>
#include <qlistbox.h>
#include "listdnd.h"

typedef QPtrList<QListBoxItem> ListBoxItemList;

class ListBoxDnd : public ListDnd
{
    Q_OBJECT
public:
    // dragModes are enumerated in ListDnd
    ListBoxDnd( QListBox * eventSource, const char * name = 0 );
    
signals:
    void dropped( QListBoxItem * );
    void dragged( QListBoxItem * );

public slots:
    void confirmDrop( QListBoxItem * );

protected:
    virtual bool dropEvent( QDropEvent * event );
    virtual bool mouseMoveEvent( QMouseEvent * event );
    virtual void updateLine( const QPoint & pos );
    virtual bool canDecode( QDragEnterEvent * event );

private:
    QListBoxItem * itemAt( QPoint pos );
    int buildList( ListBoxItemList & list );
    void insertList( ListBoxItemList & list );
    void removeList( ListBoxItemList & list );
};

#endif //LISTBOXDND_H
