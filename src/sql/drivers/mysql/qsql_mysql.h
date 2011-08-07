/****************************************************************************
**
** Definition of MySQL driver classes
**
** Created : 001103
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QSQL_MYSQL_H
#define QSQL_MYSQL_H

#include <qsqldriver.h>
#include <qsqlresult.h>
#include <qsqlfield.h>
#include <qsqlindex.h>

#if defined (Q_OS_WIN32)
#include <qt_windows.h>
#endif

#include <mysql.h>

#ifdef QT_PLUGIN
#define Q_EXPORT_SQLDRIVER_MYSQL
#else
#define Q_EXPORT_SQLDRIVER_MYSQL Q_EXPORT
#endif

class QMYSQLDriverPrivate;
class QMYSQLResultPrivate;
class QMYSQLDriver;
class QSqlRecordInfo;

class QMYSQLResult : public QSqlResult
{
    friend class QMYSQLDriver;
public:
    QMYSQLResult( const QMYSQLDriver* db );
    ~QMYSQLResult();

    MYSQL_RES* result();
protected:
    void		cleanup();
    bool		fetch( int i );
    bool		fetchNext();
    bool		fetchLast();
    bool		fetchFirst();
    QVariant		data( int field );
    bool		isNull( int field );
    bool		reset ( const QString& query );
    int			size();
    int			numRowsAffected();
private:
    QMYSQLResultPrivate* d;
};

class Q_EXPORT_SQLDRIVER_MYSQL QMYSQLDriver : public QSqlDriver
{
    friend class QMYSQLResult;
public:
    QMYSQLDriver( QObject * parent=0, const char * name=0 );
    QMYSQLDriver( MYSQL * con, QObject * parent=0, const char * name=0 );
    ~QMYSQLDriver();
    bool		hasFeature( DriverFeature f ) const;
    bool		open( const QString & db,
			      const QString & user = QString::null,
			      const QString & password = QString::null,
			      const QString & host = QString::null,
			      int port = -1 );
    void		close();
    QSqlQuery		createQuery() const;
    QStringList		tables( const QString& user ) const;
    QSqlIndex		primaryIndex( const QString& tablename ) const;
    QSqlRecord		record( const QString& tablename ) const;
    QSqlRecord		record( const QSqlQuery& query ) const;
    QSqlRecordInfo	recordInfo( const QString& tablename ) const;
    QSqlRecordInfo	recordInfo( const QSqlQuery& query ) const;
    QString		formatValue( const QSqlField* field,
				     bool trimStrings ) const;
    MYSQL*		mysql();
    // ### remove me for 4.0
    bool open( const QString& db,
	       const QString& user,
	       const QString& password,
	       const QString& host,
	       int port,
	       const QString& connOpts );
    
protected:
    bool		beginTransaction();
    bool		commitTransaction();
    bool		rollbackTransaction();
private:
    void		init();
    QMYSQLDriverPrivate* d;
};


#endif
