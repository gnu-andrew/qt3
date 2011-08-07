/****************************************************************************
**
** Definition of QSqlField class
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

#ifndef QSQLFIELD_H
#define QSQLFIELD_H

#ifndef QT_H
#include "qstring.h"
#include "qvariant.h"
#endif // QT_H

#if !defined( QT_MODULE_SQL ) || defined( QT_LICENSE_PROFESSIONAL )
#define QM_EXPORT_SQL
#else
#define QM_EXPORT_SQL Q_EXPORT
#endif

#ifndef QT_NO_SQL

class QSqlFieldPrivate
{
public:
    QVariant::Type type;
};

class QM_EXPORT_SQL QSqlField
{
public:
    QSqlField( const QString& fieldName = QString::null, QVariant::Type type = QVariant::Invalid );
    QSqlField( const QSqlField& other );
    QSqlField& operator=( const QSqlField& other );
    bool operator==(const QSqlField& other) const;
    virtual ~QSqlField();

    virtual void	setValue( const QVariant& value );
    virtual QVariant	value() const;
    virtual void	setName( const QString& name );
    QString		name() const;
    virtual void	setNull();
    bool		isNull() const;
    virtual void	setReadOnly( bool readOnly );
    bool		isReadOnly() const;
    void		clear( bool nullify = TRUE );
    QVariant::Type	type() const;

private:
    QString       nm;
    QVariant      val;
    uint          ro: 1;
    uint          nul: 1;
    QSqlFieldPrivate* d;
};

inline QVariant QSqlField::value() const
{ return val; }

inline QString QSqlField::name() const
{ return nm; }

inline bool QSqlField::isNull() const
{ return nul; }

inline bool QSqlField::isReadOnly() const
{ return ro; }

inline QVariant::Type QSqlField::type() const
{ return d->type; }


/******************************************/
/*******     QSqlFieldInfo Class     ******/
/******************************************/

struct QSqlFieldInfoPrivate;

class QM_EXPORT_SQL QSqlFieldInfo
{
public:
    QSqlFieldInfo( const QString& name = QString::null,
		   QVariant::Type typ = QVariant::Invalid,
		   int required = -1,
		   int len = -1,
		   int prec = -1,
		   const QVariant& defValue = QVariant(),
		   int sqlType = 0,
		   bool generated = TRUE,
		   bool trim = FALSE,
		   bool calculated = FALSE );
    QSqlFieldInfo( const QSqlFieldInfo & other );
    QSqlFieldInfo( const QSqlField & other, bool generated = TRUE );
    virtual ~QSqlFieldInfo();
    QSqlFieldInfo& operator=( const QSqlFieldInfo& other );
    bool operator==( const QSqlFieldInfo& f ) const;

    QSqlField		toField() const;
    int			isRequired() const;
    QVariant::Type	type() const;
    int			length() const;
    int			precision() const;
    QVariant		defaultValue() const;
    QString		name() const;
    int			typeID() const;
    bool		isGenerated() const;
    bool		isTrim() const;
    bool		isCalculated() const;

    virtual void	setTrim( bool trim );
    virtual void	setGenerated( bool gen );
    virtual void	setCalculated( bool calc );

private:
    QSqlFieldInfoPrivate* d;
};


#endif	// QT_NO_SQL
#endif
