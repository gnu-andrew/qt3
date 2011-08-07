/****************************************************************************
**
** Definition of QAccel class
**
** Created : 950419
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

#ifndef QACCEL_H
#define QACCEL_H

#ifndef QT_H
#include "qobject.h"
#include "qkeysequence.h"
#endif // QT_H

#ifndef QT_NO_ACCEL

class QAccelPrivate;

class Q_EXPORT QAccel : public QObject			// accelerator class
{
    Q_OBJECT
public:
    QAccel( QWidget *parent, const char *name=0 );
    QAccel( QWidget* watch, QObject *parent, const char *name=0 );
    ~QAccel();

    bool isEnabled() const;
    void setEnabled( bool );

    uint count() const;

    int insertItem( const QKeySequence& key, int id=-1);
    void removeItem( int id );
    void clear();

    QKeySequence key( int id );
    int findKey( const QKeySequence& key ) const;

    bool isItemEnabled( int id ) const;
    void setItemEnabled( int id, bool enable );

    bool connectItem( int id,  const QObject *receiver, const char* member );
    bool disconnectItem( int id,  const QObject *receiver, const char* member );

    void repairEventFilter();

    void setWhatsThis( int id, const QString& );
    QString whatsThis( int id ) const;
    void setIgnoreWhatsThis( bool );
    bool ignoreWhatsThis() const;

    static QKeySequence shortcutKey( const QString & );
    static QString keyToString(QKeySequence k );
    static QKeySequence stringToKey( const QString & );

signals:
    void activated( int id );
    void activatedAmbiguously( int id );

protected:
    bool eventFilter( QObject *, QEvent * );

private:
    QAccelPrivate * d;

private:
#if defined(Q_DISABLE_COPY)
    QAccel( const QAccel & );
    QAccel &operator=( const QAccel & );
#endif
    friend class QAccelPrivate;
    friend class QAccelManager;
};

#endif // QT_NO_ACCEL
#endif // QACCEL_H
