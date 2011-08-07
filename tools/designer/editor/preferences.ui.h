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

void PreferencesBase::init()
{
    QFontDatabase fdb;
    comboFamily->insertStringList( fdb.families() );
    listElements->setCurrentItem( listElements->firstItem() );
    currentElement = "";
}

void PreferencesBase::destroy()
{

}

void PreferencesBase::colorClicked()
{
    QColor c = QColorDialog::getColor( currentStyle.color, this, "editor_getcolor_dlg" );
    if ( c.isValid() ) {
	currentStyle.color = c;
	setColorPixmap( c );
    }
}

void PreferencesBase::reInit()
{
    styles = Config::readStyles( path );
    currentElement = "";
    elementChanged( "Comment" );
    for ( int i = 0; i < comboFamily->count(); ++i ) {
	if ( listElements->text( i ) == "Comment" ) {
	    listElements->setCurrentItem( i );
	    break;
	}
    }
    checkWordWrap->setChecked( Config::wordWrap( path ) );
    checkCompletion->setChecked( Config::completion( path ) );
    checkParenMatching->setChecked( Config::parenMatching( path ) );
    spinTabSize->setValue( Config::indentTabSize( path ) );
    spinIndentSize->setValue( Config::indentIndentSize( path ) );
    checkKeepTabs->setChecked( Config::indentKeepTabs( path ) );
    checkAutoIndent->setChecked( Config::indentAutoIndent( path ) );
}

void PreferencesBase::save()
{
    if ( !currentElement.isEmpty() ) {
	styles.remove( currentElement );
	styles.insert( currentElement, currentStyle );
	currentElement = "";
    }

    QSettings settings;
    Config::saveStyles( styles, path );
    Config::setWordWrap( checkWordWrap->isChecked(), path );
    Config::setCompletion( checkCompletion->isChecked(), path );
    Config::setParenMatching( checkParenMatching->isChecked(), path );
    Config::setIndentTabSize( spinTabSize->value(), path );
    Config::setIndentIndentSize( spinIndentSize->value(), path );
    Config::setIndentKeepTabs( checkKeepTabs->isChecked(), path );
    Config::setIndentAutoIndent( checkAutoIndent->isChecked(), path );
}

void PreferencesBase::updatePreview()
{
    editPreview->setFont( currentStyle.font );
    QPalette pal = editPreview->palette();
    pal.setColor( QPalette::Active, QColorGroup::Text, currentStyle.color );
    pal.setColor( QPalette::Active, QColorGroup::Foreground, currentStyle.color );
    editPreview->setPalette( pal );
}

void PreferencesBase::boldChanged( bool b )
{
    currentStyle.font.setBold( b );
    updatePreview();
}

void PreferencesBase::elementChanged( const QString &element )
{
    if ( !currentElement.isEmpty() ) {
	styles.remove( currentElement );
	styles.insert( currentElement, currentStyle );
	currentElement = "";
    }
    QMap<QString, ConfigStyle>::Iterator it = styles.find( element );
    if ( it == styles.end() )
	return;
    ConfigStyle s = *it;
    currentStyle = s;
    comboFamily->lineEdit()->setText( s.font.family() );
    spinSize->setValue( s.font.pointSize() );
    checkBold->setChecked( s.font.bold() );
    checkItalic->setChecked( s.font.italic() );
    checkUnderline->setChecked( s.font.underline() );
    setColorPixmap( s.color );
    currentElement = element;
    updatePreview();
}

void PreferencesBase::familyChanged( const QString &f )
{
    QString oldFamily = currentStyle.font.family();
    currentStyle.font.setFamily( f );
    if ( currentElement == "Standard" ) {
	for ( QMap<QString, ConfigStyle>::Iterator it = styles.begin(); it != styles.end(); ++it ) {
	    if ( (*it).font.family() == oldFamily )
		(*it).font.setFamily( f );
	}
    }
    updatePreview();
}

void PreferencesBase::italicChanged( bool b )
{
    currentStyle.font.setItalic( b );
    updatePreview();
}

void PreferencesBase::setColorPixmap( const QColor &c )
{
    QPixmap pm( 20, 20 );
    pm.fill( c );
    buttonColor->setPixmap( pm );
    updatePreview();
}

void PreferencesBase::setPath( const QString &p )
{
    path = p;
}

void PreferencesBase::sizeChanged( int s )
{
    int oldSize = currentStyle.font.pointSize();
    currentStyle.font.setPointSize( s );
    if ( currentElement == "Standard" ) {
	for ( QMap<QString, ConfigStyle>::Iterator it = styles.begin(); it != styles.end(); ++it ) {
	    if ( (*it).font.pointSize() == oldSize )
		(*it).font.setPointSize( s );
	}
    }
    updatePreview();
}

void PreferencesBase::underlineChanged( bool b )
{
    currentStyle.font.setUnderline( b );
    updatePreview();
}
