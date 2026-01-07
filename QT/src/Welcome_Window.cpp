#include "../include/Welcome_Window.h"
#include "../include/QT_General.h"


void chooseTrain(struct Welcome_Window* WW)
{
    WW->window->close();  // Ferme la fenêtre Welcome

    struct Train_Modele_Window* TMW = new struct Train_Modele_Window;
    Train_Modele_Window_Create(TMW);
}

void chooseApply(struct Welcome_Window* WW)
{
    WW->window->close();  // Ferme la fenêtre Welcome

    struct Apply_Modele_Window* AMW = new struct Apply_Modele_Window;
    Apply_Modele_Window_Create(AMW);
}

void Welcome_Window_create(struct Welcome_Window* WW)
{
    WW->window = new QWidget();
    WW->window->setWindowTitle("Autom ML C++");

    QVBoxLayout* mainLayout = new QVBoxLayout(WW->window);
    mainLayout->setContentsMargins(80, 50, 80, 60);
    mainLayout->setSpacing(30);

    // 🔹 TITRE
    QLabel* title = new QLabel("Autom ML  C++");
    QFont titleFont = title->font();
    titleFont.setPointSize(28);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    // 🔹 TEXTE EXPLICATIF
    WW->description = new QLabel
    (
        "<b>Train Model:</b> If you want to train a model from scratch, by providing a database, "
        "an architecture and hyperparameters.<br><br>"
        "<b>Apply Model:</b> If you wish to apply a model trained by this same software, "
        "you must provide the application database, the training database information file, "
        "and the model file."
    );

    QFont descFont = WW->description->font();
    descFont.setPointSize(14);
    WW->description->setFont(descFont);
    WW->description->setWordWrap(true);
    WW->description->setAlignment(Qt::AlignLeft | Qt::AlignTop);
    mainLayout->addWidget(WW->description);

    // 🔹 Stretch léger (fait monter les boutons)
    mainLayout->addStretch(1);

    // 🔹 BOUTONS
    QHBoxLayout* buttonLayout = new QHBoxLayout();
    buttonLayout->setSpacing(160);

    WW->trainButton = new QPushButton("Train Model");
    WW->applyButton = new QPushButton("Apply Model");

    QFont btnFont;
    btnFont.setPointSize(18);
    btnFont.setBold(true);

    WW->trainButton->setFont(btnFont);
    WW->applyButton->setFont(btnFont);

    WW->trainButton->setMinimumSize(380, 140);
    WW->applyButton->setMinimumSize(380, 140);

    buttonLayout->addStretch();
    buttonLayout->addWidget(WW->trainButton);
    buttonLayout->addWidget(WW->applyButton);
    buttonLayout->addStretch();

    mainLayout->addLayout(buttonLayout);

    // Stretch plus grand sous les boutons
    mainLayout->addStretch(3);

    //  Connexions
    QObject::connect(WW->trainButton, &QPushButton::clicked, [=]() 
    {
        chooseTrain(WW);
    });

    QObject::connect(WW->applyButton, &QPushButton::clicked, [=]() 
    {
        chooseApply(WW);
    });

    WW->window->showMaximized();
}
