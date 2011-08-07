/****************************************************************************
**
** Definition of QWidgetList
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

#ifndef QWIDGETLIST_H
#define QWIDGETLIST_H

#ifndef QT_H
#include "qwidget.h"
#include "qptrlist.h"
#endif // QT_H

class Q_EXPORT QWidgetList : public QPtrList<QWidget>
{
public:
    QWidgetList() : QPtrList<QWidget>() {}
    QWidgetList( const QWidgetList &list ) : QPtrList<QWidget>(list) {}
   ~QWidgetList() { clear(); }
    QWidgetList &operator=(const QWidgetList &list)
	{ return (QWidgetList&)QPtrList<QWidget>::operator=(list); }
};

class Q_EXPORT QWidgetListIt : public QPtrListIterator<QWidget>
{
public:
    QWidgetListIt( const QWidgetList &l ) : QPtrListIterator<QWidget>(l) {}
    QWidgetListIt &operator=(const QWidgetListIt &i)
	{ return (QWidgetListIt&)QPtrListIterator<QWidget>::operator=(i); }
};

#endif // QWIDGETLIST_H
