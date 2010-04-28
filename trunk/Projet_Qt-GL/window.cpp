#include <QtGui>

#include "glwidget.h"
#include "window.h"

Window::Window()
{
    glWidgetOriginale = new GLWidget();
    glWidgetResultat = new GLWidget();

    comboBoxChoixEquation = new QComboBox;
    comboBoxChoixEquation->addItem("Equation de la Chaleur");
    comboBoxChoixEquation->addItem("Approche de Malik et Perona");
    comboBoxChoixEquation->addItem("Equation par courbure moyenne");

    buttonLancerEquation = new QPushButton("Appliquer le filtre");
    buttonLancerEquation->setFixedWidth(150);
    buttonResetImgResultat = new QPushButton(">>");
    buttonResetImgResultat->setFixedWidth(40);
    buttonSauverResultat = new QPushButton("Sauvegarder");
    buttonSauverResultat->setFixedWidth(100);

    sboxNombreIterations = new QSpinBox;
    sboxNombreIterations->setValue(20);
    sboxNombreIterations->setMinimum(1);
    sboxNombreIterations->setMaximum(1000);
    sboxNombreIterations->setFixedWidth(70);
    sboxDelta = new QDoubleSpinBox;
    sboxDelta->setValue(1.1);

    labelImgOriginale = new QLabel("<strong>Image originale</strong>");
    labelImgFiltree = new QLabel("<strong>Image filtrée</strong>");
    labelNombreIterations = new QLabel("Nombre d'itérations :");

    fLaplacien.coef = new float[9];
    fLaplacien.taille = 3;
    fLaplacien.coef[0] = 0; fLaplacien.coef[1] = 1; fLaplacien.coef[2] = 0;
    fLaplacien.coef[3] = 1; fLaplacien.coef[4] = -4;fLaplacien.coef[5] = 1;
    fLaplacien.coef[6] = 0; fLaplacien.coef[7] = 1; fLaplacien.coef[8] = 0;

    connect(buttonLancerEquation, SIGNAL(clicked()), this, SLOT(slot_buttonChoixEquationClicked()));
    connect(this, SIGNAL(signal_EquationChaleur(Image*,Image*,FiltreLineaire*,int)), glWidgetResultat, SLOT(EquationChaleur(Image*,Image*,FiltreLineaire*,int)));

    connect(buttonResetImgResultat, SIGNAL(clicked()), this, SLOT(slot_buttonResetImgResultat()));
    connect(this, SIGNAL(signal_ResetImgResultat(Image*)), glWidgetResultat, SLOT(ResetImage(Image*)));

    QGridLayout *mainLayout = new QGridLayout;
    QGridLayout *hLayout = new QGridLayout;
    QGridLayout *vLayout = new QGridLayout;

    hLayout->addWidget(labelImgOriginale, 0, 0, Qt::AlignHCenter);
    hLayout->addWidget(labelImgFiltree, 0, 2, Qt::AlignHCenter);
    hLayout->addWidget(glWidgetOriginale, 1, 0);
    hLayout->addWidget(buttonResetImgResultat, 1, 1, Qt::AlignHCenter);
    hLayout->addWidget(glWidgetResultat, 1, 2);
    hLayout->addWidget(buttonSauverResultat, 2, 2, Qt::AlignHCenter);

    vLayout->addWidget(comboBoxChoixEquation, 0, 0, Qt::AlignHCenter);

    QFormLayout *rowsLayout = new QFormLayout;
    rowsLayout->addRow("Nombre d'itérations : ", sboxNombreIterations);
    rowsLayout->addRow("Delta : ", sboxDelta);
    vLayout->addLayout(rowsLayout, 1, 0, Qt::AlignHCenter);

    vLayout->addWidget(buttonLancerEquation, 2, 0, Qt::AlignCenter);

    mainLayout->addLayout(hLayout, 0, 0);
    mainLayout->addLayout(vLayout, 0, 1);

    setLayout(mainLayout);

    setWindowTitle(tr("Methodes de debruitage par equations de diffusion"));
}

void Window::slot_buttonChoixEquationClicked()
{
    int choix = comboBoxChoixEquation->currentIndex();
    switch(choix)
    {
    case 0:
	printf("Equation de la Chaleur\n");
	emit signal_EquationChaleur(&glWidgetOriginale->image, &glWidgetResultat->image, &fLaplacien, sboxNombreIterations->value());
	break;

    default:
	printf("Erreur dans le choix\n");
	break;
    }
}

void Window::slot_buttonResetImgResultat()
{
    emit signal_ResetImgResultat(&glWidgetOriginale->image);
}
