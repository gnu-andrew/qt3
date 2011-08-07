/****************************************************************************
**
** Definition of QSqlSelectCursor class
**
** Created : 2002-11-13
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

#ifndef QSQLSELECTCURSOR_H
#define QSQLSELECTCURSOR_H

#ifndef QT_H
#include "qsqlcursor.h"
#endif // QT_H

#if !defined( QT_MODULE_SQL ) || defined( QT_LICENSE_PROFESSIONAL )
#define QM_EXPORT_SQL
#else
#define QM_EXPORT_SQL Q_EXPORT
#endif

#ifndef QT_NO_SQL

class QSqlSelectCursorPrivate;

class QM_EXPORT_SQL QSqlSelectCursor : public QSqlCursor
{
public:
    QSqlSelectCursor( const QString& query = QString::null, QSqlDatabase* db = 0 );
    QSqlSelectCursor( const QSqlSelectCursor& other );
    ~QSqlSelectCursor();
    bool exec( const QString& query );
    bool select() { return QSqlCursor::select(); }
    
protected:
    QSqlIndex primaryIndex( bool = TRUE ) const { return QSqlIndex(); }
    QSqlIndex index( const QStringList& ) const { return QSqlIndex(); }
    QSqlIndex index( const QString& ) const { return QSqlIndex(); }
    QSqlIndex index( const char* ) const { return QSqlIndex(); }
    void setPrimaryIndex( const QSqlIndex& ) {}
    void append( const QSqlFieldInfo& ) {}
    void insert( int, const QSqlFieldInfo& ) {}
    void remove( int ) {}
    void clear() {}
    void setGenerated( const QString&, bool ) {}
    void setGenerated( int, bool ) {}
    QSqlRecord*	editBuffer( bool = FALSE ) { return 0; }
    QSqlRecord*	primeInsert() { return 0; }
    QSqlRecord*	primeUpdate() { return 0; }
    QSqlRecord*	primeDelete() { return 0; }
    int	insert( bool = TRUE ) { return 0; }
    int	update( bool = TRUE ) { return 0; }
    int	del( bool = TRUE ) { return 0; }
    void setMode( int ) {}

    void setSort( const QSqlIndex& ) {}
    QSqlIndex sort() const { return QSqlIndex(); }
    void setFilter( const QString& ) {}
    QString filter() const { return QString::null; }
    void setName( const QString&, bool = TRUE ) {}
    QString name() const { return QString::null; }
    QString toString( const QString& = QString::null, const QString& = "," ) const { return QString::null; }
    bool select( const QString &, const QSqlIndex& = QSqlIndex() );

private:
    void populateCursor();
    
    QSqlSelectCursorPrivate * d;
};

#endif // QT_NO_SQL
#endif // QSQLSELECTCURSOR_H
