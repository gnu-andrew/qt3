/****************************************************************************
**
** Definition of QSqlQuery class
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

#ifndef QSQLQUERY_H
#define QSQLQUERY_H

#ifndef QT_H
#include "qobject.h"
#include "qstring.h"
#include "qvariant.h"
#include "qvaluelist.h"
#include "qsqlerror.h"
#include "qsqlfield.h"
#include "qsql.h"
#endif // QT_H

#ifndef QT_NO_SQL

class QSqlDriver;
class QSqlResult;
class QSqlDatabase;

class Q_EXPORT QSqlResultShared : public QObject, public QShared
{
    Q_OBJECT
public:
    QSqlResultShared( QSqlResult* result );
    virtual ~QSqlResultShared();
    QSqlResult* sqlResult;
    QString executedQuery;
private slots:
    void slotResultDestroyed();
};

class Q_EXPORT QSqlQuery
{
public:
    QSqlQuery( QSqlResult * r );
    QSqlQuery( const QString& query = QString::null, QSqlDatabase* db = 0 );
    Q_EXPLICIT QSqlQuery( QSqlDatabase* db );
    QSqlQuery( const QSqlQuery& other );
    QSqlQuery& operator=( const QSqlQuery& other );
    virtual ~QSqlQuery();

    bool                isValid() const;
    bool                isActive() const;
    bool	        isNull( int field ) const;
    int                 at() const;
    QString             lastQuery() const;
    int                 numRowsAffected() const;
    QSqlError	        lastError() const;
    bool                isSelect() const;
    int                 size() const;
    const QSqlDriver*   driver() const;
    const QSqlResult*   result() const;
    bool		isForwardOnly() const;
    void		setForwardOnly( bool forward );

    virtual bool	exec ( const QString& query );
    virtual QVariant    value( int i ) const;

    virtual bool	seek( int i, bool relative = FALSE );
    virtual bool        next();
    virtual bool        prev();
    virtual bool        first();
    virtual bool        last();

    // prepared query support
    bool		exec();
    bool		prepare( const QString& query );
    void		bindValue( const QString& placeholder, const QVariant& val );
    void		bindValue( int pos, const QVariant& val );
    void		addBindValue( const QVariant& val );
    // remove these overloads in 4.0
    void		bindValue( const QString& placeholder, const QVariant& val, QSql::ParameterType type );
    void		bindValue( int pos, const QVariant& val, QSql::ParameterType type );
    void		addBindValue( const QVariant& val, QSql::ParameterType type );
    QVariant		boundValue( const QString& placeholder ) const;
    QVariant		boundValue( int pos ) const;
    QMap<QString, QVariant> boundValues() const;
    QString             executedQuery() const;
    
protected:
    virtual void        beforeSeek();
    virtual void        afterSeek();

private:
    void 		init( const QString& query, QSqlDatabase* db );
    void                deref();
    bool                checkDetach();
    QSqlResultShared*   d;
};


#endif // QT_NO_SQL
#endif
