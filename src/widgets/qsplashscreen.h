/****************************************************************************
**
** Definition of QSplashScreen class
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

#ifndef QSPLASHSCREEN_H
#define QSPLASHSCREEN_H

#ifndef QT_H
#include "qpixmap.h"
#include "qwidget.h"
#endif // QT_H

#ifndef QT_NO_SPLASHSCREEN
class QSplashScreenPrivate;

class Q_EXPORT QSplashScreen : public QWidget
{
    Q_OBJECT
public:
    QSplashScreen( const QPixmap &pixmap = QPixmap(), WFlags f = 0 );
    virtual ~QSplashScreen();

    void setPixmap( const QPixmap &pixmap );
    QPixmap* pixmap() const;
    void finish( QWidget *w );
    void repaint();

public slots:
    void message( const QString &str, int flags = AlignLeft,
		  const QColor &color = black );
    void clear();

signals:
    void messageChanged( const QString &str );

protected:
    virtual void drawContents( QPainter *painter );
    void mousePressEvent( QMouseEvent * );

private:
    void drawContents();

    QSplashScreenPrivate *d;
};
#endif //QT_NO_SPLASHSCREEN
#endif
