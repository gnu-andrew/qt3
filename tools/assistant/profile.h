/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the Qt Assistant.
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
** Licensees holding valid Qt Commercial licenses may use this file in
** accordance with the Qt Commercial License Agreement provided with
** the Software.
**
** This file is provided "AS IS" with NO WARRANTY OF ANY KIND,
** INCLUDING THE WARRANTIES OF DESIGN, MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE. Trolltech reserves all rights not granted
** herein.
**
**********************************************************************/

#ifndef PROFILE_H
#define PROFILE_H

#include <qfileinfo.h>
#include <qstring.h>
#include <qstringlist.h>
#include <qmap.h>

class DocuParser;

class Profile
{
public:
    enum ProfileType { DefaultProfile, UserProfile };
    Profile();

    inline bool isValid() const;

    inline void addDCF( const QString &docfile );
    inline void addDCFIcon( const QString title, const QString &icon );
    inline void addDCFIndexPage( const QString title, const QString &indexPage );
    inline void addDCFImageDir( const QString title, const QString &imgDir );
    inline void addDCFTitle( const QString &dcf, const QString &title );
    inline void addProperty( const QString &name, const QString &value );
    inline bool hasDocFile( const QString &docFile );
    void removeDocFileEntry( const QString &title );

    inline ProfileType profileType() const { return type; }
    inline void setProfileType( ProfileType t ) { type = t; }

    inline DocuParser *docuParser() const { return dparser; }
    inline void setDocuParser( DocuParser *dp ) { dparser = dp; }

    static Profile* createDefaultProfile( const QString &docPath = QString::null );
    static QString makeRelativePath( const QString &base, const QString &path );

    int valid:1;
    ProfileType type;
    DocuParser *dparser;
    QMap<QString,QString> props;
    QMap<QString,QString> icons;
    QMap<QString,QString> indexPages;
    QMap<QString,QString> imageDirs;
    QMap<QString,QString> dcfTitles;
    QStringList docs;
};


inline bool Profile::isValid() const
{
    return valid;
}

inline void Profile::addDCFTitle(const QString &dcf, const QString &title)
{
    QString absdcf = QFileInfo(dcf).absFilePath();
    dcfTitles[title] = absdcf;
    if (docs.contains(absdcf) == 0)
	docs << absdcf;
}

inline void Profile::addDCF( const QString &docfile )
{
    if( !docs.contains( docfile ) == 0 )
	docs << docfile;
}

inline void Profile::addDCFIcon( const QString docfile,
				     const QString &icon )
{
    icons[docfile] = icon;
}

inline void Profile::addDCFIndexPage( const QString title,
				      const QString &indexPage )
{
    indexPages[title] = indexPage;
}

inline void Profile::addDCFImageDir( const QString docfile,
				     const QString &imgDir )
{
    imageDirs[docfile] = imgDir;
}

inline void Profile::addProperty( const QString &name,
				  const QString &value )
{
    props[name] = value;
}

inline bool Profile::hasDocFile( const QString &name )
{
    return docs.contains( name ) > 0;
}

#endif
