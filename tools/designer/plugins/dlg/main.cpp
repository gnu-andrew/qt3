/**********************************************************************
**
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

#include <filterinterface.h>
#include <qapplication.h>

#include "dlg2ui.h"

class DlgFilter : public ImportFilterInterface, public QLibraryInterface
{
public:
    DlgFilter();

    QRESULT queryInterface( const QUuid&, QUnknownInterface **iface );
    Q_REFCOUNT;

    QStringList featureList() const;
    QStringList import( const QString& filter, const QString& filename );

    bool init();
    void cleanup();
    bool canUnload() const;
};

DlgFilter::DlgFilter()
{
}

QRESULT DlgFilter::queryInterface( const QUuid &uuid, QUnknownInterface **iface )
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

QStringList DlgFilter::featureList() const
{
    QStringList list;
    list << "Qt Architect Dialog Files (*.dlg)" ;
    return list;
}

QStringList DlgFilter::import( const QString &, const QString& filename )
{
    Dlg2Ui d;
    return d.convertQtArchitectDlgFile( filename );
}

bool DlgFilter::init()
{
    return TRUE;
}

void DlgFilter::cleanup()
{
}

bool DlgFilter::canUnload() const
{
    return TRUE;
}

Q_EXPORT_COMPONENT()
{
    Q_CREATE_INSTANCE( DlgFilter )
}
