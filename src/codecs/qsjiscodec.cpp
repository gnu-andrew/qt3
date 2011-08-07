/****************************************************************************
**
** Implementation of QSjisCodec class
**
** Created : 990225
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
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

// Most of the code here was originally written by Serika Kurusugawa
// a.k.a. Junji Takagi, and is included in Qt with the author's permission,
// and the grateful thanks of the Trolltech team.

/*! \class QSjisCodec qsjiscodec.h
    \reentrant
    \ingroup i18n
  \brief The QSjisCodec class provides conversion to and from Shift-JIS.

  More precisely, the QSjisCodec class subclasses QTextCodec to
  provide support for Shift-JIS, an encoding of JIS X 0201 Latin, JIS
  X 0201 Kana or JIS X 0208.

  The environment variable \c UNICODEMAP_JP can be used to fine-tune
  QJisCodec, QSjisCodec and QEucJpCodec. The \l QJisCodec
  documentation describes how to use this variable.

  Most of the code here was written by Serika Kurusugawa,
  a.k.a. Junji Takagi, and is included in Qt with the author's
  permission and the grateful thanks of the Trolltech team.
  Here is the copyright statement for the code as it was at the
  point of contribution. Trolltech's subsequent modifications
  are covered by the usual copyright for Qt.

  \legalese

  Copyright (C) 1999 Serika Kurusugawa. All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  \list 1
  \i Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  \i Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.
  \endlist

  THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS "AS IS".
  ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
  ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
  OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
  HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
  OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
  SUCH DAMAGE.
*/

#include "qsjiscodec.h"

#ifndef QT_NO_BIG_CODECS

static const uchar Esc = 0x1b;

#define	IsKana(c)	(((c) >= 0xa1) && ((c) <= 0xdf))
#define	IsSjisChar1(c)	((((c) >= 0x81) && ((c) <= 0x9f)) ||	\
			 (((c) >= 0xe0) && ((c) <= 0xfc)))
#define	IsSjisChar2(c)	(((c) >= 0x40) && ((c) != 0x7f) && ((c) <= 0xfc))
#define	IsUserDefinedChar1(c)	(((c) >= 0xf0) && ((c) <= 0xfc))

#define	QValidChar(u)	((u) ? QChar((ushort)(u)) : QChar::replacement)

/*!
  Creates a Shift-JIS codec. Note that this is done automatically by
  the QApplication, you do not need construct your own.
*/
QSjisCodec::QSjisCodec() : conv(QJpUnicodeConv::newConverter(QJpUnicodeConv::Default))
{
}


/*!
  Destroys the Shift-JIS codec.
*/
QSjisCodec::~QSjisCodec()
{
    delete (QJpUnicodeConv*)conv;
    conv = 0;
}


/*!
  \reimp
*/
int QSjisCodec::mibEnum() const
{
    /*
    Name: Shift_JIS  (preferred MIME name)
    MIBenum: 17
    Source: A Microsoft code that extends csHalfWidthKatakana to include
	    kanji by adding a second byte when the value of the first
	    byte is in the ranges 81-9F or E0-EF.
    Alias: MS_Kanji
    Alias: csShiftJIS
    */
    return 17;
}

/*!
  \reimp
*/
QCString QSjisCodec::fromUnicode(const QString& uc, int& lenInOut) const
{
    int l = QMIN((int)uc.length(),lenInOut);
    int rlen = l*2+1;
    QCString rstr(rlen);
    uchar* cursor = (uchar*)rstr.data();
    for (int i=0; i<l; i++) {
	QChar ch = uc[i];
	uint j;
	if ( ch.row() == 0x00 && ch.cell() < 0x80 ) {
	    // ASCII
	    *cursor++ = ch.cell();
	} else if ((j = conv->unicodeToJisx0201(ch.row(), ch.cell())) != 0) {
	    // JIS X 0201 Latin or JIS X 0201 Kana
	    *cursor++ = j;
	} else if ((j = conv->unicodeToSjis(ch.row(), ch.cell())) != 0) {
	    // JIS X 0208
	    *cursor++ = (j >> 8);
	    *cursor++ = (j & 0xff);
	} else if ((j = conv->unicodeToSjisibmvdc(ch.row(), ch.cell())) != 0) {
	    // JIS X 0208 IBM VDC
	    *cursor++ = (j >> 8);
	    *cursor++ = (j & 0xff);
	} else if ((j = conv->unicodeToCp932(ch.row(), ch.cell())) != 0) {
	    // CP932 (for lead bytes 87, ee & ed)
	    *cursor++ = (j >> 8);
	    *cursor++ = (j & 0xff);
	} else if ((j = conv->unicodeToJisx0212(ch.row(), ch.cell())) != 0) {
	    // JIS X 0212 (can't be encoded in ShiftJIS !)
	    *cursor++ = 0x81;	// white square
	    *cursor++ = 0xa0;	// white square
	} else {
	    // Error
	    *cursor++ = '?';	// unknown char
	}
    }
    lenInOut = cursor - (uchar*)rstr.data();
    rstr.truncate(lenInOut);
    return rstr;
}

