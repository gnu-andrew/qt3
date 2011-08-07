/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Configuration.
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

#ifndef PALETTEEDITORADVANCED_H
#define PALETTEEDITORADVANCED_H

#include "paletteeditoradvancedbase.h"

class PaletteEditorAdvanced : public PaletteEditorAdvancedBase
{
    Q_OBJECT
public:
    PaletteEditorAdvanced( QWidget * parent=0, const char * name=0,
			   bool modal=FALSE, WFlags f=0 );
    ~PaletteEditorAdvanced();

    static QPalette getPalette( bool *ok, const QPalette &pal, BackgroundMode mode = PaletteBackground,
				QWidget* parent = 0, const char* name = 0 );

protected slots:
    void paletteSelected(int);

    void onCentral( int );
    void onEffect( int );

    void onChooseCentralColor();
    void onChooseEffectColor();

    void onToggleBuildEffects( bool );
    void onToggleBuildInactive( bool );
    void onToggleBuildDisabled( bool );

protected:
    void mapToActiveCentralRole( const QColor& );
    void mapToActiveEffectRole( const QColor& );
    void mapToActivePixmapRole( const QPixmap& );
    void mapToInactiveCentralRole( const QColor& );
    void mapToInactiveEffectRole( const QColor& );
    void mapToInactivePixmapRole( const QPixmap& );
    void mapToDisabledCentralRole( const QColor& );
    void mapToDisabledEffectRole( const QColor& );
    void mapToDisabledPixmapRole( const QPixmap& );


    void buildPalette();
    void buildActiveEffect();
    void buildInactive();
    void buildInactiveEffect();
    void buildDisabled();
    void buildDisabledEffect();

private:
    void setPreviewPalette( const QPalette& );
    void updateColorButtons();
    void setupBackgroundMode( BackgroundMode );

    QPalette pal() const;
    void setPal( const QPalette& );

    QColorGroup::ColorRole centralFromItem( int );
    QColorGroup::ColorRole effectFromItem( int );
    QPalette editPalette;
    QPalette previewPalette;

    int selectedPalette;
};

#endif
