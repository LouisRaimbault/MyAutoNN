#include "../include/Train_Modele_Window.h"
#include "../include/QT_General.h"

void clean_train_modele (struct Train_Modele_Window* TMW)
{
    free (TMW->TMV.path_database);
    free (TMW->TMV.config_folder);
    free (TMW->TMV.config_filename);

    for (int i = 0; i < TMW->TMV.nb_modele; ++i)
        {
            free(TMW->ctx.tab_MW[i].MV.type_modules);
            free(TMW->ctx.tab_MW[i].MV.special);
            free(TMW->ctx.tab_MW[i].MV.dim_input);
            free(TMW->ctx.tab_MW[i].MV.dim_outpout);
            free(TMW->ctx.tab_MW[i].MV.activ_function);
            free(TMW->ctx.tab_MW[i].MV.bias);
        }
    delete [] TMW->ctx.tab_MW;
    delete TMW;

}

void fprintf_float_dot(FILE* f, const char* fmt, double x)
{
    char buf[64];

    snprintf(buf, sizeof(buf), fmt, x);

    for (char* p = buf; *p; ++p)
    {
        if (*p == ',') {*p = '.'; break;}
    }

    fprintf(f, "%s", buf);
}

void write_train_config(struct Train_Modele_Values* TMV, struct TrainContext* ctx)
{
    size_t len = strlen(TMV->config_folder) + 1 + strlen(TMV->config_filename) + 1; // "/" + '\0'

    char* filename = (char*)malloc(len * sizeof(char));
    snprintf(filename, len, "%s/%s", TMV->config_folder, TMV->config_filename);

    FILE* f = fopen(filename, "w");

    /* ================= DATA CONFIG ================= */
    fprintf(f, "+------+ Data Config +------+\n\n");

    fprintf(f, "path to file #%s,\n", TMV->path_database);
    fprintf(f, "Target model #%d,\n", TMV->target_model);
    fprintf(f, "Nombre de colonnes cibles #%d,\n", TMV->nb_col_target);
    fprintf(f, "is_modele_autoencodeur #%d,\n", TMV->is_modele_autoencodeur);
    fprintf(f, "Normalize X #%d,\n", TMV->normalize_X);
    fprintf(f, "Normalize Y #%d,\n", TMV->normalize_Y);
    fprintf(f, "ratio_train #");
    fprintf_float_dot(f, "%.3f", TMV->ratio_train);
    fprintf(f, ",\n\n");

    /* ================= BATCH CONFIG ================= */
    fprintf(f, "+------+ Batchs Config +------+\n\n");

    fprintf(f, "Nb_Batch #%d,\n", TMV->nb_batch);
    fprintf(f, "Size_Batch #%d,\n", TMV->size_batch);
    fprintf(f, "Size_Seq #%d,\n\n", TMV->size_seq);

    /* ================= ARCHITECTURE CONFIG ================= */
    fprintf(f, "+------+ Architecture Config +------+\n\n");
    fprintf(f, "Nb Modeles #%d,\n\n", ctx->totalModels);

    /* ================= MODELS ================= */
    for (int m = 0; m < ctx->totalModels; ++m)
    {
        struct Modele_Values* MV = &ctx->tab_MW[m].MV;

        fprintf(f, "+-+ Modele %d +-+\n\n", m + 1);
        fprintf(f, "nb_Layer #%d,\n\n", MV->nb_layer);

        for (int i = 0; i < MV->nb_layer; ++i)
        {
            if (i == 0) {fprintf(f, "+ Input Layer +\n");}
            else
                {
                    if (i == MV->nb_layer - 1) {fprintf(f, "+ Output Layer +\n");}
                    else {fprintf(f, "+ Hidden Layer %d +\n", i);}
                }

            fprintf(f, "type_module #%d,\n", MV->type_modules[i]);
            fprintf(f, "special #%d,\n", MV->special[i]);

            //if (i > 0) {fprintf(f, "dim input #%d,\n", MV->dim_input[i]);}

            if (i < MV->nb_layer - 1) {fprintf(f, "dim output #%d,\n", MV->dim_outpout[i]);}

            fprintf(f, "activ function #%d,\n", MV->activ_function[i]);
            fprintf(f, "bias #%d,\n\n", MV->bias[i]);
        }

        fprintf(f, "learning rate #");
        fprintf_float_dot(f, "%.6f", MV->learning_rate);
        fprintf(f, ",\n");
        fprintf(f, "optimizer #%d,\n", MV->optimizer);
        fprintf(f, "cost function #%d,\n", MV->cost_function);
        fprintf(f, "initialize weight #0,\n");
        fprintf(f, "nb_epch #%d,\n\n\n", MV->nb_epoch);
    }

    fprintf(f, "------------------------------------------------\n");

    fclose(f);
    free(filename);
}


