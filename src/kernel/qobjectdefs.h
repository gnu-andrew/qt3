/****************************************************************************
**
** Macros and definitions related to QObject
**
** Created : 930419
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

#ifndef QOBJECTDEFS_H
#define QOBJECTDEFS_H

#ifndef QT_H
#include "qglobal.h"
#endif // QT_H


#ifndef QT_NO_TRANSLATION
# ifndef QT_NO_TEXTCODEC
// full set of tr functions
#  define QT_TR_FUNCTIONS \
    static QString tr( const char *, const char * = 0 ); \
    static QString trUtf8( const char *, const char * = 0 );
# else
// no QTextCodec, no utf8
#  define QT_TR_FUNCTIONS \
    static QString tr( const char *, const char * = 0 );
# endif
#else
// inherit the ones from QObject
# define QT_TR_FUNCTIONS
#endif

#ifndef QT_NO_PROPERTIES
# define QT_PROP_FUNCTIONS \
    virtual bool qt_property( int id, int f, QVariant* v); \
    static bool qt_static_property( QObject* , int, int, QVariant* );
#else
# define QT_PROP_FUNCTIONS
#endif

// The following macros are our "extensions" to C++
// They are used, strictly speaking, only by the moc.
struct QUObject;

#ifdef QT_MOC_CPP
#define slots			    slots
#define signals		    signals
#define Q_CLASSINFO( name, value ) Q_CLASSINFO( name, value )
#define Q_PROPERTY( text )	    Q_PROPERTY( text )
#define Q_OVERRIDE( text )	    Q_OVERRIDE( text )
#define Q_ENUMS( x )		    Q_ENUMS( x )
#define Q_SETS( x )		    Q_SETS( x )
 /* tmake ignore Q_OBJECT */
#define Q_OBJECT		    Q_OBJECT
 /* tmake ignore Q_OBJECT */
#define Q_OBJECT_FAKE		    Q_OBJECT_FAKE

#else
#define slots					// slots: in class
#define signals protected			// signals: in class
#ifndef QT_NO_EMIT
#define emit					// emit signal
#endif
#define Q_CLASSINFO( name, value )		// class info
#define Q_PROPERTY( text )			// property
#define Q_OVERRIDE( text )			// override property
#define Q_ENUMS( x )
#define Q_SETS( x )

/* tmake ignore Q_OBJECT */
#define Q_OBJECT							\
public:									\
    virtual QMetaObject *metaObject() const { 				\
         return staticMetaObject();					\
    }									\
    virtual const char *className() const;				\
    virtual void* qt_cast( const char* ); 				\
    virtual bool qt_invoke( int, QUObject* ); 				\
    virtual bool qt_emit( int, QUObject* ); 				\
    QT_PROP_FUNCTIONS							\
    static QMetaObject* staticMetaObject();				\
    QObject* qObject() { return (QObject*)this; } 			\
    QT_TR_FUNCTIONS							\
private:								\
    static QMetaObject *metaObj;

/* tmake ignore Q_OBJECT */
#define Q_OBJECT_FAKE Q_OBJECT

#endif

// macro for naming members
#ifdef METHOD
#undef METHOD
#endif
#ifdef SLOT
#undef SLOT
#endif
#ifdef SIGNAL
#undef SIGNAL
#endif

#if defined(_OLD_CPP_)
#define METHOD(a)	"0""a"
#define SLOT(a)		"1""a"
#define SIGNAL(a)	"2""a"
#else
#define METHOD(a)	"0"#a
#define SLOT(a)		"1"#a
#define SIGNAL(a)	"2"#a
#endif

#ifndef QT_CLEAN_NAMESPACE
#define METHOD_CODE	0			// member type codes
#define SLOT_CODE	1
#define SIGNAL_CODE	2
#endif

#define QMETHOD_CODE	0			// member type codes
#define QSLOT_CODE	1
#define QSIGNAL_CODE	2

class QObject;
class QMetaObject;
class QSignal;
class QConnection;
class QEvent;
struct QMetaData;
class QConnectionList;
class QConnectionListIt;
class QSignalVec;
class QObjectList;
class QObjectListIt;
class QMemberDict;

Q_EXPORT void *qt_find_obj_child( QObject *, const char *, const char * );
#define Q_CHILD(parent,type,name) \
	((type*)qt_find_obj_child(parent,#type,name))

Q_EXPORT void *qt_inheritedBy( QMetaObject *super, const QObject *cls );

template <typename T>
Q_INLINE_TEMPLATES T qt_cast(const QObject *object)
{ return (T)qt_inheritedBy( ((T)0)->staticMetaObject(), object ); }
#endif // QOBJECTDEFS_H
