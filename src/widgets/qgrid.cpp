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


#include "qgrid.h"
#ifndef QT_NO_GRID
#include "qlayout.h"
#include "qapplication.h"

/*!
    \class QGrid qgrid.h
    \brief The QGrid widget provides simple geometry management of its children.

    \ingroup geomanagement
    \ingroup appearance

    The grid places its widgets either in columns or in rows depending
    on its orientation.

    The number of rows \e or columns is defined in the constructor.
    All the grid's children will be placed and sized in accordance
    with their sizeHint() and sizePolicy().

    Use setMargin() to add space around the grid itself, and
    setSpacing() to add space between the widgets.

    \img qgrid-m.png QGrid

    \sa QVBox QHBox QGridLayout
*/

/*! \enum QGrid::Direction
    \internal
*/

/*!
    Constructs a grid widget with parent \a parent, called \a name.
    If \a orient is \c Horizontal, \a n specifies the number of
    columns. If \a orient is \c Vertical, \a n specifies the number of
    rows. The widget flags \a f are passed to the QFrame constructor.
*/
QGrid::QGrid( int n, Orientation orient, QWidget *parent, const char *name,
	      WFlags f )
    : QFrame( parent, name, f )
{
    int nCols, nRows;
    if ( orient == Horizontal ) {
	nCols = n;
	nRows = -1;
    } else {
	nCols = -1;
	nRows = n;
    }
    lay = new QGridLayout( this, nRows, nCols, 0, 0, name );
    lay->setAutoAdd( TRUE );
}



/*!
    Constructs a grid widget with parent \a parent, called \a name.
    \a n specifies the number of columns. The widget flags \a f are
    passed to the QFrame constructor.
 */
QGrid::QGrid( int n, QWidget *parent, const char *name, WFlags f )
    : QFrame( parent, name, f )
{
    lay = new QGridLayout( this, -1, n, 0, 0, name );
    lay->setAutoAdd( TRUE );
}


/*!
    Sets the spacing between the child widgets to \a space.
*/

void QGrid::setSpacing( int space )
{
    if ( layout() )
	layout()->setSpacing( space );
}


/*!\reimp
 */
void QGrid::frameChanged()
{
    if ( !layout() )
	return;
    layout()->setMargin( frameWidth() );
}


/*!
  \reimp
*/

QSize QGrid::sizeHint() const
{
    QWidget *mThis = (QWidget*)this;
    QApplication::sendPostedEvents( mThis, QEvent::ChildInserted );
    return QFrame::sizeHint();
}
#endif
