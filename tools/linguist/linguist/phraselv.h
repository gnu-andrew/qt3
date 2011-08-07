/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Linguist.
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

#ifndef PHRASELV_H
#define PHRASELV_H

#include "phrase.h"

#include <qlistview.h>

class QWhatsThis;

class PhraseLV;

#define NewPhrase PhraseLV::tr( "(New Phrase)" )

class PhraseLVI : public QListViewItem
{
public:
    enum { SourceTextShown, TargetTextShown, DefinitionText, SourceTextOriginal,
           TargetTextOriginal };

    PhraseLVI( PhraseLV *parent, const Phrase& phrase, int accelKey = 0 );

    virtual void setText( int column, const QString& text );
    virtual QString key( int column, bool ascending ) const;

    void setPhrase( const Phrase& phrase );
    Phrase phrase() const;
    int accelKey() const { return akey; }

private:
    QString makeKey( const QString& text ) const;

    int akey;
    QString sourceTextKey;
    QString targetTextKey;
};

class PhraseLV : public QListView
{
    Q_OBJECT
public:
    PhraseLV( QWidget *parent, const char *name );
    ~PhraseLV();

    virtual QSize sizeHint() const;

private:
    QWhatsThis *what;
};

#endif