/*!
  \reimp
*/
QString QSjisCodec::toUnicode(const char* chars, int len) const
{
    uint u;
    QString result;
    for (int i=0; i<len; i++) {
	uchar ch = chars[i];
	if ( ch < 0x80 || IsKana(ch) ) {
	    // JIS X 0201 Latin or JIS X 0201 Kana
	    u = conv->jisx0201ToUnicode(ch);
	    result += QValidChar(u);
	} else if ( IsSjisChar1(ch) ) {
	    // JIS X 0208
	    if ( i < len-1 ) {
		uchar c2 = chars[++i];
		if ( IsSjisChar2(c2) ) {
			if ((u = conv->sjisibmvdcToUnicode(ch, c2))) {
				result += QValidChar(u);
			} else if ((u = conv->cp932ToUnicode(ch, c2))) {
				result += QValidChar(u);
			} else if ( IsUserDefinedChar1(ch) ) {
				result += QChar::replacement;
			} else {
				u = conv->sjisToUnicode(ch, c2);
				result += QValidChar(u);
			}
		} else {
		    i--;
		    result += QChar::replacement;
		}
	    } else {
		result += QChar::replacement;
	    }
	} else {
	    result += QChar::replacement;
	}
    }
    return result;
}

/*!
  \reimp
*/
const char* QSjisCodec::name() const
{
    return "SJIS";
}

/*!
    Returns the codec's mime name.
*/
const char* QSjisCodec::mimeName() const
{
    return "Shift_JIS";
}

/*!
  \reimp
*/
int QSjisCodec::heuristicNameMatch(const char* hint) const
{
    int score = 0;
    bool ja = FALSE;
    if (qstrnicmp(hint, "ja_JP", 5) == 0 || qstrnicmp(hint, "japan", 5) == 0) {
	score += 3;
	ja = TRUE;
    } else if (qstrnicmp(hint, "ja", 2) == 0) {
	score += 2;
	ja = TRUE;
    }
    const char *p;
    if (ja) {
	p = strchr(hint, '.');
	if (p == 0) {
	    return score - 1;
	}
	p++;
    } else {
	p = hint;
    }
    if (p) {
	if ((qstricmp(p, "mscode") == 0) ||
	    (qstricmp(p, "PCK") == 0) ||
	    (qstricmp(p, "SJIS") == 0) ||
	    (simpleHeuristicNameMatch(p, "ShiftJIS") > 0) ||
	    (simpleHeuristicNameMatch(p, "x-sjis") > 0)) {
	    return score + 4;
	}
    }
    return QTextCodec::heuristicNameMatch(hint);
}

/*!
  \reimp
*/
int QSjisCodec::heuristicContentMatch(const char* chars, int len) const
{
    int score = 0;
    for (int i=0; i<len; i++) {
	uchar ch = chars[i];
	// No nulls allowed.
	if ( !ch || ch == Esc )
	    return -1;
	if ( ch < 32 && ch != '\t' && ch != '\n' && ch != '\r' ) {
	    // Suspicious
	    if ( score )
		score--;
	} else if ( ch < 0x80 ) {
	    // Inconclusive
	    score++;
	} else if ( IsKana(ch) ) {
	    // JIS X 0201 Kana
	    score++;
	} else if ( IsSjisChar1(ch) ) {
	    // JIS X 0208-1990
	    if ( i < len-1 ) {
		uchar c2 = chars[++i];
		if ( !IsSjisChar2(c2) )
		    return -1;
		score++;
	    }
	    score++;
	} else {
	    // Invalid
	    return -1;
	}
    }
    return score;
}

class QSjisDecoder : public QTextDecoder {
    uchar buf[1];
    int nbuf;
    const QJpUnicodeConv * const conv;
public:
    QSjisDecoder(const QJpUnicodeConv *c) : nbuf(0), conv(c)
    {
    }

    QString toUnicode(const char* chars, int len)
    {
	QString result;
    uint u= 0;
	for (int i=0; i<len; i++) {
	    uchar ch = chars[i];
	    switch (nbuf) {
	      case 0:
		if ( ch < 0x80 || IsKana(ch) ) {
		    // JIS X 0201 Latin or JIS X 0201 Kana
		    u = conv->jisx0201ToUnicode(ch);
		    result += QValidChar(u);
		} else if ( IsSjisChar1(ch) ) {
		    // JIS X 0208
		    buf[0] = ch;
		    nbuf = 1;
		} else {
		    // Invalid
		    result += QChar::replacement;
		}
		break;
	      case 1:
		// JIS X 0208
		if ( IsSjisChar2(ch) ) {
			if ((u = conv->sjisibmvdcToUnicode(buf[0], ch))) {
				result += QValidChar(u);
			} else if ((u = conv->cp932ToUnicode(buf[0], ch))) {
				result += QValidChar(u);
			} else if ( IsUserDefinedChar1(buf[0]) ) {
				result += QChar::replacement;
			} else {
				u = conv->sjisToUnicode(buf[0], ch);
				result += QValidChar(u);
			}
		} else {
		    // Invalid
		    result += QChar::replacement;
		}
		nbuf = 0;
		break;
	    }
	}
	return result;
    }
};

/*!
  \reimp
*/
QTextDecoder* QSjisCodec::makeDecoder() const
{
    return new QSjisDecoder(conv);
}

#endif
