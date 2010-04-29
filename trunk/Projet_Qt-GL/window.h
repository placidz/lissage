#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#include <QWidget>
#include <QtGui>
#include <QGLWidget>


extern "C"
{
#include "OutilsPGM.h"
}

class QSlider;
class QComboBox;
class QPushButton;
class QFileDialog;
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
    void signal_CopieImgOriginale(Image *source);
    void signal_CopieImgResultat(Image *source);
    void signal_EquationChaleur(Image *in , Image *out , FiltreLineaire *fl , int n);
    void signal_MalikEtPerona(Image * in , Image *out, int n, int sigma);
    void signal_BruitageUniforme(Image *in, Image *out, int b);
    void signal_BruitageImpulsionnel(Image *in, Image *out);
    void signal_ChargerImage(QString sFileName);

public slots:
    void slot_buttonChoixEquationClicked();
    void slot_buttonCopieImgOriginale();
    void slot_buttonCopieImgResultat();
    void slot_openFileDialog();

private:
    QString strFileName;

    GLWidget *glWidgetOriginale;
    GLWidget *glWidgetResultat;
    QComboBox *comboBoxChoixEquation;
    QPushButton *buttonLancerEquation;
    QPushButton *buttonCopieImgResultat;
    QPushButton *buttonCopieImgOriginale;
    QPushButton *buttonChargerImage;
    QPushButton *buttonSauverResultat;
    QLabel *labelImgOriginale;
    QLabel *labelImgResultat;
    QLabel *labelNombreIterations;
    QLabel *labelSigma;
    QSpinBox *sboxNombreIterations;
    QSpinBox *sboxSigma;

    FiltreLineaire fLaplacien;
};

#endif
