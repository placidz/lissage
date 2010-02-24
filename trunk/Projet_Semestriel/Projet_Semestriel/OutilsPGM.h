
#define MAXLIGNE 256

typedef short Pixel;

typedef struct
{
	int width, height, size;
	Pixel *data;
} Image;

int CreerImage(Image *image, int largeur, int hauteur);
int LireImage(const char *nom, Image *image);
int EcrireImage(const char *nom, Image *image);
void LibererImage(Image *i);
