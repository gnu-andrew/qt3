/****************************************************************************
**
** Definition of QLocalFs class
**
** Created : 950429
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

#ifndef QLOCALFS_H
#define QLOCALFS_H

#ifndef QT_H
#include "qnetworkprotocol.h"
#include "qdir.h"
#endif // QT_H

#ifndef QT_NO_NETWORKPROTOCOL

class Q_EXPORT QLocalFs : public QNetworkProtocol
{
    Q_OBJECT

public:
    QLocalFs();
    virtual int supportedOperations() const;

protected:
    virtual void operationListChildren( QNetworkOperation *op );
    virtual void operationMkDir( QNetworkOperation *op );
    virtual void operationRemove( QNetworkOperation *op );
    virtual void operationRename( QNetworkOperation *op );
    virtual void operationGet( QNetworkOperation *op );
    virtual void operationPut( QNetworkOperation *op );

private:
    int calcBlockSize( int totalSize ) const;
    QDir dir;

};

#endif // QT_NO_NETWORKPROTOCOL

#endif // QLOCALFS_H
