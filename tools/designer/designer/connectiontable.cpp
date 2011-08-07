 /**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Designer.
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

#include "connectiontable.h"

ConnectionTable::ConnectionTable( QWidget *parent, const char *name )
    : QTable( 0, 4, parent, name )
{
    setSorting( TRUE );
    setShowGrid( FALSE );
    setFocusStyle( FollowStyle );
    setSelectionMode( SingleRow );
    horizontalHeader()->setLabel( 0, tr( "Sender" ) );
    horizontalHeader()->setLabel( 1, tr( "Signal" ) );
    horizontalHeader()->setLabel( 2, tr( "Receiver" ) );
    horizontalHeader()->setLabel( 3, tr( "Slot" ) );
    setColumnStretchable( 0, TRUE );
    setColumnStretchable( 1, TRUE );
    setColumnStretchable( 2, TRUE );
    setColumnStretchable( 3, TRUE );
}

void ConnectionTable::sortColumn( int col, bool ascending, bool )
{
    horizontalHeader()->setSortIndicator( col, ascending );
    if ( isEditing() )
	endEdit( currEditRow(), currEditCol(), FALSE, FALSE );
    QTable::sortColumn( col, ascending, TRUE );
    setCurrentCell( 0, 0 );
    emit resorted();
}
