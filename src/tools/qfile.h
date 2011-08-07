/****************************************************************************
**
** Definition of QFile class
**
** Created : 930831
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

#ifndef QFILE_H
#define QFILE_H

#ifndef QT_H
#include "qiodevice.h"
#include "qstring.h"
#include <stdio.h>
#endif // QT_H

class QDir;
class QFilePrivate;

class Q_EXPORT QFile : public QIODevice			// file I/O device class
{
public:
    QFile();
    QFile( const QString &name );
   ~QFile();

    QString	name()	const;
    void	setName( const QString &name );

    typedef QCString (*EncoderFn)( const QString &fileName );
    typedef QString (*DecoderFn)( const QCString &localfileName );
    static QCString encodeName( const QString &fileName );
    static QString decodeName( const QCString &localFileName );
    static void setEncodingFunction( EncoderFn );
    static void setDecodingFunction( DecoderFn );

    bool	exists()   const;
    static bool exists( const QString &fileName );

    bool	remove();
    static bool remove( const QString &fileName );

    bool	open( int );
    bool	open( int, FILE * );
    bool	open( int, int );
    void	close();
    void	flush();

    Offset	size() const;
    Offset	at() const;
    bool	at( Offset );
    bool	atEnd() const;

    Q_LONG	readBlock( char *data, Q_ULONG len );
    Q_LONG	writeBlock( const char *data, Q_ULONG len );
    Q_LONG	writeBlock( const QByteArray& data )
		      { return QIODevice::writeBlock(data); }
    Q_LONG	readLine( char *data, Q_ULONG maxlen );
    Q_LONG	readLine( QString &, Q_ULONG maxlen );

    int		getch();
    int		putch( int );
    int		ungetch( int );

    int		handle() const;

    QString	errorString() const; // ### Qt 4: move into QIODevice

protected:
    void	setErrorString( const QString& ); // ### Qt 4: move into QIODevice
    QString	fn;
    FILE       *fh;
    int		fd;
    Offset	length;
    bool	ext_f;
    QFilePrivate *d; // ### Qt 4: make private

private:
    void	init();
    void	setErrorStringErrno( int );
    QCString	ungetchBuffer;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QFile( const QFile & );
    QFile &operator=( const QFile & );
#endif
};


inline QString QFile::name() const
{ return fn; }

inline QIODevice::Offset QFile::at() const
{ return ioIndex; }


#endif // QFILE_H
