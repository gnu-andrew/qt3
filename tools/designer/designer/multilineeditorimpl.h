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

#ifndef MULTILINEEDITORIMPL_H
#define MULTILINEEDITORIMPL_H

#include <qaction.h>
#include <qtextedit.h>
#include <private/qrichtext_p.h>
#include "multilineeditor.h"

class FormWindow;
class QToolBar;
class QTextDocument;

class TextEdit : public QTextEdit
{
    Q_OBJECT

public:
    TextEdit( QWidget *parent = 0, const char *name = 0 );
    QTextDocument *document() const { return QTextEdit::document(); }
    QTextParagraph *paragraph();

};

class ToolBarItem : public QAction
{
    Q_OBJECT

public:
    ToolBarItem( QWidget *parent, QWidget *toolBar,
                 const QString &label, const QString &tagstr,
		 const QIconSet &icon, const QKeySequence &key = 0 );
    ~ToolBarItem();
signals:
    void clicked( const QString &t );

protected slots:
    void wasActivated();
private:
    QString tag;
};


class MultiLineEditor : public MultiLineEditorBase
{
    Q_OBJECT

public:
    MultiLineEditor( bool call_static, bool richtextMode, QWidget *parent, QWidget *editWidget,
		     FormWindow *fw, const QString &text = QString::null );
    bool useWrapping() const;
    void setUseWrapping( bool );
    static QString getText( QWidget *parent, const QString &text, bool richtextMode, bool *useWrap );
    int exec();

protected slots:
    void okClicked();
    void applyClicked();
    void cancelClicked();
    void insertTags( const QString& );
    void insertBR();
    void showFontDialog();
    void changeWrapMode( bool );

    void closeEvent( QCloseEvent *e );

private:
    QString getStaticText();
    TextEdit *textEdit;
    QTextEdit *mlined;
    FormWindow *formwindow;
    QToolBar *basicToolBar;
    QToolBar *fontToolBar;
    QToolBar *optionsToolBar;
    QAction *wrapAction;
    int res;
    QString staticText;
    bool callStatic;
    bool oldDoWrap, doWrap;
    QVariant oldWrapMode;
    QString oldWrapString;
};

#endif
