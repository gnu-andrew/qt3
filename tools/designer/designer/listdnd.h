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

#ifndef LISTDND_H
#define LISTDND_H

#include <qobject.h>
#include <qscrollview.h>

class ListDnd : public QObject
{
    Q_OBJECT
public:
    enum DragMode { None = 0, External = 1, Internal = 2, Both = 3, Move = 4, NullDrop = 8 };
    ListDnd( QScrollView * eventSource, const char * name = 0 );
    void setDragMode( int mode );
    int dragMode() const;
    bool eventFilter( QObject *, QEvent * event );

protected:
    virtual bool dragEnterEvent( QDragEnterEvent * event );
    virtual bool dragLeaveEvent( QDragLeaveEvent * );
    virtual bool dragMoveEvent( QDragMoveEvent * event );
    virtual bool dropEvent( QDropEvent * event );
    virtual bool mousePressEvent( QMouseEvent * event );
    virtual bool mouseMoveEvent( QMouseEvent * event );
    virtual void updateLine( const QPoint & dragPos );
    virtual bool canDecode( QDragEnterEvent * event );

    QScrollView * src;
    QWidget * line;
    QPoint mousePressPos;
    QPoint dragPos;
    bool dragInside;
    bool dragDelete;
    bool dropConfirmed;
    int dMode;
};

#endif // LISTDND_H
