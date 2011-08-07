/****************************************************************************
**
** Definition of QDataStream class
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

#ifndef QDATASTREAM_H
#define QDATASTREAM_H

#ifndef QT_H
#include "qiodevice.h"
#include "qstring.h"
#endif // QT_H

#ifndef QT_NO_DATASTREAM
class Q_EXPORT QDataStream				// data stream class
{
public:
    QDataStream();
    QDataStream( QIODevice * );
    QDataStream( QByteArray, int mode );
    virtual ~QDataStream();

    QIODevice	*device() const;
    void	 setDevice( QIODevice * );
    void	 unsetDevice();

    bool	 atEnd() const;
    bool	 eof() const;

    enum ByteOrder { BigEndian, LittleEndian };
    int		 byteOrder()	const;
    void	 setByteOrder( int );

    bool	 isPrintableData() const;
    void	 setPrintableData( bool );

    int		 version() const;
    void	 setVersion( int );

    QDataStream &operator>>( Q_INT8 &i );
    QDataStream &operator>>( Q_UINT8 &i );
    QDataStream &operator>>( Q_INT16 &i );
    QDataStream &operator>>( Q_UINT16 &i );
    QDataStream &operator>>( Q_INT32 &i );
    QDataStream &operator>>( Q_UINT32 &i );
    QDataStream &operator>>( Q_INT64 &i );
    QDataStream &operator>>( Q_UINT64 &i );
#if !defined(Q_OS_WIN64)
    QDataStream &operator>>( Q_LONG &i );
    QDataStream &operator>>( Q_ULONG &i );
#endif

    QDataStream &operator>>( float &f );
    QDataStream &operator>>( double &f );
    QDataStream &operator>>( char *&str );

    QDataStream &operator<<( Q_INT8 i );
    QDataStream &operator<<( Q_UINT8 i );
    QDataStream &operator<<( Q_INT16 i );
    QDataStream &operator<<( Q_UINT16 i );
    QDataStream &operator<<( Q_INT32 i );
    QDataStream &operator<<( Q_UINT32 i );
    QDataStream &operator<<( Q_INT64 i );
    QDataStream &operator<<( Q_UINT64 i );
#if !defined(Q_OS_WIN64)
    QDataStream &operator<<( Q_LONG i );
    QDataStream &operator<<( Q_ULONG i );
#endif
    QDataStream &operator<<( float f );
    QDataStream &operator<<( double f );
    QDataStream &operator<<( const char *str );

    QDataStream &readBytes( char *&, uint &len );
    QDataStream &readRawBytes( char *, uint len );

    QDataStream &writeBytes( const char *, uint len );
    QDataStream &writeRawBytes( const char *, uint len );

private:
    QIODevice	*dev;
    bool	 owndev;
    int		 byteorder;
    bool	 printable;
    bool	 noswap;
    int		 ver;

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QDataStream( const QDataStream & );
    QDataStream &operator=( const QDataStream & );
#endif
};


/*****************************************************************************
  QDataStream inline functions
 *****************************************************************************/

inline QIODevice *QDataStream::device() const
{ return dev; }

inline bool QDataStream::atEnd() const
{ return dev ? dev->atEnd() : TRUE; }

inline bool QDataStream::eof() const
{ return atEnd(); }

inline int QDataStream::byteOrder() const
{ return byteorder; }

inline bool QDataStream::isPrintableData() const
{ return printable; }

inline void QDataStream::setPrintableData( bool p )
{ printable = p; }

inline int QDataStream::version() const
{ return ver; }

inline void QDataStream::setVersion( int v )
{ ver = v; }

inline QDataStream &QDataStream::operator>>( Q_UINT8 &i )
{ return *this >> (Q_INT8&)i; }

inline QDataStream &QDataStream::operator>>( Q_UINT16 &i )
{ return *this >> (Q_INT16&)i; }

inline QDataStream &QDataStream::operator>>( Q_UINT32 &i )
{ return *this >> (Q_INT32&)i; }

inline QDataStream &QDataStream::operator>>( Q_UINT64 &i )
{ return *this >> (Q_INT64&)i; }

#if !defined(Q_OS_WIN64)
inline QDataStream &QDataStream::operator>>( Q_ULONG &i )
{ return *this >> (Q_LONG&)i; }
#endif

inline QDataStream &QDataStream::operator<<( Q_UINT8 i )
{ return *this << (Q_INT8)i; }

inline QDataStream &QDataStream::operator<<( Q_UINT16 i )
{ return *this << (Q_INT16)i; }

inline QDataStream &QDataStream::operator<<( Q_UINT32 i )
{ return *this << (Q_INT32)i; }

inline QDataStream &QDataStream::operator<<( Q_UINT64 i )
{ return *this << (Q_INT64)i; }

#if !defined(Q_OS_WIN64)
inline QDataStream &QDataStream::operator<<( Q_ULONG i )
{ return *this << (Q_LONG)i; }
#endif

#endif // QT_NO_DATASTREAM
#endif // QDATASTREAM_H
