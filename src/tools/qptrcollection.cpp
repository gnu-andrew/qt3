/****************************************************************************
**
** Implementation of base class for all pointer based collection classes
**
** Created : 920820
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
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

#include "qptrcollection.h"

/*!
    \class QPtrCollection qptrcollection.h
    \reentrant
    \brief The QPtrCollection class is the base class of most pointer-based Qt collections.

    \ingroup collection
    \ingroup tools

    The QPtrCollection class is an abstract base class for the Qt
    \link collection.html collection classes\endlink QDict, QPtrList,
    etc. Qt also includes value based collections, e.g. QValueList,
    QMap, etc.

    A QPtrCollection only knows about the number of objects in the
    collection and the deletion strategy (see setAutoDelete()).

    A collection is implemented using the \c Item (generic collection
    item) type, which is a \c void*. The template classes that create
    the real collections cast the \c Item to the required type.
*/


/*!
    \enum QPtrCollection::Item

    This type is the generic "item" in a QPtrCollection.
*/


/*!
    \fn QPtrCollection::QPtrCollection()

    Constructs a collection. The constructor is protected because
    QPtrCollection is an abstract class.
*/

/*!
    \fn QPtrCollection::QPtrCollection( const QPtrCollection & source )

    Constructs a copy of \a source with autoDelete() set to FALSE. The
    constructor is protected because QPtrCollection is an abstract
    class.

    Note that if \a source has autoDelete turned on, copying it will
    risk memory leaks, reading freed memory, or both.
*/

/*!
    \fn QPtrCollection::~QPtrCollection()

    Destroys the collection. The destructor is protected because
    QPtrCollection is an abstract class.
*/


/*!
    \fn bool QPtrCollection::autoDelete() const

    Returns the setting of the auto-delete option. The default is FALSE.

    \sa setAutoDelete()
*/

/*!
    \fn void QPtrCollection::setAutoDelete( bool enable )

    Sets the collection to auto-delete its contents if \a enable is
    TRUE and to never delete them if \a enable is FALSE.

    If auto-deleting is turned on, all the items in a collection are
    deleted when the collection itself is deleted. This is convenient
    if the collection has the only pointer to the items.

    The default setting is FALSE, for safety. If you turn it on, be
    careful about copying the collection - you might find yourself
    with two collections deleting the same items.

    Note that the auto-delete setting may also affect other functions
    in subclasses. For example, a subclass that has a remove()
    function will remove the item from its data structure, and if
    auto-delete is enabled, will also delete the item.

    \sa autoDelete()
*/


/*!
    \fn virtual uint QPtrCollection::count() const

    Returns the number of objects in the collection.
*/

/*!
    \fn virtual void QPtrCollection::clear()

    Removes all objects from the collection. The objects will be
    deleted if auto-delete has been enabled.

    \sa setAutoDelete()
*/

/*!
    \fn void QPtrCollection::deleteItem( Item d )

    Reimplement this function if you want to be able to delete items.

    Deletes an item that is about to be removed from the collection.

    This function has to reimplemented in the collection template
    classes, and should \e only delete item \a d if auto-delete has
    been enabled.

    \warning If you reimplement this function you must also
    reimplement the destructor and call the virtual function clear()
    from your destructor. This is due to the way virtual functions and
    destructors work in C++: Virtual functions in derived classes
    cannot be called from a destructor. If you do not do this, your
    deleteItem() function will not be called when the container is
    destroyed.

    \sa newItem(), setAutoDelete()
*/

/*!
    Virtual function that creates a copy of an object that is about to
    be inserted into the collection.

    The default implementation returns the \a d pointer, i.e. no copy
    is made.

    This function is seldom reimplemented in the collection template
    classes. It is not common practice to make a copy of something
    that is being inserted.

    \sa deleteItem()
*/

QPtrCollection::Item QPtrCollection::newItem( Item d )
{
    return d;					// just return reference
}
