/****************************************************************************
**
** Definition of Windows-like style class
**
** Created : 981231
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

#ifndef QWINDOWSSTYLE_H
#define QWINDOWSSTYLE_H

#ifndef QT_H
#include "qcommonstyle.h"
#endif // QT_H

#if !defined(QT_NO_STYLE_WINDOWS) || defined(QT_PLUGIN)

#if defined(QT_PLUGIN)
#define Q_EXPORT_STYLE_WINDOWS
#else
#define Q_EXPORT_STYLE_WINDOWS Q_EXPORT
#endif


class Q_EXPORT_STYLE_WINDOWS QWindowsStyle : public QCommonStyle
{
    Q_OBJECT
public:
    QWindowsStyle();
    ~QWindowsStyle();

    void polish(QApplication*);
    void unPolish(QApplication*);

    void polish(QWidget*);
    void unPolish(QWidget*);

    void polish( QPalette & );

    virtual void polishPopupMenu( QPopupMenu* );

    // new stuff
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
		      SFlags flags = Style_Default,
		      const QStyleOption& = QStyleOption::Default ) const;

    void drawComplexControl( ComplexControl control,
			     QPainter* p,
			     const QWidget* widget,
			     const QRect& r,
			     const QColorGroup& cg,
			     SFlags flags = Style_Default,
#ifdef Q_QDOC
			     SCFlags sub = SC_All,
#else
			     SCFlags sub = (uint)SC_All,
#endif
			     SCFlags subActive = SC_None,
			     const QStyleOption& = QStyleOption::Default ) const;

    int pixelMetric( PixelMetric metric,
		     const QWidget *widget = 0 ) const;

    QSize sizeFromContents( ContentsType contents,
			    const QWidget *widget,
			    const QSize &contentsSize,
			    const QStyleOption& = QStyleOption::Default ) const;

    int styleHint(StyleHint sh, const QWidget *, const QStyleOption & = QStyleOption::Default,
		  QStyleHintReturn* = 0) const;

    QPixmap stylePixmap( StylePixmap stylepixmap,
			 const QWidget *widget = 0,
			 const QStyleOption& = QStyleOption::Default ) const;

    QRect subRect( SubRect r, const QWidget *widget ) const;


private:
    class Private;
    Private *d;

    // Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QWindowsStyle( const QWindowsStyle & );
    QWindowsStyle& operator=( const QWindowsStyle & );
#endif
};

#endif // QT_NO_STYLE_WINDOWS

#endif // QWINDOWSSTYLE_H
