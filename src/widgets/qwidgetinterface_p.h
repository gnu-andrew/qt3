/****************************************************************************
**
** ...
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the widgets module of the Qt GUI Toolkit.
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

#ifndef QWIDGETINTERFACE_P_H
#define QWIDGETINTERFACE_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of a number of Qt sources files.  This header file may change from
// version to version without notice, or even be removed.
//
// We mean it.
//
//

#ifndef QT_H
#include <private/qcom_p.h>
#include "qiconset.h"
#endif // QT_H

#ifndef QT_NO_WIDGETPLUGIN

class QWidget;

// {55184143-f18f-42c0-a8eb-71c01516019a}
#ifndef IID_QWidgetFactory
#define IID_QWidgetFactory QUuid( 0x55184143, 0xf18f, 0x42c0, 0xa8, 0xeb, 0x71, 0xc0, 0x15, 0x16, 0x1, 0x9a )
#endif

/*! To add custom widgets to the Qt Designer, implement that interface
  in your custom widget plugin.

  You also have to implement the function featureList() (\sa
  QFeatureListInterface) and return there all widgets (names of it)
  which this interface provides.
*/

struct QWidgetFactoryInterface : public QFeatureListInterface
{
public:

    /*! In the implementation create and return the widget \a widget
      here, use \a parent and \a name when creating the widget */
    virtual QWidget* create( const QString &widget, QWidget* parent = 0, const char* name = 0 ) = 0;

    /*! In the implementation return the name of the group of the
      widget \a widget */
    virtual QString group( const QString &widget ) const = 0;

    /*! In the implementation return the iconset, which should be used
      in the Qt Designer menubar and toolbar to represent the widget
      \a widget */
    virtual QIconSet iconSet( const QString &widget ) const = 0;

    /*! In the implementation return the include file which is needed
      for the widget \a widget in the generated code which uic
      generates. */
    virtual QString includeFile( const QString &widget ) const = 0;

    /*! In the implementation return the text which should be
      displayed as tooltip for the widget \a widget */
    virtual QString toolTip( const QString &widget ) const = 0;

    /*! In the implementation return the text which should be used for
      what's this help for the widget \a widget. */
    virtual QString whatsThis( const QString &widget ) const = 0;

    /*! In the implementation return TRUE here, of the \a widget
      should be able to contain other widget in the Qt Designer, else
      FALSE. */
    virtual bool isContainer( const QString &widget ) const = 0;
};

#ifdef QT_CONTAINER_CUSTOM_WIDGETS
// {15976628-e3c3-47f4-b525-d124a3caf30e}
#ifndef IID_QWidgetContainer
#define IID_QWidgetContainer QUuid( 0x15976628, 0xe3c3, 0x47f4, 0xb5, 0x25, 0xd1, 0x24, 0xa3, 0xca, 0xf3, 0x0e )
#endif

struct QWidgetContainerInterfacePrivate : public QUnknownInterface
{
public:
    virtual QWidget *containerOfWidget( const QString &f, QWidget *container ) const = 0;
    virtual bool isPassiveInteractor( const QString &f, QWidget *container ) const = 0;

    virtual bool supportsPages( const QString &f ) const = 0;

    virtual QWidget *addPage( const QString &f, QWidget *container,
			      const QString &name, int index ) const = 0;
    virtual void insertPage( const QString &f, QWidget *container,
			     const QString &name, int index, QWidget *page ) const = 0;
    virtual void removePage( const QString &f, QWidget *container, int index ) const = 0;
    virtual void movePage( const QString &f, QWidget *container, int fromIndex, int toIndex ) const = 0;
    virtual int count( const QString &key, QWidget *container ) const = 0;
    virtual int currentIndex( const QString &key, QWidget *container ) const = 0;
    virtual QString pageLabel( const QString &key, QWidget *container, int index ) const = 0;
    virtual QWidget *page( const QString &key, QWidget *container, int index ) const = 0;
    virtual void renamePage( const QString &key, QWidget *container,
			     int index, const QString &newName ) const = 0;
    virtual QWidgetList pages( const QString &f, QWidget *container ) const = 0;
    virtual QString createCode( const QString &f, const QString &container,
				const QString &page, const QString &pageName ) const = 0;
};

#endif // QT_CONTAINER_CUSTOM_WIDGETS
#endif // QT_NO_WIDGETPLUGIN
#endif // QWIDGETINTERFACE_P_H
