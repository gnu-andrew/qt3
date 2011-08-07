/****************************************************************************
**
** Definition of QSqlDriver class
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

#ifndef QSQLDRIVER_H
#define QSQLDRIVER_H

#ifndef QT_H
#include "qobject.h"
#include "qptrdict.h"
#include "qstring.h"
#include "qsqlerror.h"
#include "qsqlquery.h"
#include "qsqlfield.h"
#include "qsqlindex.h"
#include "qstringlist.h"
#include "qmap.h"
#endif // QT_H

#if !defined( QT_MODULE_SQL ) || defined( QT_LICENSE_PROFESSIONAL )
#define QM_EXPORT_SQL
#else
#define QM_EXPORT_SQL Q_EXPORT
#endif

#ifndef QT_NO_SQL

class QSqlDriverExtension;

class QSqlDatabase;

class QM_EXPORT_SQL QSqlDriver : public QObject
{
    friend class QSqlDatabase;
    Q_OBJECT
public:
    enum DriverFeature { Transactions, QuerySize, BLOB, Unicode, PreparedQueries,
			 NamedPlaceholders, PositionalPlaceholders };

    QSqlDriver( QObject * parent=0, const char * name=0 );
    ~QSqlDriver();
    bool			isOpen() const;
    bool			isOpenError() const;

    virtual bool		beginTransaction();
    virtual bool		commitTransaction();
    virtual bool		rollbackTransaction();
    virtual QStringList		tables( const QString& tableType ) const;
    virtual QSqlIndex		primaryIndex( const QString& tableName ) const;
    virtual QSqlRecord		record( const QString& tableName ) const;
    virtual QSqlRecord		record( const QSqlQuery& query ) const;
    virtual QSqlRecordInfo	recordInfo( const QString& tablename ) const;
    virtual QSqlRecordInfo	recordInfo( const QSqlQuery& query ) const;
    virtual QString		nullText() const;
    virtual QString		formatValue( const QSqlField* field, bool trimStrings = FALSE ) const;
    QSqlError			lastError() const;

    virtual bool		hasFeature( DriverFeature f ) const = 0;
    virtual bool		open( const QString & db,
				      const QString & user = QString::null,
				      const QString & password = QString::null,
				      const QString & host = QString::null,
				      int port = -1 ) = 0;
    virtual void		close() = 0;
    virtual QSqlQuery		createQuery() const = 0;

    // ### remove for 4.0
    bool			open( const QString& db,
				      const QString& user,
				      const QString& password,
				      const QString& host,
				      int port,
				      const QString& connOpts );
protected:
    virtual void		setOpen( bool o );
    virtual void		setOpenError( bool e );
    virtual void		setLastError( const QSqlError& e );
private:
    // ### This class needs a d-pointer in 4.0.
    int		          dbState;
    QSqlError	          error;
#if defined(Q_DISABLE_COPY)
    QSqlDriver( const QSqlDriver & );
    QSqlDriver &operator=( const QSqlDriver & );
#endif
};

#endif	// QT_NO_SQL
#endif
