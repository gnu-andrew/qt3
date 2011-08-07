/****************************************************************************
**
** Definition of QRegExp class
**
** Created : 950126
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

#ifndef QREGEXP_H
#define QREGEXP_H

#ifndef QT_H
#include "qstringlist.h"
#endif // QT_H

#ifndef QT_NO_REGEXP
class QRegExpEngine;
struct QRegExpPrivate;

class Q_EXPORT QRegExp
{
public:
    enum CaretMode { CaretAtZero, CaretAtOffset, CaretWontMatch };

    QRegExp();
    QRegExp( const QString& pattern, bool caseSensitive = TRUE,
	     bool wildcard = FALSE );
    QRegExp( const QRegExp& rx );
    ~QRegExp();
    QRegExp& operator=( const QRegExp& rx );

    bool operator==( const QRegExp& rx ) const;
    bool operator!=( const QRegExp& rx ) const { return !operator==( rx ); }

    bool isEmpty() const;
    bool isValid() const;
    QString pattern() const;
    void setPattern( const QString& pattern );
    bool caseSensitive() const;
    void setCaseSensitive( bool sensitive );
#ifndef QT_NO_REGEXP_WILDCARD
    bool wildcard() const;
    void setWildcard( bool wildcard );
#endif
    bool minimal() const;
    void setMinimal( bool minimal );

    bool exactMatch( const QString& str ) const;
#ifndef QT_NO_COMPAT
    int match( const QString& str, int index = 0, int *len = 0,
	       bool indexIsStart = TRUE ) const;
#endif

#if defined(Q_QDOC)
    int search( const QString& str, int offset = 0,
		CaretMode caretMode = CaretAtZero ) const;
    int searchRev( const QString& str, int offset = -1,
		   CaretMode caretMode = CaretAtZero ) const;
#else
    // ### Qt 4.0: reduce these four to two functions
    int search( const QString& str, int offset = 0 ) const;
    int search( const QString& str, int offset, CaretMode caretMode ) const;
    int searchRev( const QString& str, int offset = -1 ) const;
    int searchRev( const QString& str, int offset, CaretMode caretMode ) const;
#endif
    int matchedLength() const;
#ifndef QT_NO_REGEXP_CAPTURE
    int numCaptures() const;
    QStringList capturedTexts();
    QString cap( int nth = 0 );
    int pos( int nth = 0 );
    QString errorString();
#endif

    static QString escape( const QString& str );

private:
    void prepareEngine() const;
    void prepareEngineForMatch( const QString& str ) const;
    void invalidateEngine();

    static int caretIndex( int offset, CaretMode caretMode );

    QRegExpEngine *eng;
    QRegExpPrivate *priv;
};
#endif // QT_NO_REGEXP
#endif // QREGEXP_H
