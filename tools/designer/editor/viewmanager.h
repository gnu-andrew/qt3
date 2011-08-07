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

#ifndef VIEWMANAGER_H
#define VIEWMANAGER_H

#include <qwidget.h>
#include <qvaluelist.h>

class QChildEvent;
class MarkerWidget;
class QVBoxLayout;
class QDockArea;
class QTextParagraph;
class QLabel;
class QTimer;

class ViewManager : public QWidget
{
    Q_OBJECT

public:
    ViewManager( QWidget *parent, const char *name );

    void addView( QWidget *view );
    QWidget *currentView() const;
    void showMarkerWidget( bool );

    void setError( int line );
    void setStep( int line );
    void setStackFrame( int line );
    void clearStep();
    void clearStackFrame();
    void setBreakPoints( const QValueList<uint> &l );
    QValueList<uint> breakPoints() const;

    void emitMarkersChanged();
    MarkerWidget *marker_widget() const { return markerWidget; }

signals:
    void markersChanged();
    void expandFunction( QTextParagraph *p );
    void collapseFunction( QTextParagraph *p );
    void collapse( bool all /*else only functions*/ );
    void expand( bool all /*else only functions*/ );
    void editBreakPoints();
    void isBreakpointPossible( bool &possible, const QString &code, int line );

protected slots:
    void clearErrorMarker();
    void cursorPositionChanged( int row, int col );
    void showMessage( const QString &msg );
    void clearStatusBar();

protected:
    void childEvent( QChildEvent *e );
    void resizeEvent( QResizeEvent *e );

private:
    QWidget *curView;
    MarkerWidget *markerWidget;
    QVBoxLayout *layout;
    QDockArea *dockArea;
    QLabel *posLabel;
    QString extraText;
    QTimer *messageTimer;

};

#endif
