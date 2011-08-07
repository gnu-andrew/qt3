/**********************************************************************
** Copyright (C) 2000-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of the Qt Assistant.
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

#ifndef CONFIG_H
#define CONFIG_H

#include "profile.h"

#include <qstring.h>
#include <qstringlist.h>
#include <qpixmap.h>
#include <qmap.h>

class Profile;

class Config
{
public:

    Config();

    void load();
    void save();
    Profile *profile() const { return profil; }
    QString profileName() const { return profil->props["name"]; }
    bool validProfileName() const;
    void hideSideBar( bool b );
    bool sideBarHidden() const;
    QStringList mimePaths();

    // From profile, read only
    QStringList docFiles() const;
    QStringList docTitles() const;
    QString indexPage( const QString &title ) const;
    QString docImageDir( const QString &title ) const;
    QPixmap docIcon( const QString &title ) const;

    QStringList profiles() const;
    QString title() const;
    QString aboutApplicationMenuText() const;
    QString aboutURL() const;
    QPixmap applicationIcon() const;

    // From QSettings, read / write
    QString webBrowser() const { return webBrows; }
    void setWebBrowser( const QString &cmd ) { webBrows = cmd; }

    QString homePage() const;
    void setHomePage( const QString &hom ) { home = hom; }

    QString pdfReader() const { return pdfApp; }
    void setPdfReader( const QString &cmd ) { pdfApp = cmd; }

    int fontSize() const { return fontSiz; }
    void setFontSize( int size ) { fontSiz = size; }

    QString fontFamily() const { return fontFam; }
    void setFontFamily( const QString &fam ) { fontFam = fam; }

    QString fontFixedFamily() const { return fontFix; }
    void setFontFixedFamily( const QString &fn ) { fontFix = fn; }

    QString linkColor() const { return linkCol; }
    void setLinkColor( const QString &col ) { linkCol = col; }

    QStringList source() const;
    void setSource( const QStringList &s ) { src = s; }

    int sideBarPage() const { return sideBar; }
    void setSideBarPage( int sbp ) { sideBar = sbp; }

    QRect geometry() const { return geom; }
    void setGeometry( const QRect &geo ) { geom = geo; }

    bool isMaximized() const { return maximized; }
    void setMaximized( bool max ) { maximized = max; }

    bool isLinkUnderline() const { return linkUnder; }
    void setLinkUnderline( bool ul ) { linkUnder = ul; }

    QString mainWindowLayout() const { return mainWinLayout; }
    void setMainWindowLayout( const QString &layout ) { mainWinLayout = layout; }

    QString assistantDocPath() const;

    bool docRebuild() const { return rebuildDocs; }
    void setDocRebuild( bool rb ) { rebuildDocs = rb; }

    void saveProfile( Profile *profile );
    void loadDefaultProfile();

    static Config *configuration();
    static Config *loadConfig(const QString &profileFileName);

private:
    Config( const Config &c );
    Config& operator=( const Config &c );

    void saveSettings();

private:
    Profile *profil;

    QStringList profileNames;
    QString webBrows;
    QString home;
    QString pdfApp;
    QString fontFam;
    QString fontFix;
    QString linkCol;
    QStringList src;
    QString mainWinLayout;
    QRect geom;
    int sideBar;
    int fontSiz;
    bool maximized;
    bool linkUnder;
    bool hideSidebar;
    bool rebuildDocs;
};

#endif
