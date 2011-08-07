/****************************************************************************
**
** Definition of the dial widget
**
** Created : 990104
**
** Copyright (C) 1999-2008 Trolltech ASA.  All rights reserved.
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


#ifndef QDIAL_H
#define QDIAL_H

#ifndef QT_H
#include "qwidget.h"
#include "qrangecontrol.h"
#endif // QT_H

#ifndef QT_NO_DIAL

class QDialPrivate;

class Q_EXPORT QDial: public QWidget, public QRangeControl
{
    Q_OBJECT
    Q_PROPERTY( bool tracking READ tracking WRITE setTracking )
    Q_PROPERTY( bool wrapping READ wrapping WRITE setWrapping )
    Q_PROPERTY( int notchSize READ notchSize )
    Q_PROPERTY( double notchTarget READ notchTarget WRITE setNotchTarget )
    Q_PROPERTY( bool notchesVisible READ notchesVisible WRITE setNotchesVisible )
    Q_PROPERTY( int minValue READ minValue WRITE setMinValue )
    Q_PROPERTY( int maxValue READ maxValue WRITE setMaxValue )
    Q_PROPERTY( int lineStep READ lineStep WRITE setLineStep )
    Q_PROPERTY( int pageStep READ pageStep WRITE setPageStep )
    Q_PROPERTY( int value READ value WRITE setValue )
	
public:
    QDial( QWidget* parent=0, const char* name=0, WFlags f = 0 );
    QDial( int minValue, int maxValue, int pageStep, int value,
	   QWidget* parent=0, const char* name=0 );
    ~QDial();

    bool tracking() const;

    bool wrapping() const;

    int notchSize() const;

    virtual void setNotchTarget( double );
    double notchTarget() const;

    bool notchesVisible() const;

    QSize sizeHint() const;
    QSize minimumSizeHint() const;

    int	 minValue() const;
    int	 maxValue() const;
    void setMinValue( int );
    void setMaxValue( int );
    int	 lineStep() const;
    int	 pageStep() const;
    void setLineStep( int );
    void setPageStep( int );
    int  value() const;

public slots:
    virtual void setValue( int );
    void addLine();
    void subtractLine();
    void addPage();
    void subtractPage();
    virtual void setNotchesVisible( bool b );
    virtual void setWrapping( bool on );
    virtual void setTracking( bool enable );

signals:
    void valueChanged( int value );
    void dialPressed();
    void dialMoved( int value );
    void dialReleased();

protected:
    void resizeEvent( QResizeEvent * );
    void paintEvent( QPaintEvent * );

    void keyPressEvent( QKeyEvent * );
    void mousePressEvent( QMouseEvent * );
    void mouseReleaseEvent( QMouseEvent * );
    void mouseMoveEvent( QMouseEvent * );
#ifndef QT_NO_WHEELEVENT
    void wheelEvent( QWheelEvent * );
#endif
    void focusInEvent( QFocusEvent * );
    void focusOutEvent( QFocusEvent * );

    void valueChange();
    void rangeChange();

    virtual void repaintScreen( const QRect *cr = 0 );

private:
    QDialPrivate * d;

    int valueFromPoint( const QPoint & ) const;
    double angle( const QPoint &, const QPoint & ) const;
    QPointArray calcArrow( double &a ) const;
    QRect calcDial() const;
    int calcBigLineSize() const;
    void calcLines();

private: // Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QDial( const QDial & );
    QDial &operator=( const QDial & );
#endif

};

#endif  // QT_NO_DIAL

#endif
