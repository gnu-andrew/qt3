/****************************************************************************
**
** Taiwan Font utilities for X11
**
** Created : 20010130
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the tools module of the Qt GUI Toolkit.
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

#include "private/qfontcodecs_p.h"

#ifndef QT_NO_CODECS
#ifndef QT_NO_BIG_CODECS

extern int qt_UnicodeToBig5hkscs(uint wc, uchar *r);


int QFontBig5Codec::heuristicContentMatch(const char *, int) const
{
    return 0;
}


int QFontBig5Codec::heuristicNameMatch(const char* hint) const
{
    //qDebug("QFontBig5Codec::heuristicNameMatch(const char* hint = \"%s\")", hint);
    return ( qstricmp(hint, "big5-0") == 0 ||
	     qstricmp(hint, "big5.eten-0") == 0 )
	? 13 : 0;
}


QFontBig5Codec::QFontBig5Codec()
{
    //qDebug("QFontBig5Codec::QFontBig5Codec()");
}


const char* QFontBig5Codec::name() const
{
    //qDebug("QFontBig5Codec::name() = \"big5-0\"");
    return "big5-0";
}


int QFontBig5Codec::mibEnum() const
{
    //qDebug("QFontBig5Codec::mibEnum() = -2026");
    return -2026;
}


QString QFontBig5Codec::toUnicode(const char* /*chars*/, int /*len*/) const
{
    return QString::null;
}

unsigned short QFontBig5Codec::characterFromUnicode(const QString &str, int pos) const
{
    uchar c[2];
    if (qt_UnicodeToBig5hkscs((str.unicode() + pos)->unicode(), c) == 2 &&
        c[0] >= 0xa1 && c[0] <= 0xf9 )
        return (c[0] << 8) + c[1];
    return 0;
}

QCString QFontBig5Codec::fromUnicode(const QString& uc, int& lenInOut ) const
{
    //qDebug("QFontBig5Codec::fromUnicode(const QString& uc, int& lenInOut = %d)", lenInOut);
    QCString result(lenInOut * 2 + 1);
    uchar *rdata = (uchar *) result.data();
    const QChar *ucp = uc.unicode();

    for ( int i = 0; i < lenInOut; i++ ) {
	QChar ch(*ucp++);
	uchar c[2];

#if 0
	if ( ch.row() == 0) {
	    if ( ch.cell() == ' ' )
		ch = QChar( 0x3000 );
	    else if ( ch.cell() > ' ' && ch.cell() < 127 )
		ch = QChar( ch.cell()-' ', 255 );
	}
#endif
	if ( qt_UnicodeToBig5hkscs( ch.unicode(), c ) == 2 &&
	     c[0] >= 0xa1 && c[0] <= 0xf9 ) {
	    *rdata++ = c[0];
	    *rdata++ = c[1];
	} else {
	    //white square
	    *rdata++ = 0xa1;
	    *rdata++ = 0xbc;
	}
    }
    lenInOut *=2;
    return result;
}

/*! \internal */
void QFontBig5Codec::fromUnicode(const QChar *in, unsigned short *out, int length) const
{
    uchar c[2];
    while (length--) {
	if ( in->row() == 0x00 && in->cell() < 0x80 ) {
	    // ASCII
	    *out = in->cell();
	} else if ( qt_UnicodeToBig5hkscs( in->unicode(), c ) == 2
		    && c[0] >= 0xa1 && c[0] <= 0xf9 ) {
	    // Big5-ETen
	    *out = (c[0] << 8) | c[1];
	} else {
	    // Unknown char
	    *out = 0;
	}

	++in;
	++out;
    }
}

bool QFontBig5Codec::canEncode( QChar ch ) const
{
    //qDebug("QFontBig5Codec::canEncode( QChar ch = %02X%02X )", ch.row(), ch.cell());
    uchar c[2];
    return ( qt_UnicodeToBig5hkscs( ch.unicode(), c ) == 2 &&
	 c[0] >= 0xa1 && c[0] <= 0xf9 );
}

#endif // QT_NO_BIG_CODECS
#endif // QT_NO_CODECS
