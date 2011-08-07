/****************************************************************************
**
** Definition of QHButtonGroup class
**
** Created : 990602
**
** Copyright (C) 1999-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QHBUTTONGROUP_H
#define QHBUTTONGROUP_H

#ifndef QT_H
#include "qbuttongroup.h"
#endif // QT_H

#ifndef QT_NO_HBUTTONGROUP

class Q_EXPORT QHButtonGroup : public QButtonGroup
{
    Q_OBJECT
public:
    QHButtonGroup( QWidget* parent=0, const char* name=0 );
    QHButtonGroup( const QString &title, QWidget* parent=0, const char* name=0 );
    ~QHButtonGroup();

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QHButtonGroup( const QHButtonGroup & );
    QHButtonGroup &operator=( const QHButtonGroup & );
#endif
};


#endif // QT_NO_HBUTTONGROUP

#endif // QHBUTTONGROUP_H
