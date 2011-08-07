/****************************************************************************
**
** Text engine classes
**
** Copyright (C) 2005-2008 Trolltech ASA.  All rights reserved.
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

#include <assert.h>


QScriptItemArray::~QScriptItemArray()
{
    clear();
    free( d );
}

void QScriptItemArray::clear()
{
    if ( d ) {
	for ( unsigned int i = 0; i < d->size; i++ ) {
	    QScriptItem &si = d->items[i];
	    if ( si.fontEngine )
		si.fontEngine->deref();
	}
	d->size = 0;
    }
}

void QScriptItemArray::resize( int s )
{
    int alloc = (s + 8) >> 3 << 3;
    d = (QScriptItemArrayPrivate *)realloc( d, sizeof( QScriptItemArrayPrivate ) +
		 sizeof( QScriptItem ) * alloc );
    d->alloc = alloc;
}

void QTextEngine::shape( int item ) const
{
    assert( item < items.size() );
    QScriptItem &si = items[item];

    if ( si.num_glyphs )
	return;

    QFont::Script script = (QFont::Script)si.analysis.script;
    si.glyph_data_offset = used;

    if ( !si.fontEngine )
	si.fontEngine = fnt->engineForScript( script );
    si.fontEngine->ref();

	si.ascent = si.fontEngine->ascent();
	si.descent = si.fontEngine->descent();
    si.num_glyphs = 0;

    if ( si.fontEngine && si.fontEngine != (QFontEngine*)-1 ) {
        QShaperItem shaper_item;
        shaper_item.script = si.analysis.script;
        shaper_item.string = &string;
        shaper_item.from = si.position;
        shaper_item.length = length(item);
        shaper_item.font = si.fontEngine;
        shaper_item.num_glyphs = QMAX(int(num_glyphs - used), shaper_item.length);
        shaper_item.flags = si.analysis.bidiLevel % 2 ? RightToLeft : 0;
        shaper_item.has_positioning = FALSE;

        while (1) {
//         qDebug("    . num_glyphs=%d, used=%d, item.num_glyphs=%d", num_glyphs, used, shaper_item.num_glyphs);
            ensureSpace(shaper_item.num_glyphs);
            shaper_item.num_glyphs = num_glyphs - used;
//          qDebug("    .. num_glyphs=%d, used=%d, item.num_glyphs=%d", num_glyphs, used, shaper_item.num_glyphs);
            shaper_item.glyphs = glyphs(&si);
            shaper_item.advances = advances(&si);
            shaper_item.offsets = offsets(&si);
            shaper_item.attributes = glyphAttributes(&si);
            shaper_item.log_clusters = logClusters(&si);
            if (scriptEngines[shaper_item.script].shape(&shaper_item))
                break;
        }

        si.num_glyphs = shaper_item.num_glyphs;
        si.hasPositioning = shaper_item.has_positioning;
    }
    ((QTextEngine *)this)->used += si.num_glyphs;

    si.width = 0;
    advance_t *advances = this->advances( &si );
    advance_t *end = advances + si.num_glyphs;
    while ( advances < end ) {
//         qDebug("advances[%d] = %d", advances - this->advances(&si), *advances);
	si.width += *(advances++);
    }

    return;
}

