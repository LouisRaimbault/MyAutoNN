#ifndef MODELE_WINDOW
#define MODELE_WINDOW

#include "../include/QT_General.h"

QWidget* thickLine();
void addIntField(QVBoxLayout* parent,  char* text, QLineEdit*& edit);
void addFloatField(QVBoxLayout* parent, const char* text, QLineEdit*& edit);
void addLayerBlock(struct Modele_Window* MW, int idx);


#endif