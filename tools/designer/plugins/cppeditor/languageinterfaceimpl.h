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

#ifndef LANGUAGEINTERFACEIMPL_H
#define LANGUAGEINTERFACEIMPL_H

#include <languageinterface.h>

class LanguageInterfaceImpl : public LanguageInterface
{
public:
    LanguageInterfaceImpl( QUnknownInterface *outer = 0 );

    ulong addRef();
    ulong release();

    QRESULT queryInterface( const QUuid&, QUnknownInterface** );

    void functions( const QString &code, QValueList<Function> *funcs ) const;
    void connections( const QString &, QValueList<Connection> * ) const {};
    QString createFunctionStart( const QString &className, const QString &func,
				 const QString &returnType, const QString &access );
    QStringList definitions() const;
    QStringList definitionEntries( const QString &definition, QUnknownInterface *designerIface ) const;
    void setDefinitionEntries( const QString &definition, const QStringList &entries, QUnknownInterface *designerIface );
    QString createArguments( const QString & ) { return QString::null; }
    QString createEmptyFunction();
    bool supports( Support s ) const;
    QStringList fileFilterList() const;
    QStringList fileExtensionList() const;
    void preferedExtensions( QMap<QString, QString> &extensionMap ) const;
    void sourceProjectKeys( QStringList &keys ) const;
    QString projectKeyForExtension( const QString &extension ) const;
    QString cleanSignature( const QString &sig ) { return sig; } // #### implement me
    void loadFormCode( const QString &, const QString &,
		       QValueList<Function> &,
		       QStringList &,
		       QValueList<Connection> & );
    QString formCodeExtension() const { return ".h"; }
    bool canConnect( const QString &signal, const QString &slot );
    void compressProject( const QString &, const QString &, bool ) {}
    QString uncompressProject( const QString &, const QString & ) { return QString::null; }
    QString aboutText() const { return ""; }

    void addConnection( const QString &, const QString &,
			const QString &, const QString &,
			QString * ) {}
    void removeConnection( const QString &, const QString &,
			   const QString &, const QString &,
			   QString * ) {}
    QStrList signalNames( QObject *obj ) const;

private:
    QUnknownInterface *parent;
    ulong ref;

};

#endif
