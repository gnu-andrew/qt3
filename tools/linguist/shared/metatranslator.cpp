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

#include "metatranslator.h"

#include <qapplication.h>
#include <qcstring.h>
#include <qfile.h>
#include <qmessagebox.h>
#include <qtextcodec.h>
#include <qtextstream.h>
#include <qxml.h>

static bool encodingIsUtf8( const QXmlAttributes& atts )
{
    for ( int i = 0; i < atts.length(); i++ ) {
	// utf8="true" is a pre-3.0 syntax
	if ( atts.qName(i) == QString("utf8") ) {
	    return ( atts.value(i) == QString("true") );
	} else if ( atts.qName(i) == QString("encoding") ) {
	    return ( atts.value(i) == QString("UTF-8") );
	}
    }
    return FALSE;
}

class TsHandler : public QXmlDefaultHandler
{
public:
    TsHandler( MetaTranslator *translator )
	: tor( translator ), type( MetaTranslatorMessage::Finished ),
	  inMessage( FALSE ), ferrorCount( 0 ), contextIsUtf8( FALSE ),
	  messageIsUtf8( FALSE ) { }

    virtual bool startElement( const QString& namespaceURI,
			       const QString& localName, const QString& qName,
			       const QXmlAttributes& atts );
    virtual bool endElement( const QString& namespaceURI,
			     const QString& localName, const QString& qName );
    virtual bool characters( const QString& ch );
    virtual bool fatalError( const QXmlParseException& exception );

private:
    MetaTranslator *tor;
    MetaTranslatorMessage::Type type;
    bool inMessage;
    QString context;
    QString source;
    QString comment;
    QString translation;

    QString accum;
    int ferrorCount;
    bool contextIsUtf8;
    bool messageIsUtf8;
};

bool TsHandler::startElement( const QString& /* namespaceURI */,
			      const QString& /* localName */,
			      const QString& qName,
			      const QXmlAttributes& atts )
{
    if ( qName == QString("byte") ) {
	for ( int i = 0; i < atts.length(); i++ ) {
	    if ( atts.qName(i) == QString("value") ) {
		QString value = atts.value( i );
		int base = 10;
		if ( value.startsWith("x") ) {
		    base = 16;
		    value = value.mid( 1 );
		}
		int n = value.toUInt( 0, base );
		if ( n != 0 )
		    accum += QChar( n );
	    }
	}	
    } else {
	if ( qName == QString("context") ) {
	    context.truncate( 0 );
	    source.truncate( 0 );
	    comment.truncate( 0 );
	    translation.truncate( 0 );
	    contextIsUtf8 = encodingIsUtf8( atts );
	} else if ( qName == QString("message") ) {
	    inMessage = TRUE;
	    type = MetaTranslatorMessage::Finished;
	    source.truncate( 0 );
	    comment.truncate( 0 );
	    translation.truncate( 0 );
	    messageIsUtf8 = encodingIsUtf8( atts );
	} else if ( qName == QString("translation") ) {
	    for ( int i = 0; i < atts.length(); i++ ) {
		if ( atts.qName(i) == QString("type") ) {
		    if ( atts.value(i) == QString("unfinished") )
			type = MetaTranslatorMessage::Unfinished;
		    else if ( atts.value(i) == QString("obsolete") )
			type = MetaTranslatorMessage::Obsolete;
		    else
			type = MetaTranslatorMessage::Finished;
		}
	    }
	}
	accum.truncate( 0 );
    }
    return TRUE;
}

bool TsHandler::endElement( const QString& /* namespaceURI */,
			    const QString& /* localName */,
			    const QString& qName )
{
    if ( qName == QString("codec") || qName == QString("defaultcodec") ) {
	// "codec" is a pre-3.0 syntax
	tor->setCodec( accum );
    } else if ( qName == QString("name") ) {
	context = accum;
    } else if ( qName == QString("source") ) {
	source = accum;
    } else if ( qName == QString("comment") ) {
	if ( inMessage ) {
	    comment = accum;
	} else {
	    if ( contextIsUtf8 )
		tor->insert( MetaTranslatorMessage(context.utf8(),
			     ContextComment,
			     accum.utf8(), QString::null, TRUE,
			     MetaTranslatorMessage::Unfinished) );
	    else
		tor->insert( MetaTranslatorMessage(context.ascii(),
			     ContextComment,
			     accum.ascii(), QString::null, FALSE,
			     MetaTranslatorMessage::Unfinished) );
	}
    } else if ( qName == QString("translation") ) {
	translation = accum;
    } else if ( qName == QString("message") ) {
	if ( messageIsUtf8 )
	    tor->insert( MetaTranslatorMessage(context.utf8(), source.utf8(),
					       comment.utf8(), translation,
					       TRUE, type) );
	else
	    tor->insert( MetaTranslatorMessage(context.ascii(), source.ascii(),
					       comment.ascii(), translation,
					       FALSE, type) );
	inMessage = FALSE;
    }
    return TRUE;
}

