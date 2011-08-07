/****************************************************************************
**
** Definition of QSqlDriverInterface class
**
** Created : 2000-11-03
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QSQLDRIVERINTERFACE_H
#define QSQLDRIVERINTERFACE_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//
//

#ifndef QT_H
#include <private/qcom_p.h>
#endif // QT_H

#if !defined( QT_MODULE_SQL ) || defined( QT_LICENSE_PROFESSIONAL )
#define QM_EXPORT_SQL
#else
#define QM_EXPORT_SQL Q_EXPORT
#endif

#ifndef QT_NO_SQL

#ifndef QT_NO_COMPONENT

// {EDDD5AD5-DF3C-400c-A711-163B72FE5F61}
#ifndef IID_QSqlDriverFactory
#define IID_QSqlDriverFactory QUuid(0xeddd5ad5, 0xdf3c, 0x400c, 0xa7, 0x11, 0x16, 0x3b, 0x72, 0xfe, 0x5f, 0x61)
#endif

class QSqlDriver;

struct QM_EXPORT_SQL QSqlDriverFactoryInterface : public QFeatureListInterface
{
    virtual QSqlDriver* create( const QString& name ) = 0;
};

#endif //QT_NO_COMPONENT
#endif // QT_NO_SQL

#endif // QSQLDRIVERINTERFACE_P_H
