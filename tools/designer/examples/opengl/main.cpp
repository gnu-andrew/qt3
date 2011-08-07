/**********************************************************************
**
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

#include <widgetinterface.h>

#include <qobjectcleanuphandler.h>
#include <qpixmap.h>

#include "glwidget.h"

/* XPM */
static char * cube_xmp[] = {
"48 48 104 2",
"  	c None",
". 	c #30C230C22081",
"X 	c #30C230C230C2",
"o 	c #30C2410330C2",
"O 	c #410330C24103",
"+ 	c #410341034103",
"@ 	c #514451445144",
"# 	c #514461855144",
"$ 	c #618551446185",
"% 	c #618561856185",
"& 	c #71C671C671C6",
"* 	c #71C6820771C6",
"= 	c #965882078617",
"- 	c #861792488617",
"; 	c #965892489658",
": 	c #9658A2899658",
"> 	c #9E79A2899E79",
", 	c #AEBAAEBA9E79",
"< 	c #AEBAAEBAAEBA",
"1 	c #AEBABEFBAEBA",
"2 	c #BEFBBEFBBEFB",
"3 	c #CF3CBEFBCF3C",
"4 	c #CF3CCF3CCF3C",
"5 	c #DF7DCF3CCF3C",
"6 	c #DF7DDF7DCF3C",
"7 	c #DF7DDF7DDF7D",
"8 	c #EFBEDF7DDF7D",
"9 	c #DF7DEFBEDF7D",
"0 	c #EFBEDF7DEFBE",
"q 	c #EFBEEFBEDF7D",
"w 	c #208130C22081",
"e 	c #410351445144",
"r 	c #514451446185",
"t 	c #71C6618571C6",
"y 	c #861771C68617",
"u 	c #9E7992489E79",
"i 	c #9E79AEBA9E79",
"p 	c #BEFBAEBABEFB",
"a 	c #BEFBCF3CBEFB",
"s 	c #BEFBCF3CCF3C",
"d 	c #CF3CCF3CDF7D",
"f 	c #CF3CDF7DCF3C",
"g 	c #DF7DDF7DEFBE",
"h 	c #208120812081",
"j 	c #30C2208130C2",
"k 	c #410330C230C2",
"l 	c #4103410330C2",
"z 	c #410351444103",
"x 	c #618551445144",
"c 	c #618561855144",
"v 	c #618571C66185",
"b 	c #861771C671C6",
"n 	c #8617820771C6",
"m 	c #965882079658",
"M 	c #9E7992489658",
"N 	c #AEBAA289AEBA",
"B 	c #BEFBAEBAAEBA",
"V 	c #DF7DCF3CDF7D",
"C 	c #CF3CDF7DDF7D",
"Z 	c #208120811040",
"A 	c #965892488617",
"S 	c #104020812081",
"D 	c #861782078617",
"F 	c #861792489658",
"G 	c #965892489E79",
"H 	c #104020811040",
"J 	c #208110402081",
"K 	c #30C220812081",
"L 	c #BEFBBEFBCF3C",
"P 	c #104010401040",
"I 	c #208110401040",
"U 	c #618571C671C6",
"Y 	c #71C671C68617",
"T 	c #71C682078617",
"R 	c #CF3CBEFBBEFB",
"E 	c #CF3CCF3CBEFB",
"W 	c #000010401040",
"Q 	c #104010402081",
"! 	c #9E79AEBAAEBA",
"~ 	c #104010400000",
"^ 	c #AEBAA2899E79",
"/ 	c #71C671C66185",
"( 	c #000010400000",
") 	c #000000000000",
"_ 	c #104000001040",
"` 	c #2081208130C2",
"' 	c #9658A2899E79",
"] 	c #104000000000",
"[ 	c #000000001040",
"{ 	c #30C241034103",
"} 	c #9E79A289AEBA",
"| 	c #AEBABEFBBEFB",
" .	c #514461856185",
"..	c #9E79A2899658",
"X.	c #71C661856185",
"o.	c #30C230C24103",
"O.	c #514441035144",
"+.	c #514451444103",
"@.	c #410341035144",
"#.	c #AEBAAEBABEFB",
"$.	c #208130C230C2",
"%.	c #6185618571C6",
"&.	c #514441034103",
"*.	c #861782079658",
"                                  . X o O + @ # $ % & * = - ; : > , < 1 2 2 3 4 5 6 7 7 8 9 0 q ",
"                                w X X O + @ e r % t & y - ; : u i < < p a 3 s d f 7 7 g 9 0 9 ; ",
"                              h j X k l + z x c t v b n m ; M > N < B 2 2 a 3 4 V 6 7 7 0 9 m ; ",
"                            h h w X f d f d f V f V f 7 f 7 C 7 f V f V f d f V f 7 9 g 9 m ; ; ",
"                          Z h w j - 4 4 5 4 5 4 5 4 V f V 6 V f V f V 4 5 4 5 4 V 7 7 7 m A ; ; ",
"                        S h h j - D s 4 4 4 4 4 4 4 4 d f V f d f d 4 d 4 4 4 V C 7 7 m - ; F G ",
"                      H J Z K * D D m ; M                                   5 4 V 7 = D m - ; ; ",
"                    H h S h * y D L F ; :                                 4 f V C D D m a ; ; G ",
"                  P I H J & b n 2 2 m A M                               3 4 5 f y D = 2 2 A ; ; ",
"                P P H J U Y & p 1 2 - ; :                             4 4 d f y T D 1 p 1 ; F G ",
"              P P H J % t &   < < < m ; u                           R E 4 f b * y   B < < A ; ; ",
"            W P P Q % t v     < ! < - ; :                         L a 4 4 & * y     < < ! ; ; G ",
"          ~ P ~ I c $ %       > ^ > m ; M                       2 2 3 4 t / b       ^ i ^ ; M ; ",
"        ( P W P # $ %         > > > F ; :                     p a 3 s t U &         > > > ; ; G ",
"      ) _ ( _ @ $ #           M ; u m ; u                   p 2 2 a % v t           M ; u ; M ; ",
"    ( _ ( P P P H h h ` w X o : ; F F ; : & * D - G ' > i < 1 2 a % v t             : ; F ; : u ",
"  ) ] ( _ ~ P P I Z h w j X O m - m ; ; M & y - m ; > i N < 2 2 $ c %               m - m ; M : ",
") [ ( _ W P P Q H h h j X O { D D - F G : y T m ; u > } < p | r  .%                 D D - G : u ",
"V f 5 4 5 f V f V f V f V f V y * y m ; u f 5 f V f 5 4 V f x @ $                   y * y : u ..",
"4 d f 4 f d f d 4 d f V f V f U & U F G : d f d f d f 4 4 d @ r #                   U & U G : > ",
"3 4 3 E 4 E 4 4 3 4 4 4 5 4 5 t v X.; ; u 4 4 4 4 E 4 E 3 4 @ @ x                   t v X.; u ..",
"a 4 a o.{ +                    .% v ; G >             3 a 4 e @ #                    .% v u ' > ",
"3 2 3 X O o                   $ # $ ; ; u             2 3 2 O.+.@                   $ # $ : u > ",
"a 2 a X o +                   @ @ @ ; u '             2 2 2 z @ @                   @ @ @ u > } ",
"p < p X k l                   O.z O.; : u             1 p < O.+.@                   O.z O...> i ",
"1 < 1 o.o +                   @.z @.; G >             #.1 < z O.@                   + @.z > > } ",
"N , N X O o                   l + o ; ; u             1 1 1 O.+.@                   O l + : u > ",
"i } i o.o +                   O o o.; u o + z @ # % v } i } e @ @ } < #.| L a 4 f V o o.o G ' > ",
"> ..u o O +                   . X . ; o O + O.@ $ % & u u ..@ @ x , < 1 2 2 3 4 5 6 X . X ; u ..",
"' u : O + +                   j $.j $.o.{ O.e r % %.U G : u @ r # < < p 2 3 s d f 7 $.j $.u : u ",
"; A M l + +                   h j . K h j h K h h h h ; ; A x @ $ h h h h h K h j 6 7 7 0 : u ..",
"- m - + { @.                h h w %.w h h h h h S h S m - m # r # h S h S h h h f 7 9 g F G :   ",
"y D D l + +               Z h h t % h Z J Z J Z J Z J D y D x # $ H J Z J Z J f V 7 7 - M ;     ",
"* y * + z O.            S h h & U t                   y * y @ $  .          4 d C 7 - m ;       ",
"& / b l + +           P J Z y & t                     & & / x c $         E 5 4 V n = D         ",
"v %.v + z O.        H J H D * &                       & v %.# %  .      a 4 f V * D -           ",
"X.% % + &.z       P P H D n y                         c $ % $ c $     a 3 4 V * y D             ",
"# $ # + z @   P P H J m - D                           $ # $ # $ %   2 3 4 d * y T               ",
"@ @ x + + z W P P Q m D m                             @ @ @ $ c % 2 R E 4 & b *                 ",
"z O.e + z @ P ~ I ; ; *.                              @ z O. .% % L s 4 U & *                   ",
"&.+ &.l &.+.W P M ; m                                 + &.+ $ c X.2 3 v t &                     ",
"{ + { @.z O._ u ' ;                                   + { +  .$ % 3 v t &                       ",
"O o k + O.P P P H h h ` w X o l + z @ c X./ b n m ; u o O o $ % % % t v                         ",
"$.X $.@._ ~ P P I Z h w j X O + + @ # % v & * D - ; : X $.X  .% v %.v                           ",
"j . j _ W P P Q H h h j X O { + &.@ $ c t & y D m ; > . j . % c X.%                             ",
"w j w ` w j w ` w ` h ` h h S h h h h ` h ` w ` w j w ` w j  .% %                               ",
"h h K h h h h h h h J h J H J Z J H J h h h h h h h J h h h $ %                                 ",
"S h h h H h S h S h S h H Q H J H Q H h H h S h S h S h S h  .                                  "};


