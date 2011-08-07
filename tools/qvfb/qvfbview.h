/**********************************************************************
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of Qt/Embedded virtual framebuffer.
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

#include <qscrollview.h>

class QImage;
class QTimer;
class QAnimationWriter;
class QLock;
struct QVFbHeader;

class QVFbView : public QScrollView
{
    Q_OBJECT
public:
    QVFbView( int display_id, int w, int h, int d, QWidget *parent = 0,
		const char *name = 0, uint wflags = 0 );
    ~QVFbView();

    int displayId() const;
    int displayWidth() const;
    int displayHeight() const;
    int displayDepth() const;

    bool touchScreenEmulation() const { return emulateTouchscreen; }
    int rate() { return refreshRate; }
    bool animating() const { return !!animation; }
    QImage image() const;

    void setGamma(double gr, double gg, double gb);
    double gammaRed() const { return gred; }
    double gammaGreen() const { return ggreen; }
    double gammaBlue() const { return gblue; }
    void getGamma(int i, QRgb& rgb);
    void skinKeyPressEvent( QKeyEvent *e ) { keyPressEvent(e); }
    void skinKeyReleaseEvent( QKeyEvent *e ) { keyReleaseEvent(e); }

    double zoom() const { return zm; }

    QSize sizeHint() const;
public slots:
    void setTouchscreenEmulation( bool );

    void setRate( int );
    void setZoom( double );
    void startAnimation( const QString& );
    void stopAnimation();

protected slots:
    void timeout();

protected:
    void initLock();
    void lock();
    void unlock();
    QImage getBuffer( const QRect &r, int &leading ) const;
    void drawScreen();
    void sendMouseData( const QPoint &pos, int buttons );
    void sendKeyboardData( int unicode, int keycode, int modifiers,
			   bool press, bool repeat );
    virtual bool eventFilter( QObject *obj, QEvent *e );
    virtual void viewportPaintEvent( QPaintEvent *pe );
    virtual void contentsMousePressEvent( QMouseEvent *e );
    virtual void contentsMouseDoubleClickEvent( QMouseEvent *e );
    virtual void contentsMouseReleaseEvent( QMouseEvent *e );
    virtual void contentsMouseMoveEvent( QMouseEvent *e );
    virtual void keyPressEvent( QKeyEvent *e );
    virtual void keyReleaseEvent( QKeyEvent *e );

private:
    bool emulateTouchscreen;
    void setDirty( const QRect& );
    int shmId;
    unsigned char *data;
    QVFbHeader *hdr;
    int viewdepth; // "faked" depth
    int rsh;
    int gsh;
    int bsh;
    int rmax;
    int gmax;
    int bmax;
    double gred, ggreen, gblue;
    QRgb* gammatable;
    QLock *qwslock;
    QTimer *timer;
    int mouseFd;
    int keyboardFd;
    int refreshRate;
    QString mousePipe;
    QString keyboardPipe;
    QAnimationWriter *animation;
    int displayid;
    double zm;
};

