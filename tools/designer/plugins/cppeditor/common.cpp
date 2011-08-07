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

#include "editorinterfaceimpl.h"
#include "languageinterfaceimpl.h"
#include "preferenceinterfaceimpl.h"
#include "projectsettingsinterfaceimpl.h"
#include "sourcetemplateinterfaceimpl.h"

class CommonInterface : public QComponentInformationInterface
{
public:
    CommonInterface();
    virtual ~CommonInterface();

    QRESULT queryInterface( const QUuid&, QUnknownInterface** );
    Q_REFCOUNT;

    QString name() const { return "C++"; }
    QString description() const { return "C++ Integration"; }
    QString version() const { return "0.1"; }
    QString author() const { return "Trolltech AS"; }

private:
    LanguageInterfaceImpl *langIface;
    PreferenceInterfaceImpl *prefIface;
    ProjectSettingsInterfaceImpl *proIface;
    SourceTemplateInterfaceImpl *srcIface;

};

CommonInterface::CommonInterface()
{
    langIface = new LanguageInterfaceImpl( this );
    langIface->addRef();
    prefIface = new PreferenceInterfaceImpl( this );
    prefIface->addRef();
    proIface = new ProjectSettingsInterfaceImpl( this );
    proIface->addRef();
    srcIface = new SourceTemplateInterfaceImpl;
    srcIface->addRef();
}

CommonInterface::~CommonInterface()
{
    langIface->release();
    prefIface->release();
    proIface->release();
    srcIface->release();
}

QRESULT CommonInterface::queryInterface( const QUuid &uuid, QUnknownInterface** iface )
{
    *iface = 0;
    if ( uuid == IID_QUnknown )
	*iface = (QUnknownInterface*)this;
    else if ( uuid == IID_QComponentInformation )
	*iface = (QComponentInformationInterface*)this;
    else if ( uuid == IID_Editor )
	*iface = new EditorInterfaceImpl;
    else if ( uuid == IID_Language )
	*iface = langIface;
    else if ( uuid == IID_Preference )
	*iface = prefIface;
    else if ( uuid == IID_ProjectSettings )
	*iface = proIface;
    else if ( uuid == IID_SourceTemplate )
	*iface = srcIface;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}


Q_EXPORT_COMPONENT()
{
     Q_CREATE_INSTANCE( CommonInterface )
}
