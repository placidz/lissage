#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glgraphique.h"


GLGraphique::GLGraphique(QWidget *parent)
    : QGLWidget(parent)
{
    LigneADessiner = -1;
    imgInitiale = NULL;
    imgResultat = NULL;
    isWindowOpened = true;
    widthGraph = 768;
    heightGraph = 256;
    setFixedSize(widthGraph, heightGraph);
    trolltechPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}

GLGraphique::~GLGraphique()
{
    //LibererImage(&image);
}

void GLGraphique::BasculeImage(const Image *img, GLubyte *I)
{
    int i,j;
    for(i=0;i<img->height;i++)
	for(j=0;j<img->width;j++)
	    I[(img->height-1-i)*img->width+j]=(GLubyte) img->data[i*img->width+j];
    return;
}

void GLGraphique::initializeGL()
{
    //qglClearColor(trolltechPurple.dark());
    glClearColor(0.0, 0.0, 0.0, 1.0);
}

void GLGraphique::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLineWidth(1.0);
    if (imgInitiale != NULL && imgResultat != NULL)
    {
	int ligne = imgInitiale->image.height/2;
	glColor4fv(imgInitiale->lineColor);
	drawVueEnCoupe(&imgInitiale->image, ligne);
	glColor4fv(imgResultat->lineColor);
	drawVueEnCoupe(&imgResultat->image, ligne);
    }
    glFlush();
}

void GLGraphique::resizeGL(int width, int height)
{
    width = widthGraph;
    height = heightGraph;
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, width, 0, height);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void GLGraphique::drawVueEnCoupe(Image *in, int line)
{
    line = in->height - line;
    if (in->width > 0 && LigneADessiner != -1)
    {
	int width = in->width;
	double pas = widthGraph / (double)width;
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < in->width; i++)
	{
	    double x = i * pas;
	    glVertex2d(x, in->data[i+ LigneADessiner*in->width]);
	}
	glEnd();
    }
}

void GLGraphique::updateGraphique()
{
    updateGL();
}

void GLGraphique::closeEvent(QCloseEvent *event)
{
    if (isWindowOpened)
	event->ignore();
    else event->accept();
}
