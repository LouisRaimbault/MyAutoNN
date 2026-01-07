#ifndef GET_DATA
#define GET_DATA

#include "Modele.h"

void clean_torch_data (struct Torch_Data* TD, struct Genstruct* GS);
void affiche_info_datas (struct Genstruct* GS);
uint16_t is_float(const char* s);
uint16_t is_booleen(char** fval, int size);
void get_info_file(struct Pure_Data* PD, const char* filename);
void get_pure_data (struct Genstruct* GS);
int get_nb_modalites (char** col, int nb_rows);
float* standardize_char_array(char** data, int nb_rows, int t, struct Genstruct* GS);
void set_die (struct Genstruct* GS);
torch::Tensor* get_tensor_target (struct Genstruct* GS);
void get_tensor_data (struct Genstruct* GS);
void get_tensor_data_no_tgt (struct Genstruct* GS);
void set_Dtest_to_sequence(Torch_Data* TD, int size_seq);
void set_onehot_Ttest_to_long(Torch_Data* TD);
void export_data_info_file(struct Genstruct* GS);
void get_data (struct Genstruct* GS);

#endif