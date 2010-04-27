/****************************************************************************
 **
 ** Copyright (C) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 **
 ** This file is part of the documentation of the Qt Toolkit.
 **
 ** Licensees holding a valid Qt License Agreement may use this file in
 ** accordance with the rights, responsibilities and obligations
 ** contained therein.  Please consult your licensing agreement or
 ** contact qt-sales@nokia.com if any conditions of this licensing
 ** agreement are not clear to you.
 **
 ** Further information about Qt licensing is available at:
 ** http://trolltech.com/products/appdev/licensing.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"



GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    imgBuffer = NULL;
    LireImage("lena_bruit.pgm", &image);
    /* Allocation de la memoire des images à afficher */
    if ( (imgBuffer=(GLubyte *) malloc(sizeof(GLubyte)*image.width*image.height)) == NULL)
    {
	printf("Impossible d'allouer I1\n");
    }
    BasculeImage(&image, imgBuffer);
    setFixedSize(image.width, image.height);

    trolltechPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}

GLWidget::~GLWidget()
{
    LibererImage(&image);
}

void GLWidget::BasculeImage(const Image *img, GLubyte *I)
{
    int i,j;
    for(i=0;i<img->height;i++)
	for(j=0;j<img->width;j++)
	    I[(img->height-1-i)*img->width+j]=(GLubyte) img->data[i*img->width+j];
    return;
}

//void GLWidget::setXRotation(int angle)
//{
//    normalizeAngle(&angle);
//    if (angle != xRot) {
//	xRot = angle;
//	emit xRotationChanged(angle);
//	updateGL();
//    }
//}
//
//void GLWidget::setYRotation(int angle)
//{
//    normalizeAngle(&angle);
//    if (angle != yRot) {
//	yRot = angle;
//	emit yRotationChanged(angle);
//	updateGL();
//    }
//}
//
//void GLWidget::setZRotation(int angle)
//{
//    normalizeAngle(&angle);
//    if (angle != zRot) {
//	zRot = angle;
//	emit zRotationChanged(angle);
//	updateGL();
//    }
//}

void GLWidget::initializeGL()
{
    qglClearColor(trolltechPurple.dark());
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

void GLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glRasterPos2i(0,0);
    glDrawPixels(image.width, image.height,GL_LUMINANCE,GL_UNSIGNED_BYTE, imgBuffer);
    glFlush();
}

void GLWidget::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0,width,0,height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLWidget::EquationChaleur(Image *in, Image *out, FiltreLineaire *fl, int n)
{
    printf("LOLOLOL");
    float t=1/(float)n;
    Image tmp;
    CreerImage(&tmp, in->width,in->height);
    CopierImage(in, &tmp);
    for(int i = 0 ; i < n ; i++)
    {
	FiltrageLineaire(&tmp, out, fl, t);
	CopierImage(out, &tmp);
    }
    CopierImage(out, &image);
    BasculeImage(&image, imgBuffer);
    updateGL();
}

void GLWidget::FiltrageLineaire(Image *in, Image *out, FiltreLineaire *fl, float t)
{
    int b = (int)fl->taille/2;
    for(int i=0;i<in->height;i++)
    {
	for(int j=0;j<in->width;j++)
	{
	    float s=0;
	    int n=0;
	    for(int k=i-b;k<=i+b;k++)
	    {
		for(int l=j-b;l<=j+b;l++)
		{
		    s+=ValMiror(in,l,k)*fl->coef[n++];
		}
	    }
	    out->data[j+i*in->width] = (short)(s*t+in->data[j+i*in->width]);
	}
    }
}
