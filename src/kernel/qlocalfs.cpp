/****************************************************************************
**
** Implementation of QLocalFs class
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

#include "qlocalfs.h"

#ifndef QT_NO_NETWORKPROTOCOL

#include "qfileinfo.h"
#include "qfile.h"
#include "qurlinfo.h"
#include "qapplication.h"
#include "qurloperator.h"
#include "qguardedptr.h"

//#define QLOCALFS_DEBUG


/*!
    \class QLocalFs qlocalfs.h
    \brief The QLocalFs class is an implementation of a
    QNetworkProtocol that works on the local file system.
\if defined(commercial)
    It is part of the <a href="commercialeditions.html">Qt Enterprise Edition</a>.
\endif

    \module network

    \ingroup io

    This class is derived from QNetworkProtocol. QLocalFs is not
    normally used directly, but rather through a QUrlOperator, for
    example:
    \code
    QUrlOperator op( "file:///tmp" );
    op.listChildren(); // Asks the server to provide a directory listing
    \endcode

    This code will only work if the QLocalFs class is registered; to
    register the class, you must call qInitNetworkProtocols() before
    using a QUrlOperator with QLocalFs.

    If you really need to use QLocalFs directly, don't forget
    to set its QUrlOperator with setUrl().

    \sa \link network.html Qt Network Documentation \endlink QNetworkProtocol, QUrlOperator
*/

/*!
    Constructor.
*/

QLocalFs::QLocalFs()
    : QNetworkProtocol()
{
}

static int convertPermissions(QFileInfo *fi)
{
    int p = 0;
    if ( fi->permission( QFileInfo::ReadOwner ) )
	p |= QUrlInfo::ReadOwner;
    if ( fi->permission( QFileInfo::WriteOwner ) )
	p |= QUrlInfo::WriteOwner;
    if ( fi->permission( QFileInfo::ExeOwner ) )
	p |= QUrlInfo::ExeOwner;
    if ( fi->permission( QFileInfo::ReadGroup ) )
	p |= QUrlInfo::ReadGroup;
    if ( fi->permission( QFileInfo::WriteGroup ) )
	p |= QUrlInfo::WriteGroup;
    if ( fi->permission( QFileInfo::ExeGroup ) )
	p |= QUrlInfo::ExeGroup;
    if ( fi->permission( QFileInfo::ReadOther ) )
	p |= QUrlInfo::ReadOther;
    if ( fi->permission( QFileInfo::WriteOther ) )
	p |= QUrlInfo::WriteOther;
    if ( fi->permission( QFileInfo::ExeOther ) )
	p |= QUrlInfo::ExeOther;
    return p;
}

/*!
    \reimp
*/

void QLocalFs::operationListChildren( QNetworkOperation *op )
{
#ifdef QLOCALFS_DEBUG
    qDebug( "QLocalFs: operationListChildren" );
#endif
    op->setState( StInProgress );

    dir = QDir( url()->path() );
    dir.setNameFilter( url()->nameFilter() );
    dir.setMatchAllDirs( TRUE );
    if ( !dir.isReadable() ) {
	QString msg = tr( "Could not read directory\n%1" ).arg( url()->path() );
	op->setState( StFailed );
	op->setProtocolDetail( msg );
	op->setErrorCode( (int)ErrListChildren );
	emit finished( op );
	return;
    }

    const QFileInfoList *filist = dir.entryInfoList( QDir::All | QDir::Hidden | QDir::System );
    if ( !filist ) {
	QString msg = tr( "Could not read directory\n%1" ).arg( url()->path() );
	op->setState( StFailed );
	op->setProtocolDetail( msg );
	op->setErrorCode( (int)ErrListChildren );
	emit finished( op );
	return;
    }

    emit start( op );

    QFileInfoListIterator it( *filist );
    QFileInfo *fi;
    QValueList<QUrlInfo> infos;
    while ( ( fi = it.current() ) != 0 ) {
	++it;
	infos << QUrlInfo( fi->fileName(), convertPermissions(fi), fi->owner(), fi->group(),
			   fi->size(), fi->lastModified(), fi->lastRead(), fi->isDir(), fi->isFile(),
			   fi->isSymLink(), fi->isWritable(), fi->isReadable(), fi->isExecutable() );
    }
    emit newChildren( infos, op );
    op->setState( StDone );
    emit finished( op );
}

