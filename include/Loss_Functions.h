#ifndef LOSS_FUNCTIONS
#define LOSS_FUNCTIONS

#include "Loss_Functions.h"



torch::Tensor simple_MSELoss (torch::Tensor* output, torch::Tensor* target) ;

torch::Tensor simple_cross_entropy (torch::Tensor* output, torch::Tensor* target) ;

torch::Tensor simple_binary_cross_entropy (torch::Tensor* output, torch::Tensor* target) ;

torch::Tensor simple_L1Loss(torch::Tensor* output, torch::Tensor* target) ;

torch::Tensor simple_BCEWithLogitsLoss(torch::Tensor* output, torch::Tensor* target) ;

torch::Tensor simple_smooth_l1_loss(torch::Tensor* output, torch::Tensor* target) ;

#endif

