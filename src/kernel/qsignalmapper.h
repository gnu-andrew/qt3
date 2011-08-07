/****************************************************************************
**
** Definition of QSignalMapper class
**
** Created : 980503
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

#ifndef QSIGNALMAPPER_H
#define QSIGNALMAPPER_H

#ifndef QT_H
#include "qobject.h"
#endif // QT_H
#ifndef QT_NO_SIGNALMAPPER
class  QSignalMapperData;
struct QSignalMapperRec;


class Q_EXPORT QSignalMapper : public QObject {
    Q_OBJECT
public:
    QSignalMapper( QObject* parent, const char* name=0 );
    ~QSignalMapper();

    virtual void setMapping( const QObject* sender, int identifier );
    virtual void setMapping( const QObject* sender, const QString &identifier );
    void removeMappings( const QObject* sender );

signals:
    void mapped(int);
    void mapped(const QString &);

public slots:
    void map();

private:
    QSignalMapperData* d;
    QSignalMapperRec* getRec( const QObject* );

private slots:
    void removeMapping();
};

#endif // QT_NO_SIGNALMAPPER
#endif // QSIGNALMAPPER_H
