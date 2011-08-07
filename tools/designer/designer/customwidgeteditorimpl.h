/**********************************************************************
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

#ifndef CUSTOMWIDGETEDITORIMPL_H
#define CUSTOMWIDGETEDITORIMPL_H

#include "customwidgeteditor.h"
#include "metadatabase.h"

#include <qmap.h>
#include <qobjectlist.h>

class QTimer;
class QListBoxItem;
class MainWindow;

class CustomWidgetEditor : public CustomWidgetEditorBase
{
    Q_OBJECT

public:
    CustomWidgetEditor( QWidget *parent , MainWindow *mw);

protected slots:
    void addWidgetClicked();
    void classNameChanged( const QString & );
    void currentWidgetChanged( QListBoxItem * );
    void deleteWidgetClicked();
    void headerFileChanged( const QString & );
    void heightChanged( int );
    void includePolicyChanged( int );
    void pixmapChoosen();
    void widthChanged( int );
    void chooseHeader();
    void checkWidgetName();
    void closeClicked();
    void currentSignalChanged( QListBoxItem *i );
    void addSignal();
    void removeSignal();
    void signalNameChanged( const QString &s );
    void slotAccessChanged( const QString & );
    void slotNameChanged( const QString & );
    void addSlot();
    void removeSlot();
    void currentSlotChanged( QListViewItem * );
    void propertyTypeChanged( const QString & );
    void propertyNameChanged( const QString & );
    void addProperty();
    void removeProperty();
    void currentPropertyChanged( QListViewItem * );
    void saveDescription();
    void loadDescription();
    void horDataChanged( int );
    void verDataChanged( int );
    void widgetIsContainer( bool );
    
private:
    MetaDataBase::CustomWidget *findWidget( QListBoxItem *i );
    void setupDefinition();
    void setupSignals();
    void setupSlots();
    void setupProperties();
    void updateCustomWidgetSizes();

private:
    QMap<QListBoxItem*, MetaDataBase::CustomWidget*> customWidgets;
    QString oldName;
    QTimer *checkTimer;
    QListBoxItem *oldItem;
    MainWindow *mainWindow;
    QObjectList cwLst;

};

#endif
