/****************************************************************************
**
** Definition of QSqlForm class
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

#ifndef QSQLFORM_H
#define QSQLFORM_H

#ifndef QT_H
#include "qobject.h"
#include "qmap.h"
#endif // QT_H

#if !defined( QT_MODULE_SQL ) || defined( QT_LICENSE_PROFESSIONAL )
#define QM_EXPORT_SQL
#else
#define QM_EXPORT_SQL Q_EXPORT
#endif

#ifndef QT_NO_SQL_FORM

class QSqlField;
class QSqlRecord;
class QSqlEditorFactory;
class QSqlPropertyMap;
class QWidget;
class QSqlFormPrivate;

class QM_EXPORT_SQL QSqlForm : public QObject
{
    Q_OBJECT
public:
    QSqlForm( QObject * parent = 0, const char * name = 0 );
    ~QSqlForm();

    virtual void insert( QWidget * widget, const QString& field );
    virtual void remove( const QString& field );
    uint         count() const;

    QWidget *   widget( uint i ) const;
    QSqlField * widgetToField( QWidget * widget ) const;
    QWidget *   fieldToWidget( QSqlField * field ) const;

    void        installPropertyMap( QSqlPropertyMap * map );

    virtual void setRecord( QSqlRecord* buf );

public slots:
    virtual void readField( QWidget * widget );
    virtual void writeField( QWidget * widget );
    virtual void readFields();
    virtual void writeFields();

    virtual void clear();
    virtual void clearValues( bool nullify = FALSE );

protected:
    virtual void insert( QWidget * widget, QSqlField * field );
    virtual void remove( QWidget * widget );
    void clearMap();

private:
    virtual void sync();
    QSqlFormPrivate* d;

#if defined(Q_DISABLE_COPY) // Disabled copy constructor and operator=
    QSqlForm( const QSqlForm & );
    QSqlForm &operator=( const QSqlForm & );
#endif
};

#endif // QT_NO_SQL
#endif // QSQLFORM_H
