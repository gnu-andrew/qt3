/**********************************************************************
**
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

#ifndef EDITORINTERFACEIMPL_H
#define EDITORINTERFACEIMPL_H

#include <editorinterface.h>
#include <qobject.h>
#include <qguardedptr.h>

class QTimer;
class ViewManager;
struct DesignerInterface;

class EditorInterfaceImpl : public QObject, public EditorInterface
{
    Q_OBJECT

public:
    EditorInterfaceImpl();
    virtual ~EditorInterfaceImpl();

    QRESULT queryInterface( const QUuid&, QUnknownInterface** );
    Q_REFCOUNT;

    QWidget *editor( bool readonly, QWidget *parent, QUnknownInterface *designerIface );

    void setText( const QString &txt );
    QString text() const;
    bool isUndoAvailable() const;
    bool isRedoAvailable() const;
    void undo();
    void redo();
    void cut();
    void copy();
    void paste();
    void selectAll();
    bool find( const QString &expr, bool cs, bool wo, bool forward, bool startAtCursor );
    bool replace( const QString &find, const QString &replace, bool cs, bool wo, bool forward, bool startAtCursor, bool replaceAll );
    void gotoLine( int line );
    void indent();
    void scrollTo( const QString &txt, const QString &first );
    void splitView();
    void setContext( QObject *this_ );
    void readSettings();

    void setError( int line );
    void setStep( int ) {}
    void clearStep() {}
    void setModified( bool m );
    bool isModified() const;
    void setMode( Mode ) {}

    int numLines() const;
    void breakPoints( QValueList<uint> & ) const {}
    void setBreakPoints( const QValueList<uint> & ) {}
    void onBreakPointChange( QObject *receiver, const char *slot );
    void clearStackFrame() {}
    void setStackFrame( int ) {}

protected:
    bool eventFilter( QObject*, QEvent* );

private slots:
    void modificationChanged( bool m );
    void intervalChanged();
    void update();

private:
    QGuardedPtr<ViewManager> viewManager;
    DesignerInterface *dIface;
    QTimer *updateTimer;

};

#endif
