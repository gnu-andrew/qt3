/****************************************************************************
**
** Definition of QSqlPropertyMap class
**
** Created : 2000-11-20
**
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the sql module of the Qt GUI Toolkit.
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

#ifndef QSQLPROPERTYMAP_H
#define QSQLPROPERTYMAP_H

#ifndef QT_H
#include "qvariant.h"
#include "qstring.h"
#endif // QT_H

#ifndef QT_NO_SQL_FORM

class QWidget;
class QSqlPropertyMapPrivate;

class Q_EXPORT QSqlPropertyMap {
public:
    QSqlPropertyMap();
    virtual ~QSqlPropertyMap();

    QVariant      property( QWidget * widget );
    virtual void  setProperty( QWidget * widget, const QVariant & value );

    void insert( const QString & classname, const QString & property );
    void remove( const QString & classname );

    static QSqlPropertyMap * defaultMap();
    static void installDefaultMap( QSqlPropertyMap * map );

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QSqlPropertyMap( const QSqlPropertyMap & );
    QSqlPropertyMap &operator=( const QSqlPropertyMap & );
#endif
    QSqlPropertyMapPrivate* d;

};

#endif // QT_NO_SQL_FORM
#endif // QSQLPROPERTYMAP_H
