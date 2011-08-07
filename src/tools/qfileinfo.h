/****************************************************************************
**
** Definition of QFileInfo class
**
** Created : 950628
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

#ifndef QFILEINFO_H
#define QFILEINFO_H

#ifndef QT_H
#include "qfile.h"
#include "qdatetime.h"
#endif // QT_H


class QDir;
struct QFileInfoCache;
template <class T> class QDeepCopy;


class Q_EXPORT QFileInfo
{
public:
    enum PermissionSpec {
	ReadOwner = 04000, WriteOwner = 02000, ExeOwner = 01000,
	ReadUser  = 00400, WriteUser  = 00200, ExeUser  = 00100,
	ReadGroup = 00040, WriteGroup = 00020, ExeGroup = 00010,
	ReadOther = 00004, WriteOther = 00002, ExeOther = 00001 };

    QFileInfo();
    QFileInfo( const QString &file );
    QFileInfo( const QFile & );
#ifndef QT_NO_DIR
    QFileInfo( const QDir &, const QString &fileName );
#endif
    QFileInfo( const QFileInfo & );
   ~QFileInfo();

    QFileInfo  &operator=( const QFileInfo & );

    void	setFile( const QString &file );
    void	setFile( const QFile & );
#ifndef QT_NO_DIR
    void	setFile( const QDir &, const QString &fileName );
#endif
    bool	exists()	const;
    void	refresh()	const;
    bool	caching()	const;
    void	setCaching( bool );

    QString	filePath()	const;
    QString	fileName()	const;
#ifndef QT_NO_DIR //###
    QString	absFilePath()	const;
#endif
    QString	baseName( bool complete = FALSE ) const;
    QString	extension( bool complete = TRUE ) const;

#ifndef QT_NO_DIR //###
    QString	dirPath( bool absPath = FALSE ) const;
#endif
#ifndef QT_NO_DIR
    QDir	dir( bool absPath = FALSE )	const;
#endif
    bool	isReadable()	const;
    bool	isWritable()	const;
    bool	isExecutable()	const;
    bool 	isHidden()      const;

#ifndef QT_NO_DIR //###
    bool	isRelative()	const;
    bool	convertToAbs();
#endif

    bool	isFile()	const;
    bool	isDir()		const;
    bool	isSymLink()	const;

    QString	readLink()	const;

    QString	owner()		const;
    uint	ownerId()	const;
    QString	group()		const;
    uint	groupId()	const;

    bool	permission( int permissionSpec ) const;

#if (QT_VERSION-0 >= 0x040000)
#error "QFileInfo::size() should return QIODevice::Offset instead of uint"
#elif defined(QT_ABI_QT4)
    QIODevice::Offset size()	const;
#else
    uint	size()		const;
#endif

    QDateTime	created()	const;
    QDateTime	lastModified()	const;
    QDateTime	lastRead()	const;

private:
    void	doStat() const;
    static void slashify( QString & );
    static void makeAbs( QString & );

    QString	fn;
    QFileInfoCache *fic;
    bool	cache;
#if defined(Q_OS_UNIX)
    bool        symLink;
#endif

    void detach();
    friend class QDeepCopy< QFileInfo >;
};


inline bool QFileInfo::caching() const
{
    return cache;
}


#endif // QFILEINFO_H
