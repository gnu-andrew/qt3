/**********************************************************************
**
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Designer
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

#include <filterinterface.h>

#include <qapplication.h>
#include <qstring.h>
#include <qdir.h>

#include "kdevdlg2ui.h"

class KDevDlgFilter : public ImportFilterInterface, public QLibraryInterface
{
public:
    KDevDlgFilter();

    QRESULT queryInterface( const QUuid&, QUnknownInterface **iface );
    Q_REFCOUNT;

    QStringList featureList() const;
    QStringList import( const QString& filter, const QString& filename );

    bool init();
    void cleanup();
    bool canUnload() const;
};

KDevDlgFilter::KDevDlgFilter()
{
}

QRESULT KDevDlgFilter::queryInterface( const QUuid &uuid, QUnknownInterface **iface )
{
    *iface = 0;
    if ( uuid == IID_QUnknown )
	*iface = (QUnknownInterface*)(ImportFilterInterface*)this;
    else if ( uuid == IID_QFeatureList )
	*iface = (QFeatureListInterface*)this;
    else if ( uuid == IID_ImportFilter )
	*iface = (ImportFilterInterface*)this;
    else if ( uuid == IID_QLibrary )
	*iface = (QLibraryInterface*)this;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}

QStringList KDevDlgFilter::featureList() const
{
    QStringList list;
    list << "KDevelop Dialog Files (*.kdevdlg)" ;
    return list;
}

QStringList KDevDlgFilter::import( const QString &, const QString& filename )
{
    QFile file( filename );
    if ( !file.open( IO_ReadOnly ) )
	qWarning( "uic: Could not open file '%s' ", filename.latin1() );
    QTextStream in;
    in.setDevice( &file );

    QString name = filename.right( filename.length() - filename.findRev( QDir::separator() ) - 1 ).section( ".", 0, 0 );
    KDEVDLG2UI c( &in, name );
    QStringList files;
    c.parse();
    return c.targetFiles;
}

bool KDevDlgFilter::init()
{
    return TRUE;
}

void KDevDlgFilter::cleanup()
{
}

bool KDevDlgFilter::canUnload() const
{
    return TRUE;
}

Q_EXPORT_COMPONENT()
{
    Q_CREATE_INSTANCE( KDevDlgFilter )
}
