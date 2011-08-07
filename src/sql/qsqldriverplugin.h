/****************************************************************************
**
** Definition of QSqlDriverPlugin class
**
** Created : 2001-09-20
**
** Copyright (C) 2001-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QSQLDRIVERPLUGIN_H
#define QSQLDRIVERPLUGIN_H

#ifndef QT_H
#include "qgplugin.h"
#include "qstringlist.h"
#endif // QT_H

#ifndef QT_NO_SQL
#ifndef QT_NO_COMPONENT

class QSqlDriver;
class QSqlDriverPluginPrivate;

class Q_EXPORT QSqlDriverPlugin : public QGPlugin
{
    Q_OBJECT
public:
    QSqlDriverPlugin();
    ~QSqlDriverPlugin();

    virtual QStringList keys() const = 0;
    virtual QSqlDriver *create( const QString &key ) = 0;

private:
    QSqlDriverPluginPrivate *d;
};

#endif // QT_NO_COMPONENT
#endif // QT_NO_SQL

#endif // QSQLDRIVERPLUGIN_H
