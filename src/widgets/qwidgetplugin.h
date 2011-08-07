/****************************************************************************
**
** Definition of QWidgetPlugin class
**
** Created : 010920
**
** Copyright (C) 2001-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QWIDGETPLUGIN_H
#define QWIDGETPLUGIN_H

#ifndef QT_H
#include "qgplugin.h"
#include "qstringlist.h"
#include "qiconset.h"
#endif // QT_H
#ifndef QT_NO_WIDGETPLUGIN

#ifdef Q_WS_WIN
#ifdef QT_PLUGIN
#define QT_WIDGET_PLUGIN_EXPORT __declspec(dllexport)
#else
#define QT_WIDGET_PLUGIN_EXPORT __declspec(dllimport)
#endif
#else
#define QT_WIDGET_PLUGIN_EXPORT
#endif

class QWidgetPluginPrivate;
class QWidget;

class Q_EXPORT QWidgetPlugin : public QGPlugin
{
    Q_OBJECT
public:
    QWidgetPlugin();
    ~QWidgetPlugin();

    virtual QStringList keys() const = 0;
    virtual QWidget *create( const QString &key, QWidget *parent = 0, const char *name = 0 ) = 0;

    virtual QString group( const QString &key ) const;
    virtual QIconSet iconSet( const QString &key ) const;
    virtual QString includeFile( const QString &key ) const;
    virtual QString toolTip( const QString &key ) const;
    virtual QString whatsThis( const QString &key ) const;
    virtual bool isContainer( const QString &key ) const;

private:
    QWidgetPluginPrivate *d;
};

#ifdef QT_CONTAINER_CUSTOM_WIDGETS

class QWidgetContainerPluginPrivate;

class Q_EXPORT QWidgetContainerPlugin : public QWidgetPlugin
{

public:
    QWidgetContainerPlugin();
    ~QWidgetContainerPlugin();

    virtual QWidget* containerOfWidget( const QString &key, QWidget *container ) const;
    virtual bool isPassiveInteractor( const QString &key, QWidget *container ) const;

    virtual bool supportsPages( const QString &key ) const;

    virtual QWidget *addPage( const QString &key, QWidget *container,
			      const QString &name, int index ) const;
    virtual void insertPage( const QString &key, QWidget *container,
			     const QString &name, int index, QWidget *page ) const;
    virtual void removePage( const QString &key, QWidget *container, int index ) const;
    virtual void movePage( const QString &key, QWidget *container, int fromIndex, int toIndex ) const;
    virtual int count( const QString &key, QWidget *container ) const;
    virtual int currentIndex( const QString &key, QWidget *container ) const;
    virtual QString pageLabel( const QString &key, QWidget *container, int index ) const;
    virtual QWidget *page( const QString &key, QWidget *container, int index ) const;
    virtual void renamePage( const QString &key, QWidget *container,
			     int index, const QString &newName ) const;
    virtual QWidgetList pages( const QString &key, QWidget *container ) const;
    virtual QString createCode( const QString &key, const QString &container,
				const QString &page, const QString &pageName ) const;
};

#endif // QT_CONTAINER_CUSTOM_WIDGETS
#endif // QT_NO_WIDGETPLUGIN
#endif // QWIDGETPLUGIN_H
