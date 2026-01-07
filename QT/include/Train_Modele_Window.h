#ifndef TRAIN_MODELE_WINDOW
#define TRAIN_MODELE_WINDOW

#include "../include/QT_General.h"

void clean_train_modele (struct Train_Modele_Window* TMW);
void write_train_config(struct Train_Modele_Values* TMV, struct TrainContext* ctx);
void openNextModeleWindow(struct Train_Modele_Window* TMW);

#endif