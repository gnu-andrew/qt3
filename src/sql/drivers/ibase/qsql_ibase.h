/****************************************************************************
**
** Definition of Interbase driver classes
**
** Created : 030911
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

#ifndef QSQL_IBASE_H
#define QSQL_IBASE_H

#include "qsqlresult.h"
#include "qsqldriver.h"
#include "../cache/qsqlcachedresult.h"


class QIBaseDriverPrivate;
class QIBaseResultPrivate;
class QIBaseDriver;

class QIBaseResult : public QtSqlCachedResult
{
    friend class QIBaseDriver;
    friend class QIBaseResultPrivate;

public:
    QIBaseResult(const QIBaseDriver* db);
    virtual ~QIBaseResult();

    bool prepare(const QString& query);
    bool exec();    

protected:
    bool gotoNext(QtSqlCachedResult::RowCache* row);
    bool reset (const QString& query);
    int size();
    int numRowsAffected();

private:
    QIBaseResultPrivate* d;
};

class QIBaseDriver : public QSqlDriver
{
    friend class QIBaseDriverPrivate;
    friend class QIBaseResultPrivate;
    friend class QIBaseResult;
public:
    QIBaseDriver(QObject *parent = 0, const char *name = 0);
    QIBaseDriver(void *connection, QObject *parent = 0, const char *name = 0);
    virtual ~QIBaseDriver();
    bool hasFeature(DriverFeature f) const;
    bool open(const QString & db,
                   const QString & user,
                   const QString & password,
                   const QString & host,
                   int port,
                   const QString & connOpts);
    bool open( const QString & db,
	    const QString & user,
	    const QString & password,
	    const QString & host,
	    int port ) { return open (db, user, password, host, port, QString()); }
    void close();
    QSqlQuery createQuery() const;
    bool beginTransaction();
    bool commitTransaction();
    bool rollbackTransaction();
    QStringList tables(const QString& typeName) const;

    QSqlRecord record(const QString& tablename) const;
    QSqlRecordInfo recordInfo(const QString& tablename) const;
    QSqlIndex primaryIndex(const QString &table) const;
    QSqlRecord record(const QSqlQuery& query) const;
    QSqlRecordInfo recordInfo(const QSqlQuery& query) const;

    QString formatValue(const QSqlField* field, bool trimStrings) const;
    
private:
    QIBaseDriverPrivate* d;
};


#endif

