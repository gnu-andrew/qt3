/****************************************************************************
**
** Definition of the QSimpleRichText class
**
** Created : 990101
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

#ifndef QSIMPLERICHTEXT_H
#define QSIMPLERICHTEXT_H

#ifndef QT_H
#include "qnamespace.h"
#include "qstring.h"
#include "qregion.h"
#endif // QT_H

#ifndef QT_NO_RICHTEXT

class QPainter;
class QWidget;
class QStyleSheet;
class QBrush;
class QMimeSourceFactory;
class QSimpleRichTextData;

class Q_EXPORT QSimpleRichText
{
public:
    QSimpleRichText( const QString& text, const QFont& fnt,
		     const QString& context = QString::null, const QStyleSheet* sheet = 0);
    QSimpleRichText( const QString& text, const QFont& fnt,
		     const QString& context,  const QStyleSheet* sheet,
		     const QMimeSourceFactory* factory, int pageBreak = -1,
		     const QColor& linkColor = Qt::blue, bool linkUnderline = TRUE );
    ~QSimpleRichText();

    void setWidth( int );
    void setWidth( QPainter*, int );
    void setDefaultFont( const QFont &f );
    int width() const;
    int widthUsed() const;
    int height() const;
    void adjustSize();

    void draw( QPainter* p,  int x, int y, const QRect& clipRect,
	       const QColorGroup& cg, const QBrush* paper = 0) const;

    // obsolete
    void draw( QPainter* p,  int x, int y, const QRegion& clipRegion,
	       const QColorGroup& cg, const QBrush* paper = 0) const {
	draw( p, x, y, clipRegion.boundingRect(), cg, paper );
    }

    QString context() const;
    QString anchorAt( const QPoint& pos ) const;

    bool inText( const QPoint& pos ) const;

private:
    QSimpleRichTextData* d;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QSimpleRichText( const QSimpleRichText & );
    QSimpleRichText &operator=( const QSimpleRichText & );
#endif
};

#endif // QT_NO_RICHTEXT

#endif // QSIMPLERICHTEXT_H
