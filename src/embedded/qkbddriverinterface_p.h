/****************************************************************************
**
** Definition of Qt/Embedded Keyboard Driver Interface
**
** Created : 20020218
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the kernel module of the Qt GUI Toolkit.
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

#ifndef QKBDDRIVERINTERFACE_P_H
#define QKBDDRIVERINTERFACE_P_H

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

#ifndef QT_NO_COMPONENT

// {C7C838EA-FC3E-4905-92AD-F479E81F1D02}
#ifndef IID_QKbdDriver
#define IID_QKbdDriver QUuid( 0xc7c838ea, 0xfc3e, 0x4905, 0x92, 0xad, 0xf4, 0x79, 0xe8, 0x1f, 0x1d, 0x02)
#endif

class QWSKeyboardHandler;

struct Q_EXPORT QKbdDriverInterface : public QFeatureListInterface
{
    virtual QWSKeyboardHandler* create( const QString& driver, const QString& device ) = 0;
};

#endif // QT_NO_COMPONENT

#endif // QKBDDRIVERINTERFACE_P_H
