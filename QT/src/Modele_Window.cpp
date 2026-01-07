#include "../include/Modele_Window.h"
#include "../include/QT_General.h"
#include "../include/Train_Modele_Window.h"


QWidget* thickLine()
{
    QWidget* w = new QWidget();
    w->setFixedHeight(8);
    w->setStyleSheet("background-color: black;");
    return w;
}

void addIntField(QVBoxLayout* parent,  char* text, QLineEdit*& edit)
{
    QHBoxLayout* l = new QHBoxLayout();

    QLabel* label = new QLabel(text);
    label->setMinimumWidth(200);

    edit = new QLineEdit();
    edit->setValidator(new QIntValidator(0, 1000000, parent->parentWidget()));

    l->addWidget(label);
    l->addWidget(edit);

    parent->addLayout(l);
}

void addFloatField(QVBoxLayout* parent, const char* text, QLineEdit*& edit)
{
    QHBoxLayout* l = new QHBoxLayout();

    QLabel* label = new QLabel(text);
    label->setMinimumWidth(200);

    edit = new QLineEdit();
    edit->setValidator(new QDoubleValidator(0.0, 1000.0, 6, parent->parentWidget()));

    l->addWidget(label);
    l->addWidget(edit);

    parent->addLayout(l);
}

void addInfoField(QVBoxLayout* parent,  char* text)
{
    QHBoxLayout* l = new QHBoxLayout();

    QLabel* label = new QLabel("Dim input:");
    label->setMinimumWidth(200);

    QLabel* info = new QLabel(text);
    info->setStyleSheet("color: gray; font-style: italic;");
    info->setWordWrap(true);

    l->addWidget(label);
    l->addWidget(info);

    parent->addLayout(l);
}

QComboBox* addComboField(QVBoxLayout* layout, const char* text)
{
    QHBoxLayout* l = new QHBoxLayout();
    QLabel* label = new QLabel(text);
    label->setMinimumWidth(220);

    QComboBox* combo = new QComboBox();

    l->addWidget(label);
    l->addWidget(combo);
    layout->addLayout(l);

    return combo;
}

void addLayerBlock(struct Modele_Window* MW, int idx)
{
    QLabel* title = new QLabel(QString("Layer %1").arg(idx + 1));
    title->setStyleSheet("font-size:18px; font-weight:bold;");
    MW->layersLayout->addWidget(title);

    MW->layers_ui[idx].type_module = addComboField(MW->layersLayout, "Type module:");
    MW->layers_ui[idx].type_module->addItem("Linear", 0);
    MW->layers_ui[idx].type_module->addItem("RNN", 1);
    MW->layers_ui[idx].type_module->addItem("GRU", 2);
    MW->layers_ui[idx].type_module->addItem("LSTM", 3);

    MW->layers_ui[idx].special = addComboField(MW->layersLayout, "Special:");
    MW->layers_ui[idx].special->addItem("None", 0);
    MW->layers_ui[idx].special->addItem("Sequence to Linear", 1);

    /* ===== DIM INPUT ===== */
    if (idx == 0)
    {
        MW->layers_ui[idx].dim_input = nullptr;

        addInfoField(MW->layersLayout, "First layer input dimension is defined by the number of features.");
    }
    else
    {
        addIntField(MW->layersLayout, "Dim input:", MW->layers_ui[idx].dim_input);
    }

/* ===== DIM OUTPUT ===== */
    if (idx == MW->MV.nb_layer - 1)
    {
        MW->layers_ui[idx].dim_output = nullptr;

        QHBoxLayout* l = new QHBoxLayout();

        QLabel* label = new QLabel("Dim output:");
        label->setMinimumWidth(220);

        
        QLabel* info = new QLabel("Last layer output dimension is defined by the target dimention");
        info->setStyleSheet("color: gray; font-style: italic;");
        info->setWordWrap(true);

        l->addWidget(label);
        l->addWidget(info, 1); // prend l’espace restant
        MW->layersLayout->addLayout(l);
    }
    else
    {
        addIntField(MW->layersLayout, "Dim output:", MW->layers_ui[idx].dim_output);
    }


    MW->layers_ui[idx].activation = addComboField(MW->layersLayout, "Activation function:");
    MW->layers_ui[idx].activation->addItem("None", 0);
    MW->layers_ui[idx].activation->addItem("ReLU", 1);
    MW->layers_ui[idx].activation->addItem("Tanh", 2);
    MW->layers_ui[idx].activation->addItem("Sigmoid", 3);
    MW->layers_ui[idx].activation->addItem("LeakyReLU", 4);

    MW->layers_ui[idx].bias = addComboField(MW->layersLayout, "Bias:");

    MW->layers_ui[idx].bias->addItem("No", 0);
    MW->layers_ui[idx].bias->addItem("Yes", 1);

    MW->layersLayout->addWidget(thickLine());
}

