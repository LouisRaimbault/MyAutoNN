#ifndef GET_QUALITY_MODELE
#define GET_QUALITY_MODELE

#include "../include/Modele.h"


struct Quant_Quality get_quant_qualitys(torch::Tensor y_pred, torch::Tensor y_true);
struct Qual_Quality get_qual_qualitys(torch::Tensor y_pred, torch::Tensor y_true, int top_k);
struct Bool_Quality get_bool_qualitys(torch::Tensor y_pred, torch::Tensor y_true);
void print_qual_qualitys(struct Qual_Quality* Ql);
void print_quant_qualitys(struct Quant_Quality* Qt);
void print_bool_qualitys(struct Bool_Quality* Qb);
void tri_array(double* values, int* indices, int nb); 
void order_model_by_quality (struct Genstruct* GS, int top_k);

#endif