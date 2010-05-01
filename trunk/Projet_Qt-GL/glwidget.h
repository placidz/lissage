#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include "OutilsPGM.h"


class GLWidget : public QGLWidget
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    bool isWindowOpened;

    void Initialisation();

    Image image;
    GLubyte *imgBuffer;

    void BasculeImage(const Image *img, GLubyte *I);
    //void openFilePGM(std::string _filename);

public slots:
    void EquationChaleur(Image *in , Image *out , FiltreLineaire *fl , int n, double dt);
    void CopieImg(Image *source);
    void BruitageUniforme(Image *in, Image *out, int b);
    void BruitageImpulsionnel(Image *in, Image *out);
    void ChargerImage(QString sFileName);
    void appelMalikPerona(Image * in , Image *out, int n, double dt, int sigma);
    void appelCourbureMoyenne(Image * in , Image *out, int n, double t);
    void appelFiltreMedianItere(Image * in , Image *out, int n, FiltreLineaire filtre);

protected:
    void FiltrageLineaire(ImageD *in, ImageD *out, FiltreLineaire *fl, double t);
    void ExpansionDynamique2(ImageD *in, Image *out);
    double calculeC(ImageD *Img, int i, int j, int dir, int sigma);
    double calculeCnsew(ImageD *Img, int i, int j, int sigma);
    void malikEtPerona(ImageD *in, ImageD *out, double dt, int sigma);    
    void courbureMoyenne(ImageD *in, ImageD *out, double t);
    double calculeMiniSobel(ImageD *Img, int i, int j);
    double calculeK(ImageD *Img, int i, int j);
    void FiltrageMedian(Image *src, Image *dest, FiltreLineaire flt);
    double TriABullesMedian(float *t, int size);

    double Min(double a, double b);
    double Max(double a, double b);

    void closeEvent(QCloseEvent *event);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QColor trolltechPurple;
};

#endif
