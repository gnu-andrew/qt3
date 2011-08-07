/****************************************************************************
**
** Definition of Platinum-like style class
**
** Created : 981231
**
** Copyright (C) 1998-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QPLATINUMSTYLE_H
#define QPLATINUMSTYLE_H

#ifndef QT_H
#include "qwindowsstyle.h"
#endif // QT_H

#if !defined(QT_NO_STYLE_PLATINUM) || defined(QT_PLUGIN)

class QPalette;

#if defined(QT_PLUGIN)
#define Q_EXPORT_STYLE_PLATINUM
#else
#define Q_EXPORT_STYLE_PLATINUM Q_EXPORT
#endif

class Q_EXPORT_STYLE_PLATINUM QPlatinumStyle : public QWindowsStyle
{
    Q_OBJECT
public:
    QPlatinumStyle();
    virtual ~QPlatinumStyle();

    // new Style Stuff
    void drawPrimitive( PrimitiveElement pe,
			QPainter *p,
			const QRect &r,
			const QColorGroup &cg,
			SFlags flags = Style_Default,
			const QStyleOption& = QStyleOption::Default ) const;

    void drawControl( ControlElement element,
		      QPainter *p,
		      const QWidget *widget,
		      const QRect &r,
		      const QColorGroup &cg,
		      SFlags how = Style_Default,
		      const QStyleOption& = QStyleOption::Default ) const;

    void drawComplexControl( ComplexControl control,
			     QPainter *p,
			     const QWidget *widget,
			     const QRect &r,
			     const QColorGroup &cg,
			     SFlags how = Style_Default,
#ifdef Q_QDOC
			     SCFlags sub = SC_All,
#else
			     SCFlags sub = (uint)SC_All,
#endif
			     SCFlags subActive = SC_None,
			     const QStyleOption& = QStyleOption::Default ) const;

    QRect querySubControlMetrics( ComplexControl control,
				  const QWidget *widget,
				  SubControl sc,
				  const QStyleOption& = QStyleOption::Default ) const;

    int pixelMetric( PixelMetric metric, const QWidget *widget = 0 ) const;

    QRect subRect( SubRect r, const QWidget *widget ) const;

protected:
     QColor mixedColor(const QColor &, const QColor &) const;
    void drawRiffles( QPainter* p,  int x, int y, int w, int h,
		      const QColorGroup &g, bool horizontal ) const;
private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QPlatinumStyle( const QPlatinumStyle & );
    QPlatinumStyle& operator=( const QPlatinumStyle & );
#endif
};

#endif // QT_NO_STYLE_PLATINUM

#endif // QPLATINUMSTYLE_H
