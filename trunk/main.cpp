#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <unistd.h>
#define NB_NV_GRIS 256
#define NB_NV_GRIS_SOBEL 2048


extern "C"
{
#include "include/OutilsPGM.h"
}

typedef struct {
        unsigned long nb_pixels;
        double max;
        int classeMax;
        int taille;
        double *t;

}Histo;


typedef struct {
        int taille;
        float *coef;
} FiltreLineaire;


FiltreLineaire fl1;
Histo stHisto, stHistoModif,stHistoModif2;


#define VRAI 0
#define FAUX 255

typedef int tabTranscode[512];

Image Im,Im2,Im3;          	     // Images originales
GLubyte *I1=NULL,*I2=NULL;   // Images à afficher
int f1,f2;		     // ID fenetres
int ordre=5;		     // Taille element structurant
int k = 0;


void BasculeImage(const Image *image, GLubyte *I)
{
     int i,j;
     for(i=0;i<image->height;i++)
       for(j=0;j<image->width;j++)
         I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
     return;
}

void affichage1(void)
{
    glRasterPos2i(0,0);
    glDrawPixels(Im.width,Im.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I1);
    glFlush();
}

void AffecterIm(Image *d,int x,int y,short valeur)
{
	if(x>=0 && y>=0 && x<d->width && y<d->height)
	  d->data[y*d->width+x]=valeur;
}

short ValMiror(Image *d,int x,int y)
{
	if(x<0)
	  x=abs(x);
	if(x>=d->width)
	  x=d->width-(x-d->width)-1;
	if(y<0)
	  y=abs(y);
	if(y>=d->height)
	  y=d->height-(y-d->height)-1;
	return d->data[y*d->width+x];
}


void CopieImg(Image& in, Image& out){
	for(int i=0;i<in.height;i++){
		for(int j=0;j<in.width;j++){
			out.data[i*Im.width+j] = in.data[i*Im.width+j];
		}
	}
}

void InitImg(Image& in){
	for(int i=0;i<in.height;i++){
		for(int j=0;j<in.width;j++){
			in.data[i*Im.width+j] = FAUX;
		}
	}
}


void initHisto(Histo * _histo,int taille)
{
        int i;
        _histo->taille = taille;
        _histo->t = new double[taille];
        for (i = 0; i< taille; i++)
        {
                _histo->t[i] = 0;
        }
}


void calculHisto (Image * im1, Histo * _histo)
{
        int i,j;
        int max, classeMax;
        for (i = 0; i < im1->width; i++)
                for (j = 0; j < im1->height; j++)
                {
                        if(_histo->taille == 2048)
                                _histo->t[im1->data[j*im1->width+i] +1024] ++;
                        else
                                _histo->t[im1->data[j*im1->width+i]]++;
                }
        max = (int)_histo->t[0];
        classeMax = 0;
        for (i = 1; i< _histo->taille; i++)
        {
                if (_histo->t[i] > max)
                {
                        max = (int)_histo->t[i];
                        classeMax = i;
                }
        }
        _histo->max = max;
        _histo->classeMax = classeMax;
        _histo->nb_pixels = im1->size;
}
void expDynamique(Image *im1)
{
        double aMin, aMax, alpha, beta;
        int i, j;

        initHisto(&stHisto,2048);
        calculHisto(im1, &stHisto);

        aMin = stHisto.t[0];
        // on parcours l'histo du début vers la fin, la derniere valeur non nulle est la plus haute
        for (i = 0; i < NB_NV_GRIS_SOBEL; i++)
        {
                if (stHisto.t[i] != 0)
                {
                        aMin = i-1024;
                        break;
                }
        }

        // on parcours l'histo de la fin vers le debut,  la derniere valeur non nulle est la plus basse
        for (i = NB_NV_GRIS_SOBEL-1; i >= 0;  i--)
        {
                if (stHisto.t[i] != 0)
                {
                        aMax = i-1024;
                        break;
                }
        }

        for (i = 0; i < im1->width; i++){
                for (j = 0; j < im1->height ; j++){
                    im1->data[i+j*im1->width] = (short)(255*((im1->data[i+j*im1->width] -aMin)/(aMax - aMin)));
                }
        }
}


