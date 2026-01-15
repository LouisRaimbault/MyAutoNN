#include "../include/Rdmgen_Modele_Window.h"
#include "../include/QT_General.h"


void fill_RM (struct Rdmgen_Modele_Window* RMW)
{
    RMW->RM.path_database = strdup(RMW->editDatabase->text().toStdString().c_str());
    RMW->RM.target_model = RMW->targetModelGroup->checkedId();
    RMW->RM.is_modele_autoencodeur = RMW->autoencGroup->checkedId();
    RMW->RM.normalize_X = RMW->normXGroup->checkedId();
    RMW->RM.normalize_Y = RMW->normYGroup->checkedId();
    RMW->RM.nb_col_target = RMW->editNbColTarget->text().toInt();
    RMW->RM.ratio_train = RMW->spinRatioTrain->value();
    RMW->RM.nb_batch = RMW->editNbBatch->text().toInt();
    RMW->RM.size_batch = RMW->editSizeBatch->text().toInt();
    RMW->RM.size_seq = RMW->editSizeSeq->text().toInt();
    RMW->RM.nb_modele = RMW->editNbModele->text().toInt();
    RMW->RM.config_folder = strdup(RMW->editConfigFolder->text().toStdString().c_str());
    RMW->RM.config_filename = strdup(RMW->editConfigFilename->text().toStdString().c_str());
}



