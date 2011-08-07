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

#include <designerinterface.h>
#include <qfeatures.h>
#include <qwidget.h>
#include <templatewizardiface.h>
#ifndef QT_NO_SQL
#include "sqlformwizardimpl.h"
#endif
#include "mainwindowwizard.h"
#include <qapplication.h>

class StandardTemplateWizardInterface : public TemplateWizardInterface, public QLibraryInterface
{
public:
    StandardTemplateWizardInterface();
    virtual ~StandardTemplateWizardInterface();

    QRESULT queryInterface( const QUuid&, QUnknownInterface** );
    Q_REFCOUNT;

    QStringList featureList() const;

    void setup( const QString &templ, QWidget *widget, DesignerFormWindow *fw, QUnknownInterface *aIface );

    bool init();
    void cleanup();
    bool canUnload() const;

private:
    bool inUse;
};

StandardTemplateWizardInterface::StandardTemplateWizardInterface()
    : inUse( FALSE )
{
}

StandardTemplateWizardInterface::~StandardTemplateWizardInterface()
{
}

bool StandardTemplateWizardInterface::init()
{
    return TRUE;
}

void StandardTemplateWizardInterface::cleanup()
{
}

bool StandardTemplateWizardInterface::canUnload() const
{
    return !inUse;
}

QStringList StandardTemplateWizardInterface::featureList() const
{
    QStringList list;
#ifndef QT_NO_SQL
    list << "QDataBrowser" << "QDesignerDataBrowser" << "QDataView" << \
	"QDesignerDataView" << "QDataTable";
#endif
    list << "QMainWindow";

    return list;
}

void StandardTemplateWizardInterface::setup( const QString &templ, QWidget *widget, DesignerFormWindow *fw, QUnknownInterface *aIface )
{
    inUse = TRUE;
#ifndef QT_NO_SQL
    if ( templ == "QDesignerDataView" ||
	 templ == "QDesignerDataBrowser" ||
	 templ == "QDataView" ||
	 templ == "QDataBrowser" ||
	 templ == "QDataTable" ) {
	SqlFormWizard wizard( aIface, widget, qApp->mainWidget(), fw, 0, TRUE );
	wizard.exec();
    }
#endif
    if ( templ == "QMainWindow" ) {
	MainWindowWizardBase wizard( qApp->mainWidget(), 0, TRUE );
	wizard.setAppInterface( aIface, fw, widget );
	wizard.exec();
    }
    inUse = FALSE;
}

QRESULT StandardTemplateWizardInterface::queryInterface( const QUuid& uuid, QUnknownInterface** iface )
{
    *iface = 0;
    if ( uuid == IID_QUnknown )
	*iface = (QUnknownInterface*)(TemplateWizardInterface*) this;
    else if ( uuid == IID_QFeatureList )
	*iface = (QFeatureListInterface*)this;
    else if ( uuid == IID_TemplateWizard )
	*iface = (TemplateWizardInterface*)this;
    else if ( uuid == IID_QLibrary )
	*iface = (QLibraryInterface*)this;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}

Q_EXPORT_COMPONENT()
{
    Q_CREATE_INSTANCE( StandardTemplateWizardInterface )
}
