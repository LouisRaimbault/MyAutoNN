#include "../include/Apply_Modele_Window.h"
#include "../include/QT_General.h"

void clean_apply_modele (struct Apply_Modele_Window* AMW)
{
    free (AMW->AMV.txtpathbase);
    free (AMW->AMV.txtpathresbase);
    free (AMW->AMV.txtpathmodel);
    free (AMW->AMV.txtdestfolder);
    free (AMW->AMV.txtconfigname);

    delete AMW;

}

void write_file_apply_modele(struct Apply_Modele_Values* AMV)
{
    size_t len = strlen(AMV->txtdestfolder) + strlen(AMV->txtconfigname) + 2;

    char* filename = (char*)malloc(len * sizeof(char));
    

    // concaténation dossier + "/" + nom
    snprintf(filename, len, "%s/%s",
             AMV->txtdestfolder,
             AMV->txtconfigname);

    std::cout << "filename = " << std::string(filename) << "\n";
    FILE* f = fopen(filename, "w");


    fprintf(f, "+-+-+-+-+-+-+-+-+-+ Configuration Apply Modeles +-+-+-+-+-+-+-+-+-+-+\n\n");

    fprintf(f, "Path new base#%s,\n", AMV->txtpathbase);
    fprintf(f, "Path Resume train Base#%s,\n", AMV->txtpathresbase);
    fprintf(f, "Path Model#%s,\n\n", AMV->txtpathmodel);

    fclose(f);
    free(filename);
}


void Apply_Modele_Window_Create( struct Apply_Modele_Window* AMW)
{
    AMW->window = new QWidget();
    AMW->window->setWindowTitle("Apply Modele");

    QVBoxLayout* mainLayout = new QVBoxLayout(AMW->window);
    mainLayout->setContentsMargins(100, 50, 100, 60);
    mainLayout->setSpacing(40);

    // 🔹 TITRE
    QLabel* title = new QLabel("Apply Modele");
    QFont titleFont = title->font();
    titleFont.setPointSize(30);
    titleFont.setBold(true);
    title->setFont(titleFont);
    title->setAlignment(Qt::AlignCenter);
    mainLayout->addWidget(title);

    mainLayout->addSpacing(20);

    // 🔹 Fonction pour créer ligne label + edit + bouton
    auto addFileSelector = [&](const char* labelText, QPushButton*& button, QLineEdit*& edit) 
    {
        QHBoxLayout* layout = new QHBoxLayout();

        QLabel* label = new QLabel(labelText);
        QFont labelFont = label->font();
        labelFont.setPointSize(16);
        label->setFont(labelFont);
        label->setMinimumWidth(180);

        edit = new QLineEdit();
        edit->setReadOnly(true);
        edit->setMinimumHeight(40);

        button = new QPushButton("Browse");
        button->setMinimumSize(140, 40);

        layout->addWidget(label);
        layout->addSpacing(20);
        layout->addWidget(edit, 1); // prend tout l’espace restant
        layout->addSpacing(20);
        layout->addWidget(button);

        mainLayout->addLayout(layout);
    };

    addFileSelector("Path new Database:", AMW->pathbaseButton, AMW->editbase);
    addFileSelector("Path resume train Database:", AMW->pathresbaseButton, AMW->editresbase);
    addFileSelector("Path Model File:", AMW->pathmodelButton, AMW->editmodel);

    mainLayout->addStretch(2); // pousse les boutons vers le bas


    QFrame* line = new QFrame();
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    line->setLineWidth(3);  // trait épais
    mainLayout->addWidget(line);

    // 🔹 Destination Config File Folder
    addFileSelector("Destination Config File Folder:", AMW->pathdestButton, AMW->editdestfolder);
    AMW->editdestfolder->setPlaceholderText("Select a destination folder...");

    // 🔹 Name Config File
    QHBoxLayout* nameLayout = new QHBoxLayout();
    QLabel* nameLabel = new QLabel("Name Config File:");
    QFont labelFont = nameLabel->font();
    labelFont.setPointSize(16);
    nameLabel->setFont(labelFont);
    nameLabel->setMinimumWidth(180);

    AMW->editconfigname = new QLineEdit();
    AMW->editconfigname->setMinimumHeight(40);

    nameLayout->addWidget(nameLabel);
    nameLayout->addSpacing(20);
    nameLayout->addWidget(AMW->editconfigname, 1);

    mainLayout->addLayout(nameLayout);



    // 🔹 Bouton Valider en bas à droite
    QHBoxLayout* bottomLayout = new QHBoxLayout();
    bottomLayout->addStretch();

    AMW->valider = new QPushButton("Valider");
    AMW->valider->setMinimumSize(180, 60);
    QFont btnFont;
    btnFont.setPointSize(16);
    btnFont.setBold(true);
    AMW->valider->setFont(btnFont);

    bottomLayout->addWidget(AMW->valider);
    mainLayout->addLayout(bottomLayout);

    // 🔹 Connexions Browse
    QObject::connect(AMW->pathbaseButton, &QPushButton::clicked, [=]() 
    {
        QString file = QFileDialog::getOpenFileName(AMW->window, "Select Path new Database");
        if (!file.isEmpty()) AMW->editbase->setText(file);
    });

    QObject::connect(AMW->pathresbaseButton, &QPushButton::clicked, [=]() 
    {
        QString file = QFileDialog::getOpenFileName(AMW->window, "Select resume train Database");
        if (!file.isEmpty()) AMW->editresbase->setText(file);
    });

    QObject::connect(AMW->pathmodelButton, &QPushButton::clicked, [=]() 
    {
        QString file = QFileDialog::getOpenFileName(AMW->window, "Select Model File");
        if (!file.isEmpty()) AMW->editmodel->setText(file);
    });

    QObject::connect(AMW->pathdestButton, &QPushButton::clicked, [=]() {
    QString folder = QFileDialog::getExistingDirectory(AMW->window,"Select Destination Config Folder");
    if (!folder.isEmpty())AMW->editdestfolder->setText(folder);
    });




    QObject::connect(AMW->valider, &QPushButton::clicked, [=]() 
    {
        AMW->AMV.txtpathbase = strdup(AMW->editbase->text().toStdString().c_str());
        AMW->AMV.txtpathresbase = strdup(AMW->editresbase->text().toStdString().c_str());
        AMW->AMV.txtpathmodel = strdup(AMW->editmodel->text().toStdString().c_str());
        AMW->AMV.txtdestfolder = strdup(AMW->editdestfolder->text().toStdString().c_str());
        AMW->AMV.txtconfigname = strdup(AMW->editconfigname->text().toStdString().c_str());

        write_file_apply_modele(&(AMW->AMV));

        Completion_Window* CW = new Completion_Window;
        Completion_Window_Create(CW, 2); // 2 = apply model

        // Quand l'utilisateur clique sur OK dans la completion
        QObject::connect(CW->btnOk, &QPushButton::clicked, [AMW, CW]() 
        {
            AMW->window->deleteLater();  // supprime la Apply Window
            clean_apply_modele(AMW); // si tu as besoin de libérer la mémoire
            delete CW;                     // supprime la Completion Window
        });
    
    });

    // 🔹 Affichage plein écran
    AMW->window->showMaximized();
}

