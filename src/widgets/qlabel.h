/**********************************************************************
**
** Definition of QLabel widget class
**
** Created : 941215
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
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

#ifndef QLABEL_H
#define QLABEL_H

#ifndef QT_H
#include "qframe.h"
#endif // QT_H

#ifndef QT_NO_LABEL

class QSimpleRichText;
class QLabelPrivate;

class Q_EXPORT QLabel : public QFrame
{
    Q_OBJECT
    Q_PROPERTY( QString text READ text WRITE setText )
    Q_PROPERTY( TextFormat textFormat READ textFormat WRITE setTextFormat )
    Q_PROPERTY( QPixmap pixmap READ pixmap WRITE setPixmap )
    Q_PROPERTY( bool scaledContents READ hasScaledContents WRITE setScaledContents )
    Q_PROPERTY( Alignment alignment READ alignment WRITE setAlignment )
    Q_PROPERTY( int indent READ indent WRITE setIndent )
    Q_OVERRIDE( BackgroundMode backgroundMode DESIGNABLE true)

public:
    QLabel( QWidget *parent, const char* name=0, WFlags f=0 );
    QLabel( const QString &text, QWidget *parent, const char* name=0,
	    WFlags f=0 );
    QLabel( QWidget *buddy, const QString &,
	    QWidget *parent, const char* name=0, WFlags f=0 );
    ~QLabel();

    QString	 text()		const	{ return ltext; }
    QPixmap     *pixmap()	const	{ return lpixmap; }
#ifndef QT_NO_PICTURE
    QPicture    *picture()	const	{ return lpicture; }
#endif
#ifndef QT_NO_MOVIE
    QMovie      *movie()		const;
#endif

    TextFormat textFormat() const;
    void 	 setTextFormat( TextFormat );

    int		 alignment() const	{ return align; }
    virtual void setAlignment( int );
    int		 indent() const		{ return extraMargin; }
    void 	 setIndent( int );

    bool 	 autoResize() const	{ return autoresize; }
    virtual void setAutoResize( bool );
#ifndef QT_NO_IMAGE_SMOOTHSCALE
    bool 	hasScaledContents() const;
    void 	setScaledContents( bool );
#endif
    QSize	 sizeHint() const;
    QSize	 minimumSizeHint() const;
#ifndef QT_NO_ACCEL
    virtual void setBuddy( QWidget * );
    QWidget     *buddy() const;
#endif
    int		 heightForWidth(int) const;

    void setFont( const QFont &f );

public slots:
    virtual void setText( const QString &);
    virtual void setPixmap( const QPixmap & );
#ifndef QT_NO_PICTURE
    virtual void setPicture( const QPicture & );
#endif
#ifndef QT_NO_MOVIE
    virtual void setMovie( const QMovie & );
#endif
    virtual void setNum( int );
    virtual void setNum( double );
    void	 clear();

protected:
    void	 drawContents( QPainter * );
    void	 fontChange( const QFont & );
    void	 resizeEvent( QResizeEvent* );

private slots:
#ifndef QT_NO_ACCEL
    void	 acceleratorSlot();
    void	 buddyDied();
#endif
#ifndef QT_NO_MOVIE
    void	 movieUpdated(const QRect&);
    void	 movieResized(const QSize&);
#endif

private:
    void	init();
    void	clearContents();
    void	updateLabel( QSize oldSizeHint );
    QSize	sizeForWidth( int w ) const;
    QString	ltext;
    QPixmap    *lpixmap;
#ifndef QT_NO_PICTURE
    QPicture   *lpicture;
#endif
#ifndef QT_NO_MOVIE
    QMovie *	lmovie;
#endif
#ifndef QT_NO_ACCEL
    QWidget *	lbuddy;
#endif
    ushort	align;
    short	extraMargin;
    uint	autoresize:1;
    uint	scaledcontents :1;
    TextFormat textformat;
#ifndef QT_NO_RICHTEXT
    QSimpleRichText* doc;
#endif
#ifndef QT_NO_ACCEL
    QAccel *	accel;
#endif
    QLabelPrivate* d;

    friend class QTipLabel;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QLabel( const QLabel & );
    QLabel &operator=( const QLabel & );
#endif
};


#endif // QT_NO_LABEL

#endif // QLABEL_H
