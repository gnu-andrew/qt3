/****************************************************************************
**
** Definition of shared Qt SQL module classes
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

#ifndef QSQLCACHEDRESULT_H
#define QSQLCACHEDRESULT_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
//

#include <qglobal.h>
#include <qvariant.h>
#include <qptrvector.h>
#include <qvaluevector.h>
#include <qsqlresult.h>

#if !defined( QT_MODULE_SQL ) || defined( QT_LICENSE_PROFESSIONAL )
#define QM_EXPORT_SQL
#else
#define QM_EXPORT_SQL Q_EXPORT
#endif

#ifndef QT_NO_SQL

class QtSqlCachedResultPrivate;

class QM_EXPORT_SQL QtSqlCachedResult: public QSqlResult
{
public:
    virtual ~QtSqlCachedResult();

    typedef QValueVector<QVariant> RowCache;
    typedef QPtrVector<RowCache> RowsetCache;

protected:    
    QtSqlCachedResult(const QSqlDriver * db);

    void init(int colCount);
    void cleanup();
    bool cacheNext();

    virtual bool gotoNext(RowCache* row) = 0;

    QVariant data(int i);
    bool isNull(int i);
    bool fetch(int i);
    bool fetchNext();
    bool fetchPrev();
    bool fetchFirst();
    bool fetchLast();

    int colCount() const;

private:
    QtSqlCachedResultPrivate *d;
};


#endif

#endif
