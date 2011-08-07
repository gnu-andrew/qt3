/****************************************************************************
**
** Definition of QConnection class
**
** Created : 930417
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

#ifndef QCONNECTION_H
#define QCONNECTION_H

#ifndef QT_H
#include "qobject.h"
#endif // QT_H

class Q_EXPORT QConnection
{
public:
    QConnection( const QObject *, int, const char *memberName, int memberType );
   ~QConnection() {}

    bool     isConnected() const { return obj != 0; }

    QObject *object() const  { return obj; }	// get object/member pointer
    int  member() const  { return mbr; }
    const char *memberName() const { return mbr_name; }
    int  memberType() const  { return mbr_type; }
    int	     numArgs() const { return nargs; }

private:
    QObject *obj;				// object connected to
    int  mbr;				// member connected to
    const char *mbr_name;
    int mbr_type;
    int	     nargs;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QConnection( const QConnection & );
    QConnection &operator=( const QConnection & );
#endif
};

#define Q_DEFINED_QCONNECTION
#include "qwinexport.h"
#endif // QCONNECTION_H
