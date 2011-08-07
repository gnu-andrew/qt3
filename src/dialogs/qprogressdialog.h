/****************************************************************************
**
** Definition of QProgressDialog class
**
** Created : 970520
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

#ifndef QPROGRESSDIALOG_H
#define QPROGRESSDIALOG_H

#ifndef QT_H
#include "qsemimodal.h"
#include "qlabel.h"       // ### remove or keep for users' convenience?
#include "qprogressbar.h" // ### remove or keep for users' convenience?
#endif // QT_H

#ifndef QT_NO_PROGRESSDIALOG

class QPushButton;
class QTimer;
class QProgressDialogData;

class Q_EXPORT QProgressDialog : public QDialog
{
    Q_OBJECT
    Q_PROPERTY( bool wasCancelled READ wasCancelled DESIGNABLE false STORED false ) // ### remove in 4.0
    Q_PROPERTY( bool wasCanceled READ wasCanceled )
    Q_PROPERTY( int totalSteps READ totalSteps WRITE setTotalSteps )
    Q_PROPERTY( int progress READ progress WRITE setProgress )
    Q_PROPERTY( bool autoReset READ autoReset WRITE setAutoReset )
    Q_PROPERTY( bool autoClose READ autoClose WRITE setAutoClose )
    Q_PROPERTY( int minimumDuration READ minimumDuration WRITE setMinimumDuration )
    Q_PROPERTY( QString labelText READ labelText WRITE setLabelText )

public:
    QProgressDialog( QWidget* parent=0, const char* name=0, bool modal=FALSE,
		     WFlags f=0 );
    QProgressDialog( const QString& labelText, const QString &cancelButtonText,
		     int totalSteps, QWidget* parent=0, const char* name=0,
		     bool modal=FALSE, WFlags f=0 );
    ~QProgressDialog();

    void	setLabel( QLabel * );
    void	setCancelButton( QPushButton * );
    void	setBar( QProgressBar * );

    // ### Qt 4.0: remove wasCancelled() in 4.0
    bool	wasCancelled() const;
    inline bool	wasCanceled() const { return wasCancelled(); }

    int		totalSteps() const;
    int		progress()   const;

    QSize	sizeHint() const;

    QString     labelText() const;

    void setAutoReset( bool b );
    bool autoReset() const;
    void setAutoClose( bool b );
    bool autoClose() const;

public slots:
    void	cancel();
    void	reset();
    void	setTotalSteps( int totalSteps );
    void	setProgress( int progress );
    void	setProgress( int progress, int totalSteps );
    void	setLabelText( const QString &);
    void	setCancelButtonText( const QString &);

    void	setMinimumDuration( int ms );
public:
    int		minimumDuration() const;

signals:
    // ### remove cancelled() in 4.0
    void	cancelled();
    void	canceled();

protected:
    void	resizeEvent( QResizeEvent * );
    void	closeEvent( QCloseEvent * );
    void	styleChange( QStyle& );
    void	showEvent( QShowEvent *e );

protected slots:
    void	forceShow();

private:
    void	   init( QWidget *creator, const QString& lbl, const QString &canc,
			 int totstps);
    void	   layout();
    QLabel	  *label()  const;
    QProgressBar  *bar()    const;
    QProgressDialogData *d;
    QTimer	  *forceTimer;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QProgressDialog( const QProgressDialog & );
    QProgressDialog &operator=( const QProgressDialog & );
#endif
};

#endif // QT_NO_PROGRESSDIALOG

#endif // QPROGRESSDIALOG_H
