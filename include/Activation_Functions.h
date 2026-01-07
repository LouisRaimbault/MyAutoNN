#ifndef ACTIVATION_FUNCTIONS
#define ACTIVATION_FUNCTIONS

#include "Modele.h"


torch::Tensor simple_none(torch::Tensor* x);
torch::Tensor simple_relu(torch::Tensor* x);
torch::Tensor simple_tanh(torch::Tensor* x);
torch::Tensor simple_sigmoid(torch::Tensor* x);
torch::Tensor simple_leakyrelu(torch::Tensor* x);


#endif


