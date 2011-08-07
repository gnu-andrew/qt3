/**********************************************************************
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Designer.
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

#ifndef SOURCEFILE_H
#define SOURCEFILE_H

#include <qobject.h>
#include "timestamp.h"

struct DesignerSourceFile;
class SourceEditor;
class Project;

class SourceFile : public QObject
{
    Q_OBJECT

public:
    SourceFile( const QString &fn, bool temp, Project *p );
    ~SourceFile();

    void setText( const QString &s );
    void setModified( bool m );

    bool save( bool ignoreModified = FALSE );
    bool saveAs( bool ignoreModified = FALSE );
    bool load();
    bool close();
    bool closeEvent();
    Project *project() const;

    QString text() const;
    QString fileName() const { return filename; }
    bool isModified() const;

    void checkTimeStamp();

    DesignerSourceFile *iFace();

    void setEditor( SourceEditor *e );
    SourceEditor *editor() const { return ed; }

    static QString createUnnamedFileName( const QString &extension );

    bool isAccepted() const { return accepted; }

private:
    bool checkFileName( bool allowBreak );

private:
    QString filename;
    QString txt;
    DesignerSourceFile *iface;
    SourceEditor *ed;
    bool fileNameTemp;
    TimeStamp timeStamp;
    Project *pro;
    bool pkg;
    bool accepted;

};

#endif
