/****************************************************************************
**
** Implementation of Drag and Drop support
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

#include "qdropsite.h"

#ifndef QT_NO_DRAGANDDROP

#include "qwidget.h"


// NOT REVISED
/*!
  \class QDropSite qdropsite.h
  \brief The QDropSite class provides nothing and does nothing.

  \obsolete

  If your code uses it, you can safely delete it.

  It was used in Qt 1.x to do some drag and drop; that has since been
  folded into QWidget.

  For detailed information about drag-and-drop, see the QDragObject class.

  \sa QDragObject, QTextDrag, QImageDrag
*/

/*!
  Constructs a QDropSite to handle events for the widget \a self.

  Pass \c this as the \a self parameter.
  This enables dropping by calling QWidget::setAcceptDrops(TRUE).
*/
QDropSite::QDropSite( QWidget* self )
{
    self->setAcceptDrops( TRUE );
}

/*!
  Destroys the drop site.
*/
QDropSite::~QDropSite()
{
}

#endif // QT_NO_DRAGANDDROP
