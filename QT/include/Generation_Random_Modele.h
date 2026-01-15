#ifndef GENERATION_RANDOM_MODELE
#define GENERATION_RANDOM_MODELE

#include "QT_General.h"

int rand_int(int min, int max);
float rand_float(float min, float max);
void rdm_standard_mod (struct Rdm_Modele* RM, struct Rdm_Modele_Info* RMI);
void rdm_sequence_mod (struct Rdm_Modele* RM, struct Rdm_Modele_Info* RMI);
void gen_random_modele (struct Rdmgen_Modele_Window* RMW);
void write_configfile_rdm_modele (struct Rdmgen_Modele_Window* RMW);

#endif