#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"



GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    imgBuffer = NULL;
    image.width = 0;
    image.height = 0;
    image.size = 0;
    image.data = NULL;
    this->setFixedWidth(200);
    this->setFixedHeight(200);
    trolltechPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
}

GLWidget::~GLWidget()
{
    //LibererImage(&image);
}

void GLWidget::BasculeImage(const Image *img, GLubyte *I)
{
    int i,j;
    for(i=0;i<img->height;i++)
	for(j=0;j<img->width;j++)
	    I[(img->height-1-i)*img->width+j]=(GLubyte) img->data[i*img->width+j];
    return;
}

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

void GLWidget::CopieImg(Image *source)
{
    CopierImage(source, &image);
    BasculeImage(&image, imgBuffer);
    updateGL();
}

void GLWidget::ChargerImage(QString sFileName)
{
    //LibererImage(&image);
    LireImage(sFileName.toStdString().c_str(), &image);
    imgBuffer = NULL;
    /* Allocation de la memoire des images à afficher */
    if ( (imgBuffer=(GLubyte *) malloc(sizeof(GLubyte)*image.width*image.height)) == NULL)
    {
	printf("Impossible d'allouer I1\n");
    }
    BasculeImage(&image, imgBuffer);
    setFixedSize(image.width, image.height);
    updateGL();
}

void GLWidget::EquationChaleur(Image *in, Image *out, FiltreLineaire *fl, int n)
{
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

void GLWidget::BruitageImpulsionnel(Image *in, Image *out, int b)
{
    int val = 0;
    for (int i = 0; i < in->size; i++)
    {
	val = -b + int( double( rand() ) / ( double( RAND_MAX) + 1 ) * (2*b+1) );
	//printf("Val: %d\n", val);
	out->data[i] = in->data[i] + val;
    }
    Image out2;
    CreerImage(&out2, out->width, out->height);
    //ExpansionDynamique(in, out, &out2);
    ExpansionDynamique2(out, &out2);
    CopierImage(&out2, &image);
    BasculeImage(&image, imgBuffer);
    updateGL();
}

void GLWidget::BruitagePoivreEtSel(Image *in, Image *out)
{
    int p;
    for (int i = 0; i < in->size; i++)
    {
	p = 1 + int( double( rand() ) / ( double( RAND_MAX) + 1 ) * 20);
	if (p == 1)
	{
	    int NB = 1 + int( double( rand() ) / ( double( RAND_MAX) + 1 ) * 2 );
	    if (NB == 1)
	    {
		out->data[i] = 0;
	    }
	    else out->data[i] = 255;
	}
	else out->data[i] = in->data[i];
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

void GLWidget::ExpansionDynamique2(Image *in, Image *out)
{
    CreerImage(out, in->width, in->height);
    int val;
    int xmin = 999999, xmax = -999999;
    for (int i = 0; i < in->size; i++)
    {
	val = in->data[i];
	if (val < xmin) xmin = val;
	if (val > xmax) xmax = val;
    }
    for (int i = 0; i < out->size; i++)
    {
	val = in->data[i];
	out->data[i] = Max(0, Min(255, (val-xmin)*255/(xmax-xmin)));
    }
}

int GLWidget::Min(int a, int b)
{
    return ((a < b) ? a:b);
}

int GLWidget::Max(int a, int b)
{
    return ((a > b) ? a:b);
}
