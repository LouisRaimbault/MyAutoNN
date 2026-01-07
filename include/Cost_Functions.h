#ifndef COST_FUNCTIONS
#define COST_FUNCTIONS

#include "Modele.h"


torch::Tensor simple_mse_cost (torch::Tensor* output, torch::Tensor* target);

torch::Tensor simple_l1_cost (torch::Tensor* output, torch::Tensor* target);

torch::Tensor simple_smoothl1_cost (torch::Tensor* output, torch::Tensor* target);

torch::Tensor simple_crossentropy_cost(torch::Tensor* output, torch::Tensor* target);

torch::Tensor simple_BCEWithLogits_cost(torch::Tensor* output, torch::Tensor* target);

torch::Tensor simple_nll_cost(torch::Tensor* output, torch::Tensor* target);

torch::Tensor sequence_crossentropy_cost (torch::Tensor* output, torch::Tensor* target); 

#endif

