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

#ifndef OUTPUTWINDOW_H
#define OUTPUTWINDOW_H

#include <qtabwidget.h>
#include <qstringlist.h>
#include <qvaluelist.h>
#include <qlistview.h>

struct DesignerOutputDock;
class QTextEdit;
class QListView;

class ErrorItem : public QListViewItem
{
public:
    enum Type { Error, Warning };

    ErrorItem( QListView *parent, QListViewItem *after, const QString &message, int line,
	       const QString &locationString, QObject *locationObject );

    void paintCell( QPainter *, const QColorGroup & cg,
		    int column, int width, int alignment );

    void setRead( bool b ) { read = b; repaint(); }

    QObject *location() const { return object; }
    int line() const { return text( 2 ).toInt(); }

private:
    QObject *object;
    Type type;
    bool read;

};

class OutputWindow : public QTabWidget
{
    Q_OBJECT

public:
    OutputWindow( QWidget *parent );
    ~OutputWindow();

    void setErrorMessages( const QStringList &errors, const QValueList<uint> &lines,
			   bool clear, const QStringList &locations,
			   const QObjectList &locationObjects );
    void appendDebug( const QString& );
    void clearErrorMessages();
    void clearDebug();
    void showDebugTab();

    DesignerOutputDock *iFace();

    void shuttingDown();

    static QtMsgHandler oldMsgHandler;

private slots:
    void currentErrorChanged( QListViewItem *i );

private:
    void setupError();
    void setupDebug();

    QTextEdit *debugView;
    QListView *errorView;

    DesignerOutputDock *iface;

};

#endif
