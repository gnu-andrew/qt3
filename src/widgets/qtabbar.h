/****************************************************************************
**
** Definition of QTab and QTabBar classes
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

#ifndef QTABBAR_H
#define QTABBAR_H

#ifndef QT_H
#include "qwidget.h"
#include "qptrlist.h"
#endif // QT_H

#ifndef QT_NO_TABBAR

class QTabBar;
class QIconSet;

class Q_EXPORT QTab : public Qt
{
    friend class QTabBar;
    friend class QTabWidget;

public:
    QTab();
    virtual ~QTab();
    QTab( const QString& text );
    QTab( const QIconSet& icon, const QString& text = QString::null );

    void setText( const QString& text);
    QString text() const { return label; }
    void setIconSet( const QIconSet& icon );
    QIconSet* iconSet() const { return iconset; }
    void setRect( const QRect& rect ) { r = rect; }
    QRect rect() const { return r; }
    void setEnabled( bool enable ) { enabled = enable; }
    bool isEnabled() const { return enabled; }
    void setIdentifier( int i ) { id = i; }
    int identifier() const { return id; }

private:
    void setTabBar( QTabBar *tb );
    QString label;
    QRect r; // the bounding rectangle of this (may overlap with others)
    bool enabled;
    int id;
    QIconSet* iconset; // optional iconset
    QTabBar *tb;
};


struct QTabPrivate;
//class *QAccel;

class Q_EXPORT QTabBar: public QWidget
{
    Q_OBJECT
    Q_ENUMS( Shape )
    Q_PROPERTY( Shape shape READ shape WRITE setShape )
    Q_PROPERTY( int currentTab READ currentTab WRITE setCurrentTab )
    Q_PROPERTY( int count READ count )
    Q_PROPERTY( int keyboardFocusTab READ keyboardFocusTab )

public:
    QTabBar( QWidget* parent=0, const char* name=0 );
    ~QTabBar();

    enum Shape { RoundedAbove, RoundedBelow,
		 TriangularAbove, TriangularBelow };

    Shape shape() const;
    virtual void setShape( Shape );

    void show();

    virtual int addTab( QTab * );
    virtual int insertTab( QTab *, int index = -1 );
    virtual void removeTab( QTab * );

    virtual void setTabEnabled( int, bool );
    bool isTabEnabled( int ) const;


    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    int currentTab() const;
    int keyboardFocusTab() const;

    QTab * tab( int ) const;
    QTab * tabAt( int ) const;
    int indexOf( int ) const;
    int count() const;

    virtual void layoutTabs();
    virtual QTab * selectTab( const QPoint & p ) const;

    void 	removeToolTip( int index );
    void     	setToolTip( int index, const QString & tip );
    QString 	toolTip( int index ) const;

public slots:
    virtual void setCurrentTab( int );
    virtual void setCurrentTab( QTab * );

signals:
    void selected( int );
    void layoutChanged();

protected:
    virtual void paint( QPainter *, QTab *, bool ) const; // ### not const
    virtual void paintLabel( QPainter*, const QRect&, QTab*, bool ) const;

    void focusInEvent( QFocusEvent *e );
    void focusOutEvent( QFocusEvent *e );

    void resizeEvent( QResizeEvent * );
    void paintEvent( QPaintEvent * );
    void mousePressEvent ( QMouseEvent * );
    void mouseMoveEvent ( QMouseEvent * );
    void mouseReleaseEvent ( QMouseEvent * );
    void keyPressEvent( QKeyEvent * );
    void styleChange( QStyle& );
    void fontChange ( const QFont & );

    bool event( QEvent *e );
    
    QPtrList<QTab> * tabList();

private slots:
    void scrollTabs();

private:
    QPtrList<QTab> * l;
    QPtrList<QTab> * lstatic;
    void makeVisible( QTab* t = 0 );
    void updateArrowButtons();
    QTabPrivate * d;

    friend class QTabBarToolTip;
    friend class QTab;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QTabBar( const QTabBar & );
    QTabBar& operator=( const QTabBar & );
#endif
};


#endif // QT_NO_TABBAR

#endif // QTABBAR_H