void Rdmgen_Modele_Window_Create(struct Rdmgen_Modele_Window* RMW)
{
    RMW->window = new QWidget();
    RMW->window->setWindowTitle("Train Modele");

    // === Layout principal (pour tous les widgets) ===
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(50, 20, 50, 20);
    mainLayout->setSpacing(25);

    /* ===== TITRE ===== */
    QLabel* title = new QLabel("Train Modele");
    QFont titleFont = title->font();
    titleFont.setPointSize(30);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);
    mainLayout->addSpacing(20);

    /* ===== DATABASE ===== */
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Path Database:");
        label->setMinimumWidth(220);

        RMW->editDatabase = new QLineEdit();
        RMW->editDatabase->setReadOnly(true);

        RMW->browseDatabase = new QPushButton("Browse");

        l->addWidget(label);
        l->addWidget(RMW->editDatabase, 1);
        l->addWidget(RMW->browseDatabase);

        mainLayout->addLayout(l);
    }

    /* ===== TARGET MODEL ===== */
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Random Generator Model:");
        label->setMinimumWidth(220);

        RMW->rbTargetNormal = new QRadioButton("Standard");
        RMW->rbTargetSequence = new QRadioButton("Sequence");
        RMW->rbTargetImages = new QRadioButton("Images");

        RMW->rbTargetNormal->setChecked(true);

        RMW->targetModelGroup = new QButtonGroup(RMW->window);
        RMW->targetModelGroup->addButton(RMW->rbTargetNormal, 0);
        RMW->targetModelGroup->addButton(RMW->rbTargetSequence, 1);
        RMW->targetModelGroup->addButton(RMW->rbTargetImages, 2);

        l->addWidget(label);
        l->addWidget(RMW->rbTargetNormal);
        l->addWidget(RMW->rbTargetSequence);
        l->addWidget(RMW->rbTargetImages);

        mainLayout->addLayout(l);
    }

    /* ===== AUTOENCODER ===== */
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Autoencoder:");
        label->setMinimumWidth(220);

        RMW->rbAutoencYes = new QRadioButton("Yes");
        RMW->rbAutoencNo = new QRadioButton("No");
        RMW->rbAutoencNo->setChecked(true);

        RMW->autoencGroup = new QButtonGroup(RMW->window);
        RMW->autoencGroup->addButton(RMW->rbAutoencNo, 0);
        RMW->autoencGroup->addButton(RMW->rbAutoencYes, 1);

        l->addWidget(label);
        l->addWidget(RMW->rbAutoencYes);
        l->addWidget(RMW->rbAutoencNo);

        mainLayout->addLayout(l);
    }

    /* ===== NORMALIZATION X / Y ===== */
    auto addYesNo = [&](const char* text, QButtonGroup*& group,
                        QRadioButton*& yes, QRadioButton*& no)
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel(text);
        label->setMinimumWidth(220);

        yes = new QRadioButton("Yes");
        no = new QRadioButton("No");
        no->setChecked(true);

        group = new QButtonGroup(RMW->window);
        group->addButton(no, 0);
        group->addButton(yes, 1);

        l->addWidget(label);
        l->addWidget(yes);
        l->addWidget(no);
        mainLayout->addLayout(l);
    };

    addYesNo("Normalize X:", RMW->normXGroup, RMW->rbNormXYes, RMW->rbNormXNo);
    addYesNo("Normalize Y:", RMW->normYGroup, RMW->rbNormYYes, RMW->rbNormYNo);

    /* ===== CHAMPS NUMÉRIQUES ===== */
    auto addIntField = [&](const char* text, QLineEdit*& edit)
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel(text);
        label->setMinimumWidth(220);

        edit = new QLineEdit();
        edit->setValidator(new QIntValidator(0, 100000, RMW->window));

        l->addWidget(label);
        l->addWidget(edit);
        mainLayout->addLayout(l);
    };

    addIntField("Nb last columns as target (0 if sequence) :", RMW->editNbColTarget);
    addIntField("Nb batch:", RMW->editNbBatch);
    addIntField("Size batch:", RMW->editSizeBatch);
    addIntField("Size sequence:", RMW->editSizeSeq);

    /* ===== RATIO TRAIN ===== */
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Ratio train:");
        label->setMinimumWidth(220);

        RMW->spinRatioTrain = new QDoubleSpinBox();
        RMW->spinRatioTrain->setLocale(QLocale::C);
        RMW->spinRatioTrain->setRange(0.0, 1.0);
        RMW->spinRatioTrain->setDecimals(3);
        RMW->spinRatioTrain->setSingleStep(0.01);
        RMW->spinRatioTrain->setValue(0.8);

        l->addWidget(label);
        l->addWidget(RMW->spinRatioTrain);
        mainLayout->addLayout(l);
    }

    /* ===== LIGNES ET NB MODELE ===== */
    QFrame* lineBefore = new QFrame();
    lineBefore->setFrameShape(QFrame::HLine);
    lineBefore->setFrameShadow(QFrame::Sunken);
    lineBefore->setLineWidth(4);
    lineBefore->setMinimumHeight(8);
    mainLayout->addWidget(lineBefore);

    addIntField("Nb modele:", RMW->editNbModele);

    QFrame* lineAfter = new QFrame();
    lineAfter->setFrameShape(QFrame::HLine);
    lineAfter->setFrameShadow(QFrame::Sunken);
    lineAfter->setLineWidth(4);
    lineAfter->setMinimumHeight(8);
    mainLayout->addWidget(lineAfter);

    /* ===== CONFIG FILE DESTINATION ===== */
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Destination folder of config file:");
        label->setMinimumWidth(220);

        RMW->editConfigFolder = new QLineEdit();
        RMW->editConfigFolder->setReadOnly(true);

        RMW->browseConfigFolder = new QPushButton("Browse");

        l->addWidget(label);
        l->addWidget(RMW->editConfigFolder, 1);
        l->addWidget(RMW->browseConfigFolder);

        mainLayout->addLayout(l);
    }

    /* ===== CONFIG FILE NAME ===== */
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Name of config file:");
        label->setMinimumWidth(220);

        RMW->editConfigFilename = new QLineEdit();
        RMW->editConfigFilename->setPlaceholderText("config.txt");

        l->addWidget(label);
        l->addWidget(RMW->editConfigFilename);
        mainLayout->addLayout(l);
    }

    /* ===== BOUTONS BAS ===== */
    mainLayout->addSpacing(20);  // espace avant boutons
    QHBoxLayout* bottom = new QHBoxLayout();

    RMW->btnStandard = new QPushButton("Standard");
    RMW->btnSequence = new QPushButton("Sequence");
    RMW->btnImage    = new QPushButton("Image");

    QFont btnFont;
    btnFont.setPointSize(16);
    btnFont.setBold(true);

    for (QPushButton* b : {RMW->btnStandard, RMW->btnSequence, RMW->btnImage})
    {
        b->setFont(btnFont);
        b->setMinimumSize(220, 60);
    }

    bottom->addWidget(RMW->btnStandard);
    bottom->addStretch();
    bottom->addWidget(RMW->btnSequence);
    bottom->addStretch();
    bottom->addWidget(RMW->btnImage);

    mainLayout->addLayout(bottom);

    /* ===== SCROLL AREA + LAYOUT FENÊTRE ===== */
    QWidget* container = new QWidget();
    container->setLayout(mainLayout);

    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setWidget(container);

    QVBoxLayout* windowLayout = new QVBoxLayout(RMW->window);
    windowLayout->addWidget(scroll);

    /* ===== CONNEXIONS ===== */
    QObject::connect(RMW->browseDatabase, &QPushButton::clicked, [=]() 
    {
        QString f = QFileDialog::getOpenFileName(RMW->window, "Select database");
        if (!f.isEmpty()) { RMW->editDatabase->setText(f); }
    });

    QObject::connect(RMW->browseConfigFolder, &QPushButton::clicked, [=]()
    {
        QString dir = QFileDialog::getExistingDirectory(RMW->window, "Select destination folder");
        if (!dir.isEmpty()) { RMW->editConfigFolder->setText(dir); }
    });

    QObject::connect(RMW->btnStandard, &QPushButton::clicked, [=]() 
    {
        fill_RM(RMW);
        RMW->window->close();
        create_rdm_Modele(RMW);
    });

    QObject::connect(RMW->btnSequence, &QPushButton::clicked, [=]() 
    {
        fill_RM(RMW);
        RMW->window->close();
        create_rdm_Modele(RMW);
    });

    QObject::connect(RMW->btnImage, &QPushButton::clicked, [=]() 
    {
        fill_RM(RMW);
        RMW->window->close();
        create_rdm_Modele(RMW);
    });

    RMW->window->showMaximized();
}