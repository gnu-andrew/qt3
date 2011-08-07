/****************************************************************************
**
** Definition of QSplitter class
**
**  Created : 980105
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

#ifndef QSPLITTER_H
#define QSPLITTER_H

#ifndef QT_H
#include "qframe.h"
#include "qvaluelist.h"
#endif // QT_H

#ifndef QT_NO_SPLITTER

class QSplitterHandle;
class QSplitterPrivate;
class QSplitterLayoutStruct;
class QTextStream;

class Q_EXPORT QSplitter : public QFrame
{
    Q_OBJECT
    Q_PROPERTY( Orientation orientation READ orientation WRITE setOrientation )
    Q_PROPERTY( bool opaqueResize READ opaqueResize WRITE setOpaqueResize )
    Q_PROPERTY( int handleWidth READ handleWidth WRITE setHandleWidth )
    Q_PROPERTY( bool childrenCollapsible READ childrenCollapsible WRITE setChildrenCollapsible )

public:
    // ### Qt 4.0: remove Auto from public API
    enum ResizeMode { Stretch, KeepSize, FollowSizeHint, Auto };

    QSplitter( QWidget* parent = 0, const char* name = 0 );
    QSplitter( Orientation, QWidget* parent = 0, const char* name = 0 );
    ~QSplitter();

    virtual void setOrientation( Orientation );
    Orientation orientation() const { return orient; }

    // ### Qt 4.0: make setChildrenCollapsible() and setCollapsible() virtual

    void setChildrenCollapsible( bool );
    bool childrenCollapsible() const;

    void setCollapsible( QWidget *w, bool );
    virtual void setResizeMode( QWidget *w, ResizeMode );
    virtual void setOpaqueResize( bool = TRUE );
    bool opaqueResize() const;

    void moveToFirst( QWidget * );
    void moveToLast( QWidget * );

    void refresh() { recalc( TRUE ); }
    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    QValueList<int> sizes() const;
    void setSizes( QValueList<int> );

    int handleWidth() const;
    void setHandleWidth( int );

protected:
    void childEvent( QChildEvent * );

    bool event( QEvent * );
    void resizeEvent( QResizeEvent * );

    int idAfter( QWidget* ) const;

    void moveSplitter( QCOORD pos, int id );
    virtual void drawSplitter( QPainter*, QCOORD x, QCOORD y,
			       QCOORD w, QCOORD h );
    void styleChange( QStyle& );
    int adjustPos( int, int );
    virtual void setRubberband( int );
    void getRange( int id, int *, int * );

private:
    enum { DefaultResizeMode = 3 };

    void init();
    void recalc( bool update = FALSE );
    void doResize();
    void storeSizes();
    void getRange( int id, int *, int *, int *, int * );
    void addContribution( int, int *, int *, bool );
    int adjustPos( int, int, int *, int *, int *, int * );
    bool collapsible( QSplitterLayoutStruct * );
    void processChildEvents();
    QSplitterLayoutStruct *findWidget( QWidget * );
    QSplitterLayoutStruct *addWidget( QWidget *, bool prepend = FALSE );
    void recalcId();
    void doMove( bool backwards, int pos, int id, int delta, bool upLeft,
		 bool mayCollapse );
    void setGeo( QWidget *w, int pos, int size, bool splitterMoved );
    int findWidgetJustBeforeOrJustAfter( int id, int delta, int &collapsibleSize );
    void updateHandles();

    inline QCOORD pick( const QPoint &p ) const
    { return orient == Horizontal ? p.x() : p.y(); }
    inline QCOORD pick( const QSize &s ) const
    { return orient == Horizontal ? s.width() : s.height(); }

    inline QCOORD trans( const QPoint &p ) const
    { return orient == Vertical ? p.x() : p.y(); }
    inline QCOORD trans( const QSize &s ) const
    { return orient == Vertical ? s.width() : s.height(); }

    QSplitterPrivate *d;

    Orientation orient;
    friend class QSplitterHandle;

#ifndef QT_NO_TEXTSTREAM
    friend Q_EXPORT QTextStream& operator<<( QTextStream&, const QSplitter& );
    friend Q_EXPORT QTextStream& operator>>( QTextStream&, QSplitter& );
#endif

private:
#if defined(Q_DISABLE_COPY)
    QSplitter( const QSplitter & );
    QSplitter& operator=( const QSplitter & );
#endif
};

#ifndef QT_NO_TEXTSTREAM
Q_EXPORT QTextStream& operator<<( QTextStream&, const QSplitter& );
Q_EXPORT QTextStream& operator>>( QTextStream&, QSplitter& );
#endif

#endif // QT_NO_SPLITTER

#endif // QSPLITTER_H
