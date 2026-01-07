#include "../include/Modele.h"
#include "../include/Usefull_Functions.h"
#include "../include/Activation_Functions.h"
#include "../include/Cost_Functions.h"





torch::Tensor forward_linear(Layer* L, torch::Tensor* x)
{
    torch::nn::LinearImpl* lin = reinterpret_cast<torch::nn::LinearImpl*>(L->module);
    return lin->forward(*x);
}

torch::Tensor forward_tdl(Layer* L, torch::Tensor* x)
{
    TimeDistributedLinear* tdl = reinterpret_cast<TimeDistributedLinear*>(L->module);
    return tdl->forward(*x);
}



torch::Tensor forward_rnn(Layer* L, torch::Tensor* x)
{
    torch::nn::RNNImpl* rnn = reinterpret_cast<torch::nn::RNNImpl*>(L->module);

    torch::Tensor out_seq = std::get<0>(rnn->forward(*x));

    if (L->special == 0) 
        {
            return out_seq;
        } 

    if (L->special == 1)
        {
            return out_seq.select(1, out_seq.size(1) - 1);
        }
    return out_seq;
}

torch::Tensor forward_gru(Layer* L, torch::Tensor* x)
{
    torch::nn::GRUImpl* gru = reinterpret_cast<torch::nn::GRUImpl*>(L->module);

    
    torch::Tensor out_seq = std::get<0>(gru->forward(*x));

    if (L->special == 0) 
    {
        return out_seq;  
    } 
    else if (L->special == 1)
    {
        return out_seq.select(1, out_seq.size(1) - 1); 
    }

    return out_seq;
}

torch::Tensor forward_lstm(Layer* L, torch::Tensor* x)
{
    torch::nn::LSTMImpl* lstm = reinterpret_cast<torch::nn::LSTMImpl*>(L->module);

    
    torch::Tensor out_seq = std::get<0>(lstm->forward(*x));

    if (L->special == 0) 
    {
        return out_seq;  
    } 
    else if (L->special == 1)
    {
        return out_seq.select(1, out_seq.size(1) - 1); 
    }

    return out_seq; 
}


void modele_to(struct Modele* M, torch::Device device)
{
    for (int i = 0; i < M->nb_layers; ++i) 
    {
        switch(M->tab_layers[i].type_module) 
        {
            case 0: reinterpret_cast<torch::nn::LinearImpl*>(M->tab_layers[i].module)->to(device); break;
            case 1: reinterpret_cast<TimeDistributedLinear*>(M->tab_layers[i].module)->to(device); break;
            case 2: reinterpret_cast<torch::nn::RNNImpl*>(M->tab_layers[i].module)->to(device); break;
            case 3: reinterpret_cast<torch::nn::GRUImpl*>(M->tab_layers[i].module)->to(device); break;
            case 4: reinterpret_cast<torch::nn::LSTMImpl*>(M->tab_layers[i].module)->to(device); break;
        }
    }
}

torch::Tensor forward_pass_batches (struct Modele* M, torch::Tensor* input)
{       

    torch::Tensor out = M->tab_layers[0].forward_mod(&(M->tab_layers[0]), input);
    out = M->tab_layers[0].activation(&out);

    for (uint16_t i = 1; i < M->nb_layers; ++i) 
    {
            out = M->tab_layers[i].forward_mod(&(M->tab_layers[i]), &out);
            out = M->tab_layers[i].activation(&out);       
    }
    
    return out;
}



void modele_train(struct Modele* M, struct Torch_Batch* TB, torch::Device device )
{
    
	modele_to(M, device); 
    
    int total_batches = TB->nb_batch;   
    
    for (uint32_t epoch = 0; epoch < M->nb_epoch; ++epoch) 
    {
        if (epoch % 100 == 0) {printf("epoch %d sur %d ", epoch, M->nb_epoch);}
        
        double total_loss = 0.0;

        for (int i = 0; i < total_batches; ++i) 
            {
                
                torch::Tensor x = TB->array_batch_D[i].to(device);  // (batch_size, seq_len, t_size)
                
                torch::Tensor y = TB->array_batch_T[i].to(device);  // (batch_size, seq_len, t_size)
                M->OC.optimizer->zero_grad();

                torch::Tensor output = forward_pass_batches(M, &x);

                torch::Tensor loss = M->costfunc(&output, &y);

                loss.backward();
                
                M->OC.optimizer->step();
                
                total_loss += loss.item<double>();
                
            }
        double avg_loss = total_loss / total_batches;
        if (epoch % 100 == 0) {printf(" Loss: %lf \n", avg_loss);}
    }
}


