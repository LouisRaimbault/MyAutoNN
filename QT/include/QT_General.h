#ifndef QT_GENERAL
#define QT_GENERAL

#include <stdio.h>
#include <string>
#include <iostream> 
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QButtonGroup>
#include <QComboBox>
#include <QFileDialog>
#include <QFrame>
#include <QDoubleValidator>
#include <QFont>
#include <QScrollArea>
#include <QDoubleSpinBox>
#include <QGuiApplication>
#include <QListWidget>      // <-- pour QListWidget
#include <QAbstractItemView> // <-- pour MultiSelection
#include <QScreen>
#include <QObject>




struct Welcome_Window
{
    QWidget* window;
    QLabel* description;
    QPushButton* trainButton;
    QPushButton* applyButton;
    QPushButton* rdmgenButton;
};

struct Completion_Window
{
    QWidget* window;
    QLabel* description;
    QPushButton* btnOk;
};


struct Modele_Values
{
    float learning_rate;
    int optimizer;
    int cost_function;
    int nb_epoch;
    int nb_layer;

    int* type_modules;
    int* special;
    int* dim_input;
    int* dim_outpout;
    int* activ_function;
    int* bias;
};


struct Layer_UI
{
    QComboBox* type_module;
    QComboBox* special;
    QLineEdit* dim_input;
    QLabel*    dim_input_info; 
    QLineEdit* dim_output;
    QComboBox* activation;
    QComboBox* bias;
};


struct Modele_Window
{
    struct Modele_Values MV;
    QWidget* window;

    /* ===== paramètres globaux ===== */


    /* ===== widgets globaux ===== */
    QDoubleSpinBox* spinLearningRate;
    QLineEdit* editEpoch;
    QLineEdit* editNbLayer;
    QComboBox* comboOptimizer;
    QComboBox* comboCostFunction;
    QPushButton* valider_init;

    /* ===== zone scroll ===== */
    QScrollArea* scrollArea;
    QWidget* layersWidget;
    QVBoxLayout* layersLayout;
    Layer_UI* layers_ui;

    /* ===== valider_final ===== */
    QPushButton* valider_final;

    /* ===== data dynamique ===== */

};


struct TrainContext 
{
    Modele_Window* tab_MW;
    int currentIndex;
    int totalModels;
};


struct Train_Modele_Values
{

    char* path_database;

    int target_model;             // 1 normal, 2 sequence, 3 images
    int nb_col_target;            // int
    int is_modele_autoencodeur;   // 0 non, 1 oui
    int normalize_X;              // 0 non, 1 oui
    int normalize_Y;              // 0 non, 1 oui

    float ratio_train;            // % train (float)
    int nb_batch;
    int size_batch;
    int size_seq;
    int nb_modele;

    char* config_folder;
    char* config_filename;
};



struct Train_Modele_Window
{
    struct Train_Modele_Values TMV;
    struct TrainContext ctx;

    QWidget* window;

    // Database
    QPushButton* browseDatabase;
    QLineEdit* editDatabase;

    // Target model
    QButtonGroup* targetModelGroup;
    QRadioButton* rbTargetNormal;
    QRadioButton* rbTargetSequence;
    QRadioButton* rbTargetImages;

    // Autoencoder
    QButtonGroup* autoencGroup;
    QRadioButton* rbAutoencYes;
    QRadioButton* rbAutoencNo;

    // Normalize X
    QButtonGroup* normXGroup;
    QRadioButton* rbNormXYes;
    QRadioButton* rbNormXNo;

    // Normalize Y
    QButtonGroup* normYGroup;
    QRadioButton* rbNormYYes;
    QRadioButton* rbNormYNo;

    // Champs numériques
    QLineEdit* editNbColTarget;
    QDoubleSpinBox* spinRatioTrain;
    QLineEdit* editNbBatch;
    QLineEdit* editSizeBatch;
    QLineEdit* editSizeSeq;
    QLineEdit* editNbModele;

    // Path and name file of the configuration 
    QPushButton* browseConfigFolder;
    QLineEdit* editConfigFolder;
    QLineEdit* editConfigFilename;


    // Validation
    QPushButton* valider;
};

struct Apply_Modele_Values
{
    char* txtpathbase;
    char* txtpathresbase;
    char* txtpathmodel;
    char* txtdestfolder;            
    char* txtconfigname;  

};

struct Apply_Modele_Window
{
    struct Apply_Modele_Values AMV;
    
    QWidget* window;

    QPushButton* pathbaseButton;
    QPushButton* pathresbaseButton;
    QPushButton* pathmodelButton;
    QPushButton* pathdestButton;    
    QPushButton* valider;          

    QLineEdit* editbase;
    QLineEdit* editresbase;
    QLineEdit* editmodel;
    QLineEdit* editdestfolder;     
    QLineEdit* editconfigname;      
};


