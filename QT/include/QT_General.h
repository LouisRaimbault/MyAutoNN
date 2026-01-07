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
#include <QScreen>
#include <QObject>



struct Welcome_Window
{
    QWidget* window;
    QLabel* description;
    QPushButton* trainButton;
    QPushButton* applyButton;
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



void print_hello ();

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Welcome +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void Welcome_Window_create(struct Welcome_Window* WW);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Train Modele +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void Train_Modele_Window_Create(struct Train_Modele_Window* TMW);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Modele +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void create_modele_window(struct Modele_Window* MW, struct Train_Modele_Window* TMW);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Apply Modele +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void Apply_Modele_Window_Create(struct Apply_Modele_Window* AMW);

/* +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Section Completion +-+-+-+-+-+-+-+-+-+-+-+-+-+-+ */

void Completion_Window_Create(struct Completion_Window* CW, int type);



#endif