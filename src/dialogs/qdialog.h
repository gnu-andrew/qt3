/****************************************************************************
**
** Definition of QDialog class
**
** Created : 950502
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

#ifndef QDIALOG_H
#define QDIALOG_H

#ifndef QT_H
#include "qwidget.h"
#endif // QT_H
#ifndef QT_NO_DIALOG
#if 0
Q_OBJECT
#endif

class QPushButton;
class QDialogPrivate;

class Q_EXPORT QDialog : public QWidget
{
friend class QPushButton;
    Q_OBJECT
    Q_PROPERTY( bool sizeGripEnabled READ isSizeGripEnabled WRITE setSizeGripEnabled )
    Q_PROPERTY( bool modal READ isModal WRITE setModal )

public:
    Q_EXPLICIT QDialog( QWidget* parent=0, const char* name=0, bool modal=FALSE,
	     WFlags f=0 );
    ~QDialog();

    enum DialogCode { Rejected, Accepted };

    int		result() const { return rescode; }

    void	show();
    void	hide();
    void	move( int x, int y );
    void	move( const QPoint &p );
    void	resize( int w, int h );
    void	resize( const QSize & );
    void	setGeometry( int x, int y, int w, int h );
    void	setGeometry( const QRect & );

    void	setOrientation( Orientation orientation );
    Orientation	orientation() const;

    void	setExtension( QWidget* extension );
    QWidget*	extension() const;

    QSize	sizeHint() const;
    QSize	minimumSizeHint() const;

    void setSizeGripEnabled( bool );
    bool isSizeGripEnabled() const;

    void setModal( bool modal );
    bool isModal() const;
#ifdef Q_OS_TEMP
    bool	event( QEvent * );
#endif

public slots:
    int exec();

protected slots:
    virtual void done( int );
    virtual void accept();
    virtual void reject();

    void	showExtension( bool );

protected:
    void	setResult( int r )	{ rescode = r; }
    void	keyPressEvent( QKeyEvent * );
    void	closeEvent( QCloseEvent * );
    void	resizeEvent( QResizeEvent * );
    void	contextMenuEvent( QContextMenuEvent * );
    bool	eventFilter( QObject *, QEvent * );
    void	adjustPosition( QWidget*);

private:
    void	setDefault( QPushButton * );
    void	setMainDefault( QPushButton * );
    void	hideDefault();
#ifdef Q_OS_TEMP
    void	hideSpecial();
#endif

    int		rescode;
    uint	did_move   : 1;
    uint	has_relpos : 1;
    uint	did_resize : 1;
    uint	in_loop: 1;
    void adjustPositionInternal( QWidget*, bool useRelPos = FALSE );
    QDialogPrivate* d;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QDialog( const QDialog & );
    QDialog &operator=( const QDialog & );
#endif
};

#endif // QT_NO_DIALOG
#endif // QDIALOG_H
