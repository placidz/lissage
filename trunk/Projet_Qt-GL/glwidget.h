/****************************************************************************
 **
 ** Copyright (C) 2005-2008 Nokia Corporation and/or its subsidiary(-ies). All rights reserved.
 **
 ** This file is part of the documentation of the Qt Toolkit.
 **
 ** Licensees holding a valid Qt License Agreement may use this file in
 ** accordance with the rights, responsibilities and obligations
 ** contained therein.  Please consult your licensing agreement or
 ** contact qt-sales@nokia.com if any conditions of this licensing
 ** agreement are not clear to you.
 **
 ** Further information about Qt licensing is available at:
 ** http://trolltech.com/products/appdev/licensing.
 **
 ** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 ** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 **
 ****************************************************************************/

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

protected:
    void FiltrageLineaire(Image *in, Image *out, FiltreLineaire *fl, float t);
    /*void setXRotation(int angle);
     void setYRotation(int angle);
     void setZRotation(int angle);*/

signals:
    /*void xRotationChanged(int angle);
     void yRotationChanged(int angle);
     void zRotationChanged(int angle);*/

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

private:
    QColor trolltechPurple;
};

#endif
