/****************************************************************************
**
** Definition of QButtonGroup class
**
** Created : 950130
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

#ifndef QBUTTONGROUP_H
#define QBUTTONGROUP_H

#ifndef QT_H
#include "qgroupbox.h"
#endif // QT_H

#ifndef QT_NO_BUTTONGROUP


class QButton;
class QButtonList;


class Q_EXPORT QButtonGroup : public QGroupBox
{
    Q_OBJECT
    Q_PROPERTY( bool exclusive READ isExclusive WRITE setExclusive )
    Q_PROPERTY( bool radioButtonExclusive READ isRadioButtonExclusive WRITE setRadioButtonExclusive )
    Q_PROPERTY( int selectedId READ selectedId WRITE setButton )

public:
    QButtonGroup( QWidget* parent=0, const char* name=0 );
    QButtonGroup( const QString &title,
		  QWidget* parent=0, const char* name=0 );
    QButtonGroup( int columns, Orientation o,
		  QWidget* parent=0, const char* name=0 );
    QButtonGroup( int columns, Orientation o, const QString &title,
		  QWidget* parent=0, const char* name=0 );
    ~QButtonGroup();

    bool	isExclusive() const;
    bool	isRadioButtonExclusive() const { return radio_excl; }
    virtual void setExclusive( bool );
    virtual void setRadioButtonExclusive( bool );

public:
    int		insert( QButton *, int id=-1 );
    void	remove( QButton * );
    QButton    *find( int id ) const;
    int		id( QButton * ) const;
    int		count() const;

    virtual void setButton( int id );

    virtual void moveFocus( int );

    QButton    *selected() const;
    int    selectedId() const;

signals:
    void	pressed( int id );
    void	released( int id );
    void	clicked( int id );

protected slots:
    void	buttonPressed();
    void	buttonReleased();
    void	buttonClicked();
    void	buttonToggled( bool on );

protected:
    bool 	event( QEvent * e );

private:
    void	init();
    bool	excl_grp;
    bool	radio_excl;
    QButtonList *buttons;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QButtonGroup( const QButtonGroup & );
    QButtonGroup &operator=( const QButtonGroup & );
#endif
};


#endif // QT_NO_BUTTONGROUP

#endif // QBUTTONGROUP_H
