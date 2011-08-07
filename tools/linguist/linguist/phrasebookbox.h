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

#ifndef PHRASEBOOKBOX_H
#define PHRASEBOOKBOX_H

#include "phrase.h"

#include <qdialog.h>
#include <qvaluelist.h>

class QLineEdit;
class QPushButton;
class QListViewItem;

class PhraseLV;

class PhraseBookBox : public QDialog
{
    Q_OBJECT
public:
    PhraseBookBox( const QString& filename, const PhraseBook& phraseBook,
		   QWidget *parent = 0, const char *name = 0,
		   bool modal = FALSE );

    const PhraseBook& phraseBook() const { return pb; }

protected:
    virtual void keyPressEvent( QKeyEvent *ev );

private slots:
    void newPhrase();
    void removePhrase();
    void save();
    void sourceChanged( const QString& source );
    void targetChanged( const QString& target );
    void definitionChanged( const QString& definition );
    void selectionChanged( QListViewItem *item );

private:
    void selectItem( QListViewItem *item );
    void enableDisable();

    QLineEdit *sourceLed;
    QLineEdit *targetLed;
    QLineEdit *definitionLed;
    QPushButton *newBut;
    QPushButton *removeBut;
    PhraseLV *lv;
    QString fn;
    PhraseBook pb;
};

#endif
