/****************************************************************************
**
** Definition of QSql class
**
** Created : 2000-11-03
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

#ifndef QSQL_H
#define QSQL_H

#ifndef QT_H
#include "qglobal.h"
#endif // QT_H

#if !defined( QT_MODULE_SQL ) || defined( QT_LICENSE_PROFESSIONAL )
#define QM_EXPORT_SQL
#else
#define QM_EXPORT_SQL Q_EXPORT
#endif

#ifndef QT_NO_SQL

class QM_EXPORT_SQL QSql
{
public:
    QSql() {}
    enum Op {
	None = -1,
	Insert = 0,
	Update = 1,
	Delete = 2
    };

    enum Location {
	BeforeFirst = -1,
	AfterLast = -2
    };

    enum Confirm {
	Cancel = -1,
	No = 0,
	Yes = 1
    };

    enum ParameterType {
	In = 1,
	Out = 2,
	InOut = 3  //InOut = In | Out
    };

    enum TableType { 
	Tables = 0x01,
	SystemTables = 0x02,
	Views = 0x04,
	AllTables = 0xff
    };

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QSql( const QSql & );
    QSql &operator=( const QSql & );
#endif

};

#endif
#endif
