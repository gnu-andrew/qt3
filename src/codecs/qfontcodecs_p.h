/****************************************************************************
**
** Font utilities for X11
**
** Created : 20001101
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

#ifndef QFONTCODECS_P_H
#define QFONTCODECS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of qfontencodings_x11.cpp and qfont_x11.cpp.  This header file may
// change from version to version without notice, or even be removed.
//
// We mean it.
//
//

#ifndef QT_H
#include "qglobal.h"
#include "qtextcodec.h"
#endif // QT_H


#ifndef QT_NO_CODECS
#ifndef QT_NO_BIG_CODECS


class QJpUnicodeConv;


class QFontJis0201Codec : public QTextCodec
{
public:
    QFontJis0201Codec();

    const char *name() const;

    int mibEnum() const;

#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut ) const;
    void fromUnicode( const QChar *in, unsigned short *out, int length ) const;

    unsigned short characterFromUnicode(const QString &str, int pos) const;

    int heuristicContentMatch(const char *, int) const;
    int heuristicNameMatch(const char* hint) const;

#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::canEncode;
#endif
    bool canEncode( QChar ) const;
};


class QFontJis0208Codec : public QTextCodec
{
public:
    QFontJis0208Codec();
    ~QFontJis0208Codec();

    // Return the official name for the encoding.
    const char* name() const ;

    // Return the MIB enum for the encoding if it is listed in the
    // IANA character-sets encoding file.
    int mibEnum() const ;

    // Converts len characters from chars to Unicode.
    QString toUnicode(const char* chars, int len) const ;

    // Converts lenInOut characters (of type QChar) from the start of
    // the string uc, returning a QCString result, and also returning
    // the length of the result in lenInOut.
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut ) const;
    void fromUnicode( const QChar *in, unsigned short *out, int length ) const;

    unsigned short characterFromUnicode(const QString &str, int pos) const;

    int heuristicContentMatch(const char *, int) const;
    int heuristicNameMatch(const char* hint) const;

#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::canEncode;
#endif
    bool canEncode( QChar ) const;

private:
    QJpUnicodeConv *convJP;
};




class QFontKsc5601Codec : public QTextCodec
{
public:
    QFontKsc5601Codec();

    // Return the official name for the encoding.
    const char* name() const ;

    // Return the MIB enum for the encoding if it is listed in the
    // IANA character-sets encoding file.
    int mibEnum() const ;

    // Converts len characters from chars to Unicode.
    QString toUnicode(const char* chars, int len) const ;

    // Converts lenInOut characters (of type QChar) from the start of
    // the string uc, returning a QCString result, and also returning
    // the length of the result in lenInOut.
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut ) const;
    void fromUnicode( const QChar *in, unsigned short *out, int length ) const;

    unsigned short characterFromUnicode(const QString &str, int pos) const;

    int heuristicContentMatch(const char *, int) const;
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::canEncode;
#endif
    bool canEncode( QChar ) const;
};




class QFontGb2312Codec : public QTextCodec
{
public:
    QFontGb2312Codec();

    // Return the official name for the encoding.
    const char* name() const ;

    // Return the MIB enum for the encoding if it is listed in the
    // IANA character-sets encoding file.
    int mibEnum() const ;

    // Converts len characters from chars to Unicode.
    QString toUnicode(const char* chars, int len) const ;

    // Converts lenInOut characters (of type QChar) from the start of
    // the string uc, returning a QCString result, and also returning
    // the length of the result in lenInOut.
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut ) const;
    void fromUnicode( const QChar *in, unsigned short *out, int length ) const;

    unsigned short characterFromUnicode(const QString &str, int pos) const;

    int heuristicContentMatch(const char *, int) const;

#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::canEncode;
#endif
    bool canEncode( QChar ) const;
};




class QFontGbkCodec : public QTextCodec
{
public:
    QFontGbkCodec();

    // Return the official name for the encoding.
    const char* name() const ;

    // Return the MIB enum for the encoding if it is listed in the
    // IANA character-sets encoding file.
    int mibEnum() const ;

    // Converts len characters from chars to Unicode.
    QString toUnicode(const char* chars, int len) const ;

    // Converts lenInOut characters (of type QChar) from the start of
    // the string uc, returning a QCString result, and also returning
    // the length of the result in lenInOut.
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut ) const;
    void fromUnicode( const QChar *in, unsigned short *out, int length ) const;

    unsigned short characterFromUnicode(const QString &str, int pos) const;

    int heuristicContentMatch(const char *, int) const;
    int heuristicNameMatch(const char* hint) const;

#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::canEncode;
#endif
    bool canEncode( QChar ) const;
};




class QFontGb18030_0Codec : public QTextCodec
{
public:
    QFontGb18030_0Codec();

    // Return the official name for the encoding.
    const char* name() const ;

    // Return the MIB enum for the encoding if it is listed in the
    // IANA character-sets encoding file.
    int mibEnum() const ;

    // Converts len characters from chars to Unicode.
    QString toUnicode(const char* chars, int len) const ;

    // Converts lenInOut characters (of type QChar) from the start of
    // the string uc, returning a QCString result, and also returning
    // the length of the result in lenInOut.
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut ) const;
    void fromUnicode( const QChar *in, unsigned short *out, int length ) const;

    unsigned short characterFromUnicode(const QString &str, int pos) const;

    int heuristicContentMatch(const char *, int) const;
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::canEncode;
#endif
    bool canEncode( QChar ) const;
};




class QFontBig5Codec : public QTextCodec
{
public:
    QFontBig5Codec();

    // Return the official name for the encoding.
    const char* name() const ;

    // Return the MIB enum for the encoding if it is listed in the
    // IANA character-sets encoding file.
    int mibEnum() const ;

    // Converts len characters from chars to Unicode.
    QString toUnicode(const char* chars, int len) const ;

    // Converts lenInOut characters (of type QChar) from the start of
    // the string uc, returning a QCString result, and also returning
    // the length of the result in lenInOut.
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut ) const;
    void fromUnicode( const QChar *in, unsigned short *out, int length ) const;

    unsigned short characterFromUnicode(const QString &str, int pos) const;

    int heuristicContentMatch(const char *, int) const;
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::canEncode;
#endif
    int heuristicNameMatch(const char* hint) const;
    bool canEncode( QChar ) const;
};



class QFontBig5hkscsCodec : public QTextCodec
{
public:
    QFontBig5hkscsCodec();

    // Return the official name for the encoding.
    const char* name() const ;

    // Return the MIB enum for the encoding if it is listed in the
    // IANA character-sets encoding file.
    int mibEnum() const ;

    // Converts len characters from chars to Unicode.
    QString toUnicode(const char* chars, int len) const ;

    // Converts lenInOut characters (of type QChar) from the start of
    // the string uc, returning a QCString result, and also returning
    // the length of the result in lenInOut.
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut ) const;
    void fromUnicode( const QChar *in, unsigned short *out, int length ) const;

    unsigned short characterFromUnicode(const QString &str, int pos) const;

    int heuristicContentMatch(const char *, int) const;
    int heuristicNameMatch(const char* hint) const;
#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::canEncode;
#endif
    bool canEncode( QChar ) const;
};


class QFontLaoCodec : public QTextCodec
{
public:
    QFontLaoCodec();

    const char *name() const;

    int mibEnum() const;

#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut ) const;
    void fromUnicode( const QChar *in, unsigned short *out, int length ) const;

    unsigned short characterFromUnicode(const QString &str, int pos) const;

    int heuristicContentMatch(const char *, int) const;

#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::canEncode;
#endif
    bool canEncode( QChar ) const;
};

#endif // QT_NO_BIG_CODECS
#endif // QT_NO_CODECS

#endif // QFONTCODECS_P_H
