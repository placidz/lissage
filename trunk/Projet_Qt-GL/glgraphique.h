#ifndef GLGRAPHIQUE_H
#define GLGRAPHIQUE_H

#include <QGLWidget>
#include "glwidget.h"
#include "OutilsPGM.h"

class GLWidget;

class GLGraphique : public QGLWidget
{
    Q_OBJECT

public:
    GLGraphique(QWidget *parent = 0);
    ~GLGraphique();

    GLWidget *imgInitiale;
    GLWidget *imgResultat;

    int widthGraph;
    int heightGraph;

    bool isWindowOpened;

    bool bGraphiqueMode;
    int LigneADessiner;

    void BasculeImage(const Image *img, GLubyte *I);

    void drawVueEnCoupe(Image * in, int line);

public slots:
    void updateGraphique();

protected:
    void closeEvent(QCloseEvent *event);

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QColor trolltechPurple;
};

#endif
