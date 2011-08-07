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

#include "pixmapcollection.h"
#include "project.h"
#include "mainwindow.h"
#include <qmime.h>
#include <qdir.h>
#include <qfileinfo.h>
#include <qtextstream.h>
#include <qimage.h>

PixmapCollection::PixmapCollection( Project *pro )
    : project( pro )
{
    iface = new DesignerPixmapCollectionImpl( this );
    mimeSourceFactory = new QMimeSourceFactory();
}

PixmapCollection::~PixmapCollection()
{
    delete mimeSourceFactory;
    delete iface;
}

bool PixmapCollection::addPixmap( const Pixmap &pix, bool force )
{
    Pixmap pixmap = pix;
    savePixmap( pixmap );

    if ( !force ) {
	for ( QValueList<Pixmap>::Iterator it = pixList.begin(); it != pixList.end(); ++it ) {
	    if ( (*it).name == pixmap.name )
		return FALSE;
	}
    }

    pixList.append( pixmap );
    mimeSourceFactory->setPixmap( pixmap.name, pixmap.pix );
    project->setModified( TRUE );
    return TRUE;
}

void PixmapCollection::removePixmap( const QString &name )
{
    for ( QValueList<Pixmap>::Iterator it = pixList.begin(); it != pixList.end(); ++it ) {
	if ( (*it).name == name ) {
	    pixList.remove( it );
	    break;
	}
    }
    project->setModified( TRUE );
}

QValueList<PixmapCollection::Pixmap> PixmapCollection::pixmaps() const
{
    return pixList;
}

QString PixmapCollection::unifyName( const QString &n )
{
    QString name = n;
    bool restart = FALSE;
    int added = 1;

    for ( QValueList<Pixmap>::Iterator it = pixList.begin(); it != pixList.end(); ++it ) {
	if ( restart )
	    it = pixList.begin();
	restart = FALSE;
	if ( name == (*it).name ) {
	    name = n;
	    name += "_" + QString::number( added );
	    ++added;
	    restart = TRUE;
	}
    }

    return name;
}

void PixmapCollection::setActive( bool b )
{
    if ( b )
	QMimeSourceFactory::defaultFactory()->addFactory( mimeSourceFactory );
    else
	QMimeSourceFactory::defaultFactory()->removeFactory( mimeSourceFactory );
}

QPixmap PixmapCollection::pixmap( const QString &name )
{
    for ( QValueList<Pixmap>::Iterator it = pixList.begin(); it != pixList.end(); ++it ) {
	if ( (*it).name == name )
	    return (*it).pix;
    }
    return QPixmap();
}

void PixmapCollection::savePixmap( Pixmap &pix )
{
    if ( pix.absname == imageDir() + "/" + pix.name )
	return; // no need to save, it is already there
    QString rel = project->makeRelative( pix.absname );
    if ( rel[0] == '/' || ( rel[1] == ':' && rel[2] == '/' ) ) { // only save if file is outside the project
	mkdir();
	pix.name = unifyName( QFileInfo( pix.absname ).baseName() ) + ".png";
	pix.absname = imageDir() + "/" + pix.name;
	pix.pix.save( pix.absname, "PNG" );
    } else if ( rel.isEmpty() ) {
	mkdir();
	pix.name = unifyName( pix.name );
	pix.absname = imageDir() + "/" + pix.name;
	pix.pix.save( pix.absname, "PNG" );
    }
}

QString PixmapCollection::imageDir() const
{
    return QFileInfo( project->fileName() ).dirPath( TRUE ) + "/images";
}

void PixmapCollection::mkdir()
{
    QString f = project->fileName();
    QDir d( QFileInfo( f ).dirPath( TRUE ) );
    d.mkdir( "images" );
}

void PixmapCollection::load( const QString& filename )
{
    if ( filename.isEmpty() )
	return;
    QString absfile;
    if ( filename[0] == '/' )
	absfile = filename;
    else
	absfile = QFileInfo( project->fileName() ).dirPath( TRUE ) + "/" + filename;

    QPixmap pm( absfile );
    if ( pm.isNull() )
	return;

    Pixmap pix;
    pix.name = QFileInfo( absfile ).fileName();
    pix.absname = absfile;
    pix.pix = pm;
    addPixmap( pix, TRUE );
}

DesignerPixmapCollection *PixmapCollection::iFace()
{
    return iface;
}

bool PixmapCollection::isEmpty() const
{
    return pixList.isEmpty();
}
