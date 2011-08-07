/****************************************************************************
**
** Definition of QUrlInfo class
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

#ifndef QURLINFO_H
#define QURLINFO_H

#ifndef QT_H
#include "qdatetime.h"
#include "qstring.h"
#if defined(QT_ABI_QT4)
#include "qiodevice.h"
#endif
#endif // QT_H

class QUrlOperator;
class QUrl;
class QUrlInfoPrivate;

class Q_EXPORT QUrlInfo
{
public:
    enum PermissionSpec {
	ReadOwner = 00400, WriteOwner = 00200, ExeOwner = 00100,
	ReadGroup = 00040, WriteGroup = 00020, ExeGroup = 00010,
	ReadOther = 00004, WriteOther = 00002, ExeOther = 00001 };

    QUrlInfo();
    QUrlInfo( const QUrlOperator &path, const QString &file );
    QUrlInfo( const QUrlInfo &ui );
#if (QT_VERSION-0 >= 0x040000)
#error "QUrlInfo::QUrlInfo() should accept QIODevice::Offset instead of uint"
#elif defined(QT_ABI_QT4)
    QUrlInfo( const QString &name, int permissions, const QString &owner,
	      const QString &group, QIODevice::Offset size, const QDateTime &lastModified,
	      const QDateTime &lastRead, bool isDir, bool isFile, bool isSymLink,
	      bool isWritable, bool isReadable, bool isExecutable );
    QUrlInfo( const QUrl &url, int permissions, const QString &owner,
	      const QString &group, QIODevice::Offset size, const QDateTime &lastModified,
	      const QDateTime &lastRead, bool isDir, bool isFile, bool isSymLink,
	      bool isWritable, bool isReadable, bool isExecutable );
#else
    QUrlInfo( const QString &name, int permissions, const QString &owner,
	      const QString &group, uint size, const QDateTime &lastModified,
	      const QDateTime &lastRead, bool isDir, bool isFile, bool isSymLink,
	      bool isWritable, bool isReadable, bool isExecutable );
    QUrlInfo( const QUrl &url, int permissions, const QString &owner,
	      const QString &group, uint size, const QDateTime &lastModified,
	      const QDateTime &lastRead, bool isDir, bool isFile, bool isSymLink,
	      bool isWritable, bool isReadable, bool isExecutable );
#endif
    QUrlInfo &operator=( const QUrlInfo &ui );
    virtual ~QUrlInfo();

    virtual void setName( const QString &name );
    virtual void setDir( bool b );
    virtual void setFile( bool b );
    virtual void setSymLink( bool b );
    virtual void setOwner( const QString &s );
    virtual void setGroup( const QString &s );
#if (QT_VERSION-0 >= 0x040000)
#error "QUrlInfo::setSize() should accept QIODevice::Offset instead of uint"
#elif defined(QT_ABI_QT4)
    virtual void setSize( QIODevice::Offset size );
#else
    virtual void setSize( uint size );
#endif
    virtual void setWritable( bool b );
    virtual void setReadable( bool b );
    virtual void setPermissions( int p );
    virtual void setLastModified( const QDateTime &dt );

    bool isValid() const;

    QString name() const;
    int permissions() const;
    QString owner() const;
    QString group() const;
#if (QT_VERSION-0 >= 0x040000)
#error "QUrlInfo::size() should return QIODevice::Offset instead of uint"
#elif defined(QT_ABI_QT4)
    QIODevice::Offset size() const;
#else
    uint size() const;
#endif
    QDateTime lastModified() const;
    QDateTime lastRead() const;
    bool isDir() const;
    bool isFile() const;
    bool isSymLink() const;
    bool isWritable() const;
    bool isReadable() const;
    bool isExecutable() const;

    static bool greaterThan( const QUrlInfo &i1, const QUrlInfo &i2,
			     int sortBy );
    static bool lessThan( const QUrlInfo &i1, const QUrlInfo &i2,
			  int sortBy );
    static bool equal( const QUrlInfo &i1, const QUrlInfo &i2,
		       int sortBy );

    bool operator==( const QUrlInfo &i ) const;
private:
    QUrlInfoPrivate *d;

};

#endif
