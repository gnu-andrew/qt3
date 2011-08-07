/****************************************************************************
**
** Definition of QToolBox widget class
**
** Created : 961105
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

#ifndef QTOOLBOX_H
#define QTOOLBOX_H

#ifndef QT_H
#include <qframe.h>
#include <qiconset.h>
#endif // QT_H

#ifndef QT_NO_TOOLBOX

class QToolBoxPrivate;
class QWidgetList;

class Q_EXPORT QToolBox : public QFrame
{
    Q_OBJECT
    Q_PROPERTY( int currentIndex READ currentIndex WRITE setCurrentIndex )
    Q_PROPERTY( int count READ count )

public:
    QToolBox( QWidget *parent = 0, const char *name = 0, WFlags f = 0 );
    ~QToolBox();

    int addItem( QWidget *item, const QString &label );
    int addItem( QWidget *item, const QIconSet &iconSet, const QString &label );
    int insertItem( int index, QWidget *item, const QString &label );
    int insertItem( int index, QWidget *item, const QIconSet &iconSet, const QString &label );

    int removeItem( QWidget *item );

    void setItemEnabled( int index, bool enabled );
    bool isItemEnabled( int index ) const;

    void setItemLabel( int index, const QString &label );
    QString itemLabel( int index ) const;

    void setItemIconSet( int index, const QIconSet &iconSet );
    QIconSet itemIconSet( int index ) const;

    void setItemToolTip( int index, const QString &toolTip );
    QString itemToolTip( int index ) const;

    QWidget *currentItem() const;
    void setCurrentItem( QWidget *item );

    int currentIndex() const;
    QWidget *item( int index ) const;
    int indexOf( QWidget *item ) const;
    int count() const;

public slots:
    void setCurrentIndex( int index );

signals:
    void currentChanged( int index );

private slots:
    void buttonClicked();
    void itemDestroyed(QObject*);

protected:
    virtual void itemInserted( int index );
    virtual void itemRemoved( int index );
    void showEvent( QShowEvent *e );
    void frameChanged();
    void styleChange(QStyle&);

private:
    void relayout();

private:
    QToolBoxPrivate *d;

};


inline int QToolBox::addItem( QWidget *item, const QString &label )
{ return insertItem( -1, item, QIconSet(), label ); }
inline int QToolBox::addItem( QWidget *item, const QIconSet &iconSet,
			      const QString &label )
{ return insertItem( -1, item, iconSet, label ); }
inline int QToolBox::insertItem( int index, QWidget *item, const QString &label )
{ return insertItem( index, item, QIconSet(), label ); }

#endif // QT_NO_TOOLBOX
#endif
