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

#ifndef TRWINDOW_H
#define TRWINDOW_H

#include "phrase.h"

#include <metatranslator.h>

#include <qmainwindow.h>
#include <qptrlist.h>
#include <qdict.h>
#include <qprinter.h>
#include <qstringlist.h>
#include <qpixmap.h>
#include <qlistview.h>

class QAction;
class QDialog;
class QLabel;
class QListView;
class QListViewItem;
class QPrinter;

class Action;
class FindDialog;
class MessageEditor;

class PhraseLV;
class ContextLVI;
class Statistics;

class TrWindow : public QMainWindow
{
    Q_OBJECT
public:
    static QPixmap * pxOn;
    static QPixmap * pxOff;
    static QPixmap * pxObsolete;
    static QPixmap * pxDanger;
    static const QPixmap pageCurl();

    TrWindow();
    ~TrWindow();

    void openFile( const QString& name );

protected:
    void readConfig();
    void writeConfig();
    void closeEvent( QCloseEvent * );

signals:
    void statsChanged( int w, int c, int cs, int w2, int c2, int cs2 );
    
private slots:
    void doneAndNext();
    void prev();
    void next();
    void recentFileActivated( int );
    void setupRecentFilesMenu();
    void open();
    void save();
    void saveAs();
    void release();
    void print();
    void find();
    void findAgain();
    void replace();
    void newPhraseBook();
    void openPhraseBook();
    void closePhraseBook( int id );
    void editPhraseBook( int id );
    void printPhraseBook( int id );
    void manual();
    void revertSorting();
    void about();
    void aboutQt();

    void setupPhrase();
    bool maybeSave();
    void updateCaption();
    void showNewScope( QListViewItem *item );
    void showNewCurrent( QListViewItem *item );
    void updateTranslation( const QString& translation );
    void updateFinished( bool finished );
    void toggleFinished( QListViewItem *item, const QPoint& p, int column );
    void prevUnfinished();
    void nextUnfinished();
    void findNext( const QString& text, int where, bool matchCase );
    void revalidate();
    void toggleGuessing();
    void focusSourceList();
    void focusPhraseList();
    void updateClosePhraseBook();
    void toggleStatistics();
    void updateStatistics();

private:
    typedef QPtrList<PhraseBook> PBL;
    typedef QDict<PhraseBook> PBD;

    static QString friendlyString( const QString& str );

    void addRecentlyOpenedFile( const QString & fn, QStringList & lst );
    void setupMenuBar();
    void setupToolBars();
    void setCurrentContextItem( QListViewItem *item );
    void setCurrentMessageItem( QListViewItem *item );
    QString friendlyPhraseBookName( int k );
    bool openPhraseBook( const QString& name );
    bool savePhraseBook( QString& name, const PhraseBook& pb );
    void updateProgress();
    void updatePhraseDict();
    PhraseBook getPhrases( const QString& source );
    bool danger( const QString& source, const QString& translation,
		 bool verbose = FALSE );

    int itemToIndex( QListView * view, QListViewItem * item );
    QListViewItem * indexToItem( QListView * view, int index );
    bool searchItem( const QString & searchWhat, QListViewItem * j,
		     QListViewItem * k );
    void countStats( QListViewItem * ci, QListViewItem * mi, int& trW, int& trC, int& trCS );
    void doCharCounting( const QString& text, int& trW, int& trC, int& trCS );
    
    QListView     * plv;
    QListView     * lv;
    QListView     * slv;
    MessageEditor * me;
    QLabel        * progress;
    QLabel        * modified;
    MetaTranslator tor;
    bool dirty;
    bool messageIsShown;
    int  numFinished;
    int  numNonobsolete;
    int  numMessages;
    int  dirtyItem;
    QStringList recentFiles;
    QString     filename;

    PBD phraseDict;
    PBL phraseBooks;
    QStringList phraseBookNames;

    QPrinter printer;

    FindDialog *f;
    FindDialog *h;
    QString findText;
    int findWhere;
    bool findMatchCase;
    int foundItem;
    QListViewItem *foundScope;
    int foundWhere;
    int foundOffset;

    QPopupMenu * phrasep;
    QPopupMenu * closePhraseBookp;
    QPopupMenu * editPhraseBookp;
    QPopupMenu * printPhraseBookp;
    QPopupMenu * recentFilesMenu;
    int closePhraseBookId;
    int editPhraseBookId;
    int printPhraseBookId;
    Action *openAct;
    Action *saveAct;
    Action *saveAsAct;
    Action *releaseAct;
    Action *printAct;
    Action *exitAct;
    Action *undoAct;
    Action *redoAct;
    Action *cutAct;
    Action *copyAct;
    Action *pasteAct;
    Action *selectAllAct;
    Action *findAct;
    Action *findAgainAct;
    Action *replaceAct;
    Action *newPhraseBookAct;
    Action *openPhraseBookAct;
    Action *acceleratorsAct;
    Action *endingPunctuationAct;
    Action *phraseMatchesAct;
    Action *revertSortingAct;
    Action *aboutAct;
    Action *aboutQtAct;
    Action *manualAct;
    Action *whatsThisAct;
    Action *beginFromSourceAct;
    Action *prevAct;
    Action *nextAct;
    Action *prevUnfinishedAct;
    Action *nextUnfinishedAct;
    Action *doneAndNextAct;
    QAction *doneAndNextAlt;
    Action *doGuessesAct;
    Action *toggleStats;
    Statistics * stats;
    int  srcWords;
    int  srcChars;
    int  srcCharsSpc;
};

#endif
