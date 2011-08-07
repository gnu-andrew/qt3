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

#ifndef SIZEHANDLE_H
#define SIZEHANDLE_H

#include <qwidget.h>
#include <qintdict.h>
#include <qptrdict.h>

class QMouseEvent;
class FormWindow;
class WidgetSelection;
class QPaintEvent;

class SizeHandle : public QWidget
{
    Q_OBJECT

public:
    enum Direction { LeftTop, Top, RightTop, Right, RightBottom, Bottom, LeftBottom, Left };

    SizeHandle( FormWindow *parent, Direction d, WidgetSelection *s );
    void setWidget( QWidget *w );
    void setActive( bool a );
    void updateCursor();

    void setEnabled( bool ) {}

protected:
    void paintEvent( QPaintEvent *e );
    void mousePressEvent( QMouseEvent *e );
    void mouseMoveEvent( QMouseEvent *e );
    void mouseReleaseEvent( QMouseEvent *e );

private:
    void trySetGeometry( QWidget *w, int x, int y, int width, int height );
    void tryResize( QWidget *w, int width, int height );

private:
    QWidget *widget;
    Direction dir;
    QPoint oldPressPos;
    FormWindow *formWindow;
    WidgetSelection *sel;
    QRect geom, origGeom;
    bool active;

};

class WidgetSelection
{
public:
    WidgetSelection( FormWindow *parent, QPtrDict<WidgetSelection> *selDict );

    void setWidget( QWidget *w, bool updateDict = TRUE );
    bool isUsed() const;

    void updateGeometry();
    void hide();
    void show();
    void update();
    
    QWidget *widget() const;

protected:
    QIntDict<SizeHandle> handles;
    QWidget *wid;
    FormWindow *formWindow;
    QPtrDict<WidgetSelection> *selectionDict;

};

#endif
