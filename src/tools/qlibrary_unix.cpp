/****************************************************************************
**
** Implementation of QLibraryPrivate class
**
** Created : 000101
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
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

#include "qplatformdefs.h"
#include "private/qlibrary_p.h"

#ifndef QT_NO_LIBRARY

#if defined(QT_AOUT_UNDERSCORE)
#include <string.h>
#endif

/*
  The platform dependent implementations of
  - loadLibrary
  - freeLibrary
  - resolveSymbol

  It's not too hard to guess what the functions do.
*/

#if defined(QT_HPUX_LD) // for HP-UX < 11.x and 32 bit

bool QLibraryPrivate::loadLibrary()
{
    if ( pHnd )
	return TRUE;

    QString filename = library->library();

    pHnd = (void*)shl_load( filename.latin1(), BIND_DEFERRED | BIND_NONFATAL | DYNAMIC_PATH, 0 );
#if defined(QT_DEBUG) || defined(QT_DEBUG_COMPONENT)
    if ( !pHnd )
	qWarning( "%s: failed to load library!", filename.latin1() );
#endif
    return pHnd != 0;
}

bool QLibraryPrivate::freeLibrary()
{
    if ( !pHnd )
	return TRUE;

    if ( shl_unload( (shl_t)pHnd ) ) {
#if defined(QT_DEBUG) || defined(QT_DEBUG_COMPONENT)
	QString filename = library->library();
	qWarning( "%s: Failed to unload library!", filename.latin1() );
#endif
	return FALSE;
    }
    pHnd = 0;
    return TRUE;
}

void* QLibraryPrivate::resolveSymbol( const char* symbol )
{
    if ( !pHnd )
	return 0;

    void* address = 0;
    if ( shl_findsym( (shl_t*)&pHnd, symbol, TYPE_UNDEFINED, &address ) < 0 ) {
#if defined(QT_DEBUG_COMPONENT)
	QString filename = library->library();
	qWarning( "%s: couldn't resolve symbol \"%s\"", filename.latin1(), symbol );
#endif
    }
    return address;
}

#else // POSIX
#include <dlfcn.h>
#ifndef DL_PREFIX //for mac dlcompat
#  define DL_PREFIX(x) x
#endif

bool QLibraryPrivate::loadLibrary()
{
    if ( pHnd )
	return TRUE;

    QString filename = library->library();

    pHnd = DL_PREFIX(dlopen)( filename.latin1(), RTLD_LAZY );
#if defined(QT_DEBUG) || defined(QT_DEBUG_COMPONENT)
    if ( !pHnd )
	qWarning( "%s", DL_PREFIX(dlerror)() );
#endif
    return pHnd != 0;
}

bool QLibraryPrivate::freeLibrary()
{
    if ( !pHnd )
	return TRUE;

    if ( DL_PREFIX(dlclose)( pHnd ) ) {
#if defined(QT_DEBUG) || defined(QT_DEBUG_COMPONENT)
	qWarning( "%s", DL_PREFIX(dlerror)() );
#endif
	return FALSE;
    }

    pHnd = 0;
    return TRUE;
}

void* QLibraryPrivate::resolveSymbol( const char* symbol )
{
    if ( !pHnd )
	return 0;

#if defined(QT_AOUT_UNDERSCORE)
    // older a.out systems add an underscore in front of symbols
    char* undrscr_symbol = new char[strlen(symbol)+2];
    undrscr_symbol[0] = '_';
    strcpy(undrscr_symbol+1, symbol);
    void* address = DL_PREFIX(dlsym)( pHnd, undrscr_symbol );
    delete [] undrscr_symbol;
#else
    void* address = DL_PREFIX(dlsym)( pHnd, symbol );
#endif
#if defined(QT_DEBUG_COMPONENT)
    const char* error = DL_PREFIX(dlerror)();
    if ( error )
	qWarning( "%s", error );
#endif
    return address;
}

#endif // POSIX

#endif
