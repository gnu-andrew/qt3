/**********************************************************************
**
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

#include "projectsettingsinterfaceimpl.h"
#include "projectsettings.h"

ProjectSettingsInterfaceImpl::ProjectSettingsInterfaceImpl( QUnknownInterface *outer )
    : parent( outer ),
      ref( 0 ),
      settingsTab( 0 )
{
}

ulong ProjectSettingsInterfaceImpl::addRef()
{
    return parent ? parent->addRef() : ref++;
}

ulong ProjectSettingsInterfaceImpl::release()
{
    if ( parent )
	return parent->release();
    if ( !--ref ) {
	delete this;
	return 0;
    }
    return ref;
}

ProjectSettingsInterface::ProjectSettings *ProjectSettingsInterfaceImpl::projectSetting()
{
    if ( !settingsTab ) {
	settingsTab = new CppProjectSettings( 0 );
	settingsTab->hide();
    }
    ProjectSettings *pf = 0;
    pf = new ProjectSettings;
    pf->tab = settingsTab;
    pf->title = "C++";
    pf->receiver = pf->tab;
    pf->init_slot = SLOT( reInit( QUnknownInterface * ) );
    pf->accept_slot = SLOT( save( QUnknownInterface * ) );
    return pf;
}

QStringList ProjectSettingsInterfaceImpl::projectSettings() const
{
    return QStringList();
}

void ProjectSettingsInterfaceImpl::connectTo( QUnknownInterface * )
{
}

void ProjectSettingsInterfaceImpl::deleteProjectSettingsObject( ProjectSettings *pf )
{
    delete pf;
}

QRESULT ProjectSettingsInterfaceImpl::queryInterface( const QUuid &uuid, QUnknownInterface **iface )
{
    if ( parent )
	return parent->queryInterface( uuid, iface );

    *iface = 0;
    if ( uuid == IID_QUnknown )
	*iface = (QUnknownInterface*)this;
    else if ( uuid == IID_ProjectSettings )
	*iface = (ProjectSettingsInterface*)this;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}
