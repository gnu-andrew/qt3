/****************************************************************************
**
** Definition of QFontMetrics class
**
** Created : 940514
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

#ifndef QFONTMETRICS_H
#define QFONTMETRICS_H

#ifndef QT_H
#include "qfont.h"
#include "qrect.h"
#endif // QT_H

#ifdef Q_WS_QWS
class QFontEngine;
#endif

class QTextCodec;
class QTextParag;

class Q_EXPORT QFontMetrics
{
public:
    QFontMetrics( const QFont & );
    QFontMetrics( const QFont &, QFont::Script );
    QFontMetrics( const QFontMetrics & );
    ~QFontMetrics();

    QFontMetrics &operator=( const QFontMetrics & );

    int		ascent()	const;
    int		descent()	const;
    int		height()	const;
    int		leading()	const;
    int		lineSpacing()	const;
    int		minLeftBearing() const;
    int		minRightBearing() const;
    int		maxWidth()	const;

    bool	inFont(QChar)	const;

    int		leftBearing(QChar) const;
    int		rightBearing(QChar) const;
    int		width( const QString &, int len = -1 ) const;

    int		width( QChar ) const;
#ifndef QT_NO_COMPAT
    int		width( char c ) const { return width( (QChar) c ); }
#endif

    int 		charWidth( const QString &str, int pos ) const;
    QRect	boundingRect( const QString &, int len = -1 ) const;
    QRect	boundingRect( QChar ) const;
    QRect	boundingRect( int x, int y, int w, int h, int flags,
			      const QString& str, int len=-1, int tabstops=0,
			      int *tabarray=0, QTextParag **intern=0 ) const;
    QSize	size( int flags,
		      const QString& str, int len=-1, int tabstops=0,
		      int *tabarray=0, QTextParag **intern=0 ) const;

    int		underlinePos()	const;
    int         overlinePos()   const;
    int		strikeOutPos()	const;
    int		lineWidth()	const;

private:
    QFontMetrics( const QPainter * );

    friend class QWidget;
    friend class QPainter;
    friend class QTextFormat;
#if defined( Q_WS_MAC )
    friend class QFontPrivate;
#endif

    QFontPrivate  *d;
    QPainter      *painter;
    int		   fscript;
};


#endif // QFONTMETRICS_H
