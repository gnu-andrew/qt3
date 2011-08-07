/**********************************************************************
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
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

#ifndef CPPEDITOR_H
#define CPPEDITOR_H

#include <editor.h>

class EditorCompletion;
class EditorBrowser;
struct DesignerInterface;
class CIndent;

class  CppEditor : public Editor
{
    Q_OBJECT

public:
    CppEditor( const QString &fn, QWidget *parent, const char *name, DesignerInterface *i );
    ~CppEditor();

    virtual EditorCompletion *completionManager() { return completion; }
    virtual EditorBrowser *browserManager() { return browser; }
    void configChanged();

    bool supportsBreakPoints() const { return FALSE; }
#if defined(Q_USING)
    using QTextEdit::createPopupMenu;
#endif
    QPopupMenu *createPopupMenu( const QPoint &p );

    void paste();

private slots:
    void addInclDecl();
    void addInclImpl();
    void addForward();

protected:
    EditorCompletion *completion;
    EditorBrowser *browser;
    DesignerInterface *dIface;
    CIndent *indent;

};

#endif