/*!
    \reimp
*/

void QLocalFs::operationMkDir( QNetworkOperation *op )
{
#ifdef QLOCALFS_DEBUG
    qDebug( "QLocalFs: operationMkDir" );
#endif
    op->setState( StInProgress );
    QString dirname = op->arg( 0 );

    dir = QDir( url()->path() );
    if ( dir.mkdir( dirname ) ) {
	QFileInfo fi( dir, dirname );
	QUrlInfo inf( fi.fileName(), convertPermissions(&fi), fi.owner(), fi.group(),
		      fi.size(), fi.lastModified(), fi.lastRead(), fi.isDir(), fi.isFile(),
		      fi.isSymLink(), fi.isWritable(), fi.isReadable(), fi.isExecutable() );
	emit newChild( inf, op );
	op->setState( StDone );
	emit createdDirectory( inf, op );
	emit finished( op );
    } else {
	QString msg = tr( "Could not create directory\n%1" ).arg( dirname );
	op->setState( StFailed );
	op->setProtocolDetail( msg );
	op->setErrorCode( (int)ErrMkDir );
	emit finished( op );
    }
}

/*!
    \reimp
*/

void QLocalFs::operationRemove( QNetworkOperation *op )
{
#ifdef QLOCALFS_DEBUG
    qDebug( "QLocalFs: operationRemove" );
#endif
    op->setState( StInProgress );
    QString name = QUrl( op->arg( 0 ) ).path();
    bool deleted = FALSE;

    dir = QDir( url()->path() );

    QFileInfo fi( dir, name );
    if ( fi.isDir() ) {
	if ( dir.rmdir( name ) )
	    deleted = TRUE;
    }

    if ( deleted || dir.remove( name ) ) {
	op->setState( StDone );
	emit removed( op );
	emit finished( op );
    } else {
	QString msg = tr( "Could not remove file or directory\n%1" ).arg( name );
	op->setState( StFailed );
	op->setProtocolDetail( msg );
	op->setErrorCode( (int)ErrRemove );
	emit finished( op );
    }
}

/*!
    \reimp
*/

void QLocalFs::operationRename( QNetworkOperation *op )
{
#ifdef QLOCALFS_DEBUG
    qDebug( "QLocalFs: operationRename" );
#endif
    op->setState( StInProgress );
    QString oldname = op->arg( 0 );
    QString newname = op->arg( 1 );

    dir = QDir( url()->path() );
    if ( dir.rename( oldname, newname ) ) {
	op->setState( StDone );
	emit itemChanged( op );
	emit finished( op );
    } else {
	QString msg = tr( "Could not rename\n%1\nto\n%2" ).arg( oldname ).arg( newname );
	op->setState( StFailed );
	op->setProtocolDetail( msg );
	op->setErrorCode( (int)ErrRename );
	emit finished( op );
    }
}

/*!
    \reimp
*/

