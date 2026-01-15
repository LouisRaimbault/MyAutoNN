#include "../include/Rdmgen_Window.h"
#include "../include/QT_General.h"
#include "../include/Generation_Random_Modele.h"


void fill_Rdm_Modele_Info(struct Rdmgen_Modele_Window* RMW)
{
    struct Rdm_Modele_Window* RW = RMW->RW;
    struct Rdm_Modele_Info* RMI = &RW->RMI;

    /* ===== RANGES ===== */
    RW->RMI.range_learning_rate[0] = (float)RW->spin_lr_min->value();
    RW->RMI.range_learning_rate[1] = (float)RW->spin_lr_max->value();

    RW->RMI.range_nb_epoch[0] = RW->spin_nb_epoch_min->value();
    RW->RMI.range_nb_epoch[1] = RW->spin_nb_epoch_max->value();

    RW->RMI.range_nb_layers[0] = RW->spin_nb_layers_min->value();
    RW->RMI.range_nb_layers[1] = RW->spin_nb_layers_max->value();

    RW->RMI.range_dim[0] = RW->spin_dim_min->value();
    RW->RMI.range_dim[1] = RW->spin_dim_max->value();

    /* ===== COST FUNCTIONS ===== */
    {
        QList<QListWidgetItem*> selected = RW->list_cost_function->selectedItems();
        RW->RMI.nb_choices_cost_function = selected.size();

        if (RW->RMI.nb_choices_cost_function > 0)
            {
                RW->RMI.choices_cost_function = (int*)malloc(sizeof(int) * RW->RMI.nb_choices_cost_function);

                for (int i = 0; i < selected.size(); ++i)
                    {
                            RW->RMI.choices_cost_function[i] = RW->list_cost_function->row(selected[i]);
                            std::cout << "RW->RMI.choices_cost_function[i] = " << RW->RMI.choices_cost_function[i] << "\n";
                    }
            }
        else
            {
                RW->RMI.choices_cost_function = NULL;
            }
    }

    /* ===== OPTIMIZERS ===== */
    {
        QList<QListWidgetItem*> selected = RW->list_optimizer->selectedItems();
        RW->RMI.nb_choices_cost_optimizer = selected.size();

        if (RW->RMI.nb_choices_cost_optimizer > 0)
            {
                RW->RMI.choices_optimizer = (int*)malloc(sizeof(int) * RW->RMI.nb_choices_cost_optimizer);

                for (int i = 0; i < selected.size(); ++i)
                    {
                        RW->RMI.choices_optimizer[i] = RW->list_optimizer->row(selected[i]);
                    }
            }
        else
            {
                RW->RMI.choices_optimizer = NULL;
            }
    }

    /* ===== MODULES ===== */
    {
        QList<QListWidgetItem*> selected = RW->list_module->selectedItems();
        RW->RMI.nb_choices_module = selected.size();

        if (RW->RMI.nb_choices_module > 0)
            {
                RW->RMI.choices_module = (int*)malloc(sizeof(int) * RW->RMI.nb_choices_module);

                for (int i = 0; i < selected.size(); ++i)
                    {
                        RW->RMI.choices_module[i] = RW->list_module->row(selected[i]);

                    }
            }
        else
            {
                RW->RMI.choices_module = NULL;
            }
    }

    /* ===== ACTIVATIONS FUNCTIONS ===== */
    {
        QList<QListWidgetItem*> selected = RW->list_activation_function->selectedItems();
        RW->RMI.nb_choices_activation_function = selected.size();

        if (RW->RMI.nb_choices_activation_function > 0)
            {
                RW->RMI.choices_activation_function = (int*)malloc(sizeof(int) * RW->RMI.nb_choices_activation_function);

                for (int i = 0; i < selected.size(); ++i)
                    {
                            RW->RMI.choices_activation_function[i] = RW->list_activation_function->row(selected[i]);
                            std::cout << "RW->RMI.choices_activation_function[i] = " << RW->RMI.choices_activation_function[i] << "\n";
                    }
            }
        else
            {
                RW->RMI.choices_activation_function = NULL;
            }
    }

    /* ===== proba and evolution of special ===== */
    RW->RMI.add_bias_proba = RW->spin_add_bias_proba->value();

    /* ===== ADD BIAS PROBABILITY ===== */
    RW->RMI.add_bias_proba = RW->spin_add_bias_proba->value();

    /* ===== ACTIVATION LAST LAYER ===== */
    RW->RMI.activ_last_layers = RW->group_activ_last_layer->checkedId();
}


