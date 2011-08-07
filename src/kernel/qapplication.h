/****************************************************************************
**
** Definition of QApplication class
**
** Created : 931107
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

#ifndef QAPPLICATION_H
#define QAPPLICATION_H

#ifndef QT_H
#include "qdesktopwidget.h"
#include "qasciidict.h"
#include "qpalette.h"
#include "qtranslator.h"
#include "qstrlist.h"
#include "qstringlist.h"
#endif // QT_H

class QSessionManager;
class QStyle;
class QTranslator;
class QEventLoop;
#if defined(Q_WS_QWS)
class QWSDecoration;
#endif

#ifdef QT_THREAD_SUPPORT
class QMutex;
#endif // QT_THREAD_SUPPORT


class QApplication;
extern Q_EXPORT QApplication *qApp;		// global application object


class Q_EXPORT QApplication : public QObject
{
    Q_OBJECT
public:
    QApplication( int &argc, char **argv );
    QApplication( int &argc, char **argv, bool GUIenabled );
    enum Type { Tty, GuiClient, GuiServer };
    QApplication( int &argc, char **argv, Type );
#if defined(Q_WS_X11)
    QApplication( Display* dpy, HANDLE visual = 0, HANDLE cmap = 0 );
    QApplication( Display *dpy, int argc, char **argv,
		  HANDLE visual = 0, HANDLE cmap= 0 );
#endif
    virtual ~QApplication();

    int		    argc()	const;
    char	  **argv()	const;

    Type type() const;

#ifndef QT_NO_STYLE
    static QStyle  &style();
    static void	    setStyle( QStyle* );
    static QStyle*  setStyle( const QString& );
#endif
#ifndef Q_QDOC
    enum ColorMode { NormalColors, CustomColors };
    static ColorMode colorMode();
    static void      setColorMode( QApplication::ColorMode );
#endif

    enum ColorSpec { NormalColor=0, CustomColor=1, ManyColor=2 };
    static int	     colorSpec();
    static void      setColorSpec( int );
#ifndef QT_NO_CURSOR
    static QCursor  *overrideCursor();
    static void	     setOverrideCursor( const QCursor &, bool replace=FALSE );
    static void	     restoreOverrideCursor();
#endif
    static bool	     hasGlobalMouseTracking();
    static void	     setGlobalMouseTracking( bool enable );
#ifndef QT_NO_PALETTE
    static QPalette  palette( const QWidget* = 0 );
    static void	     setPalette( const QPalette &, bool informWidgets=FALSE,
				 const char* className = 0 );
#endif
    static QFont     font( const QWidget* = 0 );
    static void	     setFont( const QFont &, bool informWidgets=FALSE,
			      const char* className = 0 );
    static QFontMetrics fontMetrics();

    QWidget	    *mainWidget()  const;
    virtual void     setMainWidget( QWidget * );
    virtual void     polish( QWidget * );

    static QWidgetList *allWidgets();
    static QWidgetList *topLevelWidgets();

    static QDesktopWidget   *desktop();

    static QWidget     *activePopupWidget();
    static QWidget     *activeModalWidget();
#ifndef QT_NO_CLIPBOARD
    static QClipboard  *clipboard();
#endif
    QWidget	       *focusWidget() const;
    QWidget	       *activeWindow() const;

    static QWidget  *widgetAt( int x, int y, bool child=FALSE );
    static QWidget  *widgetAt( const QPoint &, bool child=FALSE );

    static QEventLoop *eventLoop();

    int		     exec();
    void	     processEvents();
    void	     processEvents( int maxtime );
    void	     processOneEvent();
    bool	     hasPendingEvents();
    int		     enter_loop();
    void	     exit_loop();
    int		     loopLevel() const;
    static void	     exit( int retcode=0 );

    static bool	     sendEvent( QObject *receiver, QEvent *event );
    static void	     postEvent( QObject *receiver, QEvent *event );
    static void	     sendPostedEvents( QObject *receiver, int event_type );
    static void	     sendPostedEvents();

    static void      removePostedEvents( QObject *receiver );

    virtual bool     notify( QObject *, QEvent * );

    static bool	     startingUp();
    static bool	     closingDown();

    static void	     flushX();
    static void flush();
    static void	     syncX();

    static void	     beep();

#ifndef QT_NO_TRANSLATION
# ifndef QT_NO_TEXTCODEC
    void	     setDefaultCodec( QTextCodec * );
    QTextCodec*	     defaultCodec() const;
# endif
    void	     installTranslator( QTranslator * );
    void	     removeTranslator( QTranslator * );
#endif
    enum Encoding { DefaultCodec, UnicodeUTF8 };
    QString	     translate( const char * context,
				const char * key,
				const char * comment = 0,
				Encoding encoding = DefaultCodec ) const;
#ifndef QT_NO_DIR
    QString   applicationDirPath();
    QString   applicationFilePath();
#endif
#ifndef QT_NO_PALETTE
    // obsolete functions
    static void      setWinStyleHighlightColor( const QColor &c ) {
	QPalette p( palette() );
	p.setColor( QColorGroup::Highlight, c );
	setPalette( p, TRUE);
    }
    static const QColor &winStyleHighlightColor() {
	return palette().active().highlight();
    }
#endif
    static void      setDesktopSettingsAware( bool );
    static bool      desktopSettingsAware();

    static void      setCursorFlashTime( int );
    static int       cursorFlashTime();

    static void      setDoubleClickInterval( int );
    static int       doubleClickInterval();
#ifndef QT_NO_WHEELEVENT
    static void      setWheelScrollLines( int );
    static int       wheelScrollLines();
#endif
    static void	     setGlobalStrut( const QSize & );
    static QSize     globalStrut();

#ifndef QT_NO_COMPONENT
    static void      setLibraryPaths( const QStringList & );
    static QStringList libraryPaths();
    static void      addLibraryPath( const QString & );
    static void      removeLibraryPath( const QString & );
#endif // QT_NO_COMPONENT
    static void setStartDragTime( int ms );
    static int startDragTime();
    static void setStartDragDistance( int l );
    static int startDragDistance();

    static void setReverseLayout( bool b );
    static bool reverseLayout();

    static int horizontalAlignment( int align );

    static bool	    isEffectEnabled( Qt::UIEffect );
    static void	    setEffectEnabled( Qt::UIEffect, bool enable = TRUE );

#if defined(Q_WS_MAC)
    virtual bool     macEventFilter( EventHandlerCallRef, EventRef );
#endif
#if defined(Q_WS_WIN)
    virtual bool     winEventFilter( MSG * );
#endif
#if defined(Q_WS_X11)
    virtual bool     x11EventFilter( XEvent * );
    virtual int	     x11ClientMessage( QWidget*, XEvent*, bool passive_only);
    int              x11ProcessEvent( XEvent* );
#endif
#if defined(Q_WS_QWS)
    virtual bool     qwsEventFilter( QWSEvent * );
    int              qwsProcessEvent( QWSEvent* );
    void             qwsSetCustomColors( QRgb *colortable, int start, int numColors );
/*!
  \internal
    Returns true if the process is GUI server
*/
    bool          qwsIsGUIServer();
