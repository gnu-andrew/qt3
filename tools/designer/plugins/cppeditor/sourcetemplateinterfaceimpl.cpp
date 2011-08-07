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

#include "sourcetemplateinterfaceimpl.h"
#include <designerinterface.h>
#include "mainfilesettings.h"
#include <qlineedit.h>
#include <qlistbox.h>

SourceTemplateInterfaceImpl::SourceTemplateInterfaceImpl()
{
}

QRESULT SourceTemplateInterfaceImpl::queryInterface( const QUuid &uuid, QUnknownInterface **iface )
{
    *iface = 0;
    if ( uuid == IID_QUnknown )
	*iface = (QUnknownInterface*)this;
    else if ( uuid == IID_QFeatureList )
	*iface = (QFeatureListInterface*)this;
    else if ( uuid == IID_SourceTemplate )
	*iface = (SourceTemplateInterface*)this;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}

QStringList SourceTemplateInterfaceImpl::featureList() const
{
    QStringList l;
    l << "C++ Main-File (main.cpp)";
    return l;
}

static QString generateMainCppCode( const QString &formname, const QString &include )
{
    QString code;
    code += "#include <qapplication.h>\n";
    code += "#include \"" + include + "\"\n";
    code += "\n";
    code += "int main( int argc, char ** argv )\n";
    code += "{\n";
    code += "    QApplication a( argc, argv );\n";
    code += "    " + formname + " w;\n";
    code += "    w.show();\n";
    code += "    a.connect( &a, SIGNAL( lastWindowClosed() ), &a, SLOT( quit() ) );\n";
    code += "    return a.exec();\n";
    code += "}\n";
    return code;
}

SourceTemplateInterface::Source SourceTemplateInterfaceImpl::create( const QString &templ,
								     QUnknownInterface *appIface )
{
    SourceTemplateInterface::Source src;
    src.type = SourceTemplateInterface::Source::Invalid;
    if ( templ == "C++ Main-File (main.cpp)" ) {
	CppMainFile dia( 0, 0, TRUE );
	dia.setup( appIface );
	if ( dia.exec() == QDialog::Accepted ) {
	    DesignerInterface *dIface = 0;
	    appIface->queryInterface( IID_Designer, (QUnknownInterface**)&dIface );
	    if ( dIface ) {
		src.type = SourceTemplateInterface::Source::FileName;
		src.filename = dia.editFileName->text();
		QString include = dIface->currentProject()->
				  formFileName( dia.listForms->text( dia.listForms->currentItem() ) );
		include.remove( include.length() - 2, 2 );
		include += "h";
		int slashFind = include.findRev('/');
		if (slashFind != -1)
		    include = include.mid(slashFind+1);
		src.code = generateMainCppCode( dia.listForms->text( dia.listForms->currentItem() ),
						include );
	    }
	}
    }
    return src;
}

QString SourceTemplateInterfaceImpl::language( const QString & ) const
{
    return "C++";
}
