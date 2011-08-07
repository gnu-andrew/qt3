/****************************************************************************
**
** Definition of QCommonStyle class
**
** Created : 980616
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

#ifndef QCOMMONSTYLE_H
#define QCOMMONSTYLE_H

#ifndef QT_H
#include "qstyle.h"
#endif // QT_H

#ifndef QT_NO_STYLE

class Q_EXPORT QCommonStyle: public QStyle
{
    Q_OBJECT

public:
    QCommonStyle();
    ~QCommonStyle();

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

    void drawControlMask( ControlElement element,
			  QPainter *p,
			  const QWidget *widget,
			  const QRect &r,
			  const QStyleOption& = QStyleOption::Default ) const;

    QRect subRect( SubRect r, const QWidget *widget ) const;

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

    void drawComplexControlMask( ComplexControl control,
				 QPainter *p,
				 const QWidget *widget,
				 const QRect &r,
				 const QStyleOption& = QStyleOption::Default ) const;

    QRect querySubControlMetrics( ComplexControl control,
				  const QWidget *widget,
				  SubControl sc,
				  const QStyleOption& = QStyleOption::Default ) const;

    SubControl querySubControl( ComplexControl control,
				const QWidget *widget,
				const QPoint &pos,
				const QStyleOption& = QStyleOption::Default ) const;

    int pixelMetric( PixelMetric m, const QWidget *widget = 0 ) const;

    QSize sizeFromContents( ContentsType s,
			    const QWidget *widget,
			    const QSize &contentsSize,
			    const QStyleOption& = QStyleOption::Default ) const;

    int styleHint(StyleHint sh, const QWidget *, const QStyleOption &, QStyleHintReturn *) const;

    QPixmap stylePixmap( StylePixmap sp,
			 const QWidget *widget = 0,
			 const QStyleOption& = QStyleOption::Default ) const;


private:
    // Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QCommonStyle( const QCommonStyle & );
    QCommonStyle &operator=( const QCommonStyle & );
#endif
};



#endif // QT_NO_STYLE

#endif // QCOMMONSTYLE_H
