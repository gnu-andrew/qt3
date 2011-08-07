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

#ifndef CONF_H
#define CONF_H

#include <qfont.h>
#include <qcolor.h>
#include <qmap.h>

struct ConfigStyle
{
    QFont font;
    QColor color;

    Q_DUMMY_COMPARISON_OPERATOR( ConfigStyle )
};

struct Config
{
    QMap<QString, ConfigStyle> styles;
    bool hasCompletion, hasParenMatching, hasWordWrap;

    static QMap<QString, ConfigStyle> defaultStyles();
    static QMap<QString, ConfigStyle> readStyles( const QString &path );
    static void saveStyles( const QMap<QString, ConfigStyle> &styles, const QString &path );
    static bool completion( const QString &path );
    static bool wordWrap( const QString &path );
    static bool parenMatching( const QString &path );
    static int indentTabSize( const QString &path );
    static int indentIndentSize( const QString &path );
    static bool indentKeepTabs( const QString &path );
    static bool indentAutoIndent( const QString &path );

    static void setCompletion( bool b, const QString &path );
    static void setWordWrap( bool b, const QString &path );
    static void setParenMatching( bool b,const QString &path );
    static void setIndentTabSize( int s, const QString &path );
    static void setIndentIndentSize( int s, const QString &path );
    static void setIndentKeepTabs( bool b, const QString &path );
    static void setIndentAutoIndent( bool b, const QString &path );

};

#endif
