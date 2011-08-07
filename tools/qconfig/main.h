/**********************************************************************
** Copyright (C) 1999-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt/Embedded.
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

#include <qmainwindow.h>
#include <qmap.h>
#include <qstringlist.h>
#include <qtextbrowser.h>

class ChoiceItem;
class QListViewItem;
class QListView;
class QLabel;

class Info : public QTextBrowser {
    Q_OBJECT
public:
    Info( QWidget* parent, const char* name=0 );

signals:
    void idClicked(const QString& name);

public slots:
    void setSource(const QString& name);
};

class Main : public QMainWindow {
    Q_OBJECT
public:
    Main();
    void loadFeatures(const QString& filename);
    void loadConfig(const QString& filename);

private:
    void createItem(const QString& ch);
    QMap<QString,QString> label;
    QMap<QString,QString> documentation;
    QMap<QString,QStringList> links;
    QMap<QString,QStringList> dependencies;
    QMap<QString,QStringList> rdependencies;
    QMap<QString,QString> section;
    QMap<QString,ChoiceItem*> item;
    QMap<QString,QListViewItem*> sectionitem;
    QStringList choices;
    QListView* lv;
    QTextBrowser* info;

private slots:
    void updateAvailability(QListViewItem* i);
    void showInfo(QListViewItem* i);
    void selectId(const QString&);
    void open();
    void save();
    void testAll();
    void about();
    void aboutQt();
};
