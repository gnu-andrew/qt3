/****************************************************************************
**
** ...
**
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the tools module of the Qt GUI Toolkit.
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

#ifndef QTHREADSTORAGE_H
#define QTHREADSTORAGE_H

#ifdef QT_THREAD_SUPPORT

#ifndef QT_H
#include "qglobal.h"
#endif // QT_H

class Q_EXPORT QThreadStorageData
{
public:
    QThreadStorageData( void (*func)(void *) );
    ~QThreadStorageData();

    void** get() const;
    void** set( void* p );

    static void finish( void** );
    int id;
};


template <class T>
class QThreadStorage
{
private:
    QThreadStorageData d;

#if defined(Q_DISABLE_COPY)
    // disable copy constructor and operator=
    QThreadStorage( const QThreadStorage & );
    QThreadStorage &operator=( const QThreadStorage & );
#endif // Q_DISABLE_COPY

    static void deleteData( void *x ) { delete (T)x; }

public:
    inline QThreadStorage() : d( deleteData ) {  }
    inline ~QThreadStorage() { }

    inline bool hasLocalData() const
    { return d.get() != 0; }

    inline T& localData()
    { void **v = d.get(); if ( !v ) v = d.set( 0 ); return *(T*)v; }

    inline T localData() const
    { void **v = d.get(); return ( v ? *(T*)v : 0 ); }

    inline void setLocalData( T t )
    { (void) d.set( t ); }
};

#endif // QT_THREAD_SUPPORT

#endif // QTHREADSTORAGE_H
