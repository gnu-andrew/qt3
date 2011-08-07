/****************************************************************************
**
** Definition of QSettings related classes
**
** Created : 990124
**
** Copyright (C) 1999-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QSETTINGS_P_H
#define QSETTINGS_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of QSettings. This header file may change from version to 
// version without notice, or even be removed.
//
// We mean it.
//
//

#ifndef QT_H
#include "qstringlist.h"
#include "qmap.h"
#include "qvaluestack.h"
#endif // QT_H

#ifndef QT_NO_SETTINGS
class QSettingsSysPrivate;

// QSettingsGroup is a map of key/value pairs
class QSettingsGroup : public QMap<QString,QString>
{
public:
    QSettingsGroup();

    bool modified;
};

// QSettingsHeading is a map of heading/group pairs
class QSettingsHeading : public QMap<QString,QSettingsGroup>
{
public:
    QSettingsHeading::Iterator git;
    void read(const QString &);
    void parseLine(QTextStream &);
};


class QSettingsPrivate
{
public:
    QSettingsPrivate( QSettings::Format format );
    ~QSettingsPrivate();

    QSettingsGroup readGroup();
    void removeGroup(const QString &);
    void writeGroup(const QString &, const QString &);
    QDateTime modificationTime();

    QStringList searchPaths;
    QMap<QString,QSettingsHeading> headings;
    QString group;
    QString heading;

    /*### static data brings threading trouble
    static QString *defProduct;
    static QString *defDomain;
    */
    QValueStack<QString> groupStack;
    QString groupPrefix;

    bool groupDirty :1;
    bool modified :1;
    bool globalScope :1;

#if !defined(QWS) && (defined(Q_WS_WIN) || defined(Q_OS_MAC))
    // system dependent implementations to use the 
    // system specific setting database (ie. registry on Windows)

    QSettingsSysPrivate *sysd;
    void	sysInit();
    void	sysClear();

#if !defined(Q_NO_BOOL_TYPE)
    bool	sysWriteEntry( const QString &, bool );
#endif
    bool	sysWriteEntry( const QString &, double );
    bool	sysWriteEntry( const QString &, int );
    bool	sysWriteEntry( const QString &, const QString & );
    bool	sysWriteEntry( const QString &, const QStringList & );
    bool	sysWriteEntry( const QString &, const QStringList &, const QChar& sep );

    QStringList sysEntryList(const QString &) const;
    QStringList sysSubkeyList(const QString &) const;

    QStringList sysReadListEntry( const QString &, bool * = 0 ) const;
    QStringList sysReadListEntry( const QString &, const QChar& sep, bool * = 0 ) const;
    QString	sysReadEntry( const QString &, const QString &def = QString::null, bool * = 0 ) const;
    int		sysReadNumEntry( const QString &, int def = 0, bool * = 0 ) const;
    double	sysReadDoubleEntry( const QString &, double def = 0, bool * = 0 ) const;
    bool	sysReadBoolEntry( const QString &, bool def = 0, bool * = 0 ) const;

    bool	sysRemoveEntry( const QString & );

    bool	sysSync();

    void	sysInsertSearchPath( QSettings::System, const QString & );
    void	sysRemoveSearchPath( QSettings::System, const QString & );
#endif

};
#endif //QT_NO_SETTINGS
#endif // QSETTINGS_P_H
