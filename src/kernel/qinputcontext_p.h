/****************************************************************************
**
** Definition of ???
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
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

#ifndef QINPUTCONTEXT_P_H
#define QINPUTCONTEXT_P_H


//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of internal files.  This header file may change from version to version
// without notice, or even be removed.
//
// We mean it.
//
//

#include "qglobal.h"

class QKeyEvent;
class QWidget;
class QFont;
class QString;


#ifdef Q_WS_X11
#include "qarray.h"
#include "qwindowdefs.h"
#include "qt_x11_p.h"
#endif

#ifdef Q_WS_WIN
#include "qt_windows.h"
#endif

#ifdef Q_WS_QWS
class QWSIMEvent;
#endif

class QInputContext
{
public:
#ifdef Q_WS_X11
    QInputContext(QWidget *); // should be a toplevel widget
    ~QInputContext();

    void setFocus();
    void setComposePosition(int, int);
    void setComposeArea(int, int, int, int);
    void reset();

    int lookupString(XKeyEvent *, QCString &, KeySym *, Status *) const;
    void setXFontSet(const QFont &);

    void *ic;
    QString text;
    QWidget *focusWidget;
    bool composing;
    QFont font;
    XFontSet fontset;
    QMemArray<bool> selectedChars;
#endif // Q_WS_X11

#ifdef Q_WS_QWS
    static void translateIMEvent( QWSIMEvent *, QWidget * );
    static void reset();
private:
    static QWidget* focusWidget;
    static QString* composition;
#endif //Q_WS_QWS

#ifdef Q_WS_WIN
    static void init();
    static void shutdown();

    static void TranslateMessage( const MSG *msg);
    static LRESULT DefWindowProc( HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam );

    static void setFont( const QWidget *w, const QFont & );
    static void setFocusHint( int x, int y, int w, int h, const QWidget *widget );
    static bool startComposition();
    static bool endComposition( QWidget *fw = 0 );
    static bool composition( LPARAM lparam );

    static void accept( QWidget *fw = 0 );
    static void enable( QWidget *w, bool b );
#endif
};

#endif // QINPUTCONTEXT_P_H
