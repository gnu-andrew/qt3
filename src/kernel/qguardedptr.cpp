/****************************************************************************
**
** Implementation of QGuardedPtr class
**
** Created : 990929
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

#include "qguardedptr.h"

/*!
    \class QGuardedPtr qguardedptr.h
    \brief The QGuardedPtr class is a template class that provides guarded pointers to QObjects.

    \ingroup objectmodel
    \mainclass

    A guarded pointer, \c{QGuardedPtr<X>}, behaves like a normal C++
    pointer \c{X*}, except that it is automatically set to 0 when
    the referenced object is destroyed (unlike normal C++ pointers,
    which become "dangling pointers" in such cases). \c X must be a
    subclass of QObject.

    Guarded pointers are useful whenever you need to store a pointer
    to a QObject that is owned by someone else and therefore might be
    destroyed while you still hold a reference to it. You can safely
    test the pointer for validity.

    Example:
    \code
	QGuardedPtr<QLabel> label = new QLabel( 0, "label" );
	label->setText( "I like guarded pointers" );

	delete (QLabel*) label; // simulate somebody destroying the label

	if ( label)
	    label->show();
	else
	    qDebug("The label has been destroyed");
    \endcode

    The program will output \c{The label has been destroyed} rather
    than dereferencing an invalid address in \c label->show().

    The functions and operators available with a QGuardedPtr are the
    same as those available with a normal unguarded pointer, except
    the pointer arithmetic operators (++, --, -, and +), which are
    normally used only with arrays of objects. Use them like normal
    pointers and you will not need to read this class documentation.

    For creating guarded pointers, you can construct or assign to them
    from an X* or from another guarded pointer of the same type. You
    can compare them with each other using operator==() and
    operator!=(), or test for 0 with isNull(). And you can dereference
    them using either the \c *x or the \c x->member notation.

    A guarded pointer will automatically cast to an X*, so you can
    freely mix guarded and unguarded pointers. This means that if you
    have a QGuardedPtr<QWidget>, you can pass it to a function that
    requires a QWidget*. For this reason, it is of little value to
    declare functions to take a QGuardedPtr as a parameter; just use
    normal pointers. Use a QGuardedPtr when you are storing a pointer
    over time.

    Note again that class \e X must inherit QObject, or a compilation
    or link error will result.
*/

/*!
    \fn QGuardedPtr::QGuardedPtr()

    Constructs a 0 guarded pointer.

    \sa isNull()
*/

/*!
    \fn QGuardedPtr::QGuardedPtr( T* p )

    Constructs a guarded pointer that points to same object as \a p
    points to.
*/

/*!
    \fn QGuardedPtr::QGuardedPtr(const QGuardedPtr<T> &p)

    Copy one guarded pointer from another. The constructed guarded
    pointer points to the same object that \a p points to (which may
    be 0).
*/

/*!
    \fn QGuardedPtr::~QGuardedPtr()

    Destroys the guarded pointer. Just like a normal pointer,
    destroying a guarded pointer does \e not destroy the object being
    pointed to.
*/

/*!
    \fn QGuardedPtr<T>& QGuardedPtr::operator=(const QGuardedPtr<T> &p)

    Assignment operator. This guarded pointer then points to the same
    object as \a p points to.
*/

/*!
    \overload QGuardedPtr<T> & QGuardedPtr::operator=(T* p)

    Assignment operator. This guarded pointer then points to the same
    object as \a p points to.
*/

/*!
    \fn bool QGuardedPtr::operator==( const QGuardedPtr<T> &p ) const

    Equality operator; implements traditional pointer semantics.
    Returns TRUE if both \a p and this guarded pointer are 0, or if
    both \a p and this pointer point to the same object; otherwise
    returns FALSE.

    \sa operator!=()
*/

/*!
    \fn bool QGuardedPtr::operator!= ( const QGuardedPtr<T>& p ) const

    Inequality operator; implements pointer semantics, the negation of
    operator==(). Returns TRUE if \a p and this guarded pointer are
    not pointing to the same object; otherwise returns FALSE.
*/

/*!
    \fn bool QGuardedPtr::isNull() const

    Returns \c TRUE if the referenced object has been destroyed or if
    there is no referenced object; otherwise returns FALSE.
*/

/*!
    \fn T* QGuardedPtr::operator->() const

    Overloaded arrow operator; implements pointer semantics. Just use
    this operator as you would with a normal C++ pointer.
*/

/*!
    \fn T& QGuardedPtr::operator*() const

    Dereference operator; implements pointer semantics. Just use this
    operator as you would with a normal C++ pointer.
*/

/*!
    \fn QGuardedPtr::operator T*() const

    Cast operator; implements pointer semantics. Because of this
    function you can pass a QGuardedPtr\<X\> to a function where an X*
    is required.
*/


/* Internal classes */


QGuardedPtrPrivate::QGuardedPtrPrivate( QObject* o)
    : QObject(0, "_ptrpriv" ), obj( o )
{
    if ( obj )
	connect( obj, SIGNAL( destroyed() ), this, SLOT( objectDestroyed() ) );
}


QGuardedPtrPrivate::~QGuardedPtrPrivate()
{
}

void QGuardedPtrPrivate::reconnect( QObject *o )
{
    if ( obj == o )
	return;
    if ( obj )
	disconnect( obj, SIGNAL( destroyed() ),
		    this, SLOT( objectDestroyed() ) );
    obj = o;
    if ( obj )
	connect( obj, SIGNAL( destroyed() ),
		 this, SLOT( objectDestroyed() ) );
}

void QGuardedPtrPrivate::objectDestroyed()
{
    obj = 0;
}
