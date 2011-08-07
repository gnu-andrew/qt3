/****************************************************************************
**
** Definition of QEffects functions
**
** Created : 000621
**
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QEFFECTS_P_H
#define QEFFECTS_P_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of qeffects.cpp, qcombobox.cpp, qpopupmenu.cpp and qtooltip.cpp.
// This header file may change from version to version without notice,
// or even be removed.
//
// We mean it.
//
//

#ifndef QT_H
#include "qnamespace.h"
#endif // QT_H

#ifndef QT_NO_EFFECTS
class QWidget;

struct QEffects
{
    enum Direction {
	LeftScroll	= 0x0001,
	RightScroll	= 0x0002,
	UpScroll	= 0x0004,
	DownScroll	= 0x0008
    };

    typedef uint DirFlags;
};

extern void Q_EXPORT qScrollEffect( QWidget*, QEffects::DirFlags dir = QEffects::DownScroll, int time = -1 );
extern void Q_EXPORT qFadeEffect( QWidget*, int time = -1 );
#endif // QT_NO_EFFECTS

#endif // QEFFECTS_P_H
