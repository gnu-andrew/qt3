/****************************************************************************
**
** Definition of the QSvgDevice class
**
** Created : 001024
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the xml module of the Qt GUI Toolkit.
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
*****************************************************************************/

#ifndef QSVGDEVICE_P_H
#define QSVGDEVICE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of the QPicture class. This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//
//

#ifndef QT_H
#include "qpaintdevice.h"
#include "qrect.h"
#include "qdom.h"
#endif // QT_H

#if !defined(QT_MODULE_XML) || defined( QT_LICENSE_PROFESSIONAL ) || defined( QT_INTERNAL_XML )
#define QM_EXPORT_SVG
#else
#define QM_EXPORT_SVG Q_EXPORT
#endif

#ifndef QT_NO_SVG

class QPainter;
class QDomNode;
class QDomNamedNodeMap;
struct QSvgDeviceState;
class QSvgDevicePrivate;

class QM_EXPORT_SVG QSvgDevice : public QPaintDevice
{
public:
    QSvgDevice();
    ~QSvgDevice();

    bool play( QPainter *p );

    QString toString() const;

    bool load( QIODevice *dev );
    bool save( QIODevice *dev );
    bool save( const QString &fileName );

    QRect boundingRect() const;
    void setBoundingRect( const QRect &r );

protected:
    virtual bool cmd ( int, QPainter*, QPDevCmdParam* );
    virtual int	 metric( int ) const;

private:
    // reading
    bool play( const QDomNode &node );
    void saveAttributes();
    void restoreAttributes();
    QColor parseColor( const QString &col );
    double parseLen( const QString &str, bool *ok=0, bool horiz=TRUE ) const;
    int lenToInt( const QDomNamedNodeMap &map, const QString &attr,
		  int def=0 ) const;
    double lenToDouble( const QDomNamedNodeMap &map, const QString &attr,
		  int def=0 ) const;
    void setStyleProperty( const QString &prop, const QString &val,
			   QPen *pen, QFont *font, int *talign );
    void setStyle( const QString &s );
    void setTransform( const QString &tr );
    void drawPath( const QString &data );

    // writing
    void appendChild( QDomElement &e, int c );
    void applyStyle( QDomElement *e, int c ) const;
    void applyTransform( QDomElement *e ) const;

    // reading
    QRect brect;			// bounding rectangle
    QDomDocument doc;			// document tree
    QDomNode current;
    QPoint curPt;
    QSvgDeviceState *curr;
    QPainter *pt;			// used by play() et al

    // writing
    bool dirtyTransform, dirtyStyle;

    QSvgDevicePrivate *d;
};

inline QRect QSvgDevice::boundingRect() const
{
    return brect;
}

#endif // QT_NO_SVG

#endif // QSVGDEVICE_P_H
