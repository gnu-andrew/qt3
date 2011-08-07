/****************************************************************************
**
** Definition of QMessageBox class
**
** Created : 950503
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

#ifndef QMESSAGEBOX_H
#define QMESSAGEBOX_H

#ifndef QT_H
#include "qdialog.h"
#endif // QT_H

#ifndef QT_NO_MESSAGEBOX

class  QLabel;
class  QPushButton;
struct QMessageBoxData;

class Q_EXPORT QMessageBox : public QDialog
{
    Q_OBJECT
    Q_ENUMS( Icon )
    Q_PROPERTY( QString text READ text WRITE setText )
    Q_PROPERTY( Icon icon READ icon WRITE setIcon )
    Q_PROPERTY( QPixmap iconPixmap READ iconPixmap WRITE setIconPixmap )
    Q_PROPERTY( TextFormat textFormat READ textFormat WRITE setTextFormat )

public:
    enum Icon { NoIcon = 0, Information = 1, Warning = 2, Critical = 3,
		Question = 4 };

    QMessageBox( QWidget* parent=0, const char* name=0 );
    QMessageBox( const QString& caption, const QString &text, Icon icon,
		 int button0, int button1, int button2,
		 QWidget* parent=0, const char* name=0, bool modal=TRUE,
		 WFlags f=WStyle_DialogBorder  );
    ~QMessageBox();

    enum { NoButton = 0, Ok = 1, Cancel = 2, Yes = 3, No = 4, Abort = 5,
	   Retry = 6, Ignore = 7, YesAll = 8, NoAll = 9, ButtonMask = 0xff,
	   Default = 0x100, Escape = 0x200, FlagMask = 0x300 };

    static int information( QWidget *parent, const QString &caption,
			    const QString& text,
			    int button0, int button1=0, int button2=0 );
    static int information( QWidget *parent, const QString &caption,
			    const QString& text,
			    const QString& button0Text = QString::null,
			    const QString& button1Text = QString::null,
			    const QString& button2Text = QString::null,
			    int defaultButtonNumber = 0,
			    int escapeButtonNumber = -1 );

    static int question( QWidget *parent, const QString &caption,
			 const QString& text,
			 int button0, int button1=0, int button2=0 );
    static int question( QWidget *parent, const QString &caption,
			 const QString& text,
			 const QString& button0Text = QString::null,
			 const QString& button1Text = QString::null,
			 const QString& button2Text = QString::null,
			 int defaultButtonNumber = 0,
			 int escapeButtonNumber = -1 );

    static int warning( QWidget *parent, const QString &caption,
			const QString& text,
			int button0, int button1, int button2=0 );
    static int warning( QWidget *parent, const QString &caption,
			const QString& text,
			const QString& button0Text = QString::null,
			const QString& button1Text = QString::null,
			const QString& button2Text = QString::null,
			int defaultButtonNumber = 0,
			int escapeButtonNumber = -1 );

    static int critical( QWidget *parent, const QString &caption,
			 const QString& text,
			 int button0, int button1, int button2=0 );
    static int critical( QWidget *parent, const QString &caption,
			 const QString& text,
			 const QString& button0Text = QString::null,
			 const QString& button1Text = QString::null,
			 const QString& button2Text = QString::null,
			 int defaultButtonNumber = 0,
			 int escapeButtonNumber = -1 );

    static void about( QWidget *parent, const QString &caption,
		       const QString& text );

    static void aboutQt( QWidget *parent,
			 const QString& caption=QString::null );

/* OBSOLETE */
    static int message( const QString &caption,
			const QString& text,
			const QString& buttonText=QString::null,
			QWidget *parent=0, const char * =0 ) {
	return QMessageBox::information( parent, caption, text,
				     buttonText.isEmpty()
				     ? tr("OK") : buttonText ) == 0;
    }

/* OBSOLETE */
    static bool query( const QString &caption,
		       const QString& text,
		       const QString& yesButtonText=QString::null,
		       const QString& noButtonText=QString::null,
		       QWidget *parent=0, const char * = 0 ) {
	return QMessageBox::information( parent, caption, text,
				     yesButtonText.isEmpty()
				     ? tr("OK") : yesButtonText,
				     noButtonText ) == 0;
    }

    QString	text() const;
    void	setText( const QString &);

    Icon	icon() const;

    void	setIcon( Icon );
    void	setIcon( const QPixmap & );

    const QPixmap *iconPixmap() const;
    void	setIconPixmap( const QPixmap & );

    QString	buttonText( int button ) const;
    void	setButtonText( int button, const QString &);

    void	adjustSize();

/* OBSOLETE */
    static QPixmap standardIcon( Icon icon, GUIStyle );

    static QPixmap standardIcon( Icon icon );

    TextFormat textFormat() const;
    void	 setTextFormat( TextFormat );

protected:
    void	resizeEvent( QResizeEvent * );
    void	showEvent( QShowEvent * );
    void	closeEvent( QCloseEvent * );
    void	keyPressEvent( QKeyEvent * );
    void	styleChanged( QStyle& );

private slots:
    void	buttonClicked();

private:
    void	init( int, int, int );
    int		indexOf( int ) const;
    void	resizeButtons();
    QLabel     *label;
    QMessageBoxData *mbd;
    void       *reserved1;
    void       *reserved2;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QMessageBox( const QMessageBox & );
    QMessageBox &operator=( const QMessageBox & );
#endif
};

/*
*  Macro to be used at the beginning of main(), e.g.
*
*   #include <qapplication.h>
*   #include <qmessagebox.h>
*   int main( int argc, char**argv )
*   {
*     QT_REQUIRE_VERSION( argc, argv, "3.0.5" )
*     ...
*   }
*/
#define QT_REQUIRE_VERSION( argc, argv, str ) { QString s=QString::fromLatin1(str);\
QString sq=QString::fromLatin1(qVersion()); if ( (sq.section('.',0,0).toInt()<<16)+\
(sq.section('.',1,1).toInt()<<8)+sq.section('.',2,2).toInt()<(s.section('.',0,0).toInt()<<16)+\
(s.section('.',1,1).toInt()<<8)+s.section('.',2,2).toInt() ){if ( !qApp){ int c=0; new \
QApplication(argc,argv);} QString s = QApplication::tr("Executable '%1' requires Qt "\
 "%2, found Qt %3.").arg(QString::fromLatin1(qAppName())).arg(QString::fromLatin1(\
str)).arg(QString::fromLatin1(qVersion()) ); QMessageBox::critical( 0, QApplication::tr(\
"Incompatible Qt Library Error" ), s, QMessageBox::Abort,0 ); qFatal(s.ascii()); }}


#endif // QT_NO_MESSAGEBOX

#endif // QMESSAGEBOX_H
