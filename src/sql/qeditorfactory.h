/****************************************************************************
**
** Definition of QEditorFactory class
**
** Created : 2000-11-03
**
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the sql module of the Qt GUI Toolkit.
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

#ifndef QEDITORFACTORY_H
#define QEDITORFACTORY_H

#ifndef QT_H
#include "qobject.h"
#include "qvariant.h"
#endif // QT_H

#if !defined( QT_MODULE_SQL ) || defined( QT_LICENSE_PROFESSIONAL )
#define QM_EXPORT_SQL
#else
#define QM_EXPORT_SQL Q_EXPORT
#endif

#ifndef QT_NO_SQL_EDIT_WIDGETS

class QM_EXPORT_SQL QEditorFactory : public QObject
{
public:
    QEditorFactory ( QObject * parent = 0, const char * name = 0 );
    ~QEditorFactory();
    
    virtual QWidget * createEditor( QWidget * parent, const QVariant & v );
    
    static QEditorFactory * defaultFactory();
    static void installDefaultFactory( QEditorFactory * factory);

private:
#if defined(Q_DISABLE_COPY) // Disabled copy constructor and operator=
    QEditorFactory( const QEditorFactory & );
    QEditorFactory &operator=( const QEditorFactory & );
#endif
}; 

#endif // QT_NO_SQL
#endif // QEDITORFACTORY_H
