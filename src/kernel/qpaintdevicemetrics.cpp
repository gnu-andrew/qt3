/****************************************************************************
**
** Implementation of QPaintDeviceMetrics class
**
** Created : 941109
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

#include "qpaintdevicemetrics.h"

/*!
    \class QPaintDeviceMetrics qpaintdevicemetrics.h
    \brief The QPaintDeviceMetrics class provides information about a
    paint device.

    \ingroup graphics
    \ingroup images

    Sometimes when drawing graphics it is necessary to obtain
    information about the physical characteristics of a paint device.
    This class provides the information. For example, to compute the
    aspect ratio of a paint device:

    \code
	QPaintDeviceMetrics pdm( myWidget );
	double aspect = (double)pdm.widthMM() / (double)pdm.heightMM();
    \endcode

    QPaintDeviceMetrics contains methods to provide the width and
    height of a device in both pixels (width() and height()) and
    millimeters (widthMM() and heightMM()), the number of colors the
    device supports (numColors()), the number of bit planes (depth()),
    and the resolution of the device (logicalDpiX() and
    logicalDpiY()).

    It is not always possible for QPaintDeviceMetrics to compute the
    values you ask for, particularly for external devices. The
    ultimate example is asking for the resolution of of a QPrinter
    that is set to "print to file": who knows what printer that file
    will end up on?
*/

/*!
    Constructs a metric for the paint device \a pd.
*/
QPaintDeviceMetrics::QPaintDeviceMetrics( const QPaintDevice *pd )
{
    pdev = (QPaintDevice *)pd;
}


/*!
    \fn int QPaintDeviceMetrics::width() const

    Returns the width of the paint device in default coordinate system
    units (e.g. pixels for QPixmap and QWidget).
*/

/*!
    \fn int QPaintDeviceMetrics::height() const

    Returns the height of the paint device in default coordinate
    system units (e.g. pixels for QPixmap and QWidget).
*/

/*!
    \fn int QPaintDeviceMetrics::widthMM() const

    Returns the width of the paint device, measured in millimeters.
*/

/*!
    \fn int QPaintDeviceMetrics::heightMM() const

    Returns the height of the paint device, measured in millimeters.
*/

/*!
    \fn int QPaintDeviceMetrics::numColors() const

    Returns the number of different colors available for the paint
    device. Since this value is an int will not be sufficient to represent
    the number of colors on 32 bit displays, in which case INT_MAX is
    returned instead.
*/

/*!
    \fn int QPaintDeviceMetrics::depth() const

    Returns the bit depth (number of bit planes) of the paint device.
*/

/*!
    \fn int QPaintDeviceMetrics::logicalDpiX() const

    Returns the horizontal resolution of the device in dots per inch,
    which is used when computing font sizes. For X, this is usually
    the same as could be computed from widthMM(), but it varies on
    Windows.
*/

/*!
    \fn int QPaintDeviceMetrics::logicalDpiY() const

    Returns the vertical resolution of the device in dots per inch,
    which is used when computing font sizes. For X, this is usually
    the same as could be computed from heightMM(), but it varies on
    Windows.
*/

/*!
    \fn int QPaintDeviceMetrics::physicalDpiX() const
    \internal
*/
/*!
    \fn int QPaintDeviceMetrics::physicalDpiY() const
    \internal
*/

