/****************************************************************************
**
** Definition of asynchronous image/movie loading classes
**
** Created : 970617
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

#ifndef QASYNCIMAGEIO_H
#define QASYNCIMAGEIO_H

#ifndef QT_H
#include "qimage.h"
#endif // QT_H

#ifndef QT_NO_ASYNC_IMAGE_IO

#if __GNUC__ - 0 > 3
#pragma GCC system_header
#endif

class Q_EXPORT QImageConsumer {
public:
    virtual void end()=0;

    // Change transfer type 1.
    virtual void changed( const QRect& ) = 0;
    virtual void frameDone() = 0;

    // Change transfer type 2.
    virtual void frameDone( const QPoint&, const QRect& ) = 0;

    virtual void setLooping( int ) = 0;
    virtual void setFramePeriod( int ) = 0;
    virtual void setSize( int, int ) = 0;
};

class Q_EXPORT QImageFormat {
public:
    virtual ~QImageFormat();
    virtual int decode( QImage& img, QImageConsumer* consumer,
			const uchar* buffer, int length ) = 0;
};

class Q_EXPORT QImageFormatType {
public:
    virtual ~QImageFormatType();
    virtual QImageFormat* decoderFor( const uchar* buffer, int length ) = 0;
    virtual const char* formatName() const = 0;
protected:
    QImageFormatType();
};

class QImageDecoderPrivate;
class Q_EXPORT QImageDecoder {
public:
    QImageDecoder( QImageConsumer* c );
    ~QImageDecoder();

    const QImage& image() { return img; }
    int decode( const uchar* buffer, int length );

    static const char* formatName( const uchar* buffer, int length );
    static QImageFormatType* format( const char* name ); // direct use - no decode()

    static QStrList inputFormats();
    static void registerDecoderFactory( QImageFormatType* );
    static void unregisterDecoderFactory( QImageFormatType* );

private:
    QImageFormat* actual_decoder;
    QImageConsumer* consumer;
    QImage img;
    QImageDecoderPrivate *d;
};

#endif // QT_NO_ASYNC_IMAGE_IO

#endif // QASYNCIMAGEIO_H
