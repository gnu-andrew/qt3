/****************************************************************************
**
** Definition of QTabDialog class
**
** Created : 960825
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

#ifndef QTABDIALOG_H
#define QTABDIALOG_H

#ifndef QT_H
#include "qdialog.h"
#include "qiconset.h"
#endif // QT_H

#ifndef QT_NO_TABDIALOG

class  QTabBar;
class  QTab;
class  QTabDialogPrivate;

class Q_EXPORT QTabDialog : public QDialog
{
    Q_OBJECT
public:
    QTabDialog( QWidget* parent=0, const char* name=0, bool modal=FALSE,
		WFlags f=0 );
    ~QTabDialog();

    void show();
    void setFont( const QFont & font );

    void addTab( QWidget *, const QString &);
    void addTab( QWidget *child, const QIconSet& iconset, const QString &label);
    void addTab( QWidget *, QTab* );

    void insertTab( QWidget *, const QString &, int index = -1);
    void insertTab( QWidget *child, const QIconSet& iconset, const QString &label, int index = -1);
    void insertTab( QWidget *, QTab*, int index = -1 );

    void changeTab( QWidget *, const QString &);
    void changeTab( QWidget *child, const QIconSet& iconset, const QString &label);

    bool isTabEnabled(  QWidget * ) const;
    void setTabEnabled( QWidget *, bool );
    bool isTabEnabled( const char* ) const; // compatibility
    void setTabEnabled( const char*, bool ); // compatibility

    void showPage( QWidget * );
    void removePage( QWidget * );
    QString tabLabel( QWidget * );

    QWidget * currentPage() const;

    void setDefaultButton( const QString &text );
    void setDefaultButton();
    bool hasDefaultButton() const;

    void setHelpButton( const QString &text );
    void setHelpButton();
    bool hasHelpButton() const;

    void setCancelButton( const QString &text );
    void setCancelButton();
    bool hasCancelButton() const;

    void setApplyButton( const QString &text );
    void setApplyButton();
    bool hasApplyButton() const;

#ifndef Q_QDOC
    void setOKButton( const QString &text = QString::null );
#endif
    void setOkButton( const QString &text );
    void setOkButton();
    bool hasOkButton() const;

protected:
    void paintEvent( QPaintEvent * );
    void resizeEvent( QResizeEvent * );
    void styleChange( QStyle& );
    void setTabBar( QTabBar* );
    QTabBar* tabBar() const;

signals:
    void aboutToShow();

    void applyButtonPressed();
    void cancelButtonPressed();
    void defaultButtonPressed();
    void helpButtonPressed();

    void currentChanged( QWidget * );
    void selected( const QString& ); // obsolete

private slots:
    void showTab( int i );

private:
    void setSizes();
    void setUpLayout();

    QTabDialogPrivate *d;
private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QTabDialog( const QTabDialog & );
    QTabDialog& operator=( const QTabDialog & );
#endif
};

#endif // QT_NO_TABDIALOG

#endif // QTABDIALOG_H
