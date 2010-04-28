#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>

extern "C"
{
#include "OutilsPGM.h"
}


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
    void BruitageImpulsionnel(Image *in, Image *out, int b);
    void BruitagePoivreEtSel(Image *in, Image *out);
    void ChargerImage(QString sFileName);

protected:
    void FiltrageLineaire(Image *in, Image *out, FiltreLineaire *fl, float t);
    void ExpansionDynamique2(Image *in, Image *out);
    int Min(int a, int b);
    int Max(int a, int b);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QColor trolltechPurple;
};

#endif
