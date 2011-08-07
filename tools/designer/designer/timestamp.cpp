 /**********************************************************************
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
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

#include "timestamp.h"
#include <qfile.h>
#include <qfileinfo.h>

class QTimer;

TimeStamp::TimeStamp( QObject *parent, const QString &f )
    : QObject( parent ), filename( f ), autoCheck( FALSE )
{
    timer = new QTimer( this );
    connect( timer, SIGNAL( timeout() ), this, SLOT( autoCheckTimeStamp() ) );
    update();
}

void TimeStamp::setFileName( const QString &f )
{
    filename = f;
    update();
}

QString TimeStamp::fileName() const
{
    return filename;
}

void TimeStamp::setAutoCheckEnabled( bool a )
{
    autoCheck = a;
    if ( autoCheck )
	timer->start( 5000 );
    else
	timer->stop();
}

void TimeStamp::update()
{
    QFile f( filename );
    if ( !f.exists() )
	return;
    lastTimeStamp = QFileInfo( f ).lastModified();
}

bool TimeStamp::isUpToDate() const
{
    QFile f( filename );
    if ( !f.exists() )
	return TRUE;
    return lastTimeStamp == QFileInfo( f ).lastModified();
}

bool TimeStamp::isAutoCheckEnabled() const
{
    return autoCheck;
}

void TimeStamp::autoCheckTimeStamp()
{
    if ( !isUpToDate() )
	emit timeStampChanged();
}
