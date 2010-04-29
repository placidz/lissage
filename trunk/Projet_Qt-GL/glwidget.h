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

    void Initialisation();

    Image image;
    GLubyte *imgBuffer;

    void BasculeImage(const Image *img, GLubyte *I);
    //void openFilePGM(std::string _filename);

public slots:
    void EquationChaleur(Image *in , Image *out , FiltreLineaire *fl , int n);
    void CopieImg(Image *source);
    void BruitageUniforme(Image *in, Image *out, int b);
    void BruitageImpulsionnel(Image *in, Image *out);
    void ChargerImage(QString sFileName);
    void appelMalikPerona(Image * in , Image *out, int n, int sigma);

protected:
    void FiltrageLineaire(ImageD *in, ImageD *out, FiltreLineaire *fl, float t);
    void ExpansionDynamique2(ImageD *in, Image *out);
    double calculeC(ImageD *Img, int i, int j, int dir, int sigma);
    double calculeCnsew(ImageD *Img, int i, int j, int sigma);
    void malikEtPerona(ImageD *in, ImageD *out, float t, int sigma);

    double Min(double a, double b);
    double Max(double a, double b);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QColor trolltechPurple;
};

#endif
