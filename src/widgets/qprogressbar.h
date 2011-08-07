/****************************************************************************
**
** Definition of QProgressBar class
**
** Created : 970520
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

#ifndef QPROGRESSBAR_H
#define QPROGRESSBAR_H

#ifndef QT_H
#include "qframe.h"
#endif // QT_H

#ifndef QT_NO_PROGRESSBAR


class QProgressBarPrivate;


class Q_EXPORT QProgressBar : public QFrame
{
    Q_OBJECT
    Q_PROPERTY( int totalSteps READ totalSteps WRITE setTotalSteps )
    Q_PROPERTY( int progress READ progress WRITE setProgress )
    Q_PROPERTY( QString progressString READ progressString )
    Q_PROPERTY( bool centerIndicator READ centerIndicator WRITE setCenterIndicator )
    Q_PROPERTY( bool indicatorFollowsStyle READ indicatorFollowsStyle WRITE setIndicatorFollowsStyle )
    Q_PROPERTY( bool percentageVisible READ percentageVisible WRITE setPercentageVisible )

public:
    QProgressBar( QWidget* parent=0, const char* name=0, WFlags f=0 );
    QProgressBar( int totalSteps, QWidget* parent=0, const char* name=0, WFlags f=0 );

    int		totalSteps() const;
    int		progress()   const;
    const QString &progressString() const;

    QSize	sizeHint() const;
    QSize	minimumSizeHint() const;

    void	setCenterIndicator( bool on );
    bool	centerIndicator() const;

    void        setIndicatorFollowsStyle( bool );
    bool	indicatorFollowsStyle() const;

    bool	percentageVisible() const;
    void	setPercentageVisible( bool );

    void	show();

public slots:
    void	reset();
    virtual void setTotalSteps( int totalSteps );
    virtual void setProgress( int progress );
    void	setProgress( int progress, int totalSteps );

protected:
    void	drawContents( QPainter * );
    virtual bool setIndicator( QString & progress_str, int progress,
			       int totalSteps );
    void styleChange( QStyle& );

private:
    int		total_steps;
    int		progress_val;
    int		percentage;
    QString	progress_str;
    bool        center_indicator    : 1;
    bool        auto_indicator	    : 1;
    bool	percentage_visible  : 1;
    QProgressBarPrivate * d;
    void         initFrame();

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QProgressBar( const QProgressBar & );
    QProgressBar &operator=( const QProgressBar & );
#endif
};


inline int QProgressBar::totalSteps() const
{
    return total_steps;
}

inline int QProgressBar::progress() const
{
    return progress_val;
}

inline const QString &QProgressBar::progressString() const
{
    return progress_str;
}

inline bool QProgressBar::centerIndicator() const
{
    return center_indicator;
}

inline bool QProgressBar::indicatorFollowsStyle() const
{
    return auto_indicator;
}

inline bool QProgressBar::percentageVisible() const
{
    return percentage_visible;
}

#endif // QT_NO_PROGRESSBAR

#endif // QPROGRESSBAR_H
