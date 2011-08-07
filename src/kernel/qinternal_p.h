/****************************************************************************
**
** Definition of some shared interal classes
**
** Created : 010427
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

#ifndef QINTERNAL_P_H
#define QINTERNAL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of a number of Qt sources files.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//
//
#ifndef QT_H
#include "qnamespace.h"
#include "qrect.h"
#include "qptrlist.h"
#include "qcstring.h"
#include "qiodevice.h"
#endif // QT_H

class QWidget;
class QPainter;
class QPixmap;

class Q_EXPORT QSharedDoubleBuffer
{
public:
    enum DoubleBufferFlags {
	NoFlags         = 0x00,
	InitBG		= 0x01,
	Force		= 0x02,
	Default		= InitBG | Force
    };
    typedef uint DBFlags;

    QSharedDoubleBuffer( DBFlags f = Default );
    QSharedDoubleBuffer( QWidget* widget,
			 int x = 0, int y = 0, int w = -1, int h = -1,
			 DBFlags f = Default );
    QSharedDoubleBuffer( QPainter* painter,
			 int x = 0, int y = 0, int w = -1, int h = -1,
			 DBFlags f = Default );
    QSharedDoubleBuffer( QWidget *widget, const QRect &r, DBFlags f = Default );
    QSharedDoubleBuffer( QPainter *painter, const QRect &r, DBFlags f = Default );
    ~QSharedDoubleBuffer();

    bool begin( QWidget* widget, int x = 0, int y = 0, int w = -1, int h = -1 );
    bool begin( QPainter* painter, int x = 0, int y = 0, int w = -1, int h = -1);
    bool begin( QWidget* widget, const QRect &r );
    bool begin( QPainter* painter, const QRect &r );
    bool end();

    QPainter* painter() const;

    bool isActive() const;
    bool isBuffered() const;
    void flush();

    static bool isDisabled() { return !dblbufr; }
    static void setDisabled( bool off ) { dblbufr = !off; }

    static void cleanup();

private:
    enum DoubleBufferState {
	Active		= 0x0100,
	BufferActive	= 0x0200,
	ExternalPainter	= 0x0400
    };
    typedef uint DBState;

    QPixmap *getPixmap();
    void releasePixmap();

    QWidget *wid;
    int rx, ry, rw, rh;
    DBFlags flags;
    DBState state;

    QPainter *p, *external_p;
    QPixmap *pix;

    static bool dblbufr;
};

inline bool QSharedDoubleBuffer::begin( QWidget* widget, const QRect &r )
{ return begin( widget, r.x(), r.y(), r.width(), r.height() ); }

inline bool QSharedDoubleBuffer::begin( QPainter *painter, const QRect &r )
{ return begin( painter, r.x(), r.y(), r.width(), r.height() ); }

inline QPainter* QSharedDoubleBuffer::painter() const
{ return p; }

inline bool QSharedDoubleBuffer::isActive() const
{ return ( state & Active ); }

inline bool QSharedDoubleBuffer::isBuffered() const
{ return ( state & BufferActive ); }


class QVirtualDestructor {
public:
    virtual ~QVirtualDestructor() {}
};

template <class T>
class QAutoDeleter : public QVirtualDestructor {
public:
    QAutoDeleter( T* p ) : ptr( p ) {}
    ~QAutoDeleter() { delete ptr; }
    T* data() const { return ptr; }
private:
    T* ptr;
};

template <class T>
T* qAutoDeleterData( QAutoDeleter<T>* ad )
{
    if ( !ad )
	return 0;
    return ad->data();
}

template <class T>
QAutoDeleter<T>* qAutoDeleter( T* p )
{
    return new QAutoDeleter<T>( p );
}

class Q_EXPORT QMembuf
{
public:
    QMembuf();
    ~QMembuf();

    void append( QByteArray *ba );
    void clear();

    bool consumeBytes( Q_ULONG nbytes, char *sink );
    QByteArray readAll();
    bool scanNewline( QByteArray *store );
    bool canReadLine() const;

    int ungetch( int ch );

    QIODevice::Offset size() const;

private:

    QPtrList<QByteArray> *buf;
    QIODevice::Offset _size;
    QIODevice::Offset _index;
};

inline void QMembuf::append( QByteArray *ba )
{ buf->append( ba ); _size += ba->size(); }

inline void QMembuf::clear()
{ buf->clear(); _size=0; _index=0; }

inline QByteArray QMembuf::readAll()
{ QByteArray ba(_size); consumeBytes(_size,ba.data()); return ba; }

inline bool QMembuf::canReadLine() const
{ return ((QMembuf*)this)->scanNewline( 0 ); }

inline QIODevice::Offset QMembuf::size() const
{ return _size; }

#endif // QINTERNAL_P_H
