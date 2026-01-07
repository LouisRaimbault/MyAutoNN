#ifndef GET_CONFIG
#define GET_CONFIG


#include "Modele.h"

void print_modele(Modele* M);
void get_config_from_configfile (const char* pathfile, struct Genstruct* GS);
void get_config_from_data (struct Genstruct* GS);

#endif