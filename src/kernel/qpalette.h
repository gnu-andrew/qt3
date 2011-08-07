/****************************************************************************
**
** Definition of QColorGroup and QPalette classes
**
** Created : 950323
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the kernel module of the Qt GUI Toolkit.
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

#ifndef QPALETTE_H
#define QPALETTE_H

#ifndef QT_H
#include "qwindowdefs.h"
#include "qcolor.h"
#include "qshared.h"
#include "qbrush.h" // QColor->QBrush conversion
#endif // QT_H

#ifndef QT_NO_PALETTE

class QColorGroupPrivate;

class Q_EXPORT QColorGroup
{
public:
    QColorGroup();
    QColorGroup( const QColor &foreground, const QColor &button,
		 const QColor &light, const QColor &dark, const QColor &mid,
		 const QColor &text, const QColor &base );
    QColorGroup( const QBrush &foreground, const QBrush &button,
		 const QBrush &light, const QBrush &dark, const QBrush &mid,
		 const QBrush &text, const QBrush &bright_text,
		 const QBrush &base, const QBrush &background);
    QColorGroup( const QColorGroup & );

   ~QColorGroup();

    QColorGroup& operator =(const QColorGroup&);

    // Do not change the order, the serialization format depends on it
    enum ColorRole { Foreground, Button, Light, Midlight, Dark, Mid,
		     Text, BrightText, ButtonText, Base, Background, Shadow,
		     Highlight, HighlightedText, Link, LinkVisited,
		     NColorRoles };

    const QColor &color( ColorRole ) const;
    const QBrush &brush( ColorRole ) const;
    void setColor( ColorRole, const QColor & );
    void setBrush( ColorRole, const QBrush & );

    const QColor &foreground()	const	{ return br[Foreground].color(); }
    const QColor &button()	const	{ return br[Button].color(); }
    const QColor &light()	const	{ return br[Light].color(); }
    const QColor &dark()	const	{ return br[Dark].color(); }
    const QColor &mid()		const	{ return br[Mid].color(); }
    const QColor &text()	const	{ return br[Text].color(); }
    const QColor &base()	const	{ return br[Base].color(); }
    const QColor &background()	const	{ return br[Background].color(); }

    const QColor &midlight()	const	{ return br[Midlight].color(); }
    const QColor &brightText()	const	{ return br[BrightText].color(); }
    const QColor &buttonText()	const	{ return br[ButtonText].color(); }
    const QColor &shadow()	const	{ return br[Shadow].color(); }
    const QColor &highlight()	const	{ return br[Highlight].color(); }
    const QColor &highlightedText() const{return br[HighlightedText].color(); }
    const QColor &link()        const   { return br[Link].color(); }
    const QColor &linkVisited() const   { return br[LinkVisited].color(); }

    bool	operator==( const QColorGroup &g ) const;
    bool	operator!=( const QColorGroup &g ) const
	{ return !(operator==(g)); }

private:
    QBrush *br;
    QColorGroupPrivate * d;

    friend class QPalette;
};


class Q_EXPORT QPalette
{
public:
    QPalette();
    QPalette( const QColor &button );
    QPalette( const QColor &button, const QColor &background );
    QPalette( const QColorGroup &active, const QColorGroup &disabled,
	      const QColorGroup &inactive );
    QPalette( const QPalette & );
    ~QPalette();
    QPalette &operator=( const QPalette & );

    enum ColorGroup { Disabled, Active, Inactive, NColorGroups, Normal=Active };

    const QColor &color( ColorGroup, QColorGroup::ColorRole ) const;
    const QBrush &brush( ColorGroup, QColorGroup::ColorRole ) const;
    void setColor( ColorGroup, QColorGroup::ColorRole, const QColor & );
    void setBrush( ColorGroup, QColorGroup::ColorRole, const QBrush & );

    void setColor( QColorGroup::ColorRole, const QColor & );
    void setBrush( QColorGroup::ColorRole, const QBrush & );

    QPalette	copy() const;

    const QColorGroup &active() const { return data->active; }
    const QColorGroup &disabled() const { return data->disabled; }
    const QColorGroup &inactive() const { return data->inactive; }
#ifndef QT_NO_COMPAT
    const QColorGroup &normal() const { return active(); }
#endif

    void	setActive( const QColorGroup & );
    void	setDisabled( const QColorGroup & );
    void	setInactive( const QColorGroup & );
#ifndef QT_NO_COMPAT
    void	setNormal( const QColorGroup & cg ) { setActive(cg); }
#endif

    bool	operator==( const QPalette &p ) const;
    bool	operator!=( const QPalette &p ) const
              { return !(operator==(p)); }
    bool	isCopyOf( const QPalette & );

    int		serialNumber() const	{ return data->ser_no; }
    
    
    static QColorGroup::ColorRole foregroundRoleFromMode( Qt::BackgroundMode mode );
    static QColorGroup::ColorRole backgroundRoleFromMode( Qt::BackgroundMode mode);

private:
    void	detach();
    const QBrush &directBrush( ColorGroup, QColorGroup::ColorRole ) const;
    void directSetBrush( ColorGroup, QColorGroup::ColorRole, const QBrush& );

    struct QPalData : public QShared {
	QColorGroup disabled;
	QColorGroup active;
	int	    ser_no;
	QColorGroup inactive;
    } *data;
};


/*****************************************************************************
  QColorGroup/QPalette stream functions
 *****************************************************************************/

#ifndef QT_NO_DATASTREAM
Q_EXPORT QDataStream &operator<<( QDataStream &, const QColorGroup & );
Q_EXPORT QDataStream &operator>>( QDataStream &, QColorGroup & );

Q_EXPORT QDataStream &operator<<( QDataStream &, const QPalette & );
Q_EXPORT QDataStream &operator>>( QDataStream &, QPalette & );
#endif // QT_NO_DATASTREAM

#endif // QT_NO_PALETTE
#endif // QPALETTE_H
