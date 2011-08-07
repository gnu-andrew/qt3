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

#ifndef HELPDIALOGIMPL_H
#define HELPDIALOGIMPL_H

#include "helpdialog.h"
#include "index.h"
#include "helpwindow.h"
#include "docuparser.h"

#include <qlistbox.h>
#include <qlistview.h>
#include <qmap.h>
#include <qstringlist.h>
#include <qvalidator.h>
#include <qpopupmenu.h>
#include <qdict.h>
#include <qpopupmenu.h>

class QProgressBar;
class MainWindow;
class QTextBrowser;

class HelpNavigationListItem : public QListBoxText
{
public:
    HelpNavigationListItem( QListBox *ls, const QString &txt );

    void addLink( const QString &link );
    QStringList links() const { return linkList; }
private:
    QStringList linkList;

};

class SearchValidator : public QValidator
{
    Q_OBJECT
public:
    SearchValidator( QObject *parent, const char *name = 0 )
	: QValidator( parent, name ) {}
    ~SearchValidator() {}
    QValidator::State validate( QString &str, int & ) const;
};

class HelpNavigationContentsItem : public QListViewItem
{
public:
    HelpNavigationContentsItem( QListView *v, QListViewItem *after );
    HelpNavigationContentsItem( QListViewItem *v, QListViewItem *after );

    void setLink( const QString &lnk );
    QString link() const;

private:
    QString theLink;
};

class HelpDialog : public HelpDialogBase
{
    Q_OBJECT

public:
    HelpDialog( QWidget *parent, MainWindow *h );

    QString titleOfLink( const QString &link );
    bool eventFilter( QObject *, QEvent * );
    bool lastWindowClosed() { return lwClosed; }

    void timerEvent(QTimerEvent *e);

protected slots:
    void showTopic( int, QListBoxItem *, const QPoint & );
    void showTopic( int, QListViewItem *, const QPoint & );
    void showTopic( QListViewItem * );
    void loadIndexFile();
    void insertContents();
    void setupFullTextIndex();
    void currentTabChanged( const QString &s );
    void currentIndexChanged( QListBoxItem *i );
    void showTopic();
    void searchInIndex( const QString &s );
    void addBookmark();
    void removeBookmark();
    void currentBookmarkChanged( QListViewItem *i );
    void currentContentsChanged( QListViewItem *i );
    void startSearch();
    void showSearchHelp();

public slots:
    void initialize();
    void toggleContents();
    void toggleIndex();
    void toggleBookmarks();
    void toggleSearch();


signals:
    void showLink( const QString &s );
    void showSearchLink( const QString &s, const QStringList &terms );

private slots:
    void lastWinClosed();
    void showResultPage( int button, QListBoxItem *i, const QPoint &p );
    void showResultPage( QListBoxItem *i );
    void setIndexingProgress( int prog );
    void showItemMenu( QListBoxItem *item, const QPoint &pos );
    void showItemMenu( QListViewItem *item, const QPoint &pos );
    void insertBookmarks();
    void processEvents();

private:
    typedef QValueList<ContentItem> ContentList;
    void removeOldCacheFiles();
    void buildKeywordDB();
    Q_UINT32 getFileAges();
    void showIndexTopic();
    void showBookmarkTopic();
    void setupTitleMap();
    void saveBookmarks();
    void showContentsTopic();
    void showInitDoneMessage();
    void buildContentDict();

    QMap<QString, QString> titleMap;
    bool indexDone, bookmarksInserted, titleMapDone, contentsInserted;
    bool lwClosed;
    MainWindow *help;
    QString documentationPath;
    Index *fullTextIndex;
    QStringList terms, foundDocs;
    bool initDoneMsgShown;
    void getAllContents();
    QDict<ContentList> contentList;
    QPopupMenu *itemPopup;
    QString cacheFilesPath;
    QStringList keywordDocuments;
};

#endif
