/****************************************************************************
**
** Implementation of vertical box layout widget class
**
** Created : 990124
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
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


#include "qvbox.h"
#ifndef QT_NO_VBOX

/*!
    \class QVBox qvbox.h
    \brief The QVBox widget provides vertical geometry management of
    its child widgets.

    \ingroup geomanagement
    \ingroup appearance
    \ingroup organizers

    All its child widgets will be placed vertically and sized
    according to their sizeHint()s.

    \img qvbox-m.png QVBox

    \sa QHBox
*/


/*!
    Constructs a vbox widget called \a name with parent \a parent and
    widget flags \a f.
 */
QVBox::QVBox( QWidget *parent, const char *name, WFlags f )
    :QHBox( FALSE, parent, name, f )
{
}
#endif
