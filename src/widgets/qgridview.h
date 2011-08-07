/**********************************************************************
**
** Definition of QGridView class
**
** Created : 010523
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
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
** This file may be used under the terms of the Q Public License as
** defined by Trolltech ASA and appearing in the file LICENSE.QPL
** included in the packaging of this file.  Licensees holding valid Qt
** Commercial licenses may use this file in accordance with the Qt
** Commercial License Agreement provided with the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#ifndef QGRIDVIEW_H
#define QGRIDVIEW_H

#ifndef QT_H
#include "qscrollview.h"
#endif // QT_H

#ifndef QT_NO_GRIDVIEW

class QGridViewPrivate;

class Q_EXPORT QGridView : public QScrollView
{
    Q_OBJECT
    Q_PROPERTY( int numRows READ numRows WRITE setNumRows )
    Q_PROPERTY( int numCols READ numCols WRITE setNumCols )
    Q_PROPERTY( int cellWidth READ cellWidth WRITE setCellWidth )
    Q_PROPERTY( int cellHeight READ cellHeight WRITE setCellHeight )
public:

    QGridView( QWidget *parent=0, const char *name=0, WFlags f=0 );
   ~QGridView();

    int numRows() const;
    virtual void setNumRows( int );
    int numCols() const;
    virtual void setNumCols( int );

    int cellWidth() const;
    virtual void setCellWidth( int );
    int cellHeight() const;
    virtual void setCellHeight( int );
    
    QRect cellRect() const;
    QRect cellGeometry( int row, int column );
    QSize gridSize() const;

    int rowAt( int y ) const;
    int columnAt( int x ) const;

    void repaintCell( int row, int column, bool erase=TRUE );
    void updateCell( int row, int column );
    void ensureCellVisible( int row, int column );

protected:
    virtual void paintCell( QPainter *, int row, int col ) = 0;
    virtual void paintEmptyArea( QPainter *p, int cx, int cy, int cw, int ch );

    void drawContents( QPainter *p, int cx, int cy, int cw, int ch );

    virtual void dimensionChange( int, int );

private:
    void drawContents( QPainter* );
    void updateGrid();

    int nrows;
    int ncols;
    int cellw;
    int cellh;
    QGridViewPrivate* d;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QGridView( const QGridView & );
    QGridView &operator=( const QGridView & );
#endif
};

inline int QGridView::cellWidth() const 
{ return cellw; }

inline int QGridView::cellHeight() const 
{ return cellh; }

inline int QGridView::rowAt( int y ) const 
{ return y / cellh; }

inline int QGridView::columnAt( int x ) const 
{ return x / cellw; }

inline int QGridView::numRows() const 
{ return nrows; }

inline int QGridView::numCols() const 
{return ncols; }

inline QRect QGridView::cellRect() const
{ return QRect( 0, 0, cellw, cellh ); }

inline QSize QGridView::gridSize() const 
{ return QSize( ncols * cellw, nrows * cellh ); }



#endif // QT_NO_GRIDVIEW


#endif // QTABLEVIEW_H
