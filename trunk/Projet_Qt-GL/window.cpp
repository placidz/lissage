
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
    comboBoxChoixEquation->addItem("Médian itéré");
    comboBoxChoixEquation->addItem("- - - - - - - - - - - - - - - - - - - - - - - - - -");
    comboBoxChoixEquation->addItem("Bruitage Uniforme");
    comboBoxChoixEquation->addItem("Bruitage Impulsionnel");

    buttonLancerEquation = new QPushButton("Appliquer le filtre");
    //buttonLancerEquation->setFixedWidth(150);
    buttonCopieImgOriginale= new QPushButton("Copier Image Initiale dans Résultat");
    //buttonCopieImgOriginale->setFixedWidth(40);
    buttonCopieImgResultat = new QPushButton("Copier Image Résultat dans Initiale");
    //buttonCopieImgResultat->setFixedWidth(40);
    buttonChargerImage = new QPushButton("Ouvrir une image...");
    buttonSauverResultat = new QPushButton("Sauvegarder");
    buttonSauverResultat->setFixedWidth(100);

    sboxNombreIterations = new QSpinBox;
    sboxNombreIterations->setValue(20);
    sboxNombreIterations->setSingleStep(10);
    sboxNombreIterations->setMinimum(1);
    sboxNombreIterations->setMaximum(1000);
    sboxNombreIterations->setFixedWidth(70);
    sboxTemps = new QDoubleSpinBox;
    sboxTemps->setValue(0.2);
    sboxTemps->setSingleStep(0.01);
    sboxTemps->setFixedWidth(70);
    sboxSigma = new QSpinBox;
    sboxSigma->setValue(20);
    sboxSigma->setFixedWidth(70);
    sboxSigma->setDisabled(true);
    sboxSigma->setMinimum(1);
    sboxSigma->setMaximum(200);

    fLaplacien.coef = new float[9];
    fLaplacien.taille = 3;
    fLaplacien.coef[0] = 0; fLaplacien.coef[1] = 1; fLaplacien.coef[2] = 0;
    fLaplacien.coef[3] = 1; fLaplacien.coef[4] = -4;fLaplacien.coef[5] = 1;
    fLaplacien.coef[6] = 0; fLaplacien.coef[7] = 1; fLaplacien.coef[8] = 0;

    fMedian.coef = new float[9];
    fMedian.taille = 3;

    connect(buttonLancerEquation, SIGNAL(clicked()), this, SLOT(slot_buttonChoixEquationClicked()));
    connect(this, SIGNAL(signal_EquationChaleur(Image*,Image*,FiltreLineaire*,int,double)), glWidgetResultat, SLOT(EquationChaleur(Image*,Image*,FiltreLineaire*,int,double)));
    connect(this, SIGNAL(signal_MalikEtPerona(Image*,Image*,int,double,int)), glWidgetResultat, SLOT(appelMalikPerona(Image*,Image*,int,double,int)));
    connect(this, SIGNAL(signal_CourbureMoyenne(Image*,Image*,int,double)), glWidgetResultat, SLOT(appelCourbureMoyenne(Image*,Image*,int,double)));
    connect(this, SIGNAL(signal_FiltreMedianItere(Image*,Image*,int,FiltreLineaire)), glWidgetResultat, SLOT(appelFiltreMedianItere(Image*,Image*,int,FiltreLineaire)));
    connect(this, SIGNAL(signal_BruitageUniforme(Image*,Image*,int)), glWidgetResultat, SLOT(BruitageUniforme(Image*,Image*,int)));
    connect(this, SIGNAL(signal_BruitageImpulsionnel(Image*,Image*)), glWidgetResultat, SLOT(BruitageImpulsionnel(Image*,Image*)));

    connect(buttonCopieImgOriginale, SIGNAL(clicked()), this, SLOT(slot_buttonCopieImgOriginale()));
    connect(this, SIGNAL(signal_CopieImgOriginale(Image*)), glWidgetResultat, SLOT(CopieImg(Image*)));
    connect(buttonCopieImgResultat, SIGNAL(clicked()), this, SLOT(slot_buttonCopieImgResultat()));
    connect(this, SIGNAL(signal_CopieImgResultat(Image*)), glWidgetOriginale, SLOT(CopieImg(Image*)));
    connect(this, SIGNAL(signal_ChargerImage(QString)), glWidgetOriginale, SLOT(ChargerImage(QString)));
    connect(this, SIGNAL(signal_ChargerImage(QString)), glWidgetResultat, SLOT(ChargerImage(QString)));

    connect(buttonChargerImage, SIGNAL(clicked()), this, SLOT(slot_openFileDialog()));
    connect(comboBoxChoixEquation, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboBoxChanged(int)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QFormLayout *formLayout = new QFormLayout;

    mainLayout->addWidget(buttonChargerImage, Qt::AlignCenter);
    mainLayout->addWidget(comboBoxChoixEquation, Qt::AlignCenter);
    mainLayout->addWidget(buttonCopieImgOriginale, Qt::AlignCenter);
    mainLayout->addWidget(buttonCopieImgResultat, Qt::AlignCenter);

    QGroupBox *gpBox = new QGroupBox("Paramètres :");
    gpBox->setFlat(false);

    formLayout->setRowWrapPolicy(QFormLayout::DontWrapRows);
    formLayout->setFieldGrowthPolicy(QFormLayout::FieldsStayAtSizeHint);
    formLayout->setFormAlignment(Qt::AlignHCenter | Qt::AlignTop);
    formLayout->setLabelAlignment(Qt::AlignRight);

    formLayout->addRow("Nombre d'itérations : ", sboxNombreIterations);
    formLayout->addRow("dt : ", sboxTemps);
    formLayout->addRow("k : ", sboxSigma);

    gpBox->setLayout(formLayout);

    mainLayout->addWidget(gpBox);

    mainLayout->addWidget(buttonLancerEquation, Qt::AlignCenter);

    printf("loled\n");
    setLayout(mainLayout);
    printf("loled2\n");

    setWindowTitle(tr("Methodes de debruitage par equations de diffusion"));

    setFixedSize(250, 400);
    move(5, 200);

    glWidgetOriginale->setWindowTitle(tr("Image Initiale"));
    glWidgetOriginale->show();
    glWidgetOriginale->move(300, 200);

    glWidgetResultat->setWindowTitle(tr("Image Resultat"));
    glWidgetResultat->show();
    glWidgetResultat->move(600, 200);
}