bool TsHandler::characters( const QString& ch )
{
    QString t = ch;
    t.replace( "\r", "" );
    accum += t;
    return TRUE;
}

bool TsHandler::fatalError( const QXmlParseException& exception )
{
    if ( ferrorCount++ == 0 ) {
	QString msg;
	msg.sprintf( "Parse error at line %d, column %d (%s).",
		     exception.lineNumber(), exception.columnNumber(),
		     exception.message().latin1() );
	if ( qApp == 0 )
	    fprintf( stderr, "XML error: %s\n", msg.latin1() );
	else
	    QMessageBox::information( qApp->mainWidget(),
				      QObject::tr("Qt Linguist"), msg );
    }
    return FALSE;
}

static QString numericEntity( int ch )
{
    return QString( ch <= 0x20 ? "<byte value=\"x%1\"/>" : "&#x%1;" )
	   .arg( ch, 0, 16 );
}

static QString protect( const QCString& str )
{
    QString result;
    int len = (int) str.length();
    for ( int k = 0; k < len; k++ ) {
	switch( str[k] ) {
	case '\"':
	    result += QString( "&quot;" );
	    break;
	case '&':
	    result += QString( "&amp;" );
	    break;
	case '>':
	    result += QString( "&gt;" );
	    break;
	case '<':
	    result += QString( "&lt;" );
	    break;
	case '\'':
	    result += QString( "&apos;" );
	    break;
	default:
	    if ( (uchar) str[k] < 0x20 && str[k] != '\n' )
		result += numericEntity( (uchar) str[k] );
	    else
		result += str[k];
	}
    }
    return result;
}

static QString evilBytes( const QCString& str, bool utf8 )
{
    if ( utf8 ) {
	return protect( str );
    } else {
	QString result;
	QCString t = protect( str ).latin1();
	int len = (int) t.length();
	for ( int k = 0; k < len; k++ ) {
	    if ( (uchar) t[k] >= 0x7f )
		result += numericEntity( (uchar) t[k] );
	    else
		result += QChar( t[k] );
	}
	return result;
    }
}

MetaTranslatorMessage::MetaTranslatorMessage()
    : utfeight( FALSE ), ty( Unfinished )
{
}

MetaTranslatorMessage::MetaTranslatorMessage( const char *context,
					      const char *sourceText,
					      const char *comment,
					      const QString& translation,
					      bool utf8, Type type )
    : QTranslatorMessage( context, sourceText, comment, translation ),
      utfeight( FALSE ), ty( type )
{
    /*
      Don't use UTF-8 if it makes no difference. UTF-8 should be
      reserved for the real problematic case: non-ASCII (possibly
      non-Latin-1) characters in .ui files.
    */
    if ( utf8 ) {
	if ( sourceText != 0 ) {
	    int i = 0;
	    while ( sourceText[i] != '\0' ) {
		if ( (uchar) sourceText[i] >= 0x80 ) {
		    utfeight = TRUE;
		    break;
		}
		i++;
	    }
	}
	if ( !utfeight && comment != 0 ) {
	    int i = 0;
	    while ( comment[i] != '\0' ) {
		if ( (uchar) comment[i] >= 0x80 ) {
		    utfeight = TRUE;
		    break;
		}
		i++;
	    }
	}
    }
}

MetaTranslatorMessage::MetaTranslatorMessage( const MetaTranslatorMessage& m )
    : QTranslatorMessage( m ), utfeight( m.utfeight ), ty( m.ty )
{
}

MetaTranslatorMessage& MetaTranslatorMessage::operator=(
	const MetaTranslatorMessage& m )
{
    QTranslatorMessage::operator=( m );
    utfeight = m.utfeight;
    ty = m.ty;
    return *this;
}