#ifndef QT_NO_QWS_MANAGER
    static QWSDecoration &qwsDecoration();
    static void      qwsSetDecoration( QWSDecoration *);
#endif
#endif

#if defined(Q_OS_WIN32) || defined(Q_OS_CYGWIN)
    static WindowsVersion winVersion();
#elif defined(Q_OS_MAC)
    static MacintoshVersion macVersion();
#endif
#if defined(Q_WS_WIN)
    void	     winFocus( QWidget *, bool );
    static void	     winMouseButtonUp();
#endif

#ifndef QT_NO_SESSIONMANAGER
    // session management
    bool	     isSessionRestored() const;
    QString 	sessionId() const;
    QString 	sessionKey() const;
    virtual void     commitData( QSessionManager& sm );
    virtual void     saveState( QSessionManager& sm );
#endif
#if defined(Q_WS_X11)
    static void create_xim();
    static void close_xim();
    static bool x11_apply_settings();
#endif
    void	     wakeUpGuiThread();
#if defined(QT_THREAD_SUPPORT)
    void	     lock();
    void	     unlock(bool wakeUpGui = TRUE);
    bool	     locked();
    bool             tryLock();
#endif

signals:
    void	     lastWindowClosed();
    void	     aboutToQuit();
    void	     guiThreadAwake();

