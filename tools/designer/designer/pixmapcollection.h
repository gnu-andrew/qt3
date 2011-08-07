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

#ifndef PIXMAPCOLLECTION_H
#define PIXMAPCOLLECTION_H

#include <qstring.h>
#include <qpixmap.h>
#include <qvaluelist.h>
#include "designerappiface.h"

class QMimeSourceFactory;
class Project;

class PixmapCollection
{
public:
    struct Pixmap
    {
	QPixmap pix;
	QString name;
	QString absname;
	Q_DUMMY_COMPARISON_OPERATOR( Pixmap )
    };

    PixmapCollection( Project *pro );
    ~PixmapCollection();

    bool addPixmap( const Pixmap &pix, bool force = TRUE );
    void removePixmap( const QString &name );
    QPixmap pixmap( const QString &name );

    QValueList<Pixmap> pixmaps() const;
    bool isEmpty() const;

    void setActive( bool b );

    void load( const QString& filename );

    DesignerPixmapCollection *iFace();

private:
    QString unifyName( const QString &n );
    void savePixmap( Pixmap &pix );

    QString imageDir() const;
    void mkdir();

private:
    QValueList<Pixmap> pixList;
    QMimeSourceFactory *mimeSourceFactory;
    Project *project;
    DesignerPixmapCollectionImpl *iface;

};

#endif
