/****************************************************************************
**
** Definition of QWidgetStack class
**
** Created : 980306
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

#ifndef QWIDGETSTACK_H
#define QWIDGETSTACK_H

#ifndef QT_H
#include "qframe.h"
#include "qintdict.h"
#include "qptrdict.h"
#endif // QT_H

#ifndef QT_NO_WIDGETSTACK


class QWidgetStackPrivate;


class Q_EXPORT QWidgetStack: public QFrame
{
    Q_OBJECT
public:
    QWidgetStack( QWidget* parent=0, const char* name=0 );
    QWidgetStack( QWidget* parent, const char* name, WFlags f);

    ~QWidgetStack();

    int addWidget( QWidget *, int = -1 );
    void removeWidget( QWidget * );

    QSize sizeHint() const;
    QSize minimumSizeHint() const;
    void show();

    QWidget * widget( int ) const;
    int id( QWidget * ) const;

    QWidget * visibleWidget() const;

    void setFrameRect( const QRect & );

signals:
    void aboutToShow( int );
    void aboutToShow( QWidget * );

public slots:
    void raiseWidget( int );
    void raiseWidget( QWidget * );

protected:
    void frameChanged();
    void resizeEvent( QResizeEvent * );

    virtual void setChildGeometries();
    void childEvent( QChildEvent * );

private:
    void init();

    QWidgetStackPrivate * d;
    QIntDict<QWidget> * dict;
    QPtrDict<QWidget> * focusWidgets;
    QWidget * topWidget;
    QWidget * invisible;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QWidgetStack( const QWidgetStack & );
    QWidgetStack& operator=( const QWidgetStack & );
#endif
};

#endif // QT_NO_WIDGETSTACK

#endif // QWIDGETSTACK_H
