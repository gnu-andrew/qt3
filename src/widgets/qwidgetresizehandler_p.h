/****************************************************************************
**
** Definition of the QWidgetResizeHandler class
**
** Created : 001010
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the workspace module of the Qt GUI Toolkit.
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

#ifndef QWIDGETRESIZEHANDLER_P_H
#define QWIDGETRESIZEHANDLER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//
//

#ifndef QT_H
#include "qobject.h"
#endif // QT_H
#ifndef QT_NO_RESIZEHANDLER
class QMouseEvent;
class QKeyEvent;

class Q_EXPORT QWidgetResizeHandler : public QObject
{
    Q_OBJECT

public:
    enum Action {
	Move	= 0x01,
	Resize	= 0x02,
	Any	= Move|Resize
    };

    QWidgetResizeHandler( QWidget *parent, QWidget *cw = 0, const char *name = 0 );
    void setActive( bool b ) { setActive( Any, b ); }
    void setActive( Action ac, bool b );
    bool isActive() const { return isActive( Any ); }
    bool isActive( Action ac ) const;
    void setMovingEnabled( bool b ) { moving = b; }
    bool isMovingEnabled() const { return moving; }

    bool isButtonDown() const { return buttonDown; }

    void setExtraHeight( int h ) { extrahei = h; }
    void setSizeProtection( bool b ) { sizeprotect = b; }

    void doResize();
    void doMove();

signals:
    void activate();

protected:
    bool eventFilter( QObject *o, QEvent *e );
    void mouseMoveEvent( QMouseEvent *e );
    void keyPressEvent( QKeyEvent *e );

private:
    enum MousePosition {
	Nowhere,
	TopLeft, BottomRight, BottomLeft, TopRight,
	Top, Bottom, Left, Right,
	Center
    };

    QWidget *widget;
    QWidget *childWidget;
    QPoint moveOffset;
    QPoint invertedMoveOffset;
    MousePosition mode;
    int extrahei;
    int range;
    uint buttonDown	    :1;
    uint moveResizeMode	    :1;
    uint activeForResize    :1;
    uint sizeprotect	    :1;
    uint moving		    :1; 
    uint activeForMove	    :1;

    void setMouseCursor( MousePosition m );
    bool isMove() const {
	return moveResizeMode && mode == Center;
    }
    bool isResize() const {
	return moveResizeMode && !isMove();
    }

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QWidgetResizeHandler( const QWidgetResizeHandler & );
    QWidgetResizeHandler& operator=( const QWidgetResizeHandler & );
#endif

};

#endif //QT_NO_RESIZEHANDLER
#endif
