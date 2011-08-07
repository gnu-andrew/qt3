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

#ifndef KDEVDLG2UI_H
#define KDEVDLG2UI_H

#include <qtextstream.h>
#include <qfile.h>
#include <qstringlist.h>

class KDEVDLG2UI
{
public:
    KDEVDLG2UI( QTextStream* input, const QString& name = "dialog" );
    ~KDEVDLG2UI();

    bool parse();
    bool parse ( QStringList& get );

    QStringList targetFiles;

protected:

    bool writeDialog( const QString& name );

    QString className;
    QString line;
    QTextStream *in;
    QStringList target;

    void cleanString( QString* text );

    void indent();
    void undent();
    void wi();

    void writeClass( const QString& name );
    void writeCString( const QString& name, const QString& value );
    void writeString( const QString& name, const QString& value );
    void writeRect( const QString& name, int x, int y, int w, int h );
    void writeFont( const QString& family, int pointsize );
    void writeBool( const QString& name, bool value );
    void writeNumber( const QString& name, int value );
    void writeEnum( const QString& name, const QString& value );
    void writeSet( const QString& name, const QString& value );
    void writeItem( const QString& name, const QString& value );
    void writeColumn( const QString& name, const QString& value );
    void writeColor( const QString& name, const QString& value );
    void writeStyles( const QStringList styles, bool isFrame );
    void writeWidgetStart( const QString& qclass );
    void writeWidgetEnd();
    
private:
    int indentation;
    bool writeToFile;
    QTextStream* out;
};

#endif