void Window::setGLWidgetOriginale(GLWidget *_widget)
{
    this->glWidgetOriginale = _widget;
}

void Window::setGLWidgetResultat(GLWidget *_widget)
{
    this->glWidgetResultat = _widget;
}

GLWidget* Window::getGLWidgetOriginale()
{
    return this->glWidgetOriginale;
}

GLWidget* Window::getGLWidgetResultat()
{
    return this->glWidgetResultat;
}

void Window::slot_buttonChoixEquationClicked()
{
    int choix = comboBoxChoixEquation->currentIndex();
    switch(choix)
    {
    case 0:
	printf("Equation de la Chaleur\n");
	emit signal_EquationChaleur(&glWidgetOriginale->image, &glWidgetResultat->image, &fLaplacien, sboxNombreIterations->value(), sboxTemps->value());
	break;

    case 1:
	printf("Malik et Perona\n");
	emit signal_MalikEtPerona(&glWidgetOriginale->image, &glWidgetResultat->image, sboxNombreIterations->value(), sboxTemps->value(), sboxSigma->value());
	break;

    case 2:
	printf("Equation par courbure moyenne\n");
	emit signal_CourbureMoyenne(&glWidgetOriginale->image, &glWidgetResultat->image, sboxNombreIterations->value(), sboxTemps->value());
	break;

    case 3:
	printf("Médian itéré\n");
	emit signal_FiltreMedianItere(&glWidgetOriginale->image, &glWidgetResultat->image, sboxNombreIterations->value(), fMedian);
	break;

    case 5:
	printf("Bruitage Uniforme\n");
	emit signal_BruitageUniforme(&glWidgetOriginale->image, &glWidgetResultat->image, 20);
	break;

    case 6:
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
    }
    else return;
}

void Window::closeEvent(QCloseEvent *event)
{
    glWidgetOriginale->isWindowOpened = false;
    glWidgetResultat->isWindowOpened = false;
    glWidgetOriginale->close();
    glWidgetResultat->close();
    event->accept();
}

void Window::slot_comboBoxChanged(int val)
{
    switch(val)
    {
    case 0:
	sboxNombreIterations->setDisabled(false);
	sboxTemps->setDisabled(false);
	sboxSigma->setDisabled(true);
	break;

    case 1:
	sboxNombreIterations->setDisabled(false);
	sboxTemps->setDisabled(false);
	sboxSigma->setDisabled(false);
	break;

    case 2:
	sboxNombreIterations->setDisabled(false);
	sboxTemps->setDisabled(false);
	sboxSigma->setDisabled(true);
	break;

    case 3:
	sboxNombreIterations->setEnabled(true);
	sboxTemps->setEnabled(false);
	sboxSigma->setEnabled(false);
	break;

    case 5:
	sboxNombreIterations->setDisabled(true);
	sboxTemps->setDisabled(true);
	sboxSigma->setDisabled(true);
	break;

    case 6:
	sboxNombreIterations->setDisabled(true);
	sboxTemps->setDisabled(true);
	sboxSigma->setDisabled(true);
	break;

    default:
	QMessageBox::critical(this, "Erreur de sélection", "Erreur dans la sélection de l'opération.");
	break;
    }
}
