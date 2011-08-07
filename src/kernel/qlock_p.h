/****************************************************************************
**
** Definition of QLock class. This manages interprocess locking
**
** Created : 20000406
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
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with
** the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#ifndef QLOCK_P_H
#define QLOCK_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  This header file may
// change from version to version without notice, or even be
// removed.
//
// We mean it.
//
//

#ifndef QT_H
#include "qstring.h"
#endif // QT_H

class QLockData;

class QLock
{
public:
    QLock( const QString &filename, char id, bool create = FALSE );
    ~QLock();

    enum Type { Read, Write };

    bool isValid() const;
    void lock( Type type );
    void unlock();
    bool locked() const;

private:
    Type type;
    QLockData *data;
};


// Nice class for ensuring the lock is released.
// Just create one on the stack and the lock is automatically released
// when QLockHolder is destructed.
class QLockHolder
{
public:
    QLockHolder( QLock *l, QLock::Type type ) : qlock(l) {
	qlock->lock( type );
    }
    ~QLockHolder() { if ( locked() ) qlock->unlock(); }

    void lock( QLock::Type type ) { qlock->lock( type ); }
    void unlock() { qlock->unlock(); }
    bool locked() const { return qlock->locked(); }

private:
    QLock *qlock;
};

#endif

