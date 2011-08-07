/****************************************************************************
**
** Implementation of QObjectCleanupHandler class
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
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

#include "qobjectcleanuphandler.h"
#include "qobjectlist.h"

/*!
    \class QObjectCleanupHandler qobjectcleanuphandler.h
    \brief The QObjectCleanupHandler class watches the lifetime of multiple QObjects.

    \ingroup objectmodel

    A QObjectCleanupHandler is useful whenever you need to know when a
    number of \l{QObject}s that are owned by someone else have been
    deleted. This is important, for example, when referencing memory
    in an application that has been allocated in a shared library.

    Example:

    \code
    class FactoryComponent : public FactoryInterface, public QLibraryInterface
    {
    public:
	...

	QObject *createObject();

	bool init();
	void cleanup();
	bool canUnload() const;

    private:
	QObjectCleanupHandler objects;
    };

    // allocate a new object, and add it to the cleanup handler
    QObject *FactoryComponent::createObject()
    {
	return objects.add( new QObject() );
    }

    // QLibraryInterface implementation
    bool FactoryComponent::init()
    {
	return TRUE;
    }

    void FactoryComponent::cleanup()
    {
    }

    // it is only safe to unload the library when all QObject's have been destroyed
    bool FactoryComponent::canUnload() const
    {
	return objects.isEmpty();
    }
    \endcode
*/

/*!
    Constructs an empty QObjectCleanupHandler.
*/
QObjectCleanupHandler::QObjectCleanupHandler()
: QObject(), cleanupObjects( 0 )
{
}

/*!
    Destroys the cleanup handler. All objects in this cleanup handler
    will be deleted.
*/
QObjectCleanupHandler::~QObjectCleanupHandler()
{
    clear();
}

/*!
    Adds \a object to this cleanup handler and returns the pointer to
    the object.
*/
QObject* QObjectCleanupHandler::add( QObject* object )
{
    if ( !object )
	return 0;

    if ( !cleanupObjects ) {
	cleanupObjects = new QObjectList;
 	cleanupObjects->setAutoDelete( TRUE );
    }
    connect( object, SIGNAL(destroyed(QObject*)), this, SLOT(objectDestroyed(QObject*)) );
    cleanupObjects->insert( 0, object );
    return object;
}

/*!
    Removes the \a object from this cleanup handler. The object will
    not be destroyed.
*/
void QObjectCleanupHandler::remove( QObject *object )
{
    if ( !cleanupObjects )
	return;
    if ( cleanupObjects->findRef( object ) >= 0 ) {
	(void) cleanupObjects->take();
	disconnect( object, SIGNAL(destroyed(QObject*)), this, SLOT(objectDestroyed(QObject*)) );
    }
}

/*!
    Returns TRUE if this cleanup handler is empty or if all objects in
    this cleanup handler have been destroyed; otherwise return FALSE.
*/
bool QObjectCleanupHandler::isEmpty() const
{
    return cleanupObjects ? cleanupObjects->isEmpty() : TRUE;
}

/*!
    Deletes all objects in this cleanup handler. The cleanup handler
    becomes empty.
*/
void QObjectCleanupHandler::clear()
{
    delete cleanupObjects;
    cleanupObjects = 0;
}

void QObjectCleanupHandler::objectDestroyed( QObject*object )
{
    if ( cleanupObjects )
	cleanupObjects->setAutoDelete( FALSE );

    remove( object );

    if ( cleanupObjects )
	cleanupObjects->setAutoDelete( TRUE );
}