public slots:
    void	     quit();
    void	     closeAllWindows();
    void	     aboutQt();

#if defined(Q_WS_QWS)
protected:
    void setArgs(int, char **);
#endif

protected:
    bool event(QEvent *);

private:
    void construct( int &argc, char **argv, Type );
    void initialize( int, char ** );
    void init_precmdline();
    void process_cmdline( int* argcptr, char ** argv );
    bool internalNotify( QObject *, QEvent * );
#if defined(Q_WS_QWS)
    static QWidget *findChildWidget( const QWidget *p, const QPoint &pos );
    static QWidget *findWidget( const QObjectList&, const QPoint &, bool rec );
#endif

#if defined(Q_WS_MAC)
    bool do_mouse_down(Point *, bool *);
    static QMAC_PASCAL OSStatus globalEventProcessor(EventHandlerCallRef,  EventRef, void *);
    static QMAC_PASCAL OSStatus globalAppleEventProcessor(const AppleEvent *, AppleEvent *, long);
    static QMAC_PASCAL void qt_context_timer_callbk(EventLoopTimerRef, void *);
    static QMAC_PASCAL void qt_select_timer_callbk(EventLoopTimerRef, void *);
    static bool qt_mac_apply_settings();
    friend class QMacInputMethod;
    friend QMAC_PASCAL OSStatus qt_window_event(EventHandlerCallRef, EventRef, void *);
    friend void qt_mac_update_os_settings();
    friend bool qt_set_socket_handler( int, int, QObject *, bool);
    friend void qt_mac_destroy_widget(QWidget *);
    friend void qt_init(int *, char **, QApplication::Type);
#endif

#ifdef QT_THREAD_SUPPORT
    static QMutex   *qt_mutex;
#endif // QT_THREAD_SUPPORT

    int		     app_argc;
    char	   **app_argv;
    bool	     quit_now;
    int		     quit_code;
    static QStyle   *app_style;
    static int	     app_cspec;
#ifndef QT_NO_PALETTE
    static QPalette *app_pal;
#endif
    static QFont    *app_font;
#ifndef QT_NO_CURSOR
    static QCursor  *app_cursor;
#endif
    static QEventLoop* eventloop;
    static int	     app_tracking;
    static bool	     is_app_running;
    static bool	     is_app_closing;
    static bool	     app_exit_loop;
    static int	     loop_level;
    static QWidget  *main_widget;
    static QWidget  *focus_widget;
    static QWidget  *active_window;
    static bool	     obey_desktop_settings;
    static int	     cursor_flash_time;
    static int	     mouse_double_click_time;
    static int	     wheel_scroll_lines;
    static int	     composedUnicode; // Value, meta-composed character

    static bool	     animate_ui;
    static bool	     animate_menu;
    static bool	     animate_tooltip;
    static bool	     animate_combo;
    static bool	     fade_menu;
    static bool	     fade_tooltip;
    static bool	     animate_toolbox;
    static bool	     widgetCount; // Coupled with -widgetcount switch
    static bool	     metaComposeUnicode;

    QValueList<QTranslator*> *translators;
#ifndef QT_NO_SESSIONMANAGER
    QSessionManager *session_manager;
    QString	     session_id;
    static QString* session_key;
    bool	     is_session_restored;
