#include "../include/Modele.h"
#include "../include/Usefull_Functions.h"
#include "../include/Activation_Functions.h"
#include "../include/Cost_Functions.h"




TimeDistributedLinear::TimeDistributedLinear(int in_features, int out_features, bool bias) 
{
    linear = register_module("linear", torch::nn::Linear(in_features, out_features));
}


torch::Tensor TimeDistributedLinear::forward(const torch::Tensor& x) 
{
    auto batch = x.size(0);
    auto seq   = x.size(1);
    auto feat  = x.size(2);

    auto x2 = x.reshape({batch * seq, feat});
    auto y2 = linear(x2);
    return y2.reshape({batch, seq, y2.size(1)});
}
