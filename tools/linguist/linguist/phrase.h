/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Linguist.
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

#ifndef PHRASE_H
#define PHRASE_H

#include <qstring.h>
#include <qvaluelist.h>

class Phrase
{
public:
    Phrase() { }
    Phrase( const QString& source, const QString& target,
	    const QString& definition );

    QString source() const { return s; }
    void setSource( const QString& ns ) { s = ns; }
    QString target() const { return t; }
    void setTarget( const QString& nt ) { t = nt; }
    QString definition() const { return d; }
    void setDefinition ( const QString& nd ) { d = nd; }

private:
    QString s;
    QString t;
    QString d;
};

bool operator==( const Phrase& p, const Phrase& q );
inline bool operator!=( const Phrase& p, const Phrase& q ) {
    return !( p == q );
}

class PhraseBook : public QValueList<Phrase>
{
public:
    PhraseBook() { }

    bool load( const QString& filename );
    bool save( const QString& filename ) const;
};

#endif
