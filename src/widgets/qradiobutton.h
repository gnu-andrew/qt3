/****************************************************************************
**
** Definition of QRadioButton class
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

#ifndef QRADIOBUTTON_H
#define QRADIOBUTTON_H

#ifndef QT_H
#include "qbutton.h"
#endif // QT_H

#ifndef QT_NO_RADIOBUTTON

class Q_EXPORT QRadioButton : public QButton
{
    Q_OBJECT
    Q_PROPERTY( bool checked READ isChecked WRITE setChecked )
    Q_OVERRIDE( bool autoMask DESIGNABLE true SCRIPTABLE true )

public:
    QRadioButton( QWidget *parent, const char* name=0 );
    QRadioButton( const QString &text, QWidget *parent, const char* name=0 );

    bool    isChecked() const;

    QSize    sizeHint() const;

public slots:
    virtual void    setChecked( bool check );

protected:
    bool    hitButton( const QPoint & ) const;
    void    drawButton( QPainter * );
    void    drawButtonLabel( QPainter * );
    void    updateMask();

    void    resizeEvent( QResizeEvent* );

private:
    void    init();

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QRadioButton( const QRadioButton & );
    QRadioButton &operator=( const QRadioButton & );
#endif
};


inline bool QRadioButton::isChecked() const
{ return isOn(); }

#endif // QT_NO_RADIOBUTTON

#endif // QRADIOBUTTON_H
