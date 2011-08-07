/****************************************************************************
**
** Definition of QWidgetIntDict
**
** Created : 950116
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

#ifndef QWIDGETINTDICT_H
#define QWIDGETINTDICT_H

#ifndef QT_H
#include "qwidget.h"
#include "qintdict.h"
#endif // QT_H


#if defined(Q_TEMPLATEDLL)
//Q_TEMPLATE_EXTERN template class Q_EXPORT QIntDict<QWidget>;
//Q_TEMPLATE_EXTERN template class Q_EXPORT QIntDictIterator<QWidget>;
#endif


class Q_EXPORT QWidgetIntDict : public QIntDict<QWidget>
{
public:
    QWidgetIntDict(int size=17) : QIntDict<QWidget>(size) {}
    QWidgetIntDict( const QWidgetIntDict &dict ) : QIntDict<QWidget>(dict) {}
   ~QWidgetIntDict() { clear(); }
    QWidgetIntDict &operator=(const QWidgetIntDict &dict)
	{ return (QWidgetIntDict&)QIntDict<QWidget>::operator=(dict); }
};

class Q_EXPORT QWidgetIntDictIt : public QIntDictIterator<QWidget>
{
public:
    QWidgetIntDictIt( const QWidgetIntDict &d ) : QIntDictIterator<QWidget>(d) {}
    QWidgetIntDictIt &operator=(const QWidgetIntDictIt &i)
	{ return (QWidgetIntDictIt&)QIntDictIterator<QWidget>::operator=(i); }
};


#endif
