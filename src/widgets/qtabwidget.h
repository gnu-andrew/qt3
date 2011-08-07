/****************************************************************************
**
** Definition of QTabWidget class
**
** Created : 990318
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

#ifndef QTABWIDGET_H
#define QTABWIDGET_H

#ifndef QT_H
#include "qwidget.h"
#include "qiconset.h"
#endif // QT_H

#ifndef QT_NO_TABWIDGET

class QTabBar;
class QTabWidgetData;
class QTab;
class QWidgetStack;


class Q_EXPORT QTabWidget : public QWidget
{
    Q_OBJECT
    Q_ENUMS( TabPosition )
    Q_ENUMS( TabShape )
    Q_PROPERTY( TabPosition tabPosition READ tabPosition WRITE setTabPosition )
    Q_PROPERTY( TabShape tabShape READ tabShape WRITE setTabShape )
    Q_PROPERTY( int margin READ margin WRITE setMargin )
    Q_PROPERTY( int currentPage READ currentPageIndex WRITE setCurrentPage )
    Q_PROPERTY( int count READ count )
    Q_OVERRIDE( bool autoMask DESIGNABLE true SCRIPTABLE true )

public:
    QTabWidget( QWidget *parent = 0, const char *name = 0, WFlags f = 0 );
    ~QTabWidget();

    virtual void addTab( QWidget *, const QString & ); // ### make these inline in 4.0
    virtual void addTab( QWidget *child, const QIconSet& iconset,
			 const QString &label );
    virtual void addTab( QWidget *, QTab* );

    virtual void insertTab( QWidget *, const QString &, int index = -1 );
    virtual void insertTab( QWidget *child, const QIconSet& iconset,
			    const QString &label, int index = -1 );
    virtual void insertTab( QWidget *, QTab*, int index = -1 );

    void changeTab( QWidget *, const QString &);
    void changeTab( QWidget *child, const QIconSet& iconset,
		    const QString &label );

    bool isTabEnabled(  QWidget * ) const;
    void setTabEnabled( QWidget *, bool );

    void setCornerWidget( QWidget * w, Qt::Corner corner = Qt::TopRight );
    QWidget * cornerWidget( Qt::Corner corner = Qt::TopRight ) const;

    QString tabLabel( QWidget * ) const;
    void setTabLabel( QWidget *p, const QString &l );

    QIconSet tabIconSet( QWidget * w ) const;
    void setTabIconSet( QWidget * w, const QIconSet & iconset );

    void removeTabToolTip( QWidget * w );
    void setTabToolTip( QWidget * w, const QString & tip );
    QString tabToolTip( QWidget * w ) const;

    QWidget * currentPage() const;
    QWidget *page( int ) const;
    QString label( int ) const;
    int currentPageIndex() const;
    int indexOf( QWidget * ) const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    enum TabPosition { Top, Bottom };
    TabPosition tabPosition() const;
    void setTabPosition( TabPosition );

    enum TabShape { Rounded, Triangular };
    TabShape tabShape() const;
    void setTabShape( TabShape s );

    int margin() const;
    void setMargin( int );

    int count() const;

public slots:
    void setCurrentPage( int );
    virtual void showPage( QWidget * );
    virtual void removePage( QWidget * );

protected:
    void showEvent( QShowEvent * );
    void resizeEvent( QResizeEvent * );
    void setTabBar( QTabBar * );
    QTabBar* tabBar() const;
    void styleChange( QStyle& );
    void updateMask();
    bool eventFilter( QObject *, QEvent * );

signals:
    void currentChanged( QWidget * );
#ifndef Q_QDOC
    void selected( const QString& );
#endif

private slots:
    void showTab( int );

private:
    QTabWidgetData *d;
    void setUpLayout( bool = FALSE );
    friend class QTabDialog;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QTabWidget( const QTabWidget & );
    QTabWidget& operator=( const QTabWidget & );
#endif
};

#endif // QT_NO_TABWIDGET

#endif // QTABWIDGET_H
