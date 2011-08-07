/****************************************************************************
**
** Definition of QScrollView class
**
** Created : 970523
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
#ifndef QSCROLLVIEW_H
#define QSCROLLVIEW_H

#ifndef QT_H
#include "qframe.h"
#include "qscrollbar.h"
#endif // QT_H

#ifndef QT_NO_SCROLLVIEW

class QScrollViewData;

class Q_EXPORT QScrollView : public QFrame
{
    Q_OBJECT
    Q_ENUMS( ResizePolicy ScrollBarMode )
    Q_PROPERTY( ResizePolicy resizePolicy READ resizePolicy WRITE setResizePolicy )
    Q_PROPERTY( ScrollBarMode vScrollBarMode READ vScrollBarMode WRITE setVScrollBarMode )
    Q_PROPERTY( ScrollBarMode hScrollBarMode READ hScrollBarMode WRITE setHScrollBarMode )
    Q_PROPERTY( int visibleWidth READ visibleWidth )
    Q_PROPERTY( int visibleHeight READ visibleHeight )
    Q_PROPERTY( int contentsWidth READ contentsWidth )
    Q_PROPERTY( int contentsHeight READ contentsHeight )
    Q_PROPERTY( int contentsX READ contentsX )
    Q_PROPERTY( int contentsY READ contentsY )
#ifndef QT_NO_DRAGANDDROP
    Q_PROPERTY( bool dragAutoScroll READ dragAutoScroll WRITE setDragAutoScroll )
#endif

public:
    QScrollView(QWidget* parent=0, const char* name=0, WFlags f=0);
    ~QScrollView();

    enum ResizePolicy { Default, Manual, AutoOne, AutoOneFit };
    virtual void setResizePolicy( ResizePolicy );
    ResizePolicy resizePolicy() const;

    void styleChange( QStyle & );
    void removeChild(QWidget* child);
    virtual void addChild( QWidget* child, int x=0, int y=0 );
    virtual void moveChild( QWidget* child, int x, int y );
    int childX(QWidget* child);
    int childY(QWidget* child);
    bool childIsVisible(QWidget* child) { return child->isVisible(); } // obsolete functions
    void showChild(QWidget* child, bool yes=TRUE) {
	if ( yes )
	    child->show();
	else
	    child->hide();
    }

    enum ScrollBarMode { Auto, AlwaysOff, AlwaysOn };

    ScrollBarMode vScrollBarMode() const;
    virtual void  setVScrollBarMode( ScrollBarMode );

    ScrollBarMode hScrollBarMode() const;
    virtual void  setHScrollBarMode( ScrollBarMode );

    QWidget*     cornerWidget() const;
    virtual void setCornerWidget(QWidget*);

    // ### 4.0: Consider providing a factory function for scrollbars
    //          (e.g. make the two following functions virtual)
    QScrollBar*  horizontalScrollBar() const;
    QScrollBar*  verticalScrollBar() const;
    QWidget*	 viewport() const;
    QWidget*	 clipper() const;

    int		visibleWidth() const;
    int		visibleHeight() const;

    int		contentsWidth() const;
    int		contentsHeight() const;
    int		contentsX() const;
    int		contentsY() const;

    void	resize( int w, int h );
    void	resize( const QSize& );
    void	show();

    void	updateContents( int x, int y, int w, int h );
    void	updateContents( const QRect& r );
    void 	updateContents();
    void	repaintContents( int x, int y, int w, int h, bool erase=TRUE );
    void	repaintContents( const QRect& r, bool erase=TRUE );
    void 	repaintContents( bool erase=TRUE );
    void	contentsToViewport( int x, int y, int& vx, int& vy ) const;
    void	viewportToContents( int vx, int vy, int& x, int& y ) const;
    QPoint	contentsToViewport( const QPoint& ) const;
    QPoint	viewportToContents( const QPoint& ) const;
    void	enableClipper( bool y );

    void	setStaticBackground( bool y );
    bool	hasStaticBackground() const;

    QSize	viewportSize( int, int ) const;
    QSize	sizeHint() const;
    QSize	minimumSizeHint() const;

    void	removeChild(QObject* child);

    bool	isHorizontalSliderPressed();
    bool	isVerticalSliderPressed();

#ifndef QT_NO_DRAGANDDROP
    virtual void setDragAutoScroll( bool b );
    bool	 dragAutoScroll() const;
#endif

signals:
    void	contentsMoving(int x, int y);
    void	horizontalSliderPressed();
    void	horizontalSliderReleased();
    void	verticalSliderPressed();
    void	verticalSliderReleased();

public slots:
    virtual void resizeContents( int w, int h );
    void	scrollBy( int dx, int dy );
    virtual void setContentsPos( int x, int y );
    void	ensureVisible(int x, int y);
    void	ensureVisible(int x, int y, int xmargin, int ymargin);
    void	center(int x, int y);
    void	center(int x, int y, float xmargin, float ymargin);

    void	updateScrollBars(); // ### virtual in 4.0
    void	setEnabled( bool enable );

protected:
    virtual void drawContents(QPainter*, int cx, int cy, int cw, int ch);
    virtual void drawContentsOffset(QPainter*, int ox, int oy,
		    int cx, int cy, int cw, int ch);


    virtual void contentsMousePressEvent( QMouseEvent* );
    virtual void contentsMouseReleaseEvent( QMouseEvent* );
    virtual void contentsMouseDoubleClickEvent( QMouseEvent* );
    virtual void contentsMouseMoveEvent( QMouseEvent* );
#ifndef QT_NO_DRAGANDDROP
    virtual void contentsDragEnterEvent( QDragEnterEvent * );
    virtual void contentsDragMoveEvent( QDragMoveEvent * );
    virtual void contentsDragLeaveEvent( QDragLeaveEvent * );
    virtual void contentsDropEvent( QDropEvent * );
#endif
#ifndef QT_NO_WHEELEVENT
    virtual void contentsWheelEvent( QWheelEvent * );
#endif
    virtual void contentsContextMenuEvent( QContextMenuEvent * );


    virtual void viewportPaintEvent( QPaintEvent* );
    virtual void viewportResizeEvent( QResizeEvent* );
    virtual void viewportMousePressEvent( QMouseEvent* );
    virtual void viewportMouseReleaseEvent( QMouseEvent* );
    virtual void viewportMouseDoubleClickEvent( QMouseEvent* );
    virtual void viewportMouseMoveEvent( QMouseEvent* );
#ifndef QT_NO_DRAGANDDROP
    virtual void viewportDragEnterEvent( QDragEnterEvent * );
    virtual void viewportDragMoveEvent( QDragMoveEvent * );
    virtual void viewportDragLeaveEvent( QDragLeaveEvent * );
    virtual void viewportDropEvent( QDropEvent * );
#endif
#ifndef QT_NO_WHEELEVENT
    virtual void viewportWheelEvent( QWheelEvent * );
#endif
    virtual void viewportContextMenuEvent( QContextMenuEvent * );

    void	frameChanged();

    virtual void setMargins(int left, int top, int right, int bottom);
    int leftMargin() const;
    int topMargin() const;
    int rightMargin() const;
    int bottomMargin() const;

    bool focusNextPrevChild( bool next );

    virtual void setHBarGeometry(QScrollBar& hbar, int x, int y, int w, int h);
    virtual void setVBarGeometry(QScrollBar& vbar, int x, int y, int w, int h);

    void resizeEvent(QResizeEvent*);
    void  mousePressEvent( QMouseEvent * );
    void  mouseReleaseEvent( QMouseEvent * );
    void  mouseDoubleClickEvent( QMouseEvent * );
    void  mouseMoveEvent( QMouseEvent * );
#ifndef QT_NO_WHEELEVENT
    void  wheelEvent( QWheelEvent * );
#endif
    void contextMenuEvent( QContextMenuEvent * );
    bool eventFilter( QObject *, QEvent *e );

    void setCachedSizeHint( const QSize &sh ) const;
    QSize cachedSizeHint() const;
    void fontChange( const QFont & );

private:
    void drawContents( QPainter* );
    void moveContents(int x, int y);

    QScrollViewData* d;

private slots:
    void hslide(int);
    void vslide(int);
    void hbarIsPressed();
    void hbarIsReleased();
    void vbarIsPressed();
    void vbarIsReleased();
#ifndef QT_NO_DRAGANDDROP
    void doDragAutoScroll();
    void startDragAutoScroll();
    void stopDragAutoScroll();
#endif

private: // Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QScrollView( const QScrollView & );
    QScrollView &operator=( const QScrollView & );
#endif
    void changeFrameRect(const QRect&);

public:
    void disableSizeHintCaching();

};

#endif // QT_NO_SCROLLVIEW

#endif // QSCROLLVIEW_H
