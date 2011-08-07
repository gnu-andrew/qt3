/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt Configuration.
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

#include "colorbutton.h"

#include <qapplication.h>
#include <qcolordialog.h>
#include <qpainter.h>
#include <qdragobject.h>
#include <qstyle.h>


ColorButton::ColorButton(QWidget *parent, const char *name)
    : QButton(parent, name), mousepressed(FALSE)
{
    setAcceptDrops(TRUE);
    col = black;
    connect(this, SIGNAL(clicked()), SLOT(changeColor()));
}


ColorButton::ColorButton(const QColor &c, QWidget *parent, const char *name)
    : QButton(parent, name)
{
    setAcceptDrops(TRUE);
    col = c;
    connect(this, SIGNAL(clicked()), SLOT(changeColor()));
}


void ColorButton::setColor(const QColor &c)
{
    col = c;
    update();
}


void ColorButton::changeColor()
{
    QColor c = QColorDialog::getColor(col, qApp->activeWindow());

    if (c.isValid()) {
	setColor(c);
	emit colorChanged(color());
    }
}


QSize ColorButton::sizeHint() const
{
    return QSize(40, 25);
}


QSize ColorButton::minimumSizeHint() const
{
    return QSize(40, 25);
}


void ColorButton::drawButton(QPainter *p)
{
    style().drawPrimitive(QStyle::PE_ButtonBevel, p, rect(), colorGroup(),
			  isDown() ? QStyle::Style_Down : QStyle::Style_Raised);
    drawButtonLabel(p);

    if (hasFocus())
	style().drawPrimitive(QStyle::PE_FocusRect, p,
			      style().subRect(QStyle::SR_PushButtonFocusRect, this),
			      colorGroup(), QStyle::Style_Default);
}


void ColorButton::drawButtonLabel(QPainter *p)
{
    QColor pen = (isEnabled() ?
		  (hasFocus() ? palette().active().buttonText() :
		   palette().inactive().buttonText())
		  : palette().disabled().buttonText());
    p->setPen( pen );
    p->setBrush(col);
    p->drawRect(width() / 4, height() / 4, width() / 2, height() / 2);
}


void ColorButton::dragEnterEvent(QDragEnterEvent *e)
{
    if (! QColorDrag::canDecode(e)) {
	e->ignore();
	return;
    }
}


void ColorButton::dragMoveEvent(QDragMoveEvent *e)
{
    if (! QColorDrag::canDecode(e)) {
	e->ignore();
	return;
    }

    e->accept();
}


void ColorButton::dropEvent(QDropEvent *e)
{
    if (! QColorDrag::canDecode(e)) {
	e->ignore();
	return;
    }

    QColor c;
    QColorDrag::decode(e, c);
    setColor(c);
    emit colorChanged(color());
}


void ColorButton::mousePressEvent(QMouseEvent *e)
{
    presspos = e->pos();
    mousepressed = TRUE;
    QButton::mousePressEvent(e);
}


void ColorButton::mouseReleaseEvent(QMouseEvent *e)
{
    mousepressed = FALSE;
    QButton::mouseReleaseEvent(e);
}


void ColorButton::mouseMoveEvent(QMouseEvent *e)
{
    if (! mousepressed)
	return;

    if ((presspos - e->pos()).manhattanLength() > QApplication::startDragDistance()) {
	mousepressed = FALSE;
	setDown(FALSE);

	QColorDrag *cd = new QColorDrag(color(), this);
	cd->dragCopy();
    }
}
