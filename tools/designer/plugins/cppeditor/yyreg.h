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

#ifndef YYREG_H
#define YYREG_H

#include <qstringlist.h>
#include <qvaluelist.h>

class CppFunction
{
public:
    CppFunction() : cnst( FALSE ), lineno1( 0 ), lineno2( 0 ) { }

    void setReturnType( const QString& r ) { ret = r; }
    void setScopedName( const QString& n ) { nam = n; }
    void setParameterList( const QStringList& p ) { params = p; }
    void setConst( bool c ) { cnst = c; }
    void setBody( const QString& b ) { bod = b; }
    void setDocumentation( const QString& d ) { doc = d; }
    void setLineNums( int functionStart, int openingBrace, int closingBrace ) {
	lineno0 = functionStart;
	lineno1 = openingBrace;
	lineno2 = closingBrace;
    }

    const QString& returnType() const { return ret; }
    const QString& scopedName() const { return nam; }
    const QStringList& parameterList() const { return params; }
    bool isConst() const { return cnst; }
    QString prototype() const;
    const QString& body() const { return bod; }
    const QString& documentation() const { return doc; }
    int functionStartLineNum() const { return lineno0; }
    int openingBraceLineNum() const { return lineno1; }
    int closingBraceLineNum() const { return lineno2; }

#if defined(Q_FULL_TEMPLATE_INSTANTIATION)
    bool operator==( const CppFunction& ) const { return FALSE; }
#endif

private:
    QString ret;
    QString nam;
    QStringList params;
    bool cnst;
    QString bod;
    QString doc;
    int lineno0;
    int lineno1;
    int lineno2;
};

void extractCppFunctions( const QString& code, QValueList<CppFunction> *flist );
QString canonicalCppProto( const QString& proto );

#endif
