/****************************************************************************
**
** Definition of QThumbWheel class
**
** Created : 010205
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

#ifndef QTHUMBWHEEL_H
#define QTHUMBWHEEL_H

#ifndef QT_H
#include "qframe.h"
#include "qrangecontrol.h"
#endif // QT_H

#ifndef QT_NO_THUMBWHEEL

class QThumbWheel : public QFrame, public QRangeControl
{
    Q_OBJECT

public:
    QThumbWheel( QWidget *parent=0, const char *name=0 );
    ~QThumbWheel();

    virtual void 	setOrientation( Orientation );
    Orientation  	orientation() const;
    virtual void	setTracking( bool enable );
    bool		tracking() const;
    virtual void	setTransmissionRatio( double r );
    double 		transmissionRatio() const;

public slots:
    virtual void setValue( int );

signals:
    void 	valueChanged( int value );

protected:
    void 	valueChange();
    void 	rangeChange();
    void 	stepChange();

    void 	keyPressEvent( QKeyEvent * );
    void 	mousePressEvent( QMouseEvent * );
    void 	mouseReleaseEvent( QMouseEvent * );
    void 	mouseMoveEvent( QMouseEvent * );
    void 	wheelEvent( QWheelEvent * );
    void	focusInEvent( QFocusEvent *e );
    void	focusOutEvent( QFocusEvent *e );

    void 	drawContents( QPainter * );

private:
    void 	init();
    int		valueFromPosition( const QPoint & );


    double 	rat;
    int		pressedAt;
    Orientation orient;
    uint	track : 1;
    uint	mousePressed : 1;

    class QThumbWheelPrivate;
    QThumbWheelPrivate *d;
};

inline QThumbWheel::Orientation QThumbWheel::orientation() const
{
    return orient;
}

inline bool QThumbWheel::tracking() const
{
    return (bool)track;
}

inline double QThumbWheel::transmissionRatio() const
{
    return rat;
}

#endif // QT_NO_WHEEL

#endif // QWHEEL_H