void create_modele_window(struct Modele_Window* MW, struct Train_Modele_Window* TMW)
{
    MW->window = new QWidget();
    MW->window->setWindowTitle("Modele Window");

    QVBoxLayout* mainLayout = new QVBoxLayout(MW->window);
    mainLayout->setContentsMargins(100, 50, 100, 60);
    mainLayout->setSpacing(30);

    /* ===== TITRE ===== */
    QLabel* title = new QLabel(QString("Modèle %1").arg(TMW->ctx.currentIndex + 1));
    QFont f = title->font();
    f.setPointSize(30);
    f.setBold(true);
    title->setFont(f);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    mainLayout->addSpacing(20);

    /* ===== LEARNING RATE ===== */
    QHBoxLayout* l = new QHBoxLayout();
    QLabel* label = new QLabel("Learning rate:");
    label->setMinimumWidth(220);

    MW->spinLearningRate = new QDoubleSpinBox();
    MW->spinLearningRate->setLocale(QLocale::C);   // force le point
    MW->spinLearningRate->setRange(0.0, 10.0);
    MW->spinLearningRate->setDecimals(5);
    MW->spinLearningRate->setSingleStep(0.001);
    MW->spinLearningRate->setValue(0.001);

    l->addWidget(label);
    l->addWidget(MW->spinLearningRate);
    mainLayout->addLayout(l);     

    addIntField(mainLayout, "Nb epoch:", MW->editEpoch);
    addIntField(mainLayout, "Nb layers:", MW->editNbLayer);
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Cost function:");
        label->setMinimumWidth(220);

        MW->comboCostFunction = new QComboBox();

        MW->comboCostFunction->addItem("Mean Squared Error (MSE) [continuous tensor]", 0);
        MW->comboCostFunction->addItem("Mean Absolute Error (L1) [continuous tensor]", 1);
        MW->comboCostFunction->addItem("Smooth L1 / Huber [continuous tensor]", 2);
        MW->comboCostFunction->addItem("Cross Entropy [class indices]", 3);
        MW->comboCostFunction->addItem("Binary Cross-Entropy with logits [binary tensor]", 4);
        MW->comboCostFunction->addItem("Squence cross entropy [class indices]", 5);

        MW->comboCostFunction->setCurrentIndex(0);

        l->addWidget(label);
        l->addWidget(MW->comboCostFunction);
        mainLayout->addLayout(l);
    }

    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Optimizer:");
        label->setMinimumWidth(220);

        MW->comboOptimizer = new QComboBox();

        MW->comboOptimizer->addItem("SGD", 0);
        MW->comboOptimizer->addItem("Adam", 1);
        MW->comboOptimizer->addItem("AdamW", 2);
        MW->comboOptimizer->addItem("RMSProp", 3);
        MW->comboOptimizer->addItem("Adagrad", 4);
        MW->comboOptimizer->addItem("LBFGS", 5);
        MW->comboOptimizer->setCurrentIndex(2); 

        l->addWidget(label);
        l->addWidget(MW->comboOptimizer);
        mainLayout->addLayout(l);
    }
    

    /* ===== BOUTON VALIDER ===== */
    QHBoxLayout* bottom = new QHBoxLayout();
    bottom->addStretch();

    MW->valider_init = new QPushButton("Valider");
    MW->valider_init->setMinimumSize(180, 40);
    bottom->addWidget(MW->valider_init);

    mainLayout->addLayout(bottom);

    /* ===== TRAIT NOIR ===== */
    mainLayout->addWidget(thickLine());

    /* ===== SCROLL AREA ===== */
    MW->layersWidget = new QWidget();
    MW->layersLayout = new QVBoxLayout(MW->layersWidget);
    MW->layersLayout->setSpacing(30);

    MW->scrollArea = new QScrollArea();
    MW->scrollArea->setWidgetResizable(true);
    MW->scrollArea->setWidget(MW->layersWidget);

    mainLayout->addWidget(MW->scrollArea);

    /* ===== LOGIQUE VALIDER ===== */
    QObject::connect(MW->valider_init, &QPushButton::clicked, [=]() 
    {

        MW->MV.learning_rate = MW->spinLearningRate->text().toFloat();
        MW->MV.nb_epoch = MW->editEpoch->text().toInt();
        MW->MV.nb_layer = MW->editNbLayer->text().toInt();
        MW->MV.cost_function = MW->comboCostFunction->currentData().toInt();
        MW->MV.optimizer     = MW->comboOptimizer->currentData().toInt();
        /* nettoyage UI précédente */
        QLayoutItem* item;
        while ((item = MW->layersLayout->takeAt(0)) != nullptr)
        {
            delete item->widget();
            delete item;
        }

        /* allocation C */
        MW->MV.type_modules   = (int*)malloc(MW->MV.nb_layer * sizeof(int));
        MW->MV.special        = (int*)malloc(MW->MV.nb_layer * sizeof(int));
        MW->MV.dim_input      = (int*)malloc(MW->MV.nb_layer * sizeof(int));
        MW->MV.dim_outpout    = (int*)malloc(MW->MV.nb_layer * sizeof(int));
        MW->MV.activ_function = (int*)malloc(MW->MV.nb_layer * sizeof(int));
        MW->MV.bias           = (int*)malloc(MW->MV.nb_layer * sizeof(int));
        MW->layers_ui = (Layer_UI*)malloc(MW->MV.nb_layer * sizeof(Layer_UI));

        /* création dynamique UI */
        for (int i = 0; i < MW->MV.nb_layer; i++) {addLayerBlock(MW, i);}

        MW->valider_final = new QPushButton("Valider");
        MW->valider_final->setMinimumSize(180, 40);

        // Layout séparé pour le placer à droite
        QHBoxLayout* finalLayout = new QHBoxLayout();
        finalLayout->addStretch();
        finalLayout->addWidget(MW->valider_final);
        mainLayout->addLayout(finalLayout);

        QObject::connect(MW->valider_final, &QPushButton::clicked, [=]() 
        {
            for (int i = 0; i < MW->MV.nb_layer; ++i)
            {
                MW->MV.type_modules[i] = MW->layers_ui[i].type_module->currentData().toInt();
                MW->MV.special[i] = MW->layers_ui[i].special->currentData().toInt();

                if (i == 0) MW->MV.dim_input[i] = 0;
                else MW->MV.dim_input[i] = MW->layers_ui[i].dim_input->text().toInt();

                if (i == MW->MV.nb_layer - 1) MW->MV.dim_outpout[i] = 0;
                else MW->MV.dim_outpout[i] = MW->layers_ui[i].dim_output->text().toInt();

                MW->MV.activ_function[i] = MW->layers_ui[i].activation->currentData().toInt();
                MW->MV.bias[i] = MW->layers_ui[i].bias->currentData().toInt();
            }

            if (TMW->ctx.currentIndex == TMW->ctx.totalModels - 1)
            {
               
                write_train_config(&(TMW->TMV), &(TMW->ctx));

                
                Completion_Window* CW = new Completion_Window;
                Completion_Window_Create(CW, 1); // 1 = train

               
                QObject::connect(CW->btnOk, &QPushButton::clicked, [MW, TMW, CW]() 
                {
                    MW->window->deleteLater();
                    clean_train_modele (TMW);
                    delete CW;     // 
                    
                });
            }
            else
            {
                MW->window->deleteLater();
            }
        });
    });

    MW->window->showMaximized();
}