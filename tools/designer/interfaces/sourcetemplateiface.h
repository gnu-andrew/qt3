 /**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
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

#ifndef SOURCETEMPLATEIFACE_H
#define SOURCETEMPLATEIFACE_H

//
//  W A R N I N G  --  PRIVATE INTERFACES
//  --------------------------------------
//
// This file and the interfaces declared in the file are not
// public. It exists for internal purpose. This header file and
// interfaces may change from version to version (even binary
// incompatible) without notice, or even be removed.
//
// We mean it.
//
//

#include <private/qcom_p.h>
#include <qstring.h>

// {1b3446a4-1c71-424b-8789-1f34eb5697d8}
#ifndef IID_SourceTemplate
#define IID_SourceTemplate QUuid( 0x1b3446a4, 0x1c71, 0x424b, 0x87, 0x89, 0x1f, 0x34, 0xeb, 0x56, 0x97, 0xd8 )
#endif

struct SourceTemplateInterface : public QFeatureListInterface
{
    struct Source
    {
	QString code;
	enum Type { FileName, Unnamed, Invalid } type;
	QString filename;
	QString extension;
    };
    virtual Source create( const QString &templ, QUnknownInterface *appIface ) = 0;
    virtual QString language( const QString &templ ) const = 0;

};

#endif
