/****************************************************************************
**
** Definition of some Qt private functions.
**
** Created : 000228
**
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the kernel module of the Qt GUI Toolkit.
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

#ifndef QAPPLICATION_P_H
#define QAPPLICATION_P_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of qapplication_*.cpp, qwidget*.cpp, qcolor_x11.cpp, qfiledialog.cpp
// and many other.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//
//

#ifndef QT_H
#endif // QT_H

class QWidget;
class QObject;
class QClipboard;
class QKeyEvent;
class QMouseEvent;
class QWheelEvent;

extern Q_EXPORT bool qt_modal_state();
extern Q_EXPORT void qt_enter_modal( QWidget* );
extern Q_EXPORT void qt_leave_modal( QWidget* );

extern bool qt_is_gui_used;
#ifndef QT_NO_CLIPBOARD
extern QClipboard *qt_clipboard;
#endif

#if defined (Q_OS_WIN32) || defined (Q_OS_CYGWIN)
extern Qt::WindowsVersion qt_winver;
const int QT_TABLET_NPACKETQSIZE = 128;
# ifdef Q_OS_TEMP
  extern DWORD qt_cever;
# endif
#elif defined (Q_OS_MAC)
extern Qt::MacintoshVersion qt_macver;
#endif

#if defined (Q_WS_X11)
extern int qt_ncols_option;
#endif


extern void qt_dispatchEnterLeave( QWidget*, QWidget* );
extern bool qt_tryModalHelper( QWidget *, QWidget ** = 0 );

#endif
