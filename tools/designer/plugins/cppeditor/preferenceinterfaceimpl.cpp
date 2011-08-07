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

#include "preferenceinterfaceimpl.h"
#include <preferences.h>

PreferenceInterfaceImpl::PreferenceInterfaceImpl( QUnknownInterface *outer )
    : parent( outer ),
      ref( 0 ),
      cppEditorSyntax( 0 )
{
}

PreferenceInterfaceImpl::~PreferenceInterfaceImpl()
{
}

ulong PreferenceInterfaceImpl::addRef()
{
    return parent ? parent->addRef() : ref++;
}

ulong PreferenceInterfaceImpl::release()
{
    if ( parent )
	return parent->release();
    if ( !--ref ) {
	delete this;
	return 0;
    }
    return ref;
}

QRESULT PreferenceInterfaceImpl::queryInterface( const QUuid &uuid, QUnknownInterface** iface )
{
    if ( parent )
	return parent->queryInterface( uuid, iface );

    *iface = 0;
    if ( uuid == IID_QUnknown )
	*iface = (QUnknownInterface*)this;
    else if ( uuid == IID_Preference )
	*iface = (PreferenceInterface*)this;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}

PreferenceInterface::Preference *PreferenceInterfaceImpl::preference()
{
    if ( !cppEditorSyntax ) {
	cppEditorSyntax = new PreferencesBase( 0, "cppeditor_syntax" );
	( (PreferencesBase*)cppEditorSyntax )->setPath( "/Trolltech/CppEditor/" );
	cppEditorSyntax->hide();
    }
    Preference *pf = 0;
    pf = new Preference;
    pf->tab = cppEditorSyntax;
    pf->title = "C++ Editor";
    pf->receiver = pf->tab;
    pf->init_slot = SLOT( reInit() );
    pf->accept_slot = SLOT( save() );
    return pf;
}

void PreferenceInterfaceImpl::deletePreferenceObject( Preference *p )
{
    delete p;
}
