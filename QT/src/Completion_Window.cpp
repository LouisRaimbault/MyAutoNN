#include "../include/Completion_Window.h"
#include "../include/QT_General.h"



void Completion_Window_Create(struct Completion_Window* CW, int type)
{
    CW->window = new QWidget();
    CW->window->setWindowTitle("Configuration Complete");

    // Plein écran
    CW->window->showMaximized();

    QVBoxLayout* mainLayout = new QVBoxLayout(CW->window);
    mainLayout->setContentsMargins(50, 50, 50, 50);
    mainLayout->setSpacing(40);

    // Texte
    CW->description = new QLabel();
    CW->description->setAlignment(Qt::AlignCenter);
    QFont f = CW->description->font();
    f.setPointSize(28);
    f.setBold(true);
    CW->description->setFont(f);

    // Texte selon type
    switch(type)
    {
        case 1:
            CW->description->setText("Training configuration completed successfully!");
            break;
        case 2:
            CW->description->setText("Application configuration completed successfully!");
            break;
        default:
            CW->description->setText("Configuration completed successfully!");
            break;
    }

    mainLayout->addStretch(1);
    mainLayout->addWidget(CW->description);
    mainLayout->addStretch(1);

    // Bouton OK
    CW->btnOk = new QPushButton("OK");
    CW->btnOk->setMinimumSize(180, 60);

    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();
    bottomLayout->addWidget(CW->btnOk);
    bottomLayout->addStretch();

    mainLayout->addLayout(bottomLayout);

    // Connexion bouton OK
    QObject::connect(CW->btnOk, &QPushButton::clicked, [CW]() {
        CW->window->close();
        CW->window->deleteLater();
    });

    CW->window->showMaximized();
}