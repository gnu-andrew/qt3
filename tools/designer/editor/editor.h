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

#ifndef EDITOR_H
#define EDITOR_H

#include <qtextedit.h>

struct Config;
class ParenMatcher;
class EditorCompletion;
class EditorBrowser;
class QAccel;

class Editor : public QTextEdit
{
    Q_OBJECT

public:
    enum Selection {
	Error = 3,
	Step = 4
    };

    Editor( const QString &fn, QWidget *parent, const char *name );
    ~Editor();
    virtual void load( const QString &fn );
    virtual void save( const QString &fn );
    QTextDocument *document() const { return QTextEdit::document(); }
    void placeCursor( const QPoint &p, QTextCursor *c ) { QTextEdit::placeCursor( p, c ); }
    void setDocument( QTextDocument *doc ) { QTextEdit::setDocument( doc ); }
    QTextCursor *textCursor() const { return QTextEdit::textCursor(); }
    void repaintChanged() { QTextEdit::repaintChanged(); }

    virtual EditorCompletion *completionManager() { return 0; }
    virtual EditorBrowser *browserManager() { return 0; }
    virtual void configChanged();

    Config *config() { return cfg; }

    void setErrorSelection( int line );
    void setStepSelection( int line );
    void clearStepSelection();
    void clearSelections();

    virtual bool supportsErrors() const { return TRUE; }
    virtual bool supportsBreakPoints() const { return TRUE; }
    virtual void makeFunctionVisible( QTextParagraph * ) {}

    void drawCursor( bool b ) { QTextEdit::drawCursor( b ); }

    QPopupMenu *createPopupMenu( const QPoint &p );
    bool eventFilter( QObject *o, QEvent *e );

    void setEditable( bool b ) { editable = b; }

protected:
    void doKeyboardAction( KeyboardAction action );
    void keyPressEvent( QKeyEvent *e );

signals:
    void clearErrorMarker();
    void intervalChanged();

private slots:
    void cursorPosChanged( QTextCursor *c );
    void doChangeInterval();
    void commentSelection();
    void uncommentSelection();

protected:
    ParenMatcher *parenMatcher;
    QString filename;
    Config *cfg;
    bool hasError;
    QAccel *accelComment, *accelUncomment;
    bool editable;

};

#endif
