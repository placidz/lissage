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

#include <QtGui>

#include "glwidget.h"
#include "window.h"

Window::Window()
{
    glWidgetOriginale = new GLWidget();
    glWidgetResultat = new GLWidget();
    combobox = new QComboBox;
    combobox->addItem("Equation de la Chaleur");
    combobox->addItem("Malik et Perona");
    combobox->addItem("Equation par courbure moyenne");
    button = new QPushButton("Appliquer le filtre");
    labelImgOriginale = new QLabel("Image originale");
    labelImgFiltree = new QLabel("Image filtrée");

    fLaplacien.coef = new float[9];
    fLaplacien.taille = 3;
    fLaplacien.coef[0] = 0; fLaplacien.coef[1] = 1; fLaplacien.coef[2] = 0;
    fLaplacien.coef[3] = 1; fLaplacien.coef[4] = -4;fLaplacien.coef[5] = 1;
    fLaplacien.coef[6] = 0; fLaplacien.coef[7] = 1; fLaplacien.coef[8] = 0;

    connect(button, SIGNAL(clicked()), this, SLOT(slot_clicked()));
    connect(this, SIGNAL(signal_EquationChaleur(Image*,Image*,FiltreLineaire*,int)), glWidgetResultat, SLOT(EquationChaleur(Image*,Image*,FiltreLineaire*,int)));

    /*connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setXRotation(int)));
     connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
     connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setYRotation(int)));
     connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
     connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(setZRotation(int)));
     connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));*/

    QGridLayout *mainLayout = new QGridLayout;
    QGridLayout *hLayout = new QGridLayout;
    QVBoxLayout *vLayout = new QVBoxLayout;


    hLayout->addWidget(labelImgOriginale, 0, 0);
    hLayout->addWidget(labelImgFiltree, 0, 1);
    hLayout->addWidget(glWidgetOriginale, 1, 0);
    hLayout->addWidget(glWidgetResultat, 1, 1);

    vLayout->addWidget(combobox);
    vLayout->addWidget(button);

    mainLayout->addLayout(hLayout, 0, 0);
    mainLayout->addLayout(vLayout, 0, 1);

    setLayout(mainLayout);

    setWindowTitle(tr("Methodes de debruitage par equations de diffusion"));
}

void Window::slot_clicked()
{
    emit signal_EquationChaleur(&glWidgetOriginale->image, &glWidgetResultat->image, &fLaplacien, 20);
}
