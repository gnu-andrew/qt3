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

#include <metatranslator.h>

#include <qcstring.h>
#include <qmap.h>

typedef QMap<QCString, MetaTranslatorMessage> TMM;
typedef QValueList<MetaTranslatorMessage> TML;

/*
  Augments a MetaTranslator with trivially derived translations.

  For example, if "Enabled:" is consistendly translated as "Eingeschaltet:" no
  matter the context or the comment, "Eingeschaltet:" is added as the
  translation of any untranslated "Enabled:" text and is marked Unfinished.
*/

void applySameTextHeuristic( MetaTranslator *tor, bool verbose )
{
    TMM translated;
    TMM avoid;
    TMM::Iterator t;
    TML untranslated;
    TML::Iterator u;
    TML all = tor->messages();
    TML::Iterator it;
    int inserted = 0;

    for ( it = all.begin(); it != all.end(); ++it ) {
	if ( (*it).type() == MetaTranslatorMessage::Unfinished ) {
	    if ( (*it).translation().isEmpty() )
		untranslated.append( *it );
	} else {
	    QCString key = (*it).sourceText();
	    t = translated.find( key );
	    if ( t != translated.end() ) {
		/*
		  The same source text is translated at least two
		  different ways. Do nothing then.
		*/
		if ( (*t).translation() != (*it).translation() ) {
		    translated.remove( key );
		    avoid.insert( key, *it );
		}
	    } else if ( !avoid.contains(key) &&
			!(*it).translation().isEmpty() ) {
		translated.insert( key, *it );
	    }
	}
    }

    for ( u = untranslated.begin(); u != untranslated.end(); ++u ) {
	QCString key = (*u).sourceText();
	t = translated.find( key );
	if ( t != translated.end() ) {
	    MetaTranslatorMessage m( *u );
	    m.setTranslation( (*t).translation() );
	    tor->insert( m );
	    inserted++;
	}
    }
    if ( verbose && inserted != 0 )
	fprintf( stderr, " same-text heuristic provided %d translation%s\n",
		 inserted, inserted == 1 ? "" : "s" );
}
