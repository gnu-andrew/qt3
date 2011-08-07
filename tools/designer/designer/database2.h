/**********************************************************************
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Designer.
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
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with
** the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#ifndef DATABASE2_H
#define DATABASE2_H

#include "qfeatures.h"

#ifndef QT_NO_SQL
#include <qdataview.h>
#include <qdatabrowser.h>
#include <qsqlcursor.h>
#include <qstring.h>

class QSqlDatabase;
class QSqlForm;

class DatabaseSupport2
{
public:
    DatabaseSupport2();
    virtual ~DatabaseSupport2() {}

    void initPreview( const QString &connection, const QString &table, QObject *o,
		      const QMap<QString, QString> &databaseControls );

protected:
    QSqlDatabase* con;
    QSqlForm* frm;
    QString tbl;
    QMap<QString, QString> dbControls;
    QObject *parent;

};

class QDesignerDataBrowser2 : public QDataBrowser, public DatabaseSupport2
{
    Q_OBJECT

public:
    QDesignerDataBrowser2( QWidget *parent, const char *name );

protected:
    bool event( QEvent* e );
};

class QDesignerDataView2 : public QDataView, public DatabaseSupport2
{
    Q_OBJECT

public:
    QDesignerDataView2( QWidget *parent, const char *name );

protected:
    bool event( QEvent* e );

};

#define DatabaseSupport DatabaseSupport2
#define QDesignerDataBrowser QDesignerDataBrowser2
#define QDesignerDataView QDesignerDataView2

#endif

#endif