void openNextModeleWindow(struct Train_Modele_Window* TMW) 
{
    if (TMW->ctx.currentIndex >= TMW->ctx.totalModels) 
    {
        // toutes les fenêtres ont été ouvertes et fermées
        return;
    }

    Modele_Window* MW = &(TMW->ctx.tab_MW[TMW->ctx.currentIndex]);
    create_modele_window(MW, TMW);

    // Quand la fenêtre est fermée, on passe à la suivante
    QObject::connect(MW->window, &QWidget::destroyed, [TMW]() 
    {
        TMW->ctx.currentIndex++;
        openNextModeleWindow(TMW);  // ouvre la fenêtre suivante
    });
}

void Train_Modele_Window_Create(struct Train_Modele_Window* TMW)
{
    TMW->window = new QWidget();
    TMW->window->setWindowTitle("Train Modele");

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

        TMW->editDatabase = new QLineEdit();
        TMW->editDatabase->setReadOnly(true);

        TMW->browseDatabase = new QPushButton("Browse");

        l->addWidget(label);
        l->addWidget(TMW->editDatabase, 1);
        l->addWidget(TMW->browseDatabase);

        mainLayout->addLayout(l);
    }

    /* ===== TARGET MODEL ===== */
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Target Model:");
        label->setMinimumWidth(220);

        TMW->rbTargetNormal = new QRadioButton("Normal");
        TMW->rbTargetSequence = new QRadioButton("Sequence");
        TMW->rbTargetImages = new QRadioButton("Images");

        TMW->rbTargetNormal->setChecked(true);

        TMW->targetModelGroup = new QButtonGroup(TMW->window);
        TMW->targetModelGroup->addButton(TMW->rbTargetNormal, 0);
        TMW->targetModelGroup->addButton(TMW->rbTargetSequence, 1);
        TMW->targetModelGroup->addButton(TMW->rbTargetImages, 2);

        l->addWidget(label);
        l->addWidget(TMW->rbTargetNormal);
        l->addWidget(TMW->rbTargetSequence);
        l->addWidget(TMW->rbTargetImages);

        mainLayout->addLayout(l);
    }

    /* ===== AUTOENCODER ===== */
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Autoencoder:");
        label->setMinimumWidth(220);

        TMW->rbAutoencYes = new QRadioButton("Yes");
        TMW->rbAutoencNo = new QRadioButton("No");
        TMW->rbAutoencNo->setChecked(true);

        TMW->autoencGroup = new QButtonGroup(TMW->window);
        TMW->autoencGroup->addButton(TMW->rbAutoencNo, 0);
        TMW->autoencGroup->addButton(TMW->rbAutoencYes, 1);

        l->addWidget(label);
        l->addWidget(TMW->rbAutoencYes);
        l->addWidget(TMW->rbAutoencNo);

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

        group = new QButtonGroup(TMW->window);
        group->addButton(no, 0);
        group->addButton(yes, 1);

        l->addWidget(label);
        l->addWidget(yes);
        l->addWidget(no);
        mainLayout->addLayout(l);
    };

    addYesNo("Normalize X:", TMW->normXGroup, TMW->rbNormXYes, TMW->rbNormXNo);
    addYesNo("Normalize Y:", TMW->normYGroup, TMW->rbNormYYes, TMW->rbNormYNo);

    /* ===== CHAMPS NUMÉRIQUES ===== */
    auto addIntField = [&](const char* text, QLineEdit*& edit)
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel(text);
        label->setMinimumWidth(220);

        edit = new QLineEdit();
        edit->setValidator(new QIntValidator(0, 100000, TMW->window));

        l->addWidget(label);
        l->addWidget(edit);
        mainLayout->addLayout(l);
    };

    addIntField("Nb last columns as target (0 if sequence) :", TMW->editNbColTarget);
    addIntField("Nb batch:", TMW->editNbBatch);
    addIntField("Size batch:", TMW->editSizeBatch);
    addIntField("Size sequence:", TMW->editSizeSeq);

    /* ===== RATIO TRAIN ===== */
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Ratio train:");
        label->setMinimumWidth(220);

        TMW->spinRatioTrain = new QDoubleSpinBox();
        TMW->spinRatioTrain->setLocale(QLocale::C);
        TMW->spinRatioTrain->setRange(0.0, 1.0);
        TMW->spinRatioTrain->setDecimals(3);
        TMW->spinRatioTrain->setSingleStep(0.01);
        TMW->spinRatioTrain->setValue(0.8);

        l->addWidget(label);
        l->addWidget(TMW->spinRatioTrain);
        mainLayout->addLayout(l);
    }

    /* ===== LIGNES ET NB MODELE ===== */
    QFrame* lineBefore = new QFrame();
    lineBefore->setFrameShape(QFrame::HLine);
    lineBefore->setFrameShadow(QFrame::Sunken);
    lineBefore->setLineWidth(4);
    lineBefore->setMinimumHeight(8);
    mainLayout->addWidget(lineBefore);

    addIntField("Nb modele:", TMW->editNbModele);

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

        TMW->editConfigFolder = new QLineEdit();
        TMW->editConfigFolder->setReadOnly(true);

        TMW->browseConfigFolder = new QPushButton("Browse");

        l->addWidget(label);
        l->addWidget(TMW->editConfigFolder, 1);
        l->addWidget(TMW->browseConfigFolder);

        mainLayout->addLayout(l);
    }

    /* ===== CONFIG FILE NAME ===== */
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Name of config file:");
        label->setMinimumWidth(220);

        TMW->editConfigFilename = new QLineEdit();
        TMW->editConfigFilename->setPlaceholderText("config.txt");

        l->addWidget(label);
        l->addWidget(TMW->editConfigFilename);
        mainLayout->addLayout(l);
    }

    /* ===== VALIDER ===== */
    mainLayout->addStretch(2);

    QHBoxLayout* bottom = new QHBoxLayout();
    bottom->addStretch();
    TMW->valider = new QPushButton("Valider");
    TMW->valider->setMinimumSize(180, 60);
    bottom->addWidget(TMW->valider);
    mainLayout->addLayout(bottom);

    /* ===== SCROLL AREA ===== */
    QWidget* container = new QWidget();
    container->setLayout(mainLayout);

    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setWidget(container);

    QVBoxLayout* windowLayout = new QVBoxLayout(TMW->window);
    windowLayout->addWidget(scroll);

    /* ===== CONNEXIONS ===== */
    QObject::connect(TMW->browseDatabase, &QPushButton::clicked, [=]() 
    {
        QString f = QFileDialog::getOpenFileName(TMW->window, "Select database");
        if (!f.isEmpty()) { TMW->editDatabase->setText(f); }
    });

    QObject::connect(TMW->browseConfigFolder, &QPushButton::clicked, [=]()
    {
        QString dir = QFileDialog::getExistingDirectory(TMW->window, "Select destination folder");
        if (!dir.isEmpty()) { TMW->editConfigFolder->setText(dir); }
    });

    QObject::connect(TMW->valider, &QPushButton::clicked, [=]()
    {
        TMW->TMV.path_database = strdup(TMW->editDatabase->text().toStdString().c_str());
        TMW->TMV.target_model = TMW->targetModelGroup->checkedId();
        TMW->TMV.is_modele_autoencodeur = TMW->autoencGroup->checkedId();
        TMW->TMV.normalize_X = TMW->normXGroup->checkedId();
        TMW->TMV.normalize_Y = TMW->normYGroup->checkedId();
        TMW->TMV.nb_col_target = TMW->editNbColTarget->text().toInt();
        TMW->TMV.ratio_train = TMW->spinRatioTrain->value();
        TMW->TMV.nb_batch = TMW->editNbBatch->text().toInt();
        TMW->TMV.size_batch = TMW->editSizeBatch->text().toInt();
        TMW->TMV.size_seq = TMW->editSizeSeq->text().toInt();
        TMW->TMV.nb_modele = TMW->editNbModele->text().toInt();
        TMW->TMV.config_folder = strdup(TMW->editConfigFolder->text().toStdString().c_str());
        TMW->TMV.config_filename = strdup(TMW->editConfigFilename->text().toStdString().c_str());

        TMW->ctx.tab_MW = new Modele_Window[TMW->TMV.nb_modele];
        TMW->ctx.currentIndex = 0;
        TMW->ctx.totalModels = TMW->TMV.nb_modele;

        TMW->window->close();
        openNextModeleWindow(TMW);
        
    });

    TMW->window->showMaximized();
}
