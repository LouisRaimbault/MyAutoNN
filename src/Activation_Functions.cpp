#include "../include/Activation_Functions.h"


torch::Tensor simple_none(torch::Tensor* x)
{
    return *x;
}

torch::Tensor simple_relu(torch::Tensor* x) 
{
    return torch::relu(*x);
}

torch::Tensor simple_tanh(torch::Tensor* x)  
{
    return torch::tanh(*x);
}

torch::Tensor simple_sigmoid(torch::Tensor* x) 
{
    return torch::sigmoid(*x);
}

torch::Tensor simple_leakyrelu(torch::Tensor* x) 
{
    return torch::leaky_relu(*x, 0.01);
}
