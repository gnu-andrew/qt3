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

#ifndef WORKSPACE_H
#define WORKSPACE_H

#include <qlistview.h>

class FormWindow;
class QResizeEvent;
class QCloseEvent;
class QDropEvent;
class QDragMoveEvent;
class QDragEnterEvent;
class MainWindow;
class Project;
class SourceFile;
class FormFile;
class QCompletionEdit;
class SourceEditor;

class WorkspaceItem : public QListViewItem
{
public:
    enum Type { ProjectType, FormFileType, FormSourceType, SourceFileType, ObjectType };

    WorkspaceItem( QListView *parent, Project* p );
    WorkspaceItem( QListViewItem *parent, SourceFile* sf );
    WorkspaceItem( QListViewItem *parent, FormFile* ff, Type t = FormFileType );
    WorkspaceItem( QListViewItem *parent, QObject *o, Project *p );

    void paintCell( QPainter *p, const QColorGroup &cg, int column, int width, int align );

    Type type() const { return t; }

    bool isModified() const;

    QString text( int ) const;

    void fillCompletionList( QStringList& completion );
    bool checkCompletion( const QString& completion );

    QString key( int, bool ) const; // column sorting key

    Project* project;
    SourceFile* sourceFile;
    FormFile* formFile;
    QObject *object;

    void setOpen( bool );

    void setAutoOpen( bool );
    bool isAutoOpen() const { return isOpen() && autoOpen; }

    bool useOddColor;

private:
    void init();
    bool autoOpen;
    QColor backgroundColor();
    Type t;
};

class Workspace : public QListView
{
    Q_OBJECT

public:
    Workspace( QWidget *parent , MainWindow *mw );

    void setCurrentProject( Project *pro );

    void contentsDropEvent( QDropEvent *e );
    void contentsDragEnterEvent( QDragEnterEvent *e );
    void contentsDragMoveEvent( QDragMoveEvent *e );

    void setBufferEdit( QCompletionEdit *edit );

public slots:

    void update();
    void update( FormFile* );

    void activeFormChanged( FormWindow *fw );
    void activeEditorChanged( SourceEditor *se );

protected:
    void closeEvent( QCloseEvent *e );
    bool eventFilter( QObject *, QEvent * );


private slots:
    void itemClicked( int, QListViewItem *i, const QPoint& pos  );
    void itemDoubleClicked( QListViewItem *i );
    void rmbClicked( QListViewItem *i, const QPoint& pos  );
    void bufferChosen( const QString &buffer );

    void projectDestroyed( QObject* );

    void sourceFileAdded( SourceFile* );
    void sourceFileRemoved( SourceFile* );

    void formFileAdded( FormFile* );
    void formFileRemoved( FormFile* );

    void objectAdded( QObject* );
    void objectRemoved( QObject * );

private:
    WorkspaceItem *findItem( FormFile *ff );
    WorkspaceItem *findItem( SourceFile *sf );
    WorkspaceItem *findItem( QObject *o );

    void closeAutoOpenItems();

private:
    MainWindow *mainWindow;
    Project *project;
    WorkspaceItem *projectItem;
    QCompletionEdit *bufferEdit;
    bool blockNewForms;
    void updateBufferEdit();
    bool completionDirty;
    void updateColors();

};

#endif