bool MetaTranslatorMessage::operator==( const MetaTranslatorMessage& m ) const
{
    return qstrcmp( context(), m.context() ) == 0 &&
	   qstrcmp( sourceText(), m.sourceText() ) == 0 &&
	   qstrcmp( comment(), m.comment() ) == 0;
}

bool MetaTranslatorMessage::operator<( const MetaTranslatorMessage& m ) const
{
    int delta = qstrcmp( context(), m.context() );
    if ( delta == 0 )
	delta = qstrcmp( sourceText(), m.sourceText() );
    if ( delta == 0 )
	delta = qstrcmp( comment(), m.comment() );
    return delta < 0;
}

MetaTranslator::MetaTranslator()
{
    clear();
}

MetaTranslator::MetaTranslator( const MetaTranslator& tor )
    : mm( tor.mm ), codecName( tor.codecName ), codec( tor.codec )
{
}

MetaTranslator& MetaTranslator::operator=( const MetaTranslator& tor )
{
    mm = tor.mm;
    codecName = tor.codecName;
    codec = tor.codec;
    return *this;
}

void MetaTranslator::clear()
{
    mm.clear();
    codecName = "ISO-8859-1";
    codec = 0;
}

bool MetaTranslator::load( const QString& filename )
{
    QFile f( filename );
    if ( !f.open(IO_ReadOnly) )
	return FALSE;

    QTextStream t( &f );
    QXmlInputSource in( t );
    QXmlSimpleReader reader;
    reader.setFeature( "http://xml.org/sax/features/namespaces", FALSE );
    reader.setFeature( "http://xml.org/sax/features/namespace-prefixes", TRUE );
    QXmlDefaultHandler *hand = new TsHandler( this );
    reader.setContentHandler( hand );
    reader.setErrorHandler( hand );

    bool ok = reader.parse( in );
    reader.setContentHandler( 0 );
    reader.setErrorHandler( 0 );
    delete hand;
    f.close();
    return ok;
}

bool MetaTranslator::save( const QString& filename ) const
{
    QFile f( filename );
    if ( !f.open(IO_WriteOnly) )
	return FALSE;

    QTextStream t( &f );
    t.setCodec( QTextCodec::codecForName("ISO-8859-1") );

    t << "<!DOCTYPE TS><TS>\n";
    if ( codecName != "ISO-8859-1" )
	t << "<defaultcodec>" << codecName << "</defaultcodec>\n";
    TMM::ConstIterator m = mm.begin();
    while ( m != mm.end() ) {
	TMMInv inv;
	TMMInv::Iterator i;
	bool contextIsUtf8 = m.key().utf8();
	QCString context = m.key().context();
	QCString comment = "";

	do {
	    if ( QCString(m.key().sourceText()) == ContextComment ) {
		if ( m.key().type() != MetaTranslatorMessage::Obsolete ) {
		    contextIsUtf8 = m.key().utf8();
		    comment = QCString( m.key().comment() );
		}
	    } else {
		inv.insert( *m, m.key() );
	    }
	} while ( ++m != mm.end() && QCString(m.key().context()) == context );

	t << "<context";
	if ( contextIsUtf8 )
	    t << " encoding=\"UTF-8\"";
	t << ">\n";
	t << "    <name>" << evilBytes( context, contextIsUtf8 )
	  << "</name>\n";
	if ( !comment.isEmpty() )
	    t << "    <comment>" << evilBytes( comment, contextIsUtf8 )
	      << "</comment>\n";

	for ( i = inv.begin(); i != inv.end(); ++i ) {
	    // no need for such noise
	    if ( (*i).type() == MetaTranslatorMessage::Obsolete &&
		 (*i).translation().isEmpty() )
		continue;

	    t << "    <message";
	    if ( (*i).utf8() )
		t << " encoding=\"UTF-8\"";
	    t << ">\n"
	      << "        <source>" << evilBytes( (*i).sourceText(),
						  (*i).utf8() )
	      << "</source>\n";
	    if ( !QCString((*i).comment()).isEmpty() )
		t << "        <comment>" << evilBytes( (*i).comment(),
						       (*i).utf8() )
		  << "</comment>\n";
	    t << "        <translation";
	    if ( (*i).type() == MetaTranslatorMessage::Unfinished )
		t << " type=\"unfinished\"";
	    else if ( (*i).type() == MetaTranslatorMessage::Obsolete )
		t << " type=\"obsolete\"";
	    t << ">" << protect( (*i).translation().utf8() )
	      << "</translation>\n";
	    t << "    </message>\n";
	}
	t << "</context>\n";
    }
    t << "</TS>\n";
    f.close();
    return TRUE;
}

