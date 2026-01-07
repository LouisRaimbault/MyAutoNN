#ifndef EXPORT_MODELE
#define EXPORT_MODELE

#include "Modele.h"

void write_weight_linear(FILE* f, struct Layer* L);
void write_weight_tdl(FILE* f, struct Layer* L);
void write_weight_rnn(FILE* f, struct Layer* L);
void write_weight_gru(FILE* f, struct Layer* L);
void write_weight_lstm(FILE* f, struct Layer* L);
void export_modele_file(struct Modele* M, int i);


#endif