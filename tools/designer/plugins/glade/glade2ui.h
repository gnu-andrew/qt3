/**********************************************************************
**
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

#ifndef GLADE2UI_H
#define GLADE2UI_H

#include <qdom.h>
#include <qmap.h>
#include <qstring.h>
#include <qvariant.h>

typedef QMap<QString, QString> AttributeMap;

struct GladeAction
{
    QString text;
    QString menuText;
    QString toolTip;
    int accel;
    QString iconSet;
};

struct GladeConnection
{
    QString sender;
    QString signal;
    QString slot;
};

class Glade2Ui
{
public:
    Glade2Ui();

    QStringList convertGladeFile( const QString& fileName );

private:
    QString imageName( const QString& fileName );
    QString opening( const QString& tag,
		     const AttributeMap& attr = AttributeMap() );
    QString closing( const QString& tag );
    QString atom( const QString& tag,
		  const AttributeMap& attr = AttributeMap() );
    void error( const QString& message );
    void syntaxError();
    QString getTextValue( const QDomNode& node );
    void emitHeader();
    void emitFooter();
    void emitSimpleValue( const QString& tag, const QString& value,
			  const AttributeMap& attr = AttributeMap() );
    void emitOpening( const QString& tag,
		      const AttributeMap& attr = AttributeMap() );
    void emitClosing( const QString& tag );
    void emitAtom( const QString& tag,
		   const AttributeMap& attr = AttributeMap() );
    void emitVariant( const QVariant& val,
		      const QString& stringType = "string" );
    void emitProperty( const QString& prop, const QVariant& val,
		       const QString& stringType = "string" );
    void emitFontProperty( const QString& prop, int pointSize, bool bold );
    void emitAttribute( const QString& prop, const QVariant& val,
			const QString& stringType = "string" );
    QString gtk2qtClass( const QString& gtkClass,
			 const QValueList<QDomElement>& childWidgets );

    int matchAccelOnActivate( const QDomElement& accel );
    void emitGtkMenu( const QDomElement& menu );
    void emitGtkMenuBarChildWidgets(
	    const QValueList<QDomElement>& childWidgets );
    void emitGtkToolbarChildWidgets(
	    const QValueList<QDomElement>& childWidgets );
    void emitPushButton( const QString& text, const QString& name );
    void attach( AttributeMap *attr, int leftAttach, int rightAttach,
		 int topAttach, int bottomAttach );
    void emitSpacer( const QString& orientation, int leftAttach = -1,
		     int rightAttach = -1, int topAttach = -1,
		     int bottomAttach = -1 );
    void emitPixmap( const QString& imageName, int leftAttach = -1,
		     int rightAttach = -1, int topAttach = -1,
		     int bottomAttach = -1 );
    void emitGnomeAbout( QString copyright, QString authors, QString comments );
    void emitGnomeAppChildWidgetsPass1(
	    const QValueList<QDomElement>& childWidgets );
    void doPass2( const QValueList<QDomElement>& childWidgets,
		  QValueList<QDomElement> *menuBar,
		  QValueList<QValueList<QDomElement> > *toolBars );
    void emitGnomeAppChildWidgetsPass2(
	    const QValueList<QDomElement>& childWidgets );
    void emitGtkButtonChildWidgets( QValueList<QDomElement> childWidgets );
    void emitGtkComboChildWidgets( const QValueList<QDomElement>& childWidgets,
				   const QStringList& items );
    void emitGtkNotebookChildWidgets(
	    const QValueList<QDomElement>& childWidgets );
    void emitQListViewColumns( const QDomElement& qlistview );
    void emitGtkScrolledWindowChildWidgets(
	    const QValueList<QDomElement>& childWidgets,
	    const QString& qtClass );
    void emitGnomeDruidPage( const QDomElement& druidPage );
    void emitGtkWindowChildWidgets( const QValueList<QDomElement>& childWidgets,
				    const QString& qtClass );
    bool packEnd( const QDomElement& widget );
    void emitChildWidgets( const QValueList<QDomElement>& childWidgets,
			   bool layouted, int leftAttach = -1,
			   int rightAttach = -1, int topAttach = -1,
			   int bottomAttach = -1 );
    void emitOpeningWidget( const QString& qtClass, int leftAttach = -1,
			    int rightAttach = -1, int topAttach = -1,
			    int bottomAttach = -1 );
    bool shouldPullup( const QValueList<QDomElement>& childWidgets );
    QString emitWidget( const QDomElement& widget, bool layouted,
			int leftAttach = -1, int rightAttach = -1,
			int topAttach = -1, int bottomAttach = -1 );

    QString yyOut;
    QString yyIndentStr;
    QString yyFileName;
    QString yyProgramName;
    QString yyPixmapDirectory;
    QMap<QString, QString> yyClassNameMap;
    QMap<QString, QString> yyStockMap;
    QMap<QString, int> yyKeyMap;
    QMap<QString, QString> yyCustomWidgets;
    QMap<QString, QString> yyStockItemActions;
    QMap<QString, GladeAction> yyActions;
    QValueList<GladeConnection> yyConnections;
    QMap<QString, QString> yySlots;
    QString yyFormName;
    QMap<QString, QString> yyImages;

    int numErrors;
    int uniqueAction;
    int uniqueForm;
    int uniqueMenuBar;
    int uniqueSpacer;
    int uniqueToolBar;
};

#endif