void create_rdm_Modele(struct Rdmgen_Modele_Window* RMW)
{
    struct Rdm_Modele_Window* RW = new struct Rdm_Modele_Window;
    RMW->RW = RW;
    RW->window = new QWidget();
    RW->window->setWindowTitle("Random Model Configuration");

    // === Layout principal ===
    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(50, 20, 50, 20);
    mainLayout->setSpacing(20);

    // ===== TITRE =====
    QLabel* title = new QLabel("Random Model Configuration");
    QFont titleFont = title->font();
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);
    mainLayout->addSpacing(20);

    // ===== RANGES =====
    // Learning rate (double)
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Range Learning rate:");
        label->setMinimumWidth(220);

        QLabel* labelMin = new QLabel("min");
        QLabel* labelMax = new QLabel("max");

        RW->spin_lr_min = new QDoubleSpinBox();
        RW->spin_lr_min->setDecimals(5);
        RW->spin_lr_min->setRange(0.0, 10.0);
        RW->spin_lr_min->setSingleStep(0.0001);

        RW->spin_lr_max = new QDoubleSpinBox();
        RW->spin_lr_max->setDecimals(5);
        RW->spin_lr_max->setRange(0.0, 10.0);
        RW->spin_lr_max->setSingleStep(0.0001);

        l->addWidget(label);
        l->addWidget(labelMin);
        l->addWidget(RW->spin_lr_min);
        l->addWidget(labelMax);
        l->addWidget(RW->spin_lr_max);
        mainLayout->addLayout(l);
    }

    // Number of epochs (int)
    {
        QHBoxLayout* l = new QHBoxLayout();

        QLabel* label = new QLabel("Range Number of epochs:");
        label->setMinimumWidth(220);

        QLabel* labelMin = new QLabel("min");
        QLabel* labelMax = new QLabel("max");

        RW->spin_nb_epoch_min = new QSpinBox();
        RW->spin_nb_epoch_min->setRange(0, 100000);

        RW->spin_nb_epoch_max = new QSpinBox();
        RW->spin_nb_epoch_max->setRange(0, 100000);

        l->addWidget(label);
        l->addWidget(labelMin);
        l->addWidget(RW->spin_nb_epoch_min);
        l->addWidget(labelMax);
        l->addWidget(RW->spin_nb_epoch_max);

        mainLayout->addLayout(l);
    }

    // Number of layers (int)
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Range Number of layers:");
        label->setMinimumWidth(220);

        QLabel* labelMin = new QLabel("min");
        QLabel* labelMax = new QLabel("max");

        RW->spin_nb_layers_min = new QSpinBox();
        RW->spin_nb_layers_min->setRange(0, 100000);

        RW->spin_nb_layers_max = new QSpinBox();
        RW->spin_nb_layers_max->setRange(0, 100000);

        l->addWidget(label);
        l->addWidget(labelMin);
        l->addWidget(RW->spin_nb_layers_min);
        l->addWidget(labelMax);
        l->addWidget(RW->spin_nb_layers_max);
        mainLayout->addLayout(l);
    }

    // Layer dimension (int)
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Range Layer dimension:");
        label->setMinimumWidth(220);

        QLabel* labelMin = new QLabel("min");
        QLabel* labelMax = new QLabel("max");

        RW->spin_dim_min = new QSpinBox();
        RW->spin_dim_min->setRange(0, 100000);

        RW->spin_dim_max = new QSpinBox();
        RW->spin_dim_max->setRange(0, 100000);

        l->addWidget(label);
        l->addWidget(labelMin);
        l->addWidget(RW->spin_dim_min);
        l->addWidget(labelMax);
        l->addWidget(RW->spin_dim_max);
        mainLayout->addLayout(l);
    }

    // ===== MULTI-SELECTION LISTS =====
    // Cost Functions
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Cost Functions:");
        label->setMinimumWidth(220);

        RW->list_cost_function = new QListWidget();
        RW->list_cost_function->setSelectionMode(QAbstractItemView::MultiSelection);
        RW->list_cost_function->setMinimumHeight(100);

        RW->list_cost_function->addItem("Mean Squared Error (MSE) [continuous tensor]");
        RW->list_cost_function->addItem("Mean Absolute Error (L1) [continuous tensor]");
        RW->list_cost_function->addItem("Smooth L1 / Huber [continuous tensor]");
        RW->list_cost_function->addItem("Cross Entropy [class indices]");
        RW->list_cost_function->addItem("Binary Cross-Entropy with logits [binary tensor]");
        RW->list_cost_function->addItem("nll");
        RW->list_cost_function->addItem("Sequence cross entropy [class indices]");

        l->addWidget(label);
        l->addWidget(RW->list_cost_function, 1);
        mainLayout->addLayout(l);
    }

    // Optimizers
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Optimizers:");
        label->setMinimumWidth(220);

        RW->list_optimizer = new QListWidget();
        RW->list_optimizer->setSelectionMode(QAbstractItemView::MultiSelection);
        RW->list_optimizer->setMinimumHeight(100);

        RW->list_optimizer->addItem("SGD");
        RW->list_optimizer->addItem("Adam");
        RW->list_optimizer->addItem("AdamW");
        RW->list_optimizer->addItem("RMSProp");
        RW->list_optimizer->addItem("Adagrad");
        RW->list_optimizer->addItem("LBFGS");

        l->addWidget(label);
        l->addWidget(RW->list_optimizer, 1);
        mainLayout->addLayout(l);
    }

    // Modules
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Modules:");
        label->setMinimumWidth(220);

        RW->list_module = new QListWidget();
        RW->list_module->setSelectionMode(QAbstractItemView::MultiSelection);
        RW->list_module->setMinimumHeight(100);

        RW->list_module->addItem("Linear");
        RW->list_module->addItem("RNN");
        RW->list_module->addItem("GRU");
        RW->list_module->addItem("LSTM");

        l->addWidget(label);
        l->addWidget(RW->list_module, 1);
        mainLayout->addLayout(l);
    }

    // Activations (intermediate layers)
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Activations (intermediate layers):");
        label->setMinimumWidth(220);

        RW->list_activation_function = new QListWidget();
        RW->list_activation_function->setSelectionMode(QAbstractItemView::MultiSelection);
        RW->list_activation_function->setMinimumHeight(100);

        RW->list_activation_function->addItem("None");
        RW->list_activation_function->addItem("ReLU");
        RW->list_activation_function->addItem("Tanh");
        RW->list_activation_function->addItem("Sigmoid");
        RW->list_activation_function->addItem("LeakyReLU");

        l->addWidget(label);
        l->addWidget(RW->list_activation_function, 1);
        mainLayout->addLayout(l);
    }

        // Specials (intermediate layers)
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("special Use of Layer type");
        label->setMinimumWidth(220);

        RW->list_special_function = new QListWidget();
        RW->list_special_function->setSelectionMode(QAbstractItemView::MultiSelection);
        RW->list_special_function->setMinimumHeight(100);

        RW->list_special_function->addItem("None");
        RW->list_special_function->addItem("Sequence to linear");

        l->addWidget(label);
        l->addWidget(RW->list_special_function, 1);
        mainLayout->addLayout(l);
    }





    // use special (int)
    {
        // ===== USE SPECIAL ===
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("proba and evolution of special");
        label->setMinimumWidth(220);

        RW->spin_use_special_proba = new QSpinBox();
        RW->spin_use_special_proba->setRange(0, 100);
        RW->spin_use_special_proba->setSuffix(" (divided by 100)");
        RW->spin_use_special_proba->setValue(10);

        RW->spin_evo_special_proba = new QSpinBox();
        RW->spin_evo_special_proba->setRange(0, 100);
        RW->spin_evo_special_proba->setSuffix(" (divided by 100)");
        RW->spin_evo_special_proba->setValue(10);

        l->addWidget(label);
        l->addWidget(RW->spin_use_special_proba);
        l->addWidget(RW->spin_evo_special_proba);
        mainLayout->addLayout(l);
    }


    // Add bias probability
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Add bias probability:");
        label->setMinimumWidth(220);

        RW->spin_add_bias_proba = new QSpinBox();
        RW->spin_add_bias_proba->setRange(0, 100);
        RW->spin_add_bias_proba->setSuffix(" (divided by 100)");
        RW->spin_add_bias_proba->setValue(50);

        l->addWidget(label);
        l->addWidget(RW->spin_add_bias_proba);
        mainLayout->addLayout(l);
    }




    // Activation last layer
    {
        QHBoxLayout* l = new QHBoxLayout();
        QLabel* label = new QLabel("Activation last layer:");
        label->setMinimumWidth(220);

        RW->rb_activ_yes = new QRadioButton("Yes");
        RW->rb_activ_no  = new QRadioButton("No");
        RW->rb_activ_no->setChecked(true);

        RW->group_activ_last_layer = new QButtonGroup(RW->window);
        RW->group_activ_last_layer->addButton(RW->rb_activ_no, 0);
        RW->group_activ_last_layer->addButton(RW->rb_activ_yes, 1);

        l->addWidget(label);
        l->addWidget(RW->rb_activ_yes);
        l->addWidget(RW->rb_activ_no);
        mainLayout->addLayout(l);
    }


    // ===== VALIDATE BUTTON =====
    {
        QHBoxLayout* l = new QHBoxLayout();
        l->addStretch();  

        QPushButton* btn_validate = new QPushButton("Valider");
        btn_validate->setMinimumSize(160, 40);

        l->addWidget(btn_validate);
        mainLayout->addLayout(l);

        QObject::connect(btn_validate, &QPushButton::clicked, [RMW]()
        {
            fill_Rdm_Modele_Info(RMW);
            gen_random_modele (RMW);
            write_configfile_rdm_modele (RMW);
            std::cout << "Avant close \n";
            RMW->RW->window->close();   // pas close()
        });
    }


    // ===== SCROLL AREA =====
    QWidget* container = new QWidget();
    container->setLayout(mainLayout);

    QScrollArea* scroll = new QScrollArea();
    scroll->setWidgetResizable(true);
    scroll->setWidget(container);

    QVBoxLayout* windowLayout = new QVBoxLayout(RW->window);
    windowLayout->addWidget(scroll);

    RW->window->showMaximized();
}