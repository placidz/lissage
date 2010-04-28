#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QGLWidget>
#include <iostream>

extern "C"
{
#include "OutilsPGM.h"
}

class QSlider;
class QComboBox;
class QPushButton;
class QLabel;
class QSpinBox;
class QDoubleSpinBox;

class GLWidget;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();

signals:
    void signal_EquationChaleur(Image *in , Image *out , FiltreLineaire *fl , int n);
    void signal_ResetImgResultat(Image *originale);

public slots:
    void slot_buttonChoixEquationClicked();
    void slot_buttonResetImgResultat();

private:
    GLWidget *glWidgetOriginale;
    GLWidget *glWidgetResultat;
    QComboBox *comboBoxChoixEquation;
    QPushButton *buttonLancerEquation;
    QPushButton *buttonResetImgResultat;
    QPushButton *buttonSauverResultat;
    QLabel *labelImgOriginale;
    QLabel *labelImgFiltree;
    QLabel *labelNombreIterations;
    QSpinBox *sboxNombreIterations;
    QDoubleSpinBox *sboxDelta;

    FiltreLineaire fLaplacien;
};

#endif
