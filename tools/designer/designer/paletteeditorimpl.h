/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
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

#ifndef PALETTEEDITORIMPL_H
#define PALETTEEDITORIMPL_H

#include "paletteeditor.h"

class FormWindow;

class PaletteEditor : public PaletteEditorBase
{
    Q_OBJECT

public:
    PaletteEditor( FormWindow *fw, QWidget * parent=0, const char * name=0, bool modal=FALSE, WFlags f=0 );
    ~PaletteEditor();

    static QPalette getPalette( bool *ok, const QPalette &pal, BackgroundMode mode = PaletteBackground,
				QWidget* parent = 0, const char* name = 0, FormWindow *fw = 0 );

protected slots:
    void onChooseMainColor();
    void onChoose2ndMainColor();
    void onTune();
    void paletteSelected(int);

protected:
    void buildPalette();
    void buildActiveEffect();
    void buildInactive();
    void buildInactiveEffect();
    void buildDisabled();
    void buildDisabledEffect();

private:
    void setPreviewPalette( const QPalette& );
    void updateStyledButtons();
    void setupBackgroundMode( BackgroundMode mode ) { backgroundMode = mode; }

    QPalette pal() const;
    void setPal( const QPalette& );

    QColorGroup::ColorRole centralFromItem( int );
    QColorGroup::ColorRole effectFromItem( int );
    QPalette editPalette;
    QPalette previewPalette;

    FormWindow *formWindow;
    BackgroundMode backgroundMode;
};

#endif
