/****************************************************************************
**
** Implementation of QInterlaceStyle widget class
**
** Created : 010122
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

#if 0 // ###### not ported to new API yet
#ifndef QINTERLACESTYLE_H
#define QINTERLACESTYLE_H

#ifndef QT_H
#include "qmotifstyle.h"
#endif // QT_H

#if !defined(QT_NO_STYLE_INTERLACE) || defined(QT_PLUGIN)

#include "qpalette.h"

class Q_EXPORT QInterlaceStyle : public QMotifStyle
{
public:
    QInterlaceStyle();
    void polish( QApplication*);
    void unPolish( QApplication*);
    void polish( QWidget* );
    void unPolish( QWidget* );

    int defaultFrameWidth() const;
    QRect pushButtonContentsRect( QPushButton *btn );

    void drawFocusRect ( QPainter *, const QRect &, const QColorGroup &, const QColor * bg = 0, bool = FALSE );
    void drawButton( QPainter *p, int x, int y, int w, int h,
			     const QColorGroup &g, bool sunken = FALSE,
			     const QBrush *fill = 0 );
    void drawButtonMask ( QPainter * p, int x, int y, int w, int h );
    void drawBevelButton( QPainter *p, int x, int y, int w, int h,
			  const QColorGroup &g, bool sunken = FALSE,
			  const QBrush *fill = 0 );

    void drawPushButton( QPushButton* btn, QPainter *p);
    QSize indicatorSize () const;
    void drawIndicator ( QPainter * p, int x, int y, int w, int h, const QColorGroup & g, int state, bool down = FALSE, bool enabled = TRUE );
    void drawIndicatorMask( QPainter *p, int x, int y, int w, int h, int );
    QSize exclusiveIndicatorSize () const;
    void drawExclusiveIndicator( QPainter * p, int x, int y, int w, int h, const QColorGroup & g, bool on, bool down = FALSE, bool enabled = TRUE );
    void drawExclusiveIndicatorMask( QPainter * p, int x, int y, int w, int h, bool );
    QRect comboButtonRect ( int x, int y, int w, int h );
    void drawComboButton( QPainter *p, int x, int y, int w, int h, const QColorGroup &g, bool sunken, bool editable, bool enabled, const QBrush *fb );
    void drawPushButtonLabel( QPushButton* btn, QPainter *p);
    void drawPanel( QPainter *p, int x, int y, int w, int h,
		    const QColorGroup &, bool sunken,
		    int lineWidth, const QBrush *fill );

    void scrollBarMetrics( const QScrollBar* sb, int &sliderMin, int &sliderMax, int &sliderLength, int &buttonDim );
    void drawScrollBarControls( QPainter* p, const QScrollBar* sb, int sliderStart, uint controls, uint activeControl );
    void drawSlider( QPainter * p, int x, int y, int w, int h, const QColorGroup & g, Orientation, bool tickAbove, bool tickBelow );
    void drawSliderGroove( QPainter * p, int x, int y, int w, int h, const QColorGroup & g, QCOORD c, Orientation );
    int splitterWidth() const;
    void drawSplitter( QPainter *p, int x, int y, int w, int h,
		      const QColorGroup &g, Orientation orient);

    int buttonDefaultIndicatorWidth() const;
    int setSliderThickness() const;
    QSize scrollBarExtent() const;

private:
    QPalette oldPalette;
};

#endif // QT_NO_STYLE_INTERLACE

#endif
#endif
