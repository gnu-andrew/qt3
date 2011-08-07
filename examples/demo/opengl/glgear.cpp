/****************************************************************************
**
** Copyright (C) 1992-2008 Trolltech ASA.  All rights reserved.
**
** This file is part of an example program for Qt.  This example
** program may be used, distributed and modified without limitation.
**
*****************************************************************************/
//
// Draws a gear.
//
// Portions of this code have been borrowed from Brian Paul's Mesa
// distribution.
//

#include "glgear.h"

#include <math.h>

#if defined(Q_CC_MSVC)
#pragma warning(disable:4305) // init: truncation from const double to float
#endif

/*
 * Draw a gear wheel.  You'll probably want to call this function when
 * building a display list since we do a lot of trig here.
 *
 * Input:  inner_radius - radius of hole at center
 *	   outer_radius - radius at center of teeth
 *	   width - width of gear
 *	   teeth - number of teeth
 *	   tooth_depth - depth of tooth
 */
static void gear( GLfloat inner_radius, GLfloat outer_radius, GLfloat width,
		  GLint teeth, GLfloat tooth_depth )
{
    GLint i;
    GLfloat r0, r1, r2;
    GLfloat angle, da;
    GLfloat u, v, len;

    r0 = inner_radius;
    r1 = outer_radius - tooth_depth/2.0;
    r2 = outer_radius + tooth_depth/2.0;

    const double pi = 3.14159264;
    da = 2.0*pi / teeth / 4.0;

    glShadeModel( GL_FLAT );

    glNormal3f( 0.0, 0.0, 1.0 );

    /* draw front face */
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
	angle = i * 2.0*pi / teeth;
	glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
	glVertex3f( r1*cos(angle), r1*sin(angle), width*0.5 );
	glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
	glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    }
    glEnd();

    /* draw front sides of teeth */
    glBegin( GL_QUADS );
    da = 2.0*pi / teeth / 4.0;
    for (i=0;i<teeth;i++) {
	angle = i * 2.0*pi / teeth;

	glVertex3f( r1*cos(angle),      r1*sin(angle),	  width*0.5 );
	glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),	  width*0.5 );
	glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), width*0.5 );
	glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), width*0.5 );
    }
    glEnd();


    glNormal3f( 0.0, 0.0, -1.0 );

    /* draw back face */
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
	angle = i * 2.0*pi / teeth;
	glVertex3f( r1*cos(angle), r1*sin(angle), -width*0.5 );
	glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
	glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
	glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
    }
    glEnd();

    /* draw back sides of teeth */
    glBegin( GL_QUADS );
    da = 2.0*pi / teeth / 4.0;
    for (i=0;i<teeth;i++) {
	angle = i * 2.0*pi / teeth;

	glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
	glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
	glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),	  -width*0.5 );
	glVertex3f( r1*cos(angle),      r1*sin(angle),	  -width*0.5 );
    }
    glEnd();


    /* draw outward faces of teeth */
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<teeth;i++) {
	angle = i * 2.0*pi / teeth;

	glVertex3f( r1*cos(angle),      r1*sin(angle),	   width*0.5 );
	glVertex3f( r1*cos(angle),      r1*sin(angle),	  -width*0.5 );
	u = r2*cos(angle+da) - r1*cos(angle);
	v = r2*sin(angle+da) - r1*sin(angle);
	len = sqrt( u*u + v*v );
	u /= len;
	v /= len;
	glNormal3f( v, -u, 0.0 );
	glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),	   width*0.5 );
	glVertex3f( r2*cos(angle+da),   r2*sin(angle+da),	  -width*0.5 );
	glNormal3f( cos(angle), sin(angle), 0.0 );
	glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da),  width*0.5 );
	glVertex3f( r2*cos(angle+2*da), r2*sin(angle+2*da), -width*0.5 );
	u = r1*cos(angle+3*da) - r2*cos(angle+2*da);
	v = r1*sin(angle+3*da) - r2*sin(angle+2*da);
	glNormal3f( v, -u, 0.0 );
	glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da),  width*0.5 );
	glVertex3f( r1*cos(angle+3*da), r1*sin(angle+3*da), -width*0.5 );
	glNormal3f( cos(angle), sin(angle), 0.0 );
    }

    glVertex3f( r1*cos(0.0), r1*sin(0.0), width*0.5 );
    glVertex3f( r1*cos(0.0), r1*sin(0.0), -width*0.5 );

    glEnd();


    glShadeModel( GL_SMOOTH );

    /* draw inside radius cylinder */
    glBegin( GL_QUAD_STRIP );
    for (i=0;i<=teeth;i++) {
	angle = i * 2.0*pi / teeth;
	glNormal3f( -cos(angle), -sin(angle), 0.0 );
	glVertex3f( r0*cos(angle), r0*sin(angle), -width*0.5 );
	glVertex3f( r0*cos(angle), r0*sin(angle), width*0.5 );
    }
    glEnd();

}

