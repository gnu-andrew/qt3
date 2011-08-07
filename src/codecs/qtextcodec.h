/****************************************************************************
**
** Definition of QTextCodec class
**
** Created : 981015
**
** Copyright (C) 1998-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QTEXTCODEC_H
#define QTEXTCODEC_H

#ifndef QT_H
#include "qstring.h"
#endif // QT_H

#ifndef QT_NO_TEXTCODEC

class QTextCodec;
class QIODevice;
class QFont;

class Q_EXPORT QTextEncoder {
public:
    virtual ~QTextEncoder();
    virtual QCString fromUnicode(const QString& uc, int& lenInOut) = 0;
};

class Q_EXPORT QTextDecoder {
public:
    virtual ~QTextDecoder();
    virtual QString toUnicode(const char* chars, int len) = 0;
};

class Q_EXPORT QTextCodec {
public:
    virtual ~QTextCodec();

#ifndef QT_NO_CODECS
    static QTextCodec* loadCharmap(QIODevice*);
    static QTextCodec* loadCharmapFile(QString filename);
#endif //QT_NO_CODECS
    static QTextCodec* codecForMib(int mib);
    static QTextCodec* codecForName(const char* hint, int accuracy=0);
    static QTextCodec* codecForContent(const char* chars, int len);
    static QTextCodec* codecForIndex(int i);
    static QTextCodec* codecForLocale();
    static void setCodecForLocale(QTextCodec *c);

    static QTextCodec* codecForTr();
    static void setCodecForTr(QTextCodec *c);
    static QTextCodec* codecForCStrings();
    static void setCodecForCStrings(QTextCodec *c);

    static void deleteAllCodecs();

    static const char* locale();

    virtual const char* name() const = 0;
    virtual const char* mimeName() const;
    virtual int mibEnum() const = 0;

    virtual QTextDecoder* makeDecoder() const;
    virtual QTextEncoder* makeEncoder() const;

    virtual QString toUnicode(const char* chars, int len) const;
    virtual QCString fromUnicode(const QString& uc, int& lenInOut) const;

    QCString fromUnicode(const QString& uc) const;
    QString toUnicode(const QByteArray&, int len) const;
    QString toUnicode(const QByteArray&) const;
    QString toUnicode(const QCString&, int len) const;
    QString toUnicode(const QCString&) const;
    QString toUnicode(const char* chars) const;
    virtual bool canEncode( QChar ) const;
    virtual bool canEncode( const QString& ) const;

    virtual int heuristicContentMatch(const char* chars, int len) const = 0;
    virtual int heuristicNameMatch(const char* hint) const;

    virtual QByteArray fromUnicode(const QString& uc, int from, int len) const;
    virtual unsigned short characterFromUnicode(const QString &str, int pos) const;

protected:
    QTextCodec();
    static int simpleHeuristicNameMatch(const char* name, const char* hint);

private:
    friend class QFont;
    friend class QFontEngineXLFD;
    void fromUnicodeInternal( const QChar *in, unsigned short *out,  int length );

    static QTextCodec *cftr;
    static QTextCodec *cfcs;
};

inline QTextCodec* QTextCodec::codecForTr() { return cftr; }
inline void QTextCodec::setCodecForTr(QTextCodec *c) { cftr = c; }
inline QTextCodec* QTextCodec::codecForCStrings() { return cfcs; }
inline void QTextCodec::setCodecForCStrings(QTextCodec *c) { cfcs = c; }

#endif // QT_NO_TEXTCODEC
#endif // QTEXTCODEC_H
