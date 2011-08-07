/****************************************************************************
**
** Definition of QGb18030Codec class
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

// Contributed by James Su <suzhe@gnuchina.org>

#ifndef QGB18030CODEC_H
#define QGB18030CODEC_H

#ifndef QT_H
#include "qtextcodec.h"
#endif // QT_H


#ifndef QT_NO_BIG_CODECS

#if defined(QT_PLUGIN)
#define Q_EXPORT_CODECS_CN
#else
#define Q_EXPORT_CODECS_CN Q_EXPORT
#endif

class Q_EXPORT_CODECS_CN QGb18030Codec : public QTextCodec {
public:
    QGb18030Codec();

    int mibEnum() const;
    const char* name() const;

    QTextDecoder* makeDecoder() const;

#if !defined(Q_NO_USING_KEYWORD)
    using QTextCodec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut) const;
    QString toUnicode(const char* chars, int len) const;

    int heuristicContentMatch(const char* chars, int len) const;
    int heuristicNameMatch(const char* hint) const;
};

class Q_EXPORT_CODECS_CN QGbkCodec : public QGb18030Codec {
public:
    QGbkCodec();

    int mibEnum() const;
    const char* name() const;

    QTextDecoder* makeDecoder() const;

#if !defined(Q_NO_USING_KEYWORD)
    using QGb18030Codec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut) const;
    QString toUnicode(const char* chars, int len) const;

    int heuristicContentMatch(const char* chars, int len) const;
    int heuristicNameMatch(const char* hint) const;
};

class Q_EXPORT_CODECS_CN QGb2312Codec : public QGb18030Codec {
public:
    QGb2312Codec();

    int mibEnum() const;
    const char* name() const;

    QTextDecoder* makeDecoder() const;

#if !defined(Q_NO_USING_KEYWORD)
    using QGb18030Codec::fromUnicode;
#endif
    QCString fromUnicode(const QString& uc, int& lenInOut) const;
    QString toUnicode(const char* chars, int len) const;

    int heuristicContentMatch(const char* chars, int len) const;
    int heuristicNameMatch(const char* hint) const;
};

#endif
#endif
