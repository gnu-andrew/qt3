/****************************************************************************
**
** Definition of QPaintDeviceMetrics class
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

#ifndef QPAINTDEVICEMETRICS_H
#define QPAINTDEVICEMETRICS_H

#ifndef QT_H
#include "qpaintdevice.h"
#endif // QT_H


class Q_EXPORT QPaintDeviceMetrics			// paint device metrics
{
public:
    QPaintDeviceMetrics( const QPaintDevice * );

    enum {
	PdmWidth = 1,
	PdmHeight,
	PdmWidthMM,
	PdmHeightMM,
	PdmNumColors,
	PdmDepth,
	PdmDpiX,
	PdmDpiY,
	PdmPhysicalDpiX,
	PdmPhysicalDpiY
    };

    int	  width()	const	{ return (int)pdev->metric(PdmWidth); }
    int	  height()	const	{ return (int)pdev->metric(PdmHeight); }
    int	  widthMM()	const	{ return (int)pdev->metric(PdmWidthMM); }
    int	  heightMM()	const	{ return (int)pdev->metric(PdmHeightMM); }
    int	  logicalDpiX()	const	{ return (int)pdev->metric(PdmDpiX); }
    int	  logicalDpiY()	const	{ return (int)pdev->metric(PdmDpiY); }
    int	  physicalDpiX()const	{ return (int)pdev->metric(PdmPhysicalDpiX); }
    int	  physicalDpiY()const	{ return (int)pdev->metric(PdmPhysicalDpiY); }
    int	  numColors()	const	{ return (int)pdev->metric(PdmNumColors); }
    int	  depth()	const	{ return (int)pdev->metric(PdmDepth); }

private:
    QPaintDevice *pdev;
};


#endif // QPAINTDEVICEMETRICS_H
