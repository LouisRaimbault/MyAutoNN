#include "../include/Loss_Functions.h"



torch::Tensor simple_MSELoss (torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::mse_loss(output, target);
}

torch::Tensor simple_cross_entropy (torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::cross_entropy(output, target);
}

torch::Tensor simple_binary_cross_entropy (torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::binary_cross_entropy(output, target);
}

torch::Tensor simple_L1Loss(torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::l1_loss(output, target);
}

torch::Tensor simple_BCEWithLogitsLoss(torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::binary_cross_entropy_with_logits(output, target);
}


torch::Tensor simple_smooth_l1_loss(torch::Tensor* output, torch::Tensor* target) 
{
    return torch::nn::functional::smooth_l1_loss(output, target);
}
