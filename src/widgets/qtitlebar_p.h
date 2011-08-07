/****************************************************************************
**
** Definition of some Qt private functions.
**
** Created : 000101
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QTITLEBAR_P_H
#define QTITLEBAR_P_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of qworkspace.cpp and qdockwindow.cpp.  This header file may change
// from version to version without notice, or even be removed.
//
// We mean it.
//
//


#ifndef QT_H
#include "qbutton.h"
#include "qlabel.h"
#endif // QT_H

#if !defined(QT_NO_TITLEBAR)

class QToolTip;
class QTitleBarPrivate;
class QPixmap;

class Q_EXPORT QTitleBar : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( bool autoRaise READ autoRaise WRITE setAutoRaise )
    Q_PROPERTY( bool movable READ isMovable WRITE setMovable )

public:
    QTitleBar (QWidget* w, QWidget* parent, const char* name=0);
    ~QTitleBar();

    bool isActive() const;
    bool usesActiveColor() const;
    virtual QString visibleText() const;

    bool isMovable() const;
    void setMovable(bool);

    bool autoRaise() const;
    void setAutoRaise(bool);

    QWidget *window() const;

    QSize sizeHint() const;

#ifdef QT_NO_WIDGET_TOPEXTRA
    // We provide one, since titlebar is useless otherwise.
    QString caption() const;
#endif

public slots:
    void setActive( bool );
    void setCaption( const QString& title );
    void setIcon( const QPixmap& icon );

signals:
    void doActivate();
    void doNormal();
    void doClose();
    void doMaximize();
    void doMinimize();
    void doShade();
    void showOperationMenu();
    void popupOperationMenu( const QPoint& );
    void doubleClicked();

protected:
    bool event( QEvent *);
    void resizeEvent( QResizeEvent *);
    void contextMenuEvent( QContextMenuEvent * );
    void mousePressEvent( QMouseEvent * );
    void mouseDoubleClickEvent( QMouseEvent * );
    void mouseReleaseEvent( QMouseEvent * );
    void mouseMoveEvent( QMouseEvent * );
    void enterEvent( QEvent *e );
    void leaveEvent( QEvent *e );
    void paintEvent( QPaintEvent *p );

    virtual void cutText();

private:
    void readColors();

    QTitleBarPrivate *d;
#if defined(Q_DISABLE_COPY) // Disabled copy constructor and operator=
    QTitleBar( const QTitleBar & );
    QTitleBar &operator=( const QTitleBar & );
#endif
};

#endif
#endif //QTITLEBAR_P_H
