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
    void ResetImage(Image *originale);

protected:
    void FiltrageLineaire(Image *in, Image *out, FiltreLineaire *fl, float t);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QColor trolltechPurple;
};

#endif
