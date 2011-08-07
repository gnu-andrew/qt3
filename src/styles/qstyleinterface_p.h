/****************************************************************************
**
** ...
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
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

#ifndef QSTYLEINTERFACE_P_H
#define QSTYLEINTERFACE_P_H

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

#ifndef QT_NO_STYLE
#ifndef QT_NO_COMPONENT

class QStyle;

// {FC1B6EBE-053C-49c1-A483-C377739AB9A5}
#ifndef IID_QStyleFactory
#define IID_QStyleFactory QUuid(0xfc1b6ebe, 0x53c, 0x49c1, 0xa4, 0x83, 0xc3, 0x77, 0x73, 0x9a, 0xb9, 0xa5)
#endif

struct Q_EXPORT QStyleFactoryInterface : public QFeatureListInterface
{
    virtual QStyle* create( const QString& style ) = 0;
};

#endif //QT_NO_COMPONENT
#endif //QT_NO_STYLE

#endif //QSTYLEINTERFACE_P_H
