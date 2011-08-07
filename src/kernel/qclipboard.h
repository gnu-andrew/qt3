/****************************************************************************
**
** Definition of QClipboard class
**
** Created : 960430
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

#ifndef QCLIPBOARD_H
#define QCLIPBOARD_H

#ifndef QT_H
#include "qwindowdefs.h"
#include "qobject.h"
#endif // QT_H

#ifndef QT_NO_CLIPBOARD

class QMimeSource;

class Q_EXPORT QClipboard : public QObject
{
    Q_OBJECT
private:
    QClipboard( QObject *parent=0, const char *name=0 );
    ~QClipboard();

public:
    enum Mode { Clipboard, Selection };

    void	clear( Mode mode );	       	// ### default arg = Clipboard in 4.0
    void	clear();		       	// ### remove 4.0

    bool	supportsSelection() const;
    bool	ownsSelection() const;
    bool	ownsClipboard() const;

    void	setSelectionMode(bool enable);		// ### remove 4.0
    bool	selectionModeEnabled() const; 		// ### remove 4.0

    // ### default arg mode = Clipboard in 4.0 for all of these
    QString     text( Mode mode )	 const;
    QString     text( QCString& subtype, Mode mode ) const;
    void	setText( const QString &, Mode mode );

#ifndef QT_NO_MIMECLIPBOARD
    QMimeSource *data( Mode mode ) const;
    void setData( QMimeSource*, Mode mode );

    QImage	image( Mode mode ) const;
    QPixmap	pixmap( Mode mode ) const;
    void	setImage( const QImage &, Mode mode );
    void	setPixmap( const QPixmap &, Mode mode );
#endif

    // ### remove all of these in 4.0
    QString     text()	 const;
    QString     text(QCString& subtype) const;
    void	setText( const QString &);

#ifndef QT_NO_MIMECLIPBOARD
    QMimeSource	*data() const;
    void	setData( QMimeSource* );

    QImage	image() const;
    QPixmap	pixmap() const;
    void	setImage( const QImage & );
    void	setPixmap( const QPixmap & );
#endif

signals:
    void        selectionChanged();
    void	dataChanged();

private slots:
    void	ownerDestroyed();

protected:
    void	connectNotify( const char * );
    bool	event( QEvent * );

    friend class QApplication;
    friend class QBaseApplication;
    friend class QDragManager;
    friend class QMimeSource;

private:
#if defined(Q_WS_MAC)
    void loadScrap(bool convert);
    void saveScrap();
#endif

    // Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QClipboard( const QClipboard & );
    QClipboard &operator=( const QClipboard & );
#endif
};

#endif // QT_NO_CLIPBOARD

#endif // QCLIPBOARD_H