bool MetaTranslator::release( const QString& filename, bool verbose,
			      QTranslator::SaveMode mode ) const
{
    QTranslator tor( 0 );
    int finished = 0;
    int unfinished = 0;
    int untranslated = 0;
    TMM::ConstIterator m;

    for ( m = mm.begin(); m != mm.end(); ++m ) {
	if ( m.key().type() != MetaTranslatorMessage::Obsolete ) {
	    if ( m.key().translation().isEmpty() ) {
		untranslated++;
	    } else {
		if ( m.key().type() == MetaTranslatorMessage::Unfinished )
		    unfinished++;
		else
		    finished++;

		QCString context = m.key().context();
		QCString sourceText = m.key().sourceText();
		QCString comment = m.key().comment();
		QString translation = m.key().translation();

		/*
		  Drop the comment in (context, sourceText, comment),
		  unless (context, sourceText, "") already exists, or
		  unless we already dropped the comment of (context,
		  sourceText, comment0).
		*/
		if ( comment.isEmpty()
		     || contains(context, sourceText, "")
		     || !tor.findMessage(context, sourceText, "").translation()
			    .isNull() ) {
		    tor.insert( m.key() );
		} else {
		    tor.insert( QTranslatorMessage(context, sourceText, "",
						   translation) );
		}
	    }
	}
    }

    bool saved = tor.save( filename, mode );
    if ( saved && verbose )
	fprintf( stderr,
		 " %d finished, %d unfinished and %d untranslated messages\n",
		 finished, unfinished, untranslated );
		
    return saved;
}

bool MetaTranslator::contains( const char *context, const char *sourceText,
			       const char *comment ) const
{
    return mm.find( MetaTranslatorMessage(context, sourceText, comment) ) !=
	   mm.end();
}

void MetaTranslator::insert( const MetaTranslatorMessage& m )
{
    int pos = (int)mm.count();
    TMM::Iterator n = mm.find( m );
    if ( n != mm.end() )
	pos = *n;
    mm.replace( m, pos );
}

void MetaTranslator::stripObsoleteMessages()
{
    TMM newmm;

    TMM::Iterator m = mm.begin();
    while ( m != mm.end() ) {
	if ( m.key().type() != MetaTranslatorMessage::Obsolete )
	    newmm.insert( m.key(), *m );
	++m;
    }
    mm = newmm;
}

void MetaTranslator::stripEmptyContexts()
{
    TMM newmm;

    TMM::Iterator m = mm.begin();
    while ( m != mm.end() ) {
	if ( QCString(m.key().sourceText()) == ContextComment ) {
	    TMM::Iterator n = m;
	    ++n;
	    // the context comment is followed by other messages
	    if ( n != newmm.end() &&
		 qstrcmp(m.key().context(), n.key().context()) == 0 )
		newmm.insert( m.key(), *m );
	} else {
	    newmm.insert( m.key(), *m );
	}
	++m;
    }
    mm = newmm;
}

void MetaTranslator::setCodec( const char *name )
{
    const int latin1 = 4;

    codecName = name;
    codec = QTextCodec::codecForName( name );
    if ( codec == 0 || codec->mibEnum() == latin1 )
	codec = 0;
}

QString MetaTranslator::toUnicode( const char *str, bool utf8 ) const
{
    if ( utf8 )
	return QString::fromUtf8( str );
    else if ( codec == 0 )
	return QString( str );
    else
	return codec->toUnicode( str );
}

QValueList<MetaTranslatorMessage> MetaTranslator::messages() const
{
    int n = (int)mm.count();
    TMM::ConstIterator *t = new TMM::ConstIterator[n + 1];
    TMM::ConstIterator m;
    for ( m = mm.begin(); m != mm.end(); ++m )
	t[*m] = m;

    QValueList<MetaTranslatorMessage> val;
    for ( int i = 0; i < n; i++ )
	val.append( t[i].key() );

    delete[] t;
    return val;
}

QValueList<MetaTranslatorMessage> MetaTranslator::translatedMessages() const
{
    QValueList<MetaTranslatorMessage> val;
    TMM::ConstIterator m;
    for ( m = mm.begin(); m != mm.end(); ++m ) {
	if ( m.key().type() == MetaTranslatorMessage::Finished )
	    val.append( m.key() );
    }
    return val;
}
