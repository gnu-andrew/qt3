/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Designer.
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
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with
** the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#ifndef PIXMAPCHOOSER_H
#define PIXMAPCHOOSER_H

#include <qfiledialog.h>
#include <qscrollview.h>
#include <qpixmap.h>
#include <qurl.h>

class FormWindow;

class PixmapView : public QScrollView,
		   public QFilePreview
{
    Q_OBJECT

public:
    PixmapView( QWidget *parent );
    void setPixmap( const QPixmap &pix );
    void drawContents( QPainter *p, int, int, int, int );
    void previewUrl( const QUrl &u );

private:
    QPixmap pixmap;

};

class ImageIconProvider : public QFileIconProvider
{
    Q_OBJECT

public:
    ImageIconProvider( QWidget *parent = 0, const char *name = 0 );
    ~ImageIconProvider();

    const QPixmap *pixmap( const QFileInfo &fi );

private:
    QStrList fmts;
    QPixmap imagepm;

};

QPixmap qChoosePixmap( QWidget *parent, FormWindow *fw = 0, const QPixmap &old = QPixmap(),  QString *fn = 0 );
QStringList qChoosePixmaps( QWidget *parent );

#endif