void QLocalFs::operationGet( QNetworkOperation *op )
{
#ifdef QLOCALFS_DEBUG
    qDebug( "QLocalFs: operationGet" );
#endif
    op->setState( StInProgress );
    QString from = QUrl( op->arg( 0 ) ).path();

    QFile f( from );
    if ( !f.open( IO_ReadOnly ) ) {
#ifdef QLOCALFS_DEBUG
	qDebug( "QLocalFs: could not open %s", from.latin1() );
#endif
	QString msg = tr( "Could not open\n%1" ).arg( from );
	op->setState( StFailed );
	op->setProtocolDetail( msg );
	op->setErrorCode( (int)ErrGet );
	emit finished( op );
	return;
    }

    QByteArray s;
    emit dataTransferProgress( 0, f.size(), op );
    if ( f.size() != 0 ) {
	int blockSize = calcBlockSize( f.size() );
	if ( (int)f.size() < blockSize ) {
	    s.resize( f.size() );
	    f.readBlock( s.data(), f.size() );
	    emit data( s, op );
	    emit dataTransferProgress( f.size(), f.size(), op );
#ifdef QLOCALFS_DEBUG
	    qDebug( "QLocalFs: got all %d bytes at once", f.size() );
#endif
	} else {
	    s.resize( blockSize );
	    int remaining = f.size();
	    QGuardedPtr<QObject> that = this;
	    while ( that && remaining > 0 ) {
		if ( operationInProgress() != op )
		    return;
		if ( remaining >= blockSize ) {
		    f.readBlock( s.data(), blockSize );
		    emit data( s, op );
		    emit dataTransferProgress( f.size() - remaining, f.size(), op );
		    remaining -= blockSize;
		} else {
		    s.resize( remaining );
		    f.readBlock( s.data(), remaining );
		    emit data( s, op );
		    emit dataTransferProgress( f.size() - remaining, f.size(), op );
		    remaining -= remaining;
		}
                qApp->processEvents();
	    }
	    if (!that)
		return;
#ifdef QLOCALFS_DEBUG
	    qDebug( "QLocalFs: got all %d bytes step by step", f.size() );
#endif
	    emit dataTransferProgress( f.size(), f.size(), op );
	}
    }
    op->setState( StDone );
    f.close();
    emit finished( op );
}

/*!
    \reimp
*/

void QLocalFs::operationPut( QNetworkOperation *op )
{
#ifdef QLOCALFS_DEBUG
    qDebug( "QLocalFs: operationPut" );
#endif
    op->setState( StInProgress );
    QString to = QUrl( op->arg( 0 ) ).path();

    QFile f( to );
    if ( !f.open( IO_WriteOnly ) ) {
	QString msg = tr( "Could not write\n%1" ).arg( to );
	op->setState( StFailed );
	op->setProtocolDetail( msg );
	op->setErrorCode( (int)ErrPut );
	emit finished( op );
	return;
    }

    QByteArray ba( op->rawArg( 1 ) );
    emit dataTransferProgress( 0, ba.size(), op );
    int blockSize = calcBlockSize( ba.size() );
    if ( (int)ba.size() < blockSize ) {
	f.writeBlock( ba.data(), ba.size() );
	emit dataTransferProgress( ba.size(), ba.size(), op );
    } else {
	int i = 0;
	while ( i + blockSize < (int)ba.size() - 1 ) {
	    if ( operationInProgress() != op )
		return;
	    f.writeBlock( &ba.data()[ i ], blockSize );
	    f.flush();
	    emit dataTransferProgress( i + blockSize, ba.size(), op );
	    i += blockSize;
	    QGuardedPtr<QObject> that = this;
            qApp->processEvents();
            if (!that)
                return;
	}
	if ( i < (int)ba.size() - 1 )
	    f.writeBlock( &ba.data()[ i ], ba.size() - i );
	emit dataTransferProgress( ba.size(), ba.size(), op );
    }
    op->setState( StDone );
    f.close();
    emit finished( op );
}

/*!
    \reimp
*/

int QLocalFs::supportedOperations() const
{
    return OpListChildren | OpMkDir | OpRemove | OpRename | OpGet | OpPut;
}

/*!
    \internal
*/

int QLocalFs::calcBlockSize( int totalSize ) const
{
    if ( totalSize == 0 )
	return 1024;
    int s = totalSize / 100;
    // we want a block size between 1KB and 1MB
    if ( s < 1024 )
	s = 1024;
    if ( s > 1048576 )
	s = 1048576;
    return s;
}

#endif // QT_NO_NETWORKPROTOCOL
