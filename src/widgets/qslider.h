/****************************************************************************
**
** Definition of QSlider class
**
** Created : 961019
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

#ifndef QSLIDER_H
#define QSLIDER_H

#ifndef QT_H
#include "qwidget.h"
#include "qrangecontrol.h"
#endif // QT_H

#ifndef QT_NO_SLIDER

struct QSliderPrivate;

class QTimer;

class Q_EXPORT QSlider : public QWidget, public QRangeControl
{
    Q_OBJECT
    Q_ENUMS( TickSetting )
    Q_PROPERTY( int minValue READ minValue WRITE setMinValue )
    Q_PROPERTY( int maxValue READ maxValue WRITE setMaxValue )
    Q_PROPERTY( int lineStep READ lineStep WRITE setLineStep )
    Q_PROPERTY( int pageStep READ pageStep WRITE setPageStep )
    Q_PROPERTY( int value READ value WRITE setValue )
    Q_PROPERTY( bool tracking READ tracking WRITE setTracking )
    Q_PROPERTY( Orientation orientation READ orientation WRITE setOrientation )
    Q_PROPERTY( TickSetting tickmarks READ tickmarks WRITE setTickmarks )
    Q_PROPERTY( int tickInterval READ tickInterval WRITE setTickInterval )
	
public:
    enum TickSetting { NoMarks = 0, Above = 1, Left = Above,
		       Below = 2, Right = Below, Both = 3 };

    QSlider( QWidget *parent, const char* name = 0 );
    QSlider( Orientation, QWidget *parent, const char* name = 0 );
    QSlider( int minValue, int maxValue, int pageStep, int value, Orientation,
	     QWidget *parent, const char* name = 0 );
    ~QSlider();

    virtual void	setOrientation( Orientation );
    Orientation orientation() const;
    virtual void	setTracking( bool enable );
    bool	tracking() const;
    virtual void 	setPalette( const QPalette & );

    int sliderStart() const;
    QRect sliderRect() const;
    QSize sizeHint() const;
    void setSizePolicy( QSizePolicy sp );
    void setSizePolicy( QSizePolicy::SizeType hor, QSizePolicy::SizeType ver, bool hfw = FALSE );

    QSizePolicy sizePolicy() const;
    QSize minimumSizeHint() const;

    virtual void setTickmarks( TickSetting );
    TickSetting tickmarks() const { return ticks; }

    virtual void setTickInterval( int );
    int 	tickInterval() const { return tickInt; }

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
    virtual void	setValue( int );
    void	addStep();
    void	subtractStep();
    void	addLine();
    void	subtractLine();

signals:
    void	valueChanged( int value );
    void	sliderPressed();
    void	sliderMoved( int value );
    void	sliderReleased();

protected:
    void	resizeEvent( QResizeEvent * );
    void	paintEvent( QPaintEvent * );

    void	keyPressEvent( QKeyEvent * );
    void	mousePressEvent( QMouseEvent * );
    void	mouseReleaseEvent( QMouseEvent * );
    void	mouseMoveEvent( QMouseEvent * );
#ifndef QT_NO_WHEELEVENT
    void	wheelEvent( QWheelEvent * );
#endif
    void	focusInEvent( QFocusEvent *e );
    void	focusOutEvent( QFocusEvent *e );

    void	styleChange( QStyle& );

    void	valueChange();
    void	rangeChange();

private slots:
    void	repeatTimeout();

private:
    enum State { Idle, Dragging, TimingUp, TimingDown };

    void	init();
    int		positionFromValue( int ) const;
    int		valueFromPosition( int ) const;
    void	moveSlider( int );
    void	reallyMoveSlider( int );
    void	resetState();
    int		available() const;
    int		goodPart( const QPoint& ) const;
    void	initTicks();

    QSliderPrivate *d;
    QTimer	*timer;
    QCOORD	sliderPos;
    int		sliderVal;
    QCOORD	clickOffset;
    State	state;
    bool	track;
    QCOORD	tickOffset;
    TickSetting	ticks;
    int		tickInt;
    Orientation orient;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QSlider( const QSlider & );
    QSlider &operator=( const QSlider & );
#endif
};

inline bool QSlider::tracking() const
{
    return track;
}

inline QSlider::Orientation QSlider::orientation() const
{
    return orient;
}

inline int QSlider::sliderStart() const
{
    return sliderPos;
}

inline void QSlider::setSizePolicy( QSizePolicy::SizeType hor, QSizePolicy::SizeType ver, bool hfw )
{ 
    QWidget::setSizePolicy( hor, ver, hfw ); 
}

#endif // QT_NO_SLIDER

#endif // QSLIDER_H
