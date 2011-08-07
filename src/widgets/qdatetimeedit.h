/****************************************************************************
**
** Definition of date and time edit classes
**
** Created : 001103
**
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QDATETIMEEDIT_H
#define QDATETIMEEDIT_H

#ifndef QT_H
#include "qwidget.h"
#include "qstring.h"
#include "qdatetime.h"
#endif // QT_H

#ifndef QT_NO_DATETIMEEDIT

class Q_EXPORT QDateTimeEditBase : public QWidget
{
    Q_OBJECT
public:
    QDateTimeEditBase( QWidget* parent=0, const char* name=0 )
	: QWidget( parent, name ) {}
    
    virtual bool setFocusSection( int sec ) = 0;
    virtual QString sectionFormattedText( int sec ) = 0;
    virtual void addNumber( int sec, int num ) = 0;
    virtual void removeLastNumber( int sec ) = 0;

public slots:
    virtual void stepUp() = 0;
    virtual void stepDown() = 0;

private:
#if defined(Q_DISABLE_COPY) // Disabled copy constructor and operator=
    QDateTimeEditBase( const QDateTimeEditBase & );
    QDateTimeEditBase &operator=( const QDateTimeEditBase & );
#endif
};

class QDateEditPrivate;

class Q_EXPORT QDateEdit : public QDateTimeEditBase
{
    Q_OBJECT
    Q_ENUMS( Order )
    Q_PROPERTY( Order order READ order WRITE setOrder )
    Q_PROPERTY( QDate date READ date WRITE setDate )
    Q_PROPERTY( bool autoAdvance READ autoAdvance WRITE setAutoAdvance )
    Q_PROPERTY( QDate maxValue READ maxValue WRITE setMaxValue )
    Q_PROPERTY( QDate minValue READ minValue WRITE setMinValue )

public:
    QDateEdit( QWidget* parent=0,  const char* name=0 );
    QDateEdit( const QDate& date, QWidget* parent=0,  const char* name=0 );
    ~QDateEdit();

    enum Order { DMY, MDY, YMD, YDM };

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    virtual void setDate( const QDate& date );

public:
    QDate date() const;
    virtual void setOrder( Order order );
    Order order() const;
    virtual void setAutoAdvance( bool advance );
    bool autoAdvance() const;

    virtual void setMinValue( const QDate& d ) { setRange( d, maxValue() ); }
    QDate minValue() const;
    virtual void setMaxValue( const QDate& d ) { setRange( minValue(), d ); }
    QDate maxValue() const;
    virtual void setRange( const QDate& min, const QDate& max );
    QString separator() const;
    virtual void setSeparator( const QString& s );

    // Make removeFirstNumber() virtual in QDateTimeEditBase in 4.0
    void removeFirstNumber( int sec );

signals:
    void valueChanged( const QDate& date );

protected:
    bool event( QEvent *e );
    void timerEvent( QTimerEvent * );
    void resizeEvent( QResizeEvent * );
    void stepUp();
    void stepDown();
    QString sectionFormattedText( int sec );
    void addNumber( int sec, int num );
    
    void removeLastNumber( int sec );
    bool setFocusSection( int s );

    virtual void setYear( int year );
    virtual void setMonth( int month );
    virtual void setDay( int day );
    virtual void fix();
    virtual bool outOfRange( int y, int m, int d ) const;

protected slots:
    void updateButtons();    

private:
    void init();
    int sectionOffsetEnd( int sec ) const;
    int sectionLength( int sec ) const;
    QString sectionText( int sec ) const;
    QDateEditPrivate* d;

#if defined(Q_DISABLE_COPY)
    QDateEdit( const QDateEdit & );
    QDateEdit &operator=( const QDateEdit & );
#endif
};

class QTimeEditPrivate;

class Q_EXPORT QTimeEdit : public QDateTimeEditBase
{
    Q_OBJECT
    Q_SETS( Display )
    Q_PROPERTY( QTime time READ time WRITE setTime )
    Q_PROPERTY( bool autoAdvance READ autoAdvance WRITE setAutoAdvance )
    Q_PROPERTY( QTime maxValue READ maxValue WRITE setMaxValue )
    Q_PROPERTY( QTime minValue READ minValue WRITE setMinValue )
    Q_PROPERTY( Display display READ display WRITE setDisplay )

public:
    enum Display {
	Hours	= 0x01,
	Minutes	= 0x02,
	Seconds	= 0x04,
	/*Reserved = 0x08,*/
	AMPM	= 0x10
    };

    QTimeEdit( QWidget* parent=0,  const char* name=0 );
    QTimeEdit( const QTime& time, QWidget* parent=0,  const char* name=0 );
    ~QTimeEdit();

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    virtual void setTime( const QTime& time );

public:
    QTime time() const;
    virtual void setAutoAdvance( bool advance );
    bool autoAdvance() const;

    virtual void setMinValue( const QTime& d ) { setRange( d, maxValue() ); }
    QTime minValue() const;
    virtual void setMaxValue( const QTime& d ) { setRange( minValue(), d ); }
    QTime maxValue() const;
    virtual void setRange( const QTime& min, const QTime& max );
    QString separator() const;
    virtual void setSeparator( const QString& s );

    uint display() const;
    void setDisplay( uint disp );

    // Make removeFirstNumber() virtual in QDateTimeEditBase in 4.0
    void removeFirstNumber( int sec );

signals:
    void valueChanged( const QTime& time );

protected:
    bool event( QEvent *e );
    void timerEvent( QTimerEvent *e );
    void resizeEvent( QResizeEvent * );
    void stepUp();
    void stepDown();
    QString sectionFormattedText( int sec );
    void addNumber( int sec, int num );
    void removeLastNumber( int sec );
    bool setFocusSection( int s );
    
    virtual bool outOfRange( int h, int m, int s ) const;
    virtual void setHour( int h );
    virtual void setMinute( int m );
    virtual void setSecond( int s );
    
protected slots:
    void updateButtons();

private:
    void init();
    QString sectionText( int sec );
    QTimeEditPrivate* d;

#if defined(Q_DISABLE_COPY)
    QTimeEdit( const QTimeEdit & );
    QTimeEdit &operator=( const QTimeEdit & );
#endif
};


class QDateTimeEditPrivate;

class Q_EXPORT QDateTimeEdit : public QWidget
{
    Q_OBJECT
    Q_PROPERTY( QDateTime dateTime READ dateTime WRITE setDateTime )

public:
    QDateTimeEdit( QWidget* parent=0, const char* name=0 );
    QDateTimeEdit( const QDateTime& datetime, QWidget* parent=0,
		   const char* name=0 );
    ~QDateTimeEdit();

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

public slots:
    virtual void setDateTime( const QDateTime & dt );

public:
    QDateTime dateTime() const;

    QDateEdit* dateEdit() { return de; }
    QTimeEdit* timeEdit() { return te; }

    virtual void setAutoAdvance( bool advance );
    bool autoAdvance() const;

signals:
    void valueChanged( const QDateTime& datetime );

protected:
    // ### make init() private in Qt 4.0
    void init();
    void resizeEvent( QResizeEvent * );

protected slots:
    // ### make these two functions private in Qt 4.0,
    //     and merge them into one with no parameter
    void newValue( const QDate& d );
    void newValue( const QTime& t );

private:
    QDateEdit* de;
    QTimeEdit* te;
    QDateTimeEditPrivate* d;

#if defined(Q_DISABLE_COPY)
    QDateTimeEdit( const QDateTimeEdit & );
    QDateTimeEdit &operator=( const QDateTimeEdit & );
#endif
};

#endif
#endif
