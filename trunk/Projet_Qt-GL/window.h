#ifndef WINDOW_H
#define WINDOW_H

#include <iostream>

#include <QWidget>
#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QtGui>
#include <QGLWidget>


extern "C"
{
#include "OutilsPGM.h"
}

class QAction;
class QSlider;
class QComboBox;
class QPushButton;
class QFileDialog;
class QLabel;
class QSpinBox;
class QDoubleSpinBox;

class GLWidget;

class Window
    : public QWidget
{
    Q_OBJECT

public:
    Window();
    void setGLWidgetOriginale(GLWidget *_widget);
    void setGLWidgetResultat(GLWidget *_widget);
    GLWidget* getGLWidgetOriginale();
    GLWidget* getGLWidgetResultat();

signals:
    void signal_CopieImgOriginale(Image *source);
    void signal_CopieImgResultat(Image *source);
    void signal_EquationChaleur(Image *in , Image *out , FiltreLineaire *fl , int n, double dt);
    void signal_MalikEtPerona(Image * in , Image *out, int n, double dt, int sigma);
    void signal_CourbureMoyenne(Image * in , Image *out, int n, double t);
    void signal_FiltreMedianItere(Image * in , Image *out, int n, FiltreLineaire filtre);
    void signal_BruitageUniforme(Image *in, Image *out, int b);
    void signal_BruitageImpulsionnel(Image *in, Image *out);
    void signal_ChargerImage(QString sFileName);
    //void signal_comboBoxClick();

public slots:
    void slot_buttonChoixEquationClicked();
    void slot_buttonCopieImgOriginale();
    void slot_buttonCopieImgResultat();
    void slot_openFileDialog();
    void slot_comboBoxChanged(int val);

protected:
    void closeEvent(QCloseEvent *event);

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
    QSpinBox *sboxNombreIterations;
    QSpinBox *sboxSigma;
    QDoubleSpinBox *sboxTemps;

    FiltreLineaire fLaplacien;
    FiltreLineaire fMedian;
};

#endif
