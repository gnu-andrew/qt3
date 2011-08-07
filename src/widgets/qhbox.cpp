/****************************************************************************
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

#include "qhbox.h"
#ifndef QT_NO_HBOX
#include "qlayout.h"
#include "qapplication.h"
#include "qobjectlist.h"


/*!
    \class QHBox qhbox.h
    \brief The QHBox widget provides horizontal geometry management
    for its child widgets.

    \ingroup organizers
    \ingroup geomanagement
    \ingroup appearance

    All the horizontal box's child widgets will be placed alongside
    each other and sized according to their sizeHint()s.

    Use setMargin() to add space around the edges, and use
    setSpacing() to add space between the widgets. Use
    setStretchFactor() if you want the widgets to be different sizes
    in proportion to one another. (See \link layout.html
    Layouts\endlink for more information on stretch factors.)

    \img qhbox-m.png QHBox

    \sa QHBoxLayout QVBox QGrid
*/


/*!
    Constructs an hbox widget with parent \a parent, called \a name.
    The parent, name and widget flags, \a f, are passed to the QFrame
    constructor.
*/
QHBox::QHBox( QWidget *parent, const char *name, WFlags f )
    :QFrame( parent, name, f )
{
    lay = new QHBoxLayout( this, frameWidth(), frameWidth(), name );
    lay->setAutoAdd( TRUE );
}


/*!
    Constructs a horizontal hbox if \a horizontal is TRUE, otherwise
    constructs a vertical hbox (also known as a vbox).

    This constructor is provided for the QVBox class. You should never
    need to use it directly.

    The \a parent, \a name and widget flags, \a f, are passed to the
    QFrame constructor.
*/

QHBox::QHBox( bool horizontal, QWidget *parent , const char *name, WFlags f )
    :QFrame( parent, name, f )
{
    lay = new QBoxLayout( this,
		       horizontal ? QBoxLayout::LeftToRight : QBoxLayout::Down,
			  frameWidth(), frameWidth(), name );
    lay->setAutoAdd( TRUE );
}

/*!\reimp
 */
void QHBox::frameChanged()
{
    if ( !layout() )
	return;
    layout()->setMargin( frameWidth() );
}


/*!
    Sets the spacing between the child widgets to \a space.
*/

void QHBox::setSpacing( int space )
{
    if ( layout() ) // ### why not use this->lay?
	layout()->setSpacing( space );
}


/*!
  \reimp
*/

QSize QHBox::sizeHint() const
{
    QWidget *mThis = (QWidget*)this;
    QApplication::sendPostedEvents( mThis, QEvent::ChildInserted );
    return QFrame::sizeHint();
}

/*!
    Sets the stretch factor of widget \a w to \a stretch. Returns TRUE if
    \a w is found. Otherwise returns FALSE.

    \sa QBoxLayout::setStretchFactor() \link layout.html Layouts\endlink
*/
bool QHBox::setStretchFactor( QWidget* w, int stretch )
{
    QWidget *mThis = (QWidget*)this;
    QApplication::sendPostedEvents( mThis, QEvent::ChildInserted );
    return lay->setStretchFactor( w, stretch );
}
#endif
