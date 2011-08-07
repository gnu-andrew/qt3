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

#ifndef ACTIONINTERFACE_H
#define ACTIONINTERFACE_H

#include <private/qcom_p.h>

class QAction;
class QObject;

// {bb206e09-84e5-4777-9fce-706babfab931}
#ifndef IID_Action
#define IID_Action QUuid( 0xbb206e09, 0x84e5, 0x4777, 0x9f, 0xce, 0x70, 0x6b, 0xab, 0xfa, 0xb9, 0x31 )
#endif

/*! To add actions to the Qt Designer menubars and toolbars, implement
  this interface. You have to implement the create(), group() and
  connectTo() functions.

  You also have to implement the function featureList() (\sa
  QFeatureListInterface) to return the names of all actions
  which this interface provides.
*/

class ActionInterface : public QFeatureListInterface
{
public:
    enum Location {
	Toolbar,
	Menu
    };

    /*! This functions is called to create the action with the name \a
      name. \a parent should be used as parent of the action.

      In the implementation return the QAction object for the action
      \a name.
    */
    virtual QAction* create( const QString &name, QObject* parent = 0 ) = 0;

    /*! In the implementation of the interface return the name of the
      group of the action \a name.
    */
    virtual QString group( const QString &name ) const = 0;

    /*! In the implementation of the interface return whether the
      action \a name should appear in the location \a l */
    virtual bool location( const QString &name, Location l ) const = 0;

    /*! \internal */
    virtual void connectTo( QUnknownInterface *appInterface ) = 0;
};

#endif
