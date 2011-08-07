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

#include "languageinterfaceimpl.h"
#include <qobject.h>
#include <designerinterface.h>
#include <qfile.h>
#include "yyreg.h"
#include <qmetaobject.h>

LanguageInterfaceImpl::LanguageInterfaceImpl( QUnknownInterface *outer )
    : parent( outer ), ref( 0 )
{
}

ulong LanguageInterfaceImpl::addRef()
{
    return parent ? parent->addRef() : ref++;
}

ulong LanguageInterfaceImpl::release()
{
    if ( parent )
	return parent->release();
    if ( !--ref ) {
	delete this;
	return 0;
    }
    return ref;
}

QRESULT LanguageInterfaceImpl::queryInterface( const QUuid &uuid, QUnknownInterface** iface )
{
    if ( parent )
	return parent->queryInterface( uuid, iface );

    *iface = 0;
    if ( uuid == IID_QUnknown )
	*iface = (QUnknownInterface*)this;
    else if ( uuid == IID_Language )
	*iface = (LanguageInterface*)this;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}


class NormalizeObject : public QObject
{
public:
    NormalizeObject() : QObject() {}
    static QCString normalizeSignalSlot( const char *signalSlot ) { return QObject::normalizeSignalSlot( signalSlot ); }
};

void LanguageInterfaceImpl::functions( const QString &code, QValueList<Function> *functionMap ) const
{
    QValueList<CppFunction> l;
    extractCppFunctions( code, &l );
    for ( QValueList<CppFunction>::Iterator it = l.begin(); it != l.end(); ++it ) {
	Function func;
	func.name = (*it).prototype();
	func.name.remove( 0, (*it).returnType().length() );
	if ( func.name.find( "::" ) == -1 )
	    continue;
	func.name.remove( (uint)0, func.name.find( "::" ) + 2 );
	func.body = (*it).body();
	func.returnType = (*it).returnType();
	func.start = (*it).functionStartLineNum();
	func.end = (*it).closingBraceLineNum();
	functionMap->append( func );
    }
}

QString LanguageInterfaceImpl::createFunctionStart( const QString &className, const QString &func,
						    const QString &returnType,
						    const QString & )
{
    return returnType + " " + className + "::" + func;
}

QStringList LanguageInterfaceImpl::definitions() const
{
    QStringList lst;
    lst << "Includes (in Implementation)" << "Includes (in Declaration)" << "Forward Declarations" << "Signals";
    return lst;
}

QStringList LanguageInterfaceImpl::definitionEntries( const QString &definition, QUnknownInterface *designerIface ) const
{
    DesignerInterface *iface = 0;
    designerIface->queryInterface( IID_Designer, (QUnknownInterface**) &iface );
    if ( !iface )
	return QStringList();
    DesignerFormWindow *fw = iface->currentForm();
    if ( !fw )
	return QStringList();
    QStringList lst;
    if ( definition == "Includes (in Implementation)" ) {
	lst = fw->implementationIncludes();
    } else if ( definition == "Includes (in Declaration)" ) {
	lst = fw->declarationIncludes();
    } else if ( definition == "Forward Declarations" ) {
	lst = fw->forwardDeclarations();
    } else if ( definition == "Signals" ) {
	lst = fw->signalList();
    }
    iface->release();
    return lst;
}

void LanguageInterfaceImpl::setDefinitionEntries( const QString &definition, const QStringList &entries, QUnknownInterface *designerIface )
{
    DesignerInterface *iface = 0;
    designerIface->queryInterface( IID_Designer, (QUnknownInterface**) &iface );
    if ( !iface )
	return;
    DesignerFormWindow *fw = iface->currentForm();
    if ( !fw )
	return;
    if ( definition == "Includes (in Implementation)" ) {
	fw->setImplementationIncludes( entries );
    } else if ( definition == "Includes (in Declaration)" ) {
	fw->setDeclarationIncludes( entries );
    } else if ( definition == "Forward Declarations" ) {
	fw->setForwardDeclarations( entries );
    } else if ( definition == "Signals" ) {
	fw->setSignalList( entries );
    }
    iface->release();
}

QString LanguageInterfaceImpl::createEmptyFunction()
{
    return "{\n\n}\n";
}

bool LanguageInterfaceImpl::supports( Support s ) const
{
    if ( s == ReturnType )
	return TRUE;
    if ( s == ConnectionsToCustomSlots )
	return TRUE;
    return FALSE;
}

QStringList LanguageInterfaceImpl::fileFilterList() const
{
    QStringList f;
    f << "C++ Files (*.cpp *.C *.cxx *.c++ *.c *.h *.H *.hpp *.hxx)";
    return f;

}
QStringList LanguageInterfaceImpl::fileExtensionList() const
{
    QStringList f;
    f << "cpp" << "C" << "cxx" << "c++" << "c" <<"h" << "H" << "hpp" << "hxx";
    return f;
}

QString LanguageInterfaceImpl::projectKeyForExtension( const QString &extension ) const
{
    // also handle something like foo.ut.cpp
    QString ext = extension;
    int i = ext.findRev('.');
    if ( i > -1 && i < (int)(ext.length()-1) )
        ext = ext.mid( i + 1 );
    if ( ext[ 0 ] == 'c' || ext[ 0 ] == 'C' )
	    return "SOURCES";
    return "HEADERS";
}

void LanguageInterfaceImpl::sourceProjectKeys( QStringList &keys ) const
{
    keys << "HEADERS" << "SOURCES";
}

 class CheckObject : public QObject
{
public:
    CheckObject() {}
    bool checkConnectArgs( const char *signal, const char *member ) { return QObject::checkConnectArgs( signal, 0, member ); }

};

bool LanguageInterfaceImpl::canConnect( const QString &signal, const QString &slot )
{
    CheckObject o;
    return o.checkConnectArgs( signal.latin1(), slot.latin1() );
}

void LanguageInterfaceImpl::loadFormCode( const QString &, const QString &filename,
					       QValueList<Function> &functions,
					       QStringList &,
					       QValueList<Connection> & )
{
    QFile f( filename );
    if ( !f.open( IO_ReadOnly ) )
	return;
    QTextStream ts( &f );
    QString code( ts.read() );
    this->functions( code, &functions );
}

void LanguageInterfaceImpl::preferedExtensions( QMap<QString, QString> &extensionMap ) const
{
    extensionMap.insert( "cpp", "C++ Source File" );
    extensionMap.insert( "h", "C++ Header File" );
}

QStrList LanguageInterfaceImpl::signalNames( QObject *obj ) const
{
    QStrList sigs;
    sigs = obj->metaObject()->signalNames( TRUE );
    sigs.remove( "destroyed()" );
    return sigs;
}
