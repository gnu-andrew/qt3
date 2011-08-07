/****************************************************************************
**
** Definition of QGLColormap class
**
** Created : 20010326
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the opengl module of the Qt GUI Toolkit.
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

#ifndef QGLCOLORMAP_H
#define QGLCOLORMAP_H

#ifndef QT_H
#include "qcolor.h"
#include "qmemarray.h"
#include "qshared.h"
#endif // QT_H

#if !defined( QT_MODULE_OPENGL ) || defined( QT_LICENSE_PROFESSIONAL )
#define QM_EXPORT_OPENGL
#else
#define QM_EXPORT_OPENGL Q_EXPORT
#endif

class QWidget;
class QM_EXPORT_OPENGL QGLColormap
{
public:
    QGLColormap();
    QGLColormap( const QGLColormap & );
    ~QGLColormap();
    
    QGLColormap &operator=( const QGLColormap & );
    
    bool   isEmpty() const;
    int    size() const;
    void   detach();

    void   setEntries( int count, const QRgb * colors, int base = 0 );
    void   setEntry( int idx, QRgb color );
    void   setEntry( int idx, const QColor & color );
    QRgb   entryRgb( int idx ) const;
    QColor entryColor( int idx ) const;
    int    find( QRgb color ) const;
    int    findNearest( QRgb color ) const;
    
private:
    class Private : public QShared
    {
    public:
	Private() {
	    cells.resize( 256 ); // ### hardcoded to 256 entries for now
	    cmapHandle = 0;
	}

	~Private() {
	}

	QMemArray<QRgb> cells;
	Qt::HANDLE      cmapHandle;
    };
    
    Private * d;

    friend class QGLWidget;
};

#endif
