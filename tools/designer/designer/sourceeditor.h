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

#ifndef SOURCEEDITOR_H
#define SOURCEEDITOR_H

#include "../interfaces/editorinterface.h"
#include <qvbox.h>
#include <qguardedptr.h>

class FormWindow;
class QCloseEvent;
struct LanguageInterface;
class Project;
class SourceFile;

class SourceEditor : public QVBox
{
    Q_OBJECT

public:
    SourceEditor( QWidget *parent, EditorInterface *iface, LanguageInterface *liface );
    ~SourceEditor();

    void setObject( QObject *fw, Project *p );
    QObject *object() const { return obj; }
    Project *project() const { return pro; }
    void setFunction( const QString &func, const QString &clss = QString::null );
    void setClass( const QString &clss );
    void save();
    bool saveAs();
    void setModified( bool b );
    bool isModified() const;

    static QString sourceOfObject( QObject *fw, const QString &lang, EditorInterface *iface, LanguageInterface *lIface );

    QString language() const;
    void setLanguage( const QString &l );

    void editCut();
    void editCopy();
    void editPaste();
    bool editIsUndoAvailable();
    bool editIsRedoAvailable();
    void editUndo();
    void editRedo();
    void editSelectAll();

    void configChanged();
    void refresh( bool allowSave );
    void resetContext();

    EditorInterface *editorInterface() const { return iFace; }

    void setFocus();
    int numLines() const;
    void saveBreakPoints();
    void clearStep();
    void clearStackFrame();
    void resetBreakPoints();

    QString text() const;

    void checkTimeStamp();

    SourceFile *sourceFile() const;
    FormWindow *formWindow() const;

protected:
    void closeEvent( QCloseEvent *e );


private:
    EditorInterface *iFace;
    LanguageInterface *lIface;
    QGuardedPtr<QObject> obj;
    Project *pro;
    QString lang;
    QGuardedPtr<QWidget> editor;

};

#endif
