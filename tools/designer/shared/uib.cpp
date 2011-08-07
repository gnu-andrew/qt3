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

#include "uib.h"

static const char commonStrings[] =
    "\0()\0(bool)\0(const QString&)\0(int)\0C++\0Layout1\0PNG\0QCheckBox\0"
    "QComboBox\0QDialog\0QFrame\0QGridLayout\0QGroupBox\0QHBoxLayout\0QLabel\0"
    "QLineEdit\0QListView\0QPushButton\0QRadioButton\0QVBoxLayout\0QWidget\0"
    "TextLabel1\0XPM.GZ\0accept\0autoDefault\0buddy\0caption\0clicked\0"
    "default\0destroy\0frameShadow\0frameShape\0geometry\0init\0margin\0"
    "maximumSize\0minimumSize\0name\0reject\0sizePolicy\0spacing\0text\0title\0"
    "toolTip\0unnamed\0whatsThis";

UibStrTable::UibStrTable()
    : out( table, IO_WriteOnly ), start( sizeof(commonStrings) )
{
    out.writeRawBytes( commonStrings, start );
}
