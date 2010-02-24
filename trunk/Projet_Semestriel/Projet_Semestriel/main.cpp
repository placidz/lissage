#include <stdio.h>
#include <stdlib.h>
#include "glut.h"
#include <math.h>
#include "OutilsPGM.h"

/*** DEFINES ***/
#define M_PI 3.14159265358979323846

/*** PROTOTYPES ***/

// Utilitaires images
void ModifierPixel(Image *d, int x, int y, short valeur);
void CopierImage(Image *s, Image *d);
void BasculeImage(const Image *image, GLubyte *I);
short ValMiror(Image *d, int x, int y);

// OpenGL / Glut
void initGL(void);
void affichage1(void);
void affichage2(void);
void redim(int width, int height);
void ChoixSousMenu(int value);
void ChoixMenuPrincipal(int value);
void CreerMenu(void);
void souris (int button, int state, int x, int y);
void clavier(unsigned char key, int x, int y);
int initImages(void);

// Fonctions projet
void FiltrageLineaireGauss(Image s, Image *d, float t);
float Gaussien(Image s, float t, int i);


/*** VARIABLES ***/
float t = 0.1;

typedef struct
{
	int taille;
	double* coef;
} Filtre;

int f1, f2;
Image Im1, Im2;		// Images originales
GLubyte *I1=NULL, *I2=NULL;		// Image à afficher


void ModifierPixel(Image *d, int x, int y, short valeur)
{
	if(x>=0 && y>=0 && x<d->width && y<d->height)
		d->data[y*d->width+x]=valeur;
}

void CopierImage(Image *s, Image *d)
{
	int xi, yi;
	for (yi = 0; yi < s->height; yi++)
		for (xi = 0; xi < s->width; xi++)
			ModifierPixel(d, xi, yi, s->data[yi*s->width+xi]);
}

void BasculeImage(const Image *image, GLubyte *I)
{
	int i,j;
	for(i=0;i<image->height;i++)
		for(j=0;j<image->width;j++)
			I[(image->height-1-i)*image->width+j]=(GLubyte) image->data[i*image->width+j];
	return;
}

short ValMiror(Image *d, int x, int y)
{
	if(x < 0) x = -x - 1;
	if(x >= d->width) x = d->width - (x - d->width) - 1;
	if(y < 0) y = -y - 1;
	if(y >= d->height) y = d->height - (y - d->height ) - 1;
	  
	return d->data[y*d->width+x];
}

void initGL(void)
{
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
}

void affichage_f1(void)
{
	glRasterPos2i(0,0);

	glDrawPixels(Im1.width, Im1.height, GL_LUMINANCE, GL_UNSIGNED_BYTE, I1);
	glFlush();
}

void affichage_f2(void)
{
	glRasterPos2i(0,0);
		FiltrageLineaireGauss(Im1, &Im2, t);
	glDrawPixels(Im1.width, Im1.height, GL_LUMINANCE, GL_UNSIGNED_BYTE, I2);
	glFlush();
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

void ChoixMenuPrincipal(int value)
{
	switch (value)
	{
		case 1:

			BasculeImage(&Im2, I1);
			glutPostRedisplay();
		break;
		
		case 10 :
			EcrireImage("nomImage.pgm", &Im2);
		break;
	
		case 11 :
	    	LibererImage(&Im1);
			free(I1);
			exit(0); /* On quitte */
		break;
	} 
}

void souris (int button, int state, int x, int y)
{
	switch(button)
	{
		case GLUT_LEFT_BUTTON:
		break;
	}
}

void clavier(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 27:
			exit(0);
		break;

		case 'p':
			t += 0.1;
			printf("t: %f\n", t);
		break;

		case 'm':
			if (t > 0.1) t-= 0.1;
			printf("t: %f\n", t);
		break;
	}
	glutPostRedisplay();
}

