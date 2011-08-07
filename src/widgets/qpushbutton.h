/****************************************************************************
**
** Definition of QPushButton class
**
** Created : 940221
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

#ifndef QPUSHBUTTON_H
#define QPUSHBUTTON_H

#ifndef QT_H
#include "qbutton.h"
#include "qiconset.h"
#endif // QT_H

#ifndef QT_NO_PUSHBUTTON
class QPushButtonPrivate;
class QPopupMenu;

class Q_EXPORT QPushButton : public QButton
{
    Q_OBJECT

    Q_PROPERTY( bool autoDefault READ autoDefault WRITE setAutoDefault )
    Q_PROPERTY( bool default READ isDefault WRITE setDefault )
    Q_PROPERTY( bool menuButton READ isMenuButton DESIGNABLE false )
    Q_PROPERTY( QIconSet iconSet READ iconSet WRITE setIconSet )
    Q_OVERRIDE( bool toggleButton WRITE setToggleButton )
    Q_OVERRIDE( bool on WRITE setOn )
    Q_PROPERTY( bool flat READ isFlat WRITE setFlat )
    Q_OVERRIDE( bool autoMask DESIGNABLE true SCRIPTABLE true )

public:
    QPushButton( QWidget *parent, const char* name=0 );
    QPushButton( const QString &text, QWidget *parent, const char* name=0 );
#ifndef QT_NO_ICONSET
    QPushButton( const QIconSet& icon, const QString &text, QWidget *parent, const char* name=0 );
#endif
    ~QPushButton();

    QSize	sizeHint() const;

    void	move( int x, int y );
    void	move( const QPoint &p );
    void	resize( int w, int h );
    void	resize( const QSize & );
    void	setGeometry( int x, int y, int w, int h );

    void	setGeometry( const QRect & );

    void setToggleButton( bool );

    bool	autoDefault()	const	{ return autoDefButton; }
    virtual void setAutoDefault( bool autoDef );
    bool	isDefault()	const	{ return defButton; }
    virtual void setDefault( bool def );

    virtual void setIsMenuButton( bool enable ) {  // obsolete functions
	if ( (bool)hasMenuArrow == enable )
	    return;
	hasMenuArrow = enable ? 1 : 0;
	update();
	updateGeometry();
    }
    bool	isMenuButton() const { return hasMenuArrow; }

#ifndef QT_NO_POPUPMENU
    void setPopup( QPopupMenu* popup );
    QPopupMenu* popup() const;
#endif
#ifndef QT_NO_ICONSET
    void setIconSet( const QIconSet& );
    QIconSet* iconSet() const;
#endif
    void setFlat( bool );
    bool isFlat() const;

public slots:
    virtual void setOn( bool );

protected:
    void	drawButton( QPainter * );
    void	drawButtonLabel( QPainter * );
    void	focusInEvent( QFocusEvent * );
    void	focusOutEvent( QFocusEvent * );
    void	resizeEvent( QResizeEvent * );
    void	updateMask();
private slots:
#ifndef QT_NO_POPUPMENU
    void popupPressed();
#endif
private:
    void	init();

    uint	autoDefButton	: 1;
    uint	defButton	: 1;
    uint	flt		: 1;
    uint	reserved		: 1; // UNUSED
    uint	lastEnabled	: 1; // UNUSED
    uint	hasMenuArrow	: 1;

    QPushButtonPrivate* d;

    friend class QDialog;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QPushButton( const QPushButton & );
    QPushButton &operator=( const QPushButton & );
#endif
};


#endif // QT_NO_PUSHBUTTON

#endif // QPUSHBUTTON_H