class OpenGLWidgetInterface : public WidgetInterface
{
public:
    OpenGLWidgetInterface();

    QRESULT queryInterface( const QUuid&, QUnknownInterface ** );
    Q_REFCOUNT;

    QStringList featureList() const;

    QWidget* create( const QString &classname, QWidget* parent = 0, const char* name = 0 );
    QString group( const QString& ) const;
    QIconSet iconSet( const QString& ) const;
    QString includeFile( const QString& ) const;
    QString toolTip( const QString& ) const;
    QString whatsThis( const QString& ) const;
    bool isContainer( const QString& ) const;

private:
    QObjectCleanupHandler objects;
};

OpenGLWidgetInterface::OpenGLWidgetInterface()
{
}

QStringList OpenGLWidgetInterface::featureList() const
{
    QStringList list;

    list << "GLWidget";

    return list;
}

QWidget* OpenGLWidgetInterface::create( const QString &description, QWidget* parent, const char* name )
{
    QWidget* w = 0;

    if ( description == "GLWidget" )
	w = new GLWidget( parent, name );

    objects.add( w );
    return w;
}

QString OpenGLWidgetInterface::group( const QString &description ) const
{
    if ( description == "GLWidget" )
	return "Views";
    return QString::null;
}

QIconSet OpenGLWidgetInterface::iconSet( const QString &description ) const
{
    if ( description == "GLWidget" )
	return QIconSet( (const char**)cube_xmp );
    return QIconSet();
}

