/****************************************************************************
**
** Definition of QCheckBox class
**
** Created : 940222
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QCHECKBOX_H
#define QCHECKBOX_H

#ifndef QT_H
#include "qbutton.h"
#endif // QT_H

#ifndef QT_NO_CHECKBOX

class Q_EXPORT QCheckBox : public QButton
{
    Q_OBJECT
    Q_PROPERTY( bool checked READ isChecked WRITE setChecked )
    Q_PROPERTY( bool tristate READ isTristate WRITE setTristate )
    Q_OVERRIDE( bool autoMask DESIGNABLE true SCRIPTABLE true )

public:
    QCheckBox( QWidget *parent, const char* name=0 );
    QCheckBox( const QString &text, QWidget *parent, const char* name=0 );

    bool    isChecked() const;

    void    setNoChange();

    void    setTristate(bool y=TRUE);
    bool    isTristate() const;

    QSize   sizeHint() const;

public slots:
    void    setChecked( bool check );

protected:
    void    resizeEvent( QResizeEvent* );
    void    drawButton( QPainter * );
    void    drawButtonLabel( QPainter * );
    void    updateMask();
    bool    hitButton( const QPoint &pos ) const;
private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QCheckBox( const QCheckBox & );
    QCheckBox &operator=( const QCheckBox & );
#endif
};


inline bool QCheckBox::isChecked() const
{ return isOn(); }

inline void QCheckBox::setChecked( bool check )
{ setOn( check ); }


#endif // QT_NO_CHECKBOX

#endif // QCHECKBOX_H
