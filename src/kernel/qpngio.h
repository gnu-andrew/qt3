/****************************************************************************
**
** Definition of PNG QImage IOHandler
**
** Created : 970521
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

#ifndef QPNGIO_H
#define QPNGIO_H

#ifndef QT_H
#include "qimage.h"
#endif // QT_H

#ifndef QT_NO_IMAGEIO_PNG

void qInitPngIO();

class QIODevice;

#ifndef Q_PNGEXPORT
#if !defined(QT_PLUGIN)
#define Q_PNGEXPORT Q_EXPORT
#else
#define Q_PNGEXPORT
#endif
#endif

class Q_PNGEXPORT QPNGImageWriter {
public:
    QPNGImageWriter(QIODevice*);
    ~QPNGImageWriter();

    enum DisposalMethod { Unspecified, NoDisposal, RestoreBackground, RestoreImage };
    void setDisposalMethod(DisposalMethod);
    void setLooping(int loops=0); // 0 == infinity
    void setFrameDelay(int msecs);
    void setGamma(float);

    bool writeImage(const QImage& img, int x, int y);
    bool writeImage(const QImage& img, int quality, int x, int y);
    bool writeImage(const QImage& img)
	{ return writeImage(img, 0, 0); }
    bool writeImage(const QImage& img, int quality)
	{ return writeImage(img, quality, 0, 0); }

    QIODevice* device() { return dev; }

private:
    QIODevice* dev;
    int frames_written;
    DisposalMethod disposal;
    int looping;
    int ms_delay;
    float gamma;
};

class Q_PNGEXPORT QPNGImagePacker : public QPNGImageWriter {
public:
    QPNGImagePacker(QIODevice*, int depth, int convflags);

    void setPixelAlignment(int x);
    bool packImage(const QImage& img);

private:
    QImage previous;
    int depth;
    int convflags;
    int alignx;
};

#endif // QT_NO_IMAGEIO_PNG

#endif // QPNGIO_H
