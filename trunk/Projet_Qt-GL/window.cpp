
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
    comboBoxChoixEquation->addItem("- - - - - - - - - - - - - - - - - - - - - - - - - -");
    comboBoxChoixEquation->addItem("Bruitage Uniforme");
    comboBoxChoixEquation->addItem("Bruitage Impulsionnel");

    buttonLancerEquation = new QPushButton("Appliquer le filtre");
    buttonLancerEquation->setFixedWidth(150);
    buttonCopieImgOriginale= new QPushButton(">>");
    buttonCopieImgOriginale->setFixedWidth(40);
    buttonCopieImgResultat = new QPushButton("<<");
    buttonCopieImgResultat->setFixedWidth(40);
    buttonChargerImage = new QPushButton("Charger...");
    buttonSauverResultat = new QPushButton("Sauvegarder");
    buttonSauverResultat->setFixedWidth(100);

    sboxNombreIterations = new QSpinBox;
    sboxNombreIterations->setValue(20);
    sboxNombreIterations->setMinimum(1);
    sboxNombreIterations->setMaximum(1000);
    sboxNombreIterations->setFixedWidth(70);
    sboxSigma = new QSpinBox;
    sboxSigma->setValue(20);
    //sboxSigma->setSingleStep(0.1);
    sboxSigma->setFixedWidth(60);

    labelImgOriginale = new QLabel("<strong>Image départ</strong>");
    labelImgResultat = new QLabel("<strong>Image résultat</strong>");
    labelNombreIterations = new QLabel("Nombre d'itérations :");
    labelSigma = new QLabel("Sigma : ");

    fLaplacien.coef = new float[9];
    fLaplacien.taille = 3;
    fLaplacien.coef[0] = 0; fLaplacien.coef[1] = 1; fLaplacien.coef[2] = 0;
    fLaplacien.coef[3] = 1; fLaplacien.coef[4] = -4;fLaplacien.coef[5] = 1;
    fLaplacien.coef[6] = 0; fLaplacien.coef[7] = 1; fLaplacien.coef[8] = 0;

    connect(buttonLancerEquation, SIGNAL(clicked()), this, SLOT(slot_buttonChoixEquationClicked()));
    connect(this, SIGNAL(signal_EquationChaleur(Image*,Image*,FiltreLineaire*,int)), glWidgetResultat, SLOT(EquationChaleur(Image*,Image*,FiltreLineaire*,int)));
    connect(this, SIGNAL(signal_MalikEtPerona(Image*,Image*,int,int)), glWidgetResultat, SLOT(appelMalikPerona(Image*,Image*,int,int)));
    connect(this, SIGNAL(signal_BruitageUniforme(Image*,Image*,int)), glWidgetResultat, SLOT(BruitageUniforme(Image*,Image*,int)));
    connect(this, SIGNAL(signal_BruitageImpulsionnel(Image*,Image*)), glWidgetResultat, SLOT(BruitageImpulsionnel(Image*,Image*)));

    connect(buttonCopieImgOriginale, SIGNAL(clicked()), this, SLOT(slot_buttonCopieImgOriginale()));
    connect(this, SIGNAL(signal_CopieImgOriginale(Image*)), glWidgetResultat, SLOT(CopieImg(Image*)));
    connect(buttonCopieImgResultat, SIGNAL(clicked()), this, SLOT(slot_buttonCopieImgResultat()));
    connect(this, SIGNAL(signal_CopieImgResultat(Image*)), glWidgetOriginale, SLOT(CopieImg(Image*)));
    connect(this, SIGNAL(signal_ChargerImage(QString)), glWidgetOriginale, SLOT(ChargerImage(QString)));
    connect(this, SIGNAL(signal_ChargerImage(QString)), glWidgetResultat, SLOT(ChargerImage(QString)));

    connect(buttonChargerImage, SIGNAL(clicked()), this, SLOT(slot_openFileDialog()));

    QGridLayout *mainLayout = new QGridLayout;
    QGridLayout *hLayout = new QGridLayout;
    QGridLayout *vLayout = new QGridLayout;

    hLayout->addWidget(labelImgOriginale, 0, 0, Qt::AlignHCenter);
    hLayout->addWidget(labelImgResultat, 0, 2, Qt::AlignHCenter);
    hLayout->addWidget(glWidgetOriginale, 1, 0);
    QVBoxLayout *littleLayout = new QVBoxLayout;
    littleLayout->setMargin(20);
    littleLayout->addWidget(buttonCopieImgOriginale, Qt::AlignHCenter);
    littleLayout->addWidget(buttonCopieImgResultat, Qt::AlignHCenter);
    hLayout->addLayout(littleLayout, 1, 1, Qt::AlignHCenter);

    hLayout->addWidget(glWidgetResultat, 1, 2);
    hLayout->addWidget(buttonChargerImage, 2, 0,  Qt::AlignHCenter);
    hLayout->addWidget(buttonSauverResultat, 2, 2, Qt::AlignHCenter);

    vLayout->addWidget(comboBoxChoixEquation, 0, 0, Qt::AlignHCenter);

    //QFormLayout *rowsLayout = new QFormLayout;
    QGridLayout *rowsLayout = new QGridLayout;
    rowsLayout->addWidget(labelNombreIterations, 0, 0, Qt::AlignRight);
    rowsLayout->addWidget(sboxNombreIterations, 0, 1, Qt::AlignHCenter);
    rowsLayout->addWidget(labelSigma, 1, 0, Qt::AlignRight);
    rowsLayout->addWidget(sboxSigma, 1, 1, Qt::AlignHCenter);
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

    case 1:
	printf("Malik et Perona\n");
	emit signal_MalikEtPerona(&glWidgetOriginale->image, &glWidgetResultat->image, sboxNombreIterations->value(), sboxSigma->value());
	break;

    case 2:
	printf("Equation par courbure moyenne\n");
	break;

    case 4:
	printf("Bruitage Uniforme\n");
	emit signal_BruitageUniforme(&glWidgetOriginale->image, &glWidgetResultat->image, 20);
	break;

    case 5:
	printf("Bruitage Impulsionnel\n");
	emit signal_BruitageImpulsionnel(&glWidgetOriginale->image, &glWidgetResultat->image);
	break;

    default:
	QMessageBox::critical(this, "Erreur de sélection", "Erreur dans la sélection de l'opération.");
	break;
    }
}

void Window::slot_buttonCopieImgOriginale()
{
    emit signal_CopieImgOriginale(&glWidgetOriginale->image);
}

void Window::slot_buttonCopieImgResultat()
{
    emit signal_CopieImgResultat(&glWidgetResultat->image);
}

void Window::slot_openFileDialog()
{
    QFileDialog* fd = new QFileDialog(this, "Choose an image file to open");

    fd->setFileMode(QFileDialog::ExistingFiles);
    fd->setFilter( "Images PGM (*.pgm)" );

    if ( fd->exec() == QDialog::Accepted )
    {
	strFileName = fd->selectedFiles().at(0);//fd->selectedFile();
	emit signal_ChargerImage(strFileName);
	this->setFixedWidth(glWidgetOriginale->image.width*2 + 380);
	this->setFixedHeight(glWidgetOriginale->image.height + 100);
    }
    else return;
    //adjustSize();
}