void GLGear::draw()
{
    glPushMatrix();
    glRotatef( view_rotx, 1.0, 0.0, 0.0 );
    glRotatef( view_roty, 0.0, 1.0, 0.0 );
    glRotatef( view_rotz, 0.0, 0.0, 1.0 );

    glPushMatrix();
    glTranslatef( -3.0, -2.0, 0.0 );
    glRotatef( angle, 0.0, 0.0, 1.0 );
    glCallList(gear1);
    glPopMatrix();

    glPushMatrix();
    glTranslatef( 3.1, -2.0, 0.0 );
    glRotatef( -2.0*angle-9.0, 0.0, 0.0, 1.0 );
    glCallList(gear2);
    glPopMatrix();

    glPushMatrix();
    glTranslatef( -3.1, 2.2, -1.8 );
    glRotatef( 90.0, 1.0, 0.0, 0.0 );
    glRotatef( 2.0*angle-2.0, 0.0, 0.0, 1.0 );
    glCallList(gear3);
    glPopMatrix();

    glPopMatrix();
}

GLGear::GLGear( QWidget *parent, const char *name, WFlags f )
     : GLControlWidget( parent, name, 0, f )
{
    scale = 1.0;
    setAnimationDelay( 15 );
    view_rotx = 20.0;
    view_roty = 30.0;
    view_rotz = 0.0;
    angle = 0.0;
}

void GLGear::initializeGL()
{
    static GLfloat pos[4] = {5.0, 5.0, 10.0, 1.0 };
    static GLfloat ared[4] = {0.8, 0.1, 0.0, 1.0 };
    static GLfloat agreen[4] = {0.0, 0.8, 0.2, 1.0 };
    static GLfloat ablue[4] = {0.2, 0.2, 1.0, 1.0 };

    glLightfv( GL_LIGHT0, GL_POSITION, pos );
    glEnable( GL_CULL_FACE );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    glEnable( GL_DEPTH_TEST );

    /* make the gears */
    gear1 = glGenLists(1);
    glNewList(gear1, GL_COMPILE);
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ared );
    gear( 1.0, 4.0, 1.0, 20, 0.7 );
    glEndList();

    gear2 = glGenLists(1);
    glNewList(gear2, GL_COMPILE);
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, agreen );
    gear( 0.5, 2.0, 2.0, 10, 0.7 );
    glEndList();

    gear3 = glGenLists(1);
    glNewList(gear3, GL_COMPILE);
    glMaterialfv( GL_FRONT, GL_AMBIENT_AND_DIFFUSE, ablue );
    gear( 1.3, 2.0, 0.5, 10, 0.7 );
    glEndList();

    glEnable( GL_NORMALIZE );
}


void GLGear::resizeGL( int width, int height )
{
    GLfloat w = (float) width / (float) height;
    GLfloat h = 1.0;

    glViewport( 0, 0, width, height );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum( -w, w, -h, h, 5.0, 60.0 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef( 0.0, 0.0, -40.0 );
}


void GLGear::paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glPushMatrix();
    transform();
    draw();
    drawText();
    
    glPushAttrib( GL_LIGHTING_BIT | GL_TEXTURE_BIT );
    glDisable( GL_LIGHTING );
    glDisable( GL_TEXTURE_2D );
    qglColor( green );
    glLineWidth( 1.0 );
    glBegin( GL_LINES );
    {
	glVertex3f( 0.0, 0.0, 0.0 );
	glVertex3f( 2.98, 2.98, 2.98 );
    }
    glEnd();
    renderText( 3.0, 3.0, 3.0, "Gears", QFont( "helvetica", 12, QFont::Bold, TRUE ) );
    glPopMatrix();
    glPopAttrib();
}

void GLGear::animate()
{
    angle += 2.0;
    view_roty += 1.0;
    updateGL();
}
