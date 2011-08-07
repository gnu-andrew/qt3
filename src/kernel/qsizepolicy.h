/****************************************************************************
**
** Definition of the QSizePolicy class
**
** Created : 980929
**
** Copyright (C) 1998-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QSIZEPOLICY_H
#define QSIZEPOLICY_H

#ifndef QT_H
#include "qglobal.h"
#endif // QT_H

// Documentation is in qabstractlayout.cpp.

class Q_EXPORT QSizePolicy
{
private:
    enum SizePolicy_Internal { HSize = 6, HMask = 0x3f, VMask = HMask << HSize,
                               MayGrow = 1, ExpMask = 2, MayShrink = 4 };
public:
    enum SizeType { Fixed = 0,
		    Minimum = MayGrow,
		    Maximum = MayShrink,
		    Preferred = MayGrow | MayShrink,
		    MinimumExpanding = MayGrow | ExpMask,
		    Expanding = MayGrow | MayShrink | ExpMask,
		    Ignored = ExpMask /* magic value */ };

    enum ExpandData { NoDirection = 0,
		      Horizontally = 1,
		      Vertically = 2,
#ifndef QT_NO_COMPAT
		      Horizontal = Horizontally,
		      Vertical = Vertically,
#endif
		      BothDirections = Horizontally | Vertically };

    QSizePolicy() : data( 0 ) { }

    QSizePolicy( SizeType hor, SizeType ver, bool hfw = FALSE )
	: data( hor | (ver<<HSize) | (hfw ? (Q_UINT32)(1<<2*HSize) : 0) ) { }
    QSizePolicy( SizeType hor, SizeType ver, uchar hors, uchar vers, bool hfw = FALSE );

    SizeType horData() const { return (SizeType)( data & HMask ); }
    SizeType verData() const { return (SizeType)( (data & VMask) >> HSize ); }

    bool mayShrinkHorizontally() const { return horData() & MayShrink || horData() == Ignored; }
    bool mayShrinkVertically() const { return verData() & MayShrink || verData() == Ignored; }
    bool mayGrowHorizontally() const { return horData() & MayGrow || horData() == Ignored; }
    bool mayGrowVertically() const { return verData() & MayGrow || verData() == Ignored; }

    ExpandData expanding() const
    {
	return (ExpandData)( (int)(verData() & ExpMask ? Vertically : 0) |
			     (int)(horData() & ExpMask ? Horizontally : 0) );
    }

    void setHorData( SizeType d ) { data = (Q_UINT32)(data & ~HMask) | d; }
    void setVerData( SizeType d ) { data = (Q_UINT32)(data & ~(HMask << HSize)) |
					   (d << HSize); }

    void setHeightForWidth( bool b ) { data = b ? (Q_UINT32)( data | ( 1 << 2*HSize ) )
					      : (Q_UINT32)( data & ~( 1 << 2*HSize ) );  }
    bool hasHeightForWidth() const { return data & ( 1 << 2*HSize ); }

    bool operator==( const QSizePolicy& s ) const { return data == s.data; }
    bool operator!=( const QSizePolicy& s ) const { return data != s.data; }


    uint horStretch() const { return data >> 24; }
    uint verStretch() const { return (data >> 16) & 0xff; }
    void setHorStretch( uchar sf ) { data = (data&0x00ffffff) | (uint(sf)<<24); }
    void setVerStretch( uchar sf ) { data = (data&0xff00ffff) | (uint(sf)<<16); }
    inline void transpose();

private:
    QSizePolicy( int i ) : data( (Q_UINT32)i ) { }

    Q_UINT32 data;
};

inline QSizePolicy::QSizePolicy( SizeType hor, SizeType ver, uchar hors, uchar vers, bool hfw )
    : data( hor | (ver<<HSize) | (hfw ? (Q_UINT32)(1<<2*HSize) : 0) ) {
    setHorStretch( hors );
    setVerStretch( vers );
}

inline void QSizePolicy::transpose() {
    *this = QSizePolicy( verData(), horData(), verStretch(), horStretch(),
			 hasHeightForWidth() );
}

#endif // QSIZEPOLICY_H
