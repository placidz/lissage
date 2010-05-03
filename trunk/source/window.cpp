
#include "glwidget.h"
#include "glgraphique.h"
#include "window.h"

Window::Window()
{
    glWidgetOriginale = new GLWidget();
    glWidgetOriginale->setLineColor(0.0, 0.0, 1.0);
    glWidgetResultat = new GLWidget();
    glWidgetResultat->setLineColor(1.0, 0.0, 0.0);
    glWidgetGraphique = new GLGraphique();

    glWidgetOriginale->graphique = glWidgetGraphique;
    glWidgetResultat->graphique = glWidgetGraphique;

    glWidgetGraphique->imgInitiale = glWidgetOriginale;
    glWidgetGraphique->imgResultat = glWidgetResultat;

    glWidgetOriginale->miroir = glWidgetResultat;
    glWidgetResultat->miroir = glWidgetOriginale;

    comboBoxChoixEquation = new QComboBox;
    comboBoxChoixEquation->addItem("Equation de la Chaleur");
    comboBoxChoixEquation->addItem("Approche de Malik et Perona");
    comboBoxChoixEquation->addItem("Equation par courbure moyenne");
    comboBoxChoixEquation->addItem("Médian itéré");
    comboBoxChoixEquation->addItem("- - - - - - - - - - - - - - - - - - - - - - - - - -");
    comboBoxChoixEquation->addItem("Bruitage Uniforme");
    comboBoxChoixEquation->addItem("Bruitage Impulsionnel");

    buttonLancerEquation = new QPushButton("Appliquer le filtre");
    buttonLancerEquation->setFont(QFont("", 9, QFont::Bold));
    buttonLancerEquation->setFixedWidth(150);
    buttonCopieImgOriginale= new QPushButton("Copier Image : Initiale >> Résultat");
    buttonCopieImgOriginale->setFixedWidth(220);
    buttonCopieImgResultat = new QPushButton("Copier Image : Initiale << Résultat");
    buttonCopieImgResultat->setFixedWidth(220);
    buttonChargerImage = new QPushButton("Ouvrir une image...");
    buttonChargerImage->setFixedWidth(150);
    buttonSauverResultat = new QPushButton("Sauvegarder le résultat");
    buttonSauverResultat->setFixedWidth(180);

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

    textSauvegarde = new QLineEdit;
    textSauvegarde->setFixedWidth(200);
    textSauvegarde->setText("nomFichierSauvegarde");

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
    connect(this, SIGNAL(signal_ChargerImage(QString)), glWidgetGraphique, SLOT(updateGraphique()));
    connect(buttonLancerEquation, SIGNAL(clicked()), glWidgetGraphique, SLOT(updateGraphique()));

    connect(buttonChargerImage, SIGNAL(clicked()), this, SLOT(slot_openFileDialog()));
    connect(comboBoxChoixEquation, SIGNAL(currentIndexChanged(int)), this, SLOT(slot_comboBoxChanged(int)));
    connect(buttonSauverResultat, SIGNAL(clicked()), this, SLOT(slot_buttonSauvegarder()));
    connect(this, SIGNAL(signal_SauverImage(QString,Image*)), this, SLOT(slot_sauvegarderImage(QString,Image*)));

    QVBoxLayout *mainLayout = new QVBoxLayout;
    QFormLayout *formLayout = new QFormLayout;

    mainLayout->addWidget(buttonChargerImage);
    mainLayout->setAlignment(buttonChargerImage, Qt::AlignCenter);
    mainLayout->addSpacing(20);

    mainLayout->addWidget(comboBoxChoixEquation, Qt::AlignCenter);
    mainLayout->addWidget(buttonCopieImgOriginale);
    mainLayout->setAlignment(buttonCopieImgOriginale, Qt::AlignHCenter);
    mainLayout->addWidget(buttonCopieImgResultat);
    mainLayout->setAlignment(buttonCopieImgResultat, Qt::AlignHCenter);

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

    mainLayout->addWidget(buttonLancerEquation);
    mainLayout->setAlignment(buttonLancerEquation, Qt::AlignCenter);

    mainLayout->addSpacing(30);

    mainLayout->addWidget(textSauvegarde);
    mainLayout->setAlignment(textSauvegarde, Qt::AlignHCenter);

    mainLayout->addWidget(buttonSauverResultat);
    mainLayout->setAlignment(buttonSauverResultat, Qt::AlignHCenter); 

    setLayout(mainLayout);

    setWindowTitle(tr("Methodes de debruitage par equations de diffusion"));

    setFixedSize(275, 450);
    move(5, 200);

    glWidgetOriginale->setWindowTitle(tr("Image Initiale"));
    glWidgetOriginale->show();
    glWidgetOriginale->move(300, 200);

    glWidgetResultat->setWindowTitle(tr("Image Resultat"));
    glWidgetResultat->show();
    glWidgetResultat->move(600, 200);

    glWidgetGraphique->setWindowTitle(tr("Vue en coupe"));
    glWidgetGraphique->show();
    glWidgetGraphique->move(300, 500);
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
	printf("Approche de Malik et Perona\n");
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
	emit signal_BruitageUniforme(&glWidgetOriginale->image, &glWidgetResultat->image, sboxSigma->value());
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
    glWidgetGraphique->isWindowOpened = false;
    glWidgetOriginale->close();
    glWidgetResultat->close();
    glWidgetGraphique->close();
    event->accept();
}

void Window::slot_comboBoxChanged(int val)
{
    switch(val)
    {
    case 0:
	sboxNombreIterations->setEnabled(true);
	sboxTemps->setEnabled(true);
	sboxSigma->setEnabled(false);
	break;

    case 1:
	sboxNombreIterations->setEnabled(true);
	sboxTemps->setEnabled(true);
	sboxSigma->setEnabled(true);
	break;

    case 2:
	sboxNombreIterations->setEnabled(true);
	sboxTemps->setEnabled(true);
	sboxSigma->setEnabled(false);
	break;

    case 3:
	sboxNombreIterations->setEnabled(true);
	sboxTemps->setEnabled(false);
	sboxSigma->setEnabled(false);
	break;

    case 5:
	sboxNombreIterations->setEnabled(false);
	sboxTemps->setEnabled(false);
	sboxSigma->setEnabled(true);
	break;

    case 6:
	sboxNombreIterations->setEnabled(false);
	sboxTemps->setEnabled(false);
	sboxSigma->setEnabled(false);
	break;

    default:
	QMessageBox::critical(this, "Erreur de sélection", "Erreur dans la sélection de l'opération.");
	break;
    }
}

void Window::slot_buttonSauvegarder()
{
    if (textSauvegarde->text() == "")
	textSauvegarde->setText("imageResultat");

    textSauvegarde->setText(textSauvegarde->text() + ".pgm");
    emit signal_SauverImage(textSauvegarde->text(), &glWidgetResultat->image);
}

void Window::slot_sauvegarderImage(QString sFileName, Image *in)
{
    EcrireImage(sFileName.toStdString().c_str(), in);
    QMessageBox::information(this, "Information", "L'image résultat a bien été sauvegardé sous le nom " + sFileName + " à la racine du projet.");
}