void CreerMenu(void)
{
	glutCreateMenu(ChoixMenuPrincipal);

	glutAddMenuEntry("Filtrage lineaire moyen", 1);
	glutAddMenuEntry("Filtrage median", 2);
	glutAddMenuEntry("Filtrage de Nagao", 3);
	glutAddMenuEntry("EcrireImage", 10);
	glutAddMenuEntry("Quitter", 11);
	
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

int main(int argc,char **argv)
{
	glutInit(&argc,argv);
	
	if (LireImage("MMs_bruit2.pgm",&Im1)==-1) return -1;

	CreerImage(&Im2, Im1.width,Im1.height);
	//CopierImage(&Im, &Im2);

	/* Allocation de la memoire des images à afficher */
	if ( (I1=(GLubyte *) malloc(sizeof(GLubyte)*Im1.width*Im1.height)) == NULL)
	{
		 printf("Impossible d'allouer I1\n");
		 return -1;
	}
	if ( (I2=(GLubyte *) malloc(sizeof(GLubyte)*Im1.width*Im1.height)) == NULL)
	{
		 printf("Impossible d'allouer I2\n");
		 return -1;
	}

	/* Transformation de l'image donnée en image à afficher */
	BasculeImage(&Im1, I1);
	BasculeImage(&Im1, I2);
	
    /*--------------------------Fenêtre 1----------------------------*/
	glutInitWindowSize(Im1.width,Im1.height);
	glutInitWindowPosition(200,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f1 = glutCreateWindow("Image originale");
	glutDisplayFunc(affichage_f1);
	glutReshapeFunc(redim);
	glutMouseFunc(souris);
	glutKeyboardFunc(clavier);
	CreerMenu();
	initGL();

	/*--------------------------Fenêtre 2----------------------------*/
	glutInitWindowSize(Im1.width,Im1.height);
	glutInitWindowPosition(220+Im1.width,100);
	glutInitDisplayMode(GLUT_RGBA | GLUT_SINGLE);
	f2 = glutCreateWindow("Image filtrée");
	glutDisplayFunc(affichage_f2);
	glutReshapeFunc(redim);
	glutMouseFunc(souris);
	glutKeyboardFunc(clavier);
	CreerMenu();
	initGL();

	glutMainLoop();
	return 0;
} 

void FiltrageLineaire(Image *Img1, Image *Img2, Filtre flt)
{
	//printf("FiltrageLineaire - DEBUT \n");
	double somme;
	int n;
	for (int i = 0; i < Img1->width; i++)
	{
		//printf("For1 \n");
		for (int j = 0; j < Img1->height; j++)
		{
			//printf("For2 \n");
			somme = 0;
			n = 0;
			for (int k = (int)(-flt.taille / 2); k <= (int)(flt.taille / 2); k++)
			{
				for (int l = (int)(-flt.taille / 2); l <= (int)(flt.taille / 2); l++)
				{
					//printf("For3-4 \n");
					//printf("somme=%d  l=%d   k=%d \n", somme, l, k);
					somme += ValMiror(Img1, i+k, j+l) * flt.coef[n];
					//printf("n=%d   valbase=%i   somme=%d \n", n, ValMiror(Img1, i+k, j+l), somme);
					n++;
				}
			}
			//printf("ind data=%d   somme=%f \n", j*Img2->width+i, somme);
			Img2->data[j*Img2->width+i] = (short)somme;
		}
	}
	//printf("FiltrageLineaire - FIN \n");
}

void FiltrageLineaireGauss(Image s, Image *d, float t)
{
	int sz = s.size;
	for (int i = 0; i < sz; i++)
	{
		d->data[i] = (short)(Gaussien(s, t, i) *(s.data[i]) );
		//printf("pixel %d : %f\n", i, s.data[i]);
	}
}

float Gaussien(Image s, float t, int i)
{
	float r;
	r = ((1 / (4*M_PI*t)) * exp(-(s.data[i]*s.data[i])/(4*t)));
	printf("gaussien de %d : %f\n", i, r);
	return (r);
}