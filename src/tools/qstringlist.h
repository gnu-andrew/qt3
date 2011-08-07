/****************************************************************************
**
** Definition of QStringList class
**
** Created : 990406
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

#ifndef QSTRINGLIST_H
#define QSTRINGLIST_H

#ifndef QT_H
#include "qvaluelist.h"
#include "qstring.h"
#include "qstrlist.h"
#endif // QT_H

#ifndef QT_NO_STRINGLIST

class QRegExp;
template <class T> class QDeepCopy;

#if defined(Q_TEMPLATEDLL)
// MOC_SKIP_BEGIN
//Q_TEMPLATE_EXTERN template class Q_EXPORT QValueList<QString>; 
// MOC_SKIP_END
#endif

class Q_EXPORT QStringList : public QValueList<QString>
{
public:
    QStringList() { }
    QStringList( const QStringList& l ) : QValueList<QString>(l) { }
    QStringList( const QValueList<QString>& l ) : QValueList<QString>(l) { }
    QStringList( const QString& i ) { append(i); }
#ifndef QT_NO_CAST_ASCII
    QStringList( const char* i ) { append(i); }
#endif

    static QStringList fromStrList(const QStrList&);

    void sort();

    static QStringList split( const QString &sep, const QString &str, bool allowEmptyEntries = FALSE );
    static QStringList split( const QChar &sep, const QString &str, bool allowEmptyEntries = FALSE );
#ifndef QT_NO_REGEXP
    static QStringList split( const QRegExp &sep, const QString &str, bool allowEmptyEntries = FALSE );
#endif
    QString join( const QString &sep ) const;

    QStringList grep( const QString &str, bool cs = TRUE ) const;
#ifndef QT_NO_REGEXP
    QStringList grep( const QRegExp &expr ) const;
#endif

    QStringList& gres( const QString &before, const QString &after,
		       bool cs = TRUE );
#ifndef QT_NO_REGEXP_CAPTURE
    QStringList& gres( const QRegExp &expr, const QString &after );
#endif

protected:
    void detach() { QValueList<QString>::detach(); }
    friend class QDeepCopy< QStringList >;
};

#ifndef QT_NO_DATASTREAM
class QDataStream;
extern Q_EXPORT QDataStream &operator>>( QDataStream &, QStringList& );
extern Q_EXPORT QDataStream &operator<<( QDataStream &, const QStringList& );
#endif

#endif // QT_NO_STRINGLIST
#endif // QSTRINGLIST_H
