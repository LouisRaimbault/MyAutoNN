#include "../include/QT_General.h"
#include "../include/Welcome_Window.h"
#include "../include/Train_Modele_Window.h"
#include "../include/Apply_Modele_Window.h"
#include "../include/Modele_Window.h"




int main(int argc, char** argv)
{
    print_hello();
    QApplication app(argc, argv);  

    Welcome_Window WW;
    Welcome_Window_create(&WW);
    WW.window->show();
    app.exec();

    return 0; 
}