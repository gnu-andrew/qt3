/****************************************************************************
**
** Implementation of QDataView class
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

#include "qdataview.h"

#ifndef QT_NO_SQL_VIEW_WIDGETS

#include "qsqlmanager_p.h"

class QDataViewPrivate
{
public:
    QDataViewPrivate() {}
    QSqlFormManager frm;
};


/*!
    \class QDataView qdataview.h
    \brief The QDataView class provides read-only SQL forms.

    \ingroup database
    \mainclass
    \module sql

    This class provides a form which displays SQL field data from a
    record buffer. Because QDataView does not support editing it uses
    less resources than a QDataBrowser. This class is well suited for
    displaying read-only data from a SQL database.

    If you want a to present your data in an editable form use
    QDataBrowser; if you want a table-based presentation of your data
    use QDataTable.

    The form is associated with the data view with setForm() and the
    record is associated with setRecord(). You can also pass a
    QSqlRecord to the refresh() function which will set the record to
    the given record and read the record's fields into the form.
*/

/*!
    Constructs a data view which is a child of \a parent, called \a
    name, and with widget flags \a fl.
*/

QDataView::QDataView( QWidget *parent, const char *name, WFlags fl )
    : QWidget( parent, name, fl )
{
    d = new QDataViewPrivate();
}

/*!
    Destroys the object and frees any allocated resources.
*/

QDataView::~QDataView()
{
    delete d;
}

/*!
    Clears the default form's values. If there is no default form,
    nothing happens. All the values are set to their 'zero state',
    e.g. 0 for numeric fields, "" for string fields.
*/

void QDataView::clearValues()
{
    d->frm.clearValues();
}

/*!
    Sets the form used by the data view to \a form. If a record has
    already been assigned to the data view, the form will display that
    record's data.

    \sa form()
*/

void QDataView::setForm( QSqlForm* form )
{
    d->frm.setForm( form );
}


/*!
    Returns the default form used by the data view, or 0 if there is
    none.

    \sa setForm()
*/

QSqlForm* QDataView::form()
{
    return d->frm.form();
}


/*!
    Sets the record used by the data view to \a record. If a form has
    already been assigned to the data view, the form will display the
    data from \a record in that form.

    \sa record()
*/

void QDataView::setRecord( QSqlRecord* record )
{
    d->frm.setRecord( record );
}


/*!
    Returns the default record used by the data view, or 0 if there is
    none.

    \sa setRecord()
*/

QSqlRecord* QDataView::record()
{
    return d->frm.record();
}


/*!
    Causes the default form to read its fields from the record buffer.
    If there is no default form, or no record, nothing happens.

    \sa setForm()
*/

void QDataView::readFields()
{
    d->frm.readFields();
}

/*!
    Causes the default form to write its fields to the record buffer.
    If there is no default form, or no record, nothing happens.

    \sa setForm()
*/

void QDataView::writeFields()
{
    d->frm.writeFields();
}

/*!
    Causes the default form to display the contents of \a buf. If
    there is no default form, nothing happens.The \a buf also becomes
    the default record for all subsequent calls to readFields() and
    writefields(). This slot is equivalant to calling:

    \code
    myView.setRecord( record );
    myView.readFields();
    \endcode

    \sa setRecord() readFields()
*/

void QDataView::refresh( QSqlRecord* buf )
{
    if ( buf && buf != record() )
	setRecord( buf );
    readFields();
}

#endif
