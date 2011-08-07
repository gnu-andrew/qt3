/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Assistant.
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

#include "topicchooserimpl.h"

#include <qlabel.h>
#include <qlistbox.h>
#include <qpushbutton.h>

TopicChooser::TopicChooser( QWidget *parent, const QStringList &lnkNames,
			    const QStringList &lnks, const QString &title )
    : TopicChooserBase( parent, 0, TRUE ), links( lnks ), linkNames( lnkNames )
{
    label->setText( tr( "Choose a topic for <b>%1</b>" ).arg( title ) );
    listbox->insertStringList( linkNames );
    listbox->setCurrentItem( listbox->firstItem() );
    listbox->setFocus();
}

QString TopicChooser::link() const
{
    if ( listbox->currentItem() == -1 )
	return QString::null;
    QString s = listbox->currentText();
    if ( s.isEmpty() )
	return s;
    int i = linkNames.findIndex( s );
    return links[ i ];
}

QString TopicChooser::getLink( QWidget *parent, const QStringList &lnkNames,
				      const QStringList &lnks, const QString &title )
{
    TopicChooser *dlg = new TopicChooser( parent, lnkNames, lnks, title );
    QString lnk;
    if ( dlg->exec() == QDialog::Accepted )
	lnk = dlg->link();
    delete dlg;
    return lnk;
}
