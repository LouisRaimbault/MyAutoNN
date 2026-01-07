#include "../include/Cost_Functions.h"


torch::Tensor simple_mse_cost (torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::mse_loss(*output, *target);
}

torch::Tensor simple_l1_cost(torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::l1_loss(*output, *target);
}

torch::Tensor simple_smoothl1_cost(torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::smooth_l1_loss(*output, *target);
}




torch::Tensor simple_crossentropy_cost (torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::cross_entropy(*output, *target);
}



torch::Tensor simple_nll_cost(torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::nll_loss(*output, *target);
}

torch::Tensor simple_BCEWithLogits_cost(torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::binary_cross_entropy_with_logits(*output, *target);
}



torch::Tensor sequence_crossentropy_cost (torch::Tensor* output, torch::Tensor* target) 
{
    auto B = output->size(0);
    auto T = output->size(1);
    auto C = output->size(2);

    torch::Tensor out = output->contiguous().view({B * T, C});
    torch::Tensor tgt = target->contiguous().view({B * T});

    return torch::nn::functional::cross_entropy(out, tgt);
}
