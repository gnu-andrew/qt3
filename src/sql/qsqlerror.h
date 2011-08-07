/****************************************************************************
**
** Definition of QSqlError class
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

#ifndef QSQLERROR_H
#define QSQLERROR_H

#ifndef QT_H
#include "qstring.h"
#endif // QT_H

#if !defined( QT_MODULE_SQL ) || defined( QT_LICENSE_PROFESSIONAL )
#define QM_EXPORT_SQL
#else
#define QM_EXPORT_SQL Q_EXPORT
#endif

#ifndef QT_NO_SQL

class QM_EXPORT_SQL QSqlError
{
public:
    enum Type {
	None,
	Connection,
	Statement,
	Transaction,
	Unknown
    };
    QSqlError(  const QString& driverText = QString::null,
		const QString& databaseText = QString::null,
		int type = QSqlError::None,
		int number = -1 );
    QSqlError( const QSqlError& other );
    QSqlError& operator=( const QSqlError& other );
    virtual ~QSqlError();

    QString	driverText() const;
    virtual void setDriverText( const QString& driverText );
    QString	databaseText() const;
    virtual void setDatabaseText( const QString& databaseText );
    int		type() const;
    virtual void setType( int type );
    int		number() const;
    virtual void setNumber( int number );
    QString 	text() const;
    void 	showMessage( const QString& msg = QString::null ) const;
    
private:
    QString	driverError;
    QString	databaseError;
    int		errorType;
    int	errorNumber;
};

#endif // QT_NO_SQL
#endif
