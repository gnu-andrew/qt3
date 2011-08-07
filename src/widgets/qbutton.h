/****************************************************************************
**
** Definition of QButton widget class
**
** Created : 940206
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

#ifndef QBUTTON_H
#define QBUTTON_H

#ifndef QT_H
#include "qwidget.h"
#include "qkeysequence.h"
#endif // QT_H

#ifndef QT_NO_BUTTON


class QButtonGroup;
class QToolBar;
class QButtonData;

class Q_EXPORT QButton : public QWidget
{
    Q_OBJECT
    Q_ENUMS( ToggleType ToggleState )
    Q_PROPERTY( QString text READ text WRITE setText )
    Q_PROPERTY( QPixmap pixmap READ pixmap WRITE setPixmap )
    Q_PROPERTY( QKeySequence accel READ accel WRITE setAccel )
    Q_PROPERTY( bool toggleButton READ isToggleButton )
    Q_PROPERTY( ToggleType toggleType READ toggleType )
    Q_PROPERTY( bool down READ isDown WRITE setDown DESIGNABLE false  )
    Q_PROPERTY( bool on READ isOn )
    Q_PROPERTY( ToggleState toggleState READ state )
    Q_PROPERTY( bool autoResize READ autoResize WRITE setAutoResize DESIGNABLE false )
    Q_PROPERTY( bool autoRepeat READ autoRepeat WRITE setAutoRepeat )
    Q_PROPERTY( bool exclusiveToggle READ isExclusiveToggle )

public:
    QButton( QWidget* parent=0, const char* name=0, WFlags f=0 );
    ~QButton();

    QString text() const;
    virtual void setText( const QString &);
    const QPixmap *pixmap() const;
    virtual void setPixmap( const QPixmap & );

#ifndef QT_NO_ACCEL
    QKeySequence		accel()	const;
    virtual void	setAccel( const QKeySequence& );
#endif

    bool	isToggleButton() const;

    enum ToggleType { SingleShot, Toggle, Tristate };
    ToggleType	toggleType() const;

    virtual void setDown( bool );
    bool	isDown() const;

    bool	isOn() const;

    enum ToggleState { Off, NoChange, On };
    ToggleState	state() const;

#ifndef QT_NO_COMPAT
    bool	autoResize() const;
    void	setAutoResize( bool );
#endif

    bool	autoRepeat() const;
    virtual void setAutoRepeat( bool );
    bool	isExclusiveToggle() const;

    QButtonGroup *group() const;

public slots:
    void	animateClick();
    void	toggle();

signals:
    void	pressed();
    void	released();
    void	clicked();
    void	toggled( bool );
    void	stateChanged( int );

protected:
    void	setToggleButton( bool );
    virtual void	setToggleType( ToggleType );
    void	setOn( bool );
    virtual void	setState( ToggleState );

    virtual bool hitButton( const QPoint &pos ) const;
    virtual void drawButton( QPainter * );
    virtual void drawButtonLabel( QPainter * );

    void	keyPressEvent( QKeyEvent *);
    void	keyReleaseEvent( QKeyEvent *);
    void	mousePressEvent( QMouseEvent * );
    void	mouseReleaseEvent( QMouseEvent * );
    void	mouseMoveEvent( QMouseEvent * );
    void	paintEvent( QPaintEvent * );
    void	focusInEvent( QFocusEvent * );
    void	focusOutEvent( QFocusEvent * );

    void	enabledChange( bool );

private slots:
    void	animateTimeout();
    void	autoRepeatTimeout();
    void	emulateClick();

private:
    QString	btext;
    QPixmap    *bpixmap;
    uint	toggleTyp	: 2;
    uint	buttonDown	: 1;
    uint	stat		: 2;
    uint	mlbDown		: 1;
    uint	autoresize	: 1;
    uint	animation	: 1;
    uint	repeat		: 1;
    QButtonData *d;

    friend class QButtonGroup;
    friend class QToolBar;
    void          ensureData();
    virtual void setGroup( QButtonGroup* );
    QTimer	 *timer();
    void	nextState();

private:	// Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QButton( const QButton & );
    QButton &operator=( const QButton & );
#endif
};


inline QString QButton::text() const
{
    return btext;
}

inline const QPixmap *QButton::pixmap() const
{
    return bpixmap;
}

inline bool QButton::isToggleButton() const
{
    return toggleTyp != SingleShot;
}

inline  bool QButton::isDown() const
{
    return buttonDown;
}

inline bool QButton::isOn() const
{
    return stat != Off;
}

#ifndef QT_NO_COMPAT
inline bool QButton::autoResize() const
{
    return autoresize;
}
#endif

inline bool QButton::autoRepeat() const
{
    return repeat;
}

inline QButton::ToggleState QButton::state() const
{
    return ToggleState(stat);
}

inline void QButton::setToggleButton( bool b )
{
    setToggleType( b ? Toggle : SingleShot );
}

inline void QButton::setOn( bool y )
{
    setState( y ? On : Off );
}

inline QButton::ToggleType QButton::toggleType() const
{
    return ToggleType(toggleTyp);
}


#endif // QT_NO_BUTTON

#endif // QBUTTON_H
