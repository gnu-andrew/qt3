/****************************************************************************
**
** Definition of QPen class
**
** Created : 940112
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the kernel module of the Qt GUI Toolkit.
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

#ifndef QPEN_H
#define QPEN_H

#ifndef QT_H
#include "qcolor.h"
#include "qshared.h"
#endif // QT_H


class Q_EXPORT QPen: public Qt
{
public:
    QPen();
    QPen( PenStyle );
    QPen( const QColor &color, uint width=0, PenStyle style=SolidLine );
    QPen( const QColor &cl, uint w, PenStyle s, PenCapStyle c, PenJoinStyle j);
    QPen( const QPen & );
   ~QPen();
    QPen &operator=( const QPen & );

    PenStyle	style() const		{ return data->style; }
    void	setStyle( PenStyle );
    uint	width() const		{ return data->width; }
    void	setWidth( uint );
    const QColor &color() const		{ return data->color; }
    void	setColor( const QColor & );
    PenCapStyle	capStyle() const;
    void	setCapStyle( PenCapStyle );
    PenJoinStyle joinStyle() const;
    void	setJoinStyle( PenJoinStyle );

    bool	operator==( const QPen &p ) const;
    bool	operator!=( const QPen &p ) const
					{ return !(operator==(p)); }

private:
    friend class QPainter;
#ifdef Q_WS_WIN
    friend class QFontEngineWin;
#endif

    QPen	copy()	const;
    void	detach();
    void	init( const QColor &, uint, uint );
    struct QPenData : public QShared {		// pen data
	PenStyle  style;
	uint	  width;
	QColor	  color;
	Q_UINT16  linest;
    } *data;
};


/*****************************************************************************
  QPen stream functions
 *****************************************************************************/
#ifndef QT_NO_DATASTREAM
Q_EXPORT QDataStream &operator<<( QDataStream &, const QPen & );
Q_EXPORT QDataStream &operator>>( QDataStream &, QPen & );
#endif

#endif // QPEN_H