QString OpenGLWidgetInterface::includeFile( const QString &description ) const
{
    if ( description == "GLWidget" )
        return "glwidget.h";
    return QString::null;
}

QString OpenGLWidgetInterface::toolTip( const QString &description ) const
{
    if ( description == "GLWidget" )
	return QT_TR_NOOP("OpenGL Widget");
    return QString::null;
}

QString OpenGLWidgetInterface::whatsThis( const QString &description ) const
{
    if ( description == "GLWidget" )
	return "A widget for OpenGL rendering";
    return QString::null;
}

bool OpenGLWidgetInterface::isContainer( const QString& ) const
{
    return FALSE;
}

QRESULT OpenGLWidgetInterface::queryInterface( const QUuid& uuid, QUnknownInterface **iface )
{
    *iface = 0;

    if ( uuid == IID_QUnknown )
	*iface = (QUnknownInterface*)this;
    else if ( uuid == IID_QFeatureList )
	*iface = (QFeatureListInterface*)this;
    else if ( uuid == IID_Widget )
	*iface = (WidgetInterface*)this;
    else
	return QE_NOINTERFACE;

    (*iface)->addRef();
    return QS_OK;
}

Q_EXPORT_COMPONENT()
{
    Q_CREATE_INSTANCE( OpenGLWidgetInterface );
}
