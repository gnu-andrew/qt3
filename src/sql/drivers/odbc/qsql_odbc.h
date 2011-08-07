/****************************************************************************
**
** Definition of ODBC driver classes
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

#ifndef QSQL_ODBC_H
#define QSQL_ODBC_H

#include <qmap.h>
#include <qstring.h>
#include <qsqldriver.h>
#include <qsqlfield.h>
#include <qsqlresult.h>
#include <qsqlindex.h>

#if defined (Q_OS_WIN32)
#include <qt_windows.h>
#endif

#if defined (Q_OS_MAC)
// assume we use iodbc on MAC
// comment next line out if you use a 
// unicode compatible manager
# define Q_ODBC_VERSION_2
#endif

#ifdef QT_PLUGIN
#define Q_EXPORT_SQLDRIVER_ODBC
#else
#define Q_EXPORT_SQLDRIVER_ODBC Q_EXPORT
#endif

#ifdef Q_OS_UNIX
#define HAVE_LONG_LONG 1 // force UnixODBC NOT to fall back to a struct for BIGINTs
#endif

#if defined(Q_CC_BOR)
// workaround for Borland to make sure that SQLBIGINT is defined
#  define _MSC_VER 900
#endif
#include <sql.h>
#if defined(Q_CC_BOR)
#  undef _MSC_VER
#endif

#ifndef Q_ODBC_VERSION_2
#include <sqlucode.h>
#endif

#include <sqlext.h>

class QODBCPrivate;
class QODBCDriver;
class QSqlRecordInfo;

class QODBCResult : public QSqlResult
{
    friend class QODBCDriver;
public:
    QODBCResult( const QODBCDriver * db, QODBCPrivate* p );
    ~QODBCResult();

    SQLHANDLE   statement();
    bool	prepare( const QString& query );
    bool	exec();

protected:
    bool	fetchNext();
    bool	fetchFirst();
    bool	fetchLast();
    bool	fetchPrior();
    bool	fetch(int i);
    bool	reset ( const QString& query );
    QVariant	data( int field );
    bool	isNull( int field );
    int         size();
    int         numRowsAffected();
private:
    QODBCPrivate*	d;
    typedef QMap<int,QVariant> FieldCache;
    FieldCache fieldCache;
    typedef QMap<int,bool> NullCache;
    NullCache nullCache;
};

class Q_EXPORT_SQLDRIVER_ODBC QODBCDriver : public QSqlDriver
{
public:
    QODBCDriver( QObject * parent=0, const char * name=0 );
    QODBCDriver( SQLHANDLE env, SQLHANDLE con, QObject * parent=0, const char * name=0 );
    ~QODBCDriver();
    bool		hasFeature( DriverFeature f ) const;
    bool		open( const QString & db,
			      const QString & user = QString::null,
			      const QString & password = QString::null,
			      const QString & host = QString::null,
			      int port = -1 );
    void		close();
    QSqlQuery		createQuery() const;
    QStringList		tables( const QString& user ) const;
    QSqlRecord		record( const QString& tablename ) const;
    QSqlRecord		record( const QSqlQuery& query ) const;
    QSqlRecordInfo	recordInfo( const QString& tablename ) const;
    QSqlRecordInfo	recordInfo( const QSqlQuery& query ) const;
    QSqlIndex		primaryIndex( const QString& tablename ) const;
    SQLHANDLE		environment();
    SQLHANDLE		connection();

    QString		formatValue( const QSqlField* field,
				     bool trimStrings ) const;
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
    void init();
    bool endTrans();
    void cleanup();
    QODBCPrivate* d;
};

#endif