#endif
#if defined(Q_WS_X11) && !defined (QT_NO_STYLE )
    static void x11_initialize_style();
#endif

    static QSize     app_strut;
#ifndef QT_NO_COMPONENT
    static QStringList *app_libpaths;
#endif
    static QAsciiDict<QPalette> *app_palettes;
    static QAsciiDict<QFont>    *app_fonts;

    static QWidgetList *popupWidgets;
    bool	     inPopupMode() const;
    void	     closePopup( QWidget *popup );
    void	     openPopup( QWidget *popup );
    void	     setActiveWindow( QWidget* act );

    static bool      sendSpontaneousEvent( QObject *receiver, QEvent *event );
    static void      removePostedEvent( QEvent * );

    friend class QWidget;
    friend class QETWidget;
    friend class QDialog;
    friend class QAccelManager;
    friend class QEvent;
    friend class QTranslator;
    friend class QEventLoop;
    friend Q_EXPORT void qt_ucm_initialize( QApplication * );
#if defined(Q_WS_WIN)
    friend bool qt_sendSpontaneousEvent( QObject*, QEvent* );
#endif
#if defined(Q_WS_QWS)
    friend class QInputContext;
#endif
private: // Disabled copy constructor and operator=
#if defined(Q_DISABLE_COPY)
    QApplication( const QApplication & );
    QApplication &operator=( const QApplication & );
#endif
};

inline int QApplication::argc() const
{
    return app_argc;
}

inline char **QApplication::argv() const
{
    return app_argv;
}

#if defined(Q_WS_QWS)
inline void QApplication::setArgs(int c, char **v)
{
        app_argc = c;
	    app_argv = v;
}
#endif

#ifndef QT_NO_CURSOR
inline QCursor *QApplication::overrideCursor()
{
    return app_cursor;
}
#endif
inline bool QApplication::hasGlobalMouseTracking()
{
    return app_tracking > 0;
}

inline QWidget *QApplication::mainWidget() const
{
    return main_widget;
}

inline QWidget *QApplication::focusWidget() const
{
    return focus_widget;
}

inline QWidget *QApplication::activeWindow() const
{
    return active_window;
}

inline QWidget *QApplication::widgetAt( const QPoint &p, bool child )
{
    return widgetAt( p.x(), p.y(), child );
}

inline bool QApplication::inPopupMode() const
{
    return popupWidgets != 0;
}
#ifndef QT_NO_SESSIONMANAGER
inline bool QApplication::isSessionRestored() const
{
    return is_session_restored;
}

inline QString QApplication::sessionId() const
{
    return session_id;
}

inline QString QApplication::sessionKey() const
{
    return session_key ? *session_key : QString::null;
}
#endif
inline QSize QApplication::globalStrut()
{
    return app_strut;
}

inline bool QApplication::sendEvent( QObject *receiver, QEvent *event )
{  if ( event ) event->spont = FALSE; return qApp ? qApp->notify( receiver, event ) : FALSE; }

inline bool QApplication::sendSpontaneousEvent( QObject *receiver, QEvent *event )
{ if ( event ) event->spont = TRUE; return qApp ? qApp->notify( receiver, event ) : FALSE; }

#ifdef QT_NO_TRANSLATION
// Simple versions
inline QString QApplication::translate( const char *, const char *sourceText,
					const char *, Encoding encoding ) const
{
#ifndef QT_NO_TEXTCODEC
    if ( encoding == UnicodeUTF8 )
	return QString::fromUtf8( sourceText );
    else
#endif
	return QString::fromLatin1( sourceText );
}
#endif

inline int QApplication::horizontalAlignment( int align )
{
    align &= AlignHorizontal_Mask;
    if ( align == AlignAuto ) {
	if ( reverseLayout() )
	    align = AlignRight;
	else
	    align = AlignLeft;
    }
    return align;
}

#endif // QAPPLICATION_H

