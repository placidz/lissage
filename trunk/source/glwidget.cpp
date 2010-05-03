#include <QtGui>
#include <QtOpenGL>

#include <math.h>

#include "glwidget.h"



GLWidget::GLWidget(QWidget *parent)
    : QGLWidget(parent)
{
    graphique = NULL;
    miroir = NULL;
    LigneCoupe = -1;
    isWindowOpened = true;
    imgBuffer = NULL;
    image.width = 0;
    image.height = 0;
    image.size = 0;
    image.data = NULL;
    setFixedSize(200, 200);
    //ChargerImage("lena.pgm");
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
    if (LigneCoupe != -1)
    {
	glColor4fv(lineColor);
	glBegin(GL_LINES);
	glVertex2i(0, image.height - LigneCoupe);
	glVertex2i(image.width, image.height - LigneCoupe);
	glEnd();
    }
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

void GLWidget::mousePressEvent(QMouseEvent *e)
{
    LigneCoupe = e->y();
    miroir->updateLineCoupe(e->y());
    graphique->LigneADessiner = e->y();
    graphique->updateGL();
    updateGL();
}

void GLWidget::updateLineCoupe(int h)
{
    LigneCoupe = h;
    updateGL();
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
    if ( (imgBuffer=(GLubyte *) malloc(sizeof(GLubyte)*image.size)) == NULL)
    {
	printf("Impossible d'allouer imgBuffer\n");
    }
    BasculeImage(&image, imgBuffer);
    setFixedSize(image.width, image.height);
    updateGL();
}

void GLWidget::EquationChaleur(Image *in, Image *out, FiltreLineaire *fl, int n, double dt)
{
    //float t = 0.2;
    ImageD tmp, tmpOut;
    CreerImageD(&tmp, in->width,in->height);
    CreerImageD(&tmpOut, in->width,in->height);
    CopierImageStoD(in, &tmp);
    for(int i = 0 ; i < n ; i++)
    {
	FiltrageLineaire(&tmp, &tmpOut, fl, dt);
	CopierImageD(&tmpOut, &tmp);
    }
    ExpansionDynamique2(&tmpOut, out);
    CopierImage(out, &image);
    BasculeImage(&image, imgBuffer);
    updateGL();
}

void GLWidget::appelMalikPerona(Image * in , Image *out, int n, double dt, int sigma)
{
    //float t = 0.2;
    ImageD U1, U2;
    Image temp;

    CreerImageD(&U1, in->width, in->height);
    CreerImageD(&U2, in->width, in->height);
    CreerImage(&temp, in->width, in->height);

    CopierImageStoD(in, &U1);

    for(int i = 0 ; i < n ; i++ )
    {
	malikEtPerona(&U1 , &U2, dt, sigma);
	CopierImageD(&U2 , &U1);
    }
    ExpansionDynamique2(&U1, out);
    CopierImage(out, &image);
    BasculeImage(&image, imgBuffer);
    updateGL();
}

void GLWidget::malikEtPerona(ImageD *in, ImageD *out, double dt, int sigma)
{
    double res;

    for(int j = 0; j < in->height; j++)
    {
	for(int i = 0; i < in->width; i++)
	{
	    res = ValMirorD(in, i, j) + calculeCnsew(in, i, j, sigma) * dt;
	    ModifierPixelD(out, i, j, res );
	}
    }
}


double GLWidget::calculeCnsew(ImageD *Img, int i, int j, int sigma)
{
    double res = 0;
    res += calculeC(Img, i, j, 0, sigma) * (ValMirorD(Img, i, j-1) - ValMirorD(Img, i, j) );//Nord
    res += calculeC(Img, i, j, 1, sigma) * (ValMirorD(Img, i, j+1) - ValMirorD(Img, i, j) );//Sud
    res += calculeC(Img, i, j, 2, sigma) * (ValMirorD(Img, i-1, j) - ValMirorD(Img, i, j) );//West
    res += calculeC(Img, i, j, 3, sigma) * (ValMirorD(Img, i+1, j) - ValMirorD(Img, i, j) );//Est

    return res;
}

double GLWidget::calculeC(ImageD *Img, int i, int j, int dir, int sigma)
{
    double res;
    double norme = 0;

    switch(dir)
    {
    case 0:
	norme = sqrt(pow(ValMirorD(Img, i, j-1), 2) + pow(ValMirorD(Img, i, j), 2));
	break;

    case 1:
	norme = sqrt(pow(ValMirorD(Img, i, j+1), 2) + pow(ValMirorD(Img, i, j), 2));
	break;

    case 2:
	norme = sqrt(pow(ValMirorD(Img, i-1, j), 2) + pow(ValMirorD(Img, i, j), 2));
	break;

    case 3:
	norme = sqrt(pow(ValMirorD(Img, i+1, j), 2) + pow(ValMirorD(Img, i, j), 2));
	break;
    }

    //res = exp( (double)-1 *  (pow(norme, 2)  / pow((double)sigma, 2 )));
    res = (float)1/((float)1+pow(norme/(float)sigma,2));

    return res;
}

void GLWidget::appelCourbureMoyenne(Image *in , Image *out, int n, double t)
{
    ImageD U1, U2;
    Image temp;

    CreerImageD(&U1, in->width, in->height);
    CreerImageD(&U2, in->width, in->height);
    CreerImage(&temp, in->width, in->height);

    CopierImageStoD(in, &U1);

    for(int i = 0 ; i < n ; i++ ){
	courbureMoyenne(&U1 , &U2, t);
	CopierImageD( &U2 , &U1 );
    }

    //CopierImageDtoS(&U2, &temp);
    //ExpansionDynamique(in, &temp, out);
    ExpansionDynamique2(&U2, out);
    CopierImage(out, &image);
    BasculeImage(&image, imgBuffer);
    updateGL();
}

void GLWidget::courbureMoyenne(ImageD *in, ImageD *out, double t)
{
    //int width = in->width, height = in->height;
    double res;
    ImageD tmp;

    CreerImageD(&tmp, in->width, in->height);
    CopierImageD(in, &tmp);

    for(int j = 0; j < in->height; j++){
	for(int i = 0; i < in->width; i++){
	    res = ValMirorD(in, i, j) + sqrt(calculeMiniSobel(in, i, j))*calculeK(in, i, j) * t;
	    ModifierPixelD(out, i, j, res );
	}
    }
}

double GLWidget::calculeMiniSobel(ImageD *Img, int i, int j)
{
    double res;
    res = pow( (ValMirorD(Img, i+1, j) - ValMirorD(Img, i-1, j)), 2) / 2 + pow( (ValMirorD(Img, i, j+1) - ValMirorD(Img, i, j-1)), 2) / 2;
    return res;
}

double GLWidget::calculeK(ImageD *Img, int i, int j)
{
    double res = 0;
    double epsilon = 0.001;
    res +=	( ValMirorD(Img, i+1, j) - ValMirorD(Img, i, j) ) / (epsilon +
								     sqrt(
									     pow( (ValMirorD(Img, i+1, j) - ValMirorD(Img, i, j)), 2) +
									     0.25 *
									     pow(
										     (ValMirorD(Img, i, j+1) - ValMirorD(Img, i, j-1)) +
										     0.5 * (ValMirorD(Img, i+1, j+1) - ValMirorD(Img, i+1, j-1))
										     , 2)
									     ));

    res -=	( ValMirorD(Img, i, j) - ValMirorD(Img, i-1, j) ) / (epsilon +
								     sqrt(
									     pow( (ValMirorD(Img, i, j) - ValMirorD(Img, i-1, j)), 2) +
									     0.25 *
									     pow(
										     (ValMirorD(Img, i-1, j+1) - ValMirorD(Img, i-1, j-1)) +
										     0.5 * (ValMirorD(Img, i, j+1) - ValMirorD(Img, i, j-1))
										     , 2)
									     ));

    res +=	( ValMirorD(Img, i, j+1) - ValMirorD(Img, i, j) ) / (epsilon +
								     sqrt(
									     pow( (ValMirorD(Img, i, j+1) - ValMirorD(Img, i, j)), 2) +
									     0.25 *
									     pow(
										     (ValMirorD(Img, i+1, j+1) - ValMirorD(Img, i-1, j+1)) +
										     0.5 * (ValMirorD(Img, i+1, j) - ValMirorD(Img, i-1, j))
										     , 2)
									     ));

    res -=	( ValMirorD(Img, i, j) - ValMirorD(Img, i, j-1) ) / (epsilon +
								     sqrt(
									     pow( (ValMirorD(Img, i, j) - ValMirorD(Img, i, j-1)), 2) +
									     0.25 *
									     pow(
										     (ValMirorD(Img, i+1, j) - ValMirorD(Img, i-1, j)) +
										     0.5 * (ValMirorD(Img, i+1, j-1) - ValMirorD(Img, i-1, j-1))
										     , 2)
									     ));
    return res;
}

double GLWidget::TriABullesMedian(float *t, int size)
{
    double val;
    int i, j, x;
    i = 0;
    while (i < size -1)
    {
	j = size - 1;
	while (j > i)
	{
	    if (t[j] < t[j-1])
	    {
		x = t[j];
		t[j] = t[j-1];
		t[j-1] = x;
	    }
	    j--;
	}
	i++;
    }
    val = t[(int)(size / 2)];
    return val;
}

void GLWidget::appelFiltreMedianItere(Image * in , Image *out, int n, FiltreLineaire filtre)
{

    Image temp;
    CreerImage(&temp, in->width, in->height);
    CopierImage(in, &temp);

    for(int i = 0 ; i < n ; i++ ){
	FiltrageMedian(&temp , out, filtre);
	CopierImage( out , &temp );
    }
    //ExpansionDynamique(in, &temp, out);
    CopierImage(out, &image);
    BasculeImage(&image, imgBuffer);
    updateGL();
}

void GLWidget::FiltrageMedian(Image *src, Image *dest, FiltreLineaire flt)
{
    double somme;
    int i, j, k, l, n;
    for (j = 0; j < (src->height); j++)
    {
	for (i = 0; i < (src->width); i++)
	{
	    somme = 0;
	    n = 0;
	    for (k = (int)(-flt.taille / 2); k <= (int)(flt.taille / 2); k++)
	    {
		for (l = (int)(-flt.taille / 2); l <= (int)(flt.taille / 2); l++)
		{
		    flt.coef[n] = (double)(ValMiror(src, i+k, j+l));
		    n++;
		}
	    }
	    somme = TriABullesMedian(flt.coef, flt.taille*flt.taille);
	    ModifierPixel(dest, i, j, (short)(somme));
	}
    }
}

void GLWidget::BruitageUniforme(Image *in, Image *out, int b)
{
    int val = 0;
    ImageD tmp;
    CreerImageD(&tmp, in->width, in->height);
    for (int i = 0; i < tmp.size; i++)
    {
	val = -b + int( double( rand() ) / ( double( RAND_MAX) + 1 ) * (2*b+1) );
	//printf("Val: %d\n", val);
	tmp.data[i] = in->data[i] + val;
    }
    ExpansionDynamique2(&tmp, out);
    CopierImage(out, &image);
    BasculeImage(&image, imgBuffer);
    updateGL();
}

void GLWidget::BruitageImpulsionnel(Image *in, Image *out)
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

void GLWidget::FiltrageLineaire(ImageD *in, ImageD *out, FiltreLineaire *fl, double t)
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
		    s += ValMirorD(in,l,k)*fl->coef[n++];
		}
	    }
	    out->data[j+i*in->width] = (double)(s*t+in->data[j+i*in->width]);
	}
    }
}

void GLWidget::ExpansionDynamique2(ImageD *in, Image *out)
{
    CreerImage(out, in->width, in->height);
    double val;
    double xmin = 999999, xmax = -999999;
    for (int i = 0; i < in->size; i++)
    {
	val = in->data[i];
	if (val < xmin) xmin = val;
	if (val > xmax) xmax = val;
    }
    for (int i = 0; i < out->size; i++)
    {
	val = in->data[i];
	out->data[i] = (short)(Max(0, Min(255, (val-xmin)*255/(xmax-xmin))));
    }
}

double GLWidget::Min(double a, double b)
{
    return ((a < b) ? a:b);
}

double GLWidget::Max(double a, double b)
{
    return ((a > b) ? a:b);
}

void GLWidget::closeEvent(QCloseEvent *event)
{
    if (isWindowOpened)
	event->ignore();
    else event->accept();
}

void GLWidget::setLineColor(float r, float g, float b)
{
    lineColor[0] = r;
    lineColor[1] = g;
    lineColor[2] = b;
    lineColor[3] = 1.0;
}
