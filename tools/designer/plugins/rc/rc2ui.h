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

#ifndef RC2UI_H
#define RC2UI_H

#include <qtextstream.h>
#include <qfile.h>
#include <qstringlist.h>

class RC2UI
{
public:
    RC2UI( QTextStream* input );
    ~RC2UI();

    bool parse();
    bool parse ( QStringList& get );

    QStringList targetFiles;

protected:
    enum WidgetType { IDWidget, IDPushButton, IDLabel, IDCheckBox, IDRadioButton, 
		      IDGroupBox, IDLineEdit, IDMultiLineEdit, IDIconView, IDListView,
		      IDProgressBar, IDTabWidget, IDSpinBox, IDSlider, IDComboBox, 
		      IDListBox, IDScrollBar, IDCustom, IDUnknown = 0x0100 };
    bool makeDialog();
    bool makeBitmap();
    bool makeStringTable();
    bool makeAccelerator();
    bool makeCursor();
    bool makeHTML();
    bool makeIcon();
    bool makeVersion();

    QString line;
    QTextStream *in;
    QStringList target;

    void indent();
    void undent();

    QString stripQM( const QString& string );
    QString parseNext( QString& arg, char sep = ',' );
    QStringList splitStyles( const QString& styles, char sep = '|' );
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

    void writeStyles( const QStringList styles, bool isFrame );
private:
    int indentation;
    bool writeToFile;
    QTextStream* out;

    QString useName( const QString& );

    QStringList usedNames;

    const QString blockStart1;
    const QString blockStart2;
};

#endif
