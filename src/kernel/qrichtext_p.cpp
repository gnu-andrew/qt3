/****************************************************************************
**
** Implementation of the internal Qt classes dealing with rich text
**
** Created : 990101
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

#include "qrichtext_p.h"

#ifndef QT_NO_RICHTEXT

QTextCommand::~QTextCommand() {}
QTextCommand::Commands QTextCommand::type() const { return Invalid; }


#ifndef QT_NO_TEXTCUSTOMITEM
QTextCustomItem::~QTextCustomItem() {}
void QTextCustomItem::adjustToPainter( QPainter* p){ if ( p ) width = 0; }
QTextCustomItem::Placement QTextCustomItem::placement() const { return PlaceInline; }

bool QTextCustomItem::ownLine() const { return FALSE; }
void QTextCustomItem::resize( int nwidth ){ width = nwidth; }
void QTextCustomItem::invalidate() {}

bool QTextCustomItem::isNested() const { return FALSE; }
int QTextCustomItem::minimumWidth() const { return 0; }

QString QTextCustomItem::richText() const { return QString::null; }

bool QTextCustomItem::enter( QTextCursor *, QTextDocument*&, QTextParagraph *&, int &, int &, int &, bool )
{
    return TRUE;
}
bool QTextCustomItem::enterAt( QTextCursor *, QTextDocument *&, QTextParagraph *&, int &, int &, int &, const QPoint & )
{
    return TRUE;
}
bool QTextCustomItem::next( QTextCursor *, QTextDocument *&, QTextParagraph *&, int &, int &, int & )
{
    return TRUE;
}
bool QTextCustomItem::prev( QTextCursor *, QTextDocument *&, QTextParagraph *&, int &, int &, int & )
{
    return TRUE;
}
bool QTextCustomItem::down( QTextCursor *, QTextDocument *&, QTextParagraph *&, int &, int &, int & )
{
    return TRUE;
}
bool QTextCustomItem::up( QTextCursor *, QTextDocument *&, QTextParagraph *&, int &, int &, int & )
{
    return TRUE;
}
#endif // QT_NO_TEXTCUSTOMITEM

void QTextFlow::setPageSize( int ps ) { pagesize = ps; }
#ifndef QT_NO_TEXTCUSTOMITEM
bool QTextFlow::isEmpty() { return leftItems.isEmpty() && rightItems.isEmpty(); }
#else
bool QTextFlow::isEmpty() { return TRUE; }
#endif

#ifndef QT_NO_TEXTCUSTOMITEM
void QTextTableCell::invalidate() { cached_width = -1; cached_sizehint = -1; }

void QTextTable::invalidate() { cachewidth = -1; }
#endif

QTextParagraphData::~QTextParagraphData() {}
void QTextParagraphData::join( QTextParagraphData * ) {}

QTextFormatter::~QTextFormatter() {}
void QTextFormatter::setWrapEnabled( bool b ) { wrapEnabled = b; }
void QTextFormatter::setWrapAtColumn( int c ) { wrapColumn = c; }



int QTextCursor::x() const
{
    if ( idx >= para->length() )
	return 0;
    QTextStringChar *c = para->at( idx );
    int curx = c->x;
    if ( !c->rightToLeft &&
	 c->c.isSpace() &&
	 idx > 0 &&
	 para->at( idx - 1 )->c != '\t' &&
	 !c->lineStart &&
	 ( para->alignment() & Qt::AlignJustify ) == Qt::AlignJustify )
	curx = para->at( idx - 1 )->x + para->string()->width( idx - 1 );
    if ( c->rightToLeft )
	curx += para->string()->width( idx );
    return curx;
}

int QTextCursor::y() const
{
    int dummy, line;
    para->lineStartOfChar( idx, &dummy, &line );
    return para->lineY( line );
}

int QTextCursor::globalX() const { return totalOffsetX() + para->rect().x() + x(); }
int QTextCursor::globalY() const { return totalOffsetY() + para->rect().y() + y(); }

QTextDocument *QTextCursor::document() const
{
    return para ? para->document() : 0;
}

void QTextCursor::gotoPosition( QTextParagraph* p, int index )
{
    if ( para && p != para ) {
	while ( !indices.isEmpty() && para->document() != p->document() )
	    pop();
	Q_ASSERT( indices.isEmpty() || para->document() == p->document() );
    }
    para = p;
    if ( index < 0 || index >= para->length() ) {
#if defined(QT_CHECK_RANGE)
	qWarning( "QTextCursor::gotoParagraph Index: %d out of range", index );
#endif
	if ( index < 0 || para->length() == 0 )
	    index = 0;
	else
	    index = para->length() - 1;
    }

    tmpX = -1;
    idx = index;
    fixCursorPosition();
}

bool QTextDocument::hasSelection( int id, bool visible ) const
{
    return ( selections.find( id ) != selections.end() &&
	     ( !visible ||
	       ( (QTextDocument*)this )->selectionStartCursor( id ) !=
	       ( (QTextDocument*)this )->selectionEndCursor( id ) ) );
}

void QTextDocument::setSelectionStart( int id, const QTextCursor &cursor )
{
    QTextDocumentSelection sel;
    sel.startCursor = cursor;
    sel.endCursor = cursor;
    sel.swapped = FALSE;
    selections[ id ] = sel;
}

QTextParagraph *QTextDocument::paragAt( int i ) const
{
    QTextParagraph* p = curParag;
    if ( !p || p->paragId() > i )
	p = fParag;
    while ( p && p->paragId() != i )
	p = p->next();
    ((QTextDocument*)this)->curParag = p;
    return p;
}


QTextFormat::~QTextFormat()
{
}

QTextFormat::QTextFormat()
    : fm( QFontMetrics( fn ) ), linkColor( TRUE ), logicalFontSize( 3 ), stdSize( qApp->font().pointSize() )
{
    ref = 0;

    usePixelSizes = FALSE;
    if ( stdSize == -1 ) {
	stdSize = qApp->font().pixelSize();
	usePixelSizes = TRUE;
    }

    missp = FALSE;
    ha = AlignNormal;
    collection = 0;
}

QTextFormat::QTextFormat( const QStyleSheetItem *style )
    : fm( QFontMetrics( fn ) ), linkColor( TRUE ), logicalFontSize( 3 ), stdSize( qApp->font().pointSize() )
{
    ref = 0;

    usePixelSizes = FALSE;
    if ( stdSize == -1 ) {
	stdSize = qApp->font().pixelSize();
	usePixelSizes = TRUE;
    }

    missp = FALSE;
    ha = AlignNormal;
    collection = 0;
    fn = QFont( style->fontFamily(),
		style->fontSize(),
		style->fontWeight(),
		style->fontItalic() );
    fn.setUnderline( style->fontUnderline() );
    fn.setStrikeOut( style->fontStrikeOut() );
    col = style->color();
    fm = QFontMetrics( fn );
    leftBearing = fm.minLeftBearing();
    rightBearing = fm.minRightBearing();
    hei = fm.lineSpacing();
    asc = fm.ascent() + (fm.leading()+1)/2;
    dsc = fm.descent();
    missp = FALSE;
    ha = AlignNormal;
    memset( widths, 0, 256 );
    generateKey();
    addRef();
}

QTextFormat::QTextFormat( const QFont &f, const QColor &c, QTextFormatCollection *parent )
    : fn( f ), col( c ), fm( QFontMetrics( f ) ), linkColor( TRUE ),
      logicalFontSize( 3 ), stdSize( f.pointSize() )
{
    ref = 0;
    usePixelSizes = FALSE;
    if ( stdSize == -1 ) {
	stdSize = f.pixelSize();
	usePixelSizes = TRUE;
    }
    collection = parent;
    leftBearing = fm.minLeftBearing();
    rightBearing = fm.minRightBearing();
    hei = fm.lineSpacing();
    asc = fm.ascent() + (fm.leading()+1)/2;
    dsc = fm.descent();
    missp = FALSE;
    ha = AlignNormal;
    memset( widths, 0, 256 );
    generateKey();
    addRef();
}

QTextFormat::QTextFormat( const QTextFormat &f )
    : fm( f.fm )
{
    ref = 0;
    collection = 0;
    fn = f.fn;
    col = f.col;
    leftBearing = f.leftBearing;
    rightBearing = f.rightBearing;
    memset( widths, 0, 256 );
    hei = f.hei;
    asc = f.asc;
    dsc = f.dsc;
    stdSize = f.stdSize;
    usePixelSizes = f.usePixelSizes;
    logicalFontSize = f.logicalFontSize;
    missp = f.missp;
    ha = f.ha;
    k = f.k;
    linkColor = f.linkColor;
    addRef();
}

QTextFormat& QTextFormat::operator=( const QTextFormat &f )
{
    ref = 0;
    collection = f.collection;
    fn = f.fn;
    col = f.col;
    fm = f.fm;
    leftBearing = f.leftBearing;
    rightBearing = f.rightBearing;
    memset( widths, 0, 256 );
    hei = f.hei;
    asc = f.asc;
    dsc = f.dsc;
    stdSize = f.stdSize;
    usePixelSizes = f.usePixelSizes;
    logicalFontSize = f.logicalFontSize;
    missp = f.missp;
    ha = f.ha;
    k = f.k;
    linkColor = f.linkColor;
    addRef();
    return *this;
}

void QTextFormat::update()
{
    fm = QFontMetrics( fn );
    leftBearing = fm.minLeftBearing();
    rightBearing = fm.minRightBearing();
    hei = fm.lineSpacing();
    asc = fm.ascent() + (fm.leading()+1)/2;
    dsc = fm.descent();
    memset( widths, 0, 256 );
    generateKey();
}


QPainter* QTextFormat::pntr = 0;
QFontMetrics* QTextFormat::pntr_fm = 0;
int QTextFormat::pntr_ldg=-1;
int QTextFormat::pntr_asc=-1;
int QTextFormat::pntr_hei=-1;
int QTextFormat::pntr_dsc=-1;

void QTextFormat::setPainter( QPainter *p )
{
    pntr = p;
}

QPainter*  QTextFormat::painter()
{
    return pntr;
}

void QTextFormat::applyFont( const QFont &f )
{
    QFontMetrics fm( pntr->fontMetrics() );
    if ( !pntr_fm
	|| pntr_fm->painter != pntr
	|| pntr_fm->d != fm.d
	|| !pntr->font().isCopyOf( f ) ) {
	pntr->setFont( f );
	delete pntr_fm;
	pntr_fm = new QFontMetrics( pntr->fontMetrics() );
	pntr_ldg = pntr_fm->leading();
	pntr_asc = pntr_fm->ascent()+(pntr_ldg+1)/2;
	pntr_hei = pntr_fm->lineSpacing();
	pntr_dsc = -1;
    }
}

int QTextFormat::minLeftBearing() const
{
    if ( !pntr || !pntr->isActive() )
	return leftBearing;
    applyFont( fn );
    return pntr_fm->minLeftBearing();
}

int QTextFormat::minRightBearing() const
{
    if ( !pntr || !pntr->isActive() )
	return rightBearing;
    applyFont( fn );
    return pntr_fm->minRightBearing();
}

int QTextFormat::height() const
{
    if ( !pntr || !pntr->isActive() )
	return hei;
    applyFont( fn );
    return pntr_hei;
}

int QTextFormat::ascent() const
{
    if ( !pntr || !pntr->isActive() )
	return asc;
    applyFont( fn );
    return pntr_asc;
}

int QTextFormat::descent() const
{
    if ( !pntr || !pntr->isActive() )
	return dsc;
    applyFont( fn );
    if ( pntr_dsc < 0 )
	pntr_dsc = pntr_fm->descent();
    return pntr_dsc;
}

int QTextFormat::leading() const
{
    if ( !pntr || !pntr->isActive() )
	return fm.leading();
    applyFont( fn );
    return pntr_ldg;
}

void QTextFormat::generateKey()
{
    k = getKey( fn, col, isMisspelled(), vAlign() );
}

QString QTextFormat::getKey( const QFont &fn, const QColor &col, bool misspelled, VerticalAlignment a )
{
    QString k = fn.key();
    k += '/';
    k += QString::number( (uint)col.rgb() );
    k += '/';
    k += QString::number( (int)misspelled );
    k += '/';
    k += QString::number( (int)a );
    return k;
}

QString QTextString::toString( const QMemArray<QTextStringChar> &data )
{
    QString s;
    int l = data.size();
    s.setUnicode( 0, l );
    QTextStringChar *c = data.data();
    QChar *uc = (QChar *)s.unicode();
    while ( l-- )
	*(uc++) = (c++)->c;

    return s;
}

void QTextParagraph::setSelection( int id, int start, int end )
{
    QMap<int, QTextParagraphSelection>::ConstIterator it = selections().find( id );
    if ( it != mSelections->end() ) {
	if ( start == ( *it ).start && end == ( *it ).end )
	    return;
    }

    QTextParagraphSelection sel;
    sel.start = start;
    sel.end = end;
    (*mSelections)[ id ] = sel;
    setChanged( TRUE, TRUE );
}

void QTextParagraph::removeSelection( int id )
{
    if ( !hasSelection( id ) )
	return;
    if ( mSelections )
	mSelections->remove( id );
    setChanged( TRUE, TRUE );
}

int QTextParagraph::selectionStart( int id ) const
{
    if ( !mSelections )
	return -1;
    QMap<int, QTextParagraphSelection>::ConstIterator it = mSelections->find( id );
    if ( it == mSelections->end() )
	return -1;
    return ( *it ).start;
}

int QTextParagraph::selectionEnd( int id ) const
{
    if ( !mSelections )
	return -1;
    QMap<int, QTextParagraphSelection>::ConstIterator it = mSelections->find( id );
    if ( it == mSelections->end() )
	return -1;
    return ( *it ).end;
}

bool QTextParagraph::hasSelection( int id ) const
{
    return mSelections ? mSelections->contains( id ) : FALSE;
}

bool QTextParagraph::fullSelected( int id ) const
{
    if ( !mSelections )
	return FALSE;
    QMap<int, QTextParagraphSelection>::ConstIterator it = mSelections->find( id );
    if ( it == mSelections->end() )
	return FALSE;
    return ( *it ).start == 0 && ( *it ).end == str->length() - 1;
}

int QTextParagraph::lineY( int l ) const
{
    if ( l > (int)lineStarts.count() - 1 ) {
	qWarning( "QTextParagraph::lineY: line %d out of range!", l );
	return 0;
    }

    if ( !isValid() )
	( (QTextParagraph*)this )->format();

    QMap<int, QTextLineStart*>::ConstIterator it = lineStarts.begin();
    while ( l-- > 0 )
	++it;
    return ( *it )->y;
}

int QTextParagraph::lineBaseLine( int l ) const
{
    if ( l > (int)lineStarts.count() - 1 ) {
	qWarning( "QTextParagraph::lineBaseLine: line %d out of range!", l );
	return 10;
    }

    if ( !isValid() )
	( (QTextParagraph*)this )->format();

    QMap<int, QTextLineStart*>::ConstIterator it = lineStarts.begin();
    while ( l-- > 0 )
	++it;
    return ( *it )->baseLine;
}

int QTextParagraph::lineHeight( int l ) const
{
    if ( l > (int)lineStarts.count() - 1 ) {
	qWarning( "QTextParagraph::lineHeight: line %d out of range!", l );
	return 15;
    }

    if ( !isValid() )
	( (QTextParagraph*)this )->format();

    QMap<int, QTextLineStart*>::ConstIterator it = lineStarts.begin();
    while ( l-- > 0 )
	++it;
    return ( *it )->h;
}

void QTextParagraph::lineInfo( int l, int &y, int &h, int &bl ) const
{
    if ( l > (int)lineStarts.count() - 1 ) {
	qWarning( "QTextParagraph::lineInfo: line %d out of range!", l );
	qDebug( "%d %d", (int)lineStarts.count() - 1, l );
	y = 0;
	h = 15;
	bl = 10;
	return;
    }

    if ( !isValid() )
	( (QTextParagraph*)this )->format();

    QMap<int, QTextLineStart*>::ConstIterator it = lineStarts.begin();
    while ( l-- > 0 )
	++it;
    y = ( *it )->y;
    h = ( *it )->h;
    bl = ( *it )->baseLine;
}


void QTextParagraph::setAlignment( int a )
{
    if ( a == (int)align )
	return;
    align = a;
    invalidate( 0 );
}

QTextFormatter *QTextParagraph::formatter() const
{
    if ( hasdoc )
	return document()->formatter();
    if ( pseudoDocument()->pFormatter )
	return pseudoDocument()->pFormatter;
    return ( ( (QTextParagraph*)this )->pseudoDocument()->pFormatter = new QTextFormatterBreakWords );
}

void QTextParagraph::setTabArray( int *a )
{
    delete [] tArray;
    tArray = a;
}

void QTextParagraph::setTabStops( int tw )
{
    if ( hasdoc )
	document()->setTabStops( tw );
    else
	tabStopWidth = tw;
}

QMap<int, QTextParagraphSelection> &QTextParagraph::selections() const
{
    if ( !mSelections )
	((QTextParagraph *)this)->mSelections = new QMap<int, QTextParagraphSelection>;
    return *mSelections;
}

#ifndef QT_NO_TEXTCUSTOMITEM
QPtrList<QTextCustomItem> &QTextParagraph::floatingItems() const
{
    if ( !mFloatingItems )
	((QTextParagraph *)this)->mFloatingItems = new QPtrList<QTextCustomItem>;
    return *mFloatingItems;
}
#endif

QTextStringChar::~QTextStringChar()
{
    if ( format() )
	format()->removeRef();
    if ( type ) // not Regular
	delete d.custom;
}

QTextParagraphPseudoDocument::QTextParagraphPseudoDocument():pFormatter(0),commandHistory(0), minw(0),wused(0),collection(){}
QTextParagraphPseudoDocument::~QTextParagraphPseudoDocument(){ delete pFormatter; delete commandHistory; }


#endif //QT_NO_RICHTEXT