struct Rdm_Modele
{
    float learning_rate;
    int nb_epoch;
    int optimizer;
    int cost_function;
    int nb_layers;
    int* dim_in;
    int* dim_out;
    int* bias;
    int* module;
    int* activation_function;
    int* special;
};


struct Rdm_Modele_Info
{
    int is_modele_autoencodeur;
    float range_learning_rate [2];
    int range_nb_epoch [2];
    int range_nb_layers [2];
    int* choices_cost_function;
    int nb_choices_cost_function;
    int* choices_optimizer;
    int nb_choices_cost_optimizer;
    int add_bias_proba;
    int activ_last_layers;
    int* choices_module;
    int nb_choices_module;
    int* choices_activation_function;
    int nb_choices_activation_function;
    int range_dim [2];
    int pct_break_sequence;
    int evo_break_sequence;
};


struct Rdm_Modele_Window
{
    struct Rdm_Modele_Info RMI;
    struct Rdm_Modele* tab_modele;
    QWidget* window;

    // ===== RANGES =====
    QDoubleSpinBox* spin_lr_min;
    QDoubleSpinBox* spin_lr_max;
    QSpinBox* spin_nb_epoch_min;
    QSpinBox* spin_nb_epoch_max;
    QSpinBox* spin_nb_layers_min;
    QSpinBox* spin_nb_layers_max;
    QSpinBox* spin_dim_min;
    QSpinBox* spin_dim_max;

    // ===== MULTI-SELECTION LISTS =====
    QListWidget* list_cost_function;  // multi-selection
    QListWidget* list_optimizer;      // multi-selection
    QListWidget* list_module;         // multi-selection
    QListWidget* list_activation_function;     // multi-selection (couches intermédiaires)
    QListWidget* list_special_function;     // multi-selection (couches intermédiaires)

    // ===== ADD BIAS =====
    QSpinBox* spin_add_bias_proba;

    // ===== USE SPECIAL ===
    QSpinBox* spin_use_special_proba;
    QSpinBox* spin_evo_special_proba;


    // ===== ACTIVATION LAST LAYER =====
    QButtonGroup* group_activ_last_layer;
    QRadioButton* rb_activ_yes;
    QRadioButton* rb_activ_no;

    QPushButton* btn_validate;
};






struct Rdmgen_Modele
{
    char* path_database;

    int target_model;             // 1 normal, 2 sequence, 3 images
    int nb_col_target;            // int
    int is_modele_autoencodeur;   // 0 non, 1 oui
    int normalize_X;              // 0 non, 1 oui
    int normalize_Y;              // 0 non, 1 oui

    float ratio_train;            // % train (float)
    int nb_batch;
    int size_batch;
    int size_seq;
    int nb_modele;

    char* config_folder;
    char* config_filename;
};




struct Rdmgen_Modele_Window
{
    struct Rdmgen_Modele RM;
    struct Rdm_Modele_Window* RW;

    QWidget* window;

    // Database
    QPushButton* browseDatabase;
    QLineEdit* editDatabase;

    // Target model
    QButtonGroup* targetModelGroup;
    QRadioButton* rbTargetNormal;
    QRadioButton* rbTargetSequence;
    QRadioButton* rbTargetImages;

    // Autoencoder
    QButtonGroup* autoencGroup;
    QRadioButton* rbAutoencYes;
    QRadioButton* rbAutoencNo;

    // Normalize X
    QButtonGroup* normXGroup;
    QRadioButton* rbNormXYes;
    QRadioButton* rbNormXNo;

    // Normalize Y
    QButtonGroup* normYGroup;
    QRadioButton* rbNormYYes;
    QRadioButton* rbNormYNo;

    // Champs numériques
    QLineEdit* editNbColTarget;
    QDoubleSpinBox* spinRatioTrain;
    QLineEdit* editNbBatch;
    QLineEdit* editSizeBatch;
    QLineEdit* editSizeSeq;
    QLineEdit* editNbModele;

    // Path and name file of the configuration 
    QPushButton* browseConfigFolder;
    QLineEdit* editConfigFolder;
    QLineEdit* editConfigFilename;


    // Validation buttons
    QPushButton* btnStandard;
    QPushButton* btnSequence;
    QPushButton* btnImage;
};


void print_hello ();

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Welcome +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void Welcome_Window_create(struct Welcome_Window* WW);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Train Modele +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void Train_Modele_Window_Create(struct Train_Modele_Window* TMW);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Modele +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void create_modele_window(struct Modele_Window* MW, struct Train_Modele_Window* TMW);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Apply Modele +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void Apply_Modele_Window_Create(struct Apply_Modele_Window* AMW);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Rdmgen_Modele Modele +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void Rdmgen_Modele_Window_Create( struct Rdmgen_Modele_Window* RMW);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Rdmgen_Modele Modele +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void create_rdm_Modele(struct Rdmgen_Modele_Window* RMW);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Completion +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void Completion_Window_Create(struct Completion_Window* CW, int type);



#endif