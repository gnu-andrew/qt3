/****************************************************************************
**
** Definition of print dialog.
**
** Created : 950829
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the dialogs module of the Qt GUI Toolkit.
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
** This file may be used under the terms of the Q Public License as
** defined by Trolltech ASA and appearing in the file LICENSE.QPL
** included in the packaging of this file.  Licensees holding valid Qt
** Commercial licenses may use this file in accordance with the Qt
** Commercial License Agreement provided with the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#ifndef QPRINTDIALOG_H
#define QPRINTDIALOG_H

#ifndef QT_H
#include "qdialog.h"
#endif // QT_H

#ifndef QT_NO_PRINTDIALOG

class QGroupBox;
class QPrintDialogPrivate;
class QListView;

class Q_EXPORT QPrintDialog : public QDialog
{
    Q_OBJECT
public:
    QPrintDialog( QPrinter *, QWidget* parent=0, const char* name=0 );
    ~QPrintDialog();

    static bool getPrinterSetup( QPrinter *, QWidget* = 0 );
    static void setGlobalPrintDialog( QPrintDialog * );

    void setPrinter( QPrinter *, bool = FALSE );
    QPrinter * printer() const;

    void addButton( QPushButton *but );

private slots:
    void browseClicked();
    void okClicked();

    void printerOrFileSelected( int );
    void landscapeSelected( int );
    void paperSizeSelected( int );
    void orientSelected( int );
    void pageOrderSelected( int );
    void colorModeSelected( int );
    void setNumCopies( int );
    void printRangeSelected( int );
    void setFirstPage( int );
    void setLastPage( int );

    void fileNameEditChanged( const QString &text );

private:
    QPrintDialogPrivate *d;

    QGroupBox * setupDestination();
    QGroupBox * setupOptions();
    QGroupBox * setupPaper();
    QGroupBox * setupPrinterSettings();

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QPrintDialog( const QPrintDialog & );
    QPrintDialog &operator=( const QPrintDialog & );
#endif
};

#endif

#endif // QPRINTDIALOG_H