void FiltrageLineaire(Image *in, Image *out, FiltreLineaire *fl, float t){
        int b = (int)fl->taille/2;
        for(int i=0;i<in->height;i++){
                for(int j=0;j<in->width;j++){
                        float s=0;
                        int n=0;
                        for(int k=i-b;k<=i+b;k++){
                                for(int l=j-b;l<=j+b;l++){
                                        s+=ValMiror(in,l,k)*fl->coef[n++];
                                }
                        }
                        out->data[j+i*in->width] = s*t+in->data[j+i*in->width];
                }
        }
}

void equation_chaleur(Image * in , Image *out , FiltreLineaire *fl , int n)
{
	float t=10/(float)n;

	for(int i = 0 ; i < n ; i++ )
		FiltrageLineaire(in , out , fl , t);
		CopieImg( *out , *in );
}
	
	

void affichage2(void)
{
    glRasterPos2i(0,0);
    glDrawPixels(Im2.width,Im2.height,GL_LUMINANCE,GL_UNSIGNED_BYTE,I2);
    glFlush();
}

void initGL(void)
{
     glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}


void redim(int width,int height)
{
	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0,width,0,height);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ChoixSousMenu(int value)
{
     ordre=value;
}

void ChoixMenuPrincipal(int value)
{
	switch (value)
	{
		case 6:
		for (int i=0 ; i<10 ; i++)
			equation_chaleur(&Im , &Im2 , &fl1 , 200);
				
		//expDynamique(&Im2);
		BasculeImage(&Im2, I2);
		break;
		case 10 :// EcrireImage("res.pgm",&Im2);
		break;
		case 11 : //LibererImage(&Im);
		//LibererImage(&Im2);
		free(I1);
		free(I2);
		exit(0); /* On quitte */
		break;
	}
	glutPostRedisplay();
	glutSetWindow(f2);
	glutPostRedisplay();
}


void CreerMenu(void)
{
	/*int sousMenu;

	sousMenu = glutCreateMenu(ChoixSousMenu);
	glutAddMenuEntry("1",1); 
	glutAddMenuEntry("2",2);
	glutAddMenuEntry("3",3);
	glutAddMenuEntry("4",4);
	glutAddMenuEntry("5",5);*/
	
	glutCreateMenu(ChoixMenuPrincipal);
	//glutAddSubMenu("Ordre",sousMenu);
	glutAddMenuEntry("Filtrage",6);
	glutAddMenuEntry("Quitter",11);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	
	/* Lecture de "bin0.pgm"*/
	if (LireImage(argv[1],&Im)==-1) return -1;
	
	CreerImage(&Im2,Im.width,Im.height);

	/* Allocation de la memoire des images � afficher */
	if ( (I1=(GLubyte *) malloc(sizeof(GLubyte)*Im.width*Im.height)) == NULL)
	{
		printf("Impossible d'allouer I1\n");
		return -1;
	}
	if ( (I2=(GLubyte *) malloc(sizeof(GLubyte)*Im.width*Im.height)) == NULL)
	{
		printf("Impossible d'allouer I2\n");
		return -1;
	}

	BasculeImage(&Im,I1);
	BasculeImage(&Im,I2);
	
	fl1.coef = new float[9];
	fl1.taille = 3;
	fl1.coef[0] = 0 ;fl1.coef[1] = 1  ;fl1.coef[2] = 0;
	fl1.coef[3] = 1 ;fl1.coef[4] = -4  ;fl1.coef[5] = 1;
	fl1.coef[6] = 0 ;fl1.coef[7] = 1  ;fl1.coef[8] = 0;


    /*--------------------------Fen�tre 1----------------------------*/
	glutInitWindowSize(Im.width,Im.height);
	glutInitWindowPosition(400,200);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f1=glutCreateWindow("Originale");
	glutDisplayFunc(affichage1);
	glutReshapeFunc(redim);
	CreerMenu();
	initGL();

	/*--------------------------Fen�tre 2----------------------------*/
	glutInitWindowSize(Im2.width,Im2.height);
	glutInitWindowPosition(700,200);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2=glutCreateWindow("Filtree");
	glutDisplayFunc(affichage2);
	glutReshapeFunc(redim);
	initGL();
	
	glutMainLoop();
	return 0;
}
