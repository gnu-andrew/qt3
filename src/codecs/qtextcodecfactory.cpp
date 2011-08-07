/****************************************************************************
**
** Implementation of QTextCodecFactory class
**
** Created : 010130
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

#include "qtextcodecfactory.h"

#ifndef QT_NO_TEXTCODEC

#ifndef QT_NO_COMPONENT
#include "qapplication.h"
#include "qcleanuphandler.h"
#include <private/qpluginmanager_p.h>
#include "qtextcodecinterface_p.h"

#ifdef QT_THREAD_SUPPORT
#  include <private/qmutexpool_p.h>
#endif // QT_THREAD_SUPPORT

#include <stdlib.h>


static QPluginManager<QTextCodecFactoryInterface> *manager = 0;
static QSingleCleanupHandler< QPluginManager<QTextCodecFactoryInterface> > cleanup_manager;

static void create_manager()
{
    if ( manager ) // already created
	return;

#ifdef QT_THREAD_SUPPORT
    // protect manager creation
    QMutexLocker locker( qt_global_mutexpool ?
			 qt_global_mutexpool->get( &manager ) : 0);

    // we check the manager pointer again to make sure that another thread
    // has not created the manager before us.

    if ( manager ) // already created
	return;
#endif

    manager =
	new QPluginManager<QTextCodecFactoryInterface>(IID_QTextCodecFactory,
						       QApplication::libraryPaths(), "/codecs",
						       FALSE);
    Q_CHECK_PTR( manager );
    cleanup_manager.set( &manager );
}

#endif // QT_NO_COMPONENT


QTextCodec *QTextCodecFactory::createForName(const QString &name)
{
    QTextCodec *codec = 0;

#ifndef QT_NO_COMPONENT

    // make sure the manager is created
    create_manager();

    QInterfacePtr<QTextCodecFactoryInterface> iface;
    manager->queryInterface(name, &iface );

    if (iface)
	codec = iface->createForName(name);

#endif // QT_NO_COMPONENT

    return codec;
}


QTextCodec *QTextCodecFactory::createForMib(int mib)
{
    QTextCodec *codec = 0;

#ifndef QT_NO_COMPONENT

    // make sure the manager is created
    create_manager();

    QInterfacePtr<QTextCodecFactoryInterface> iface;
    manager->queryInterface("MIB-" + QString::number(mib), &iface );

    if (iface)
	codec = iface->createForMib(mib);

#endif // QT_NO_COMPONENT

    return codec;
}


#endif // QT_NO_TEXTCODEC
