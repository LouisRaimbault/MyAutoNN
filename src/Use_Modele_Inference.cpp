#include "../include/Modele.h"
#include "../include/Use_Modele.h"
#include "../include/Get_Quality_Modele.h"


void softmodele_to (struct SoftModele* SM, torch::Device device)
{

    // Hidden layers
    for (int i = 0; i < SM->nb_layers; ++i) 
    {
        switch(SM->type_modules[i]) 
        {
            case 0: reinterpret_cast<torch::nn::LinearImpl*>(SM->modules[i])->to(device); break;
            case 1: reinterpret_cast<TimeDistributedLinear*>(SM->modules[i])->to(device); break;
            case 2: reinterpret_cast<torch::nn::RNNImpl*>(SM->modules[i])->to(device); break;
            case 3: reinterpret_cast<torch::nn::GRUImpl*>(SM->modules[i])->to(device); break;
            case 4: reinterpret_cast<torch::nn::LSTMImpl*>(SM->modules[i])->to(device); break;
        }
    }
}

torch::Tensor inf_forward_linear(void* module, torch::Tensor* x) 
{
    torch::nn::LinearImpl* lin = reinterpret_cast<torch::nn::LinearImpl*>(module);
    return lin->forward(*x);
}

torch::Tensor inf_forward_tdl(void* module, torch::Tensor* x)
{
    TimeDistributedLinear* tdl = reinterpret_cast<TimeDistributedLinear*>(module);
    return tdl->forward(*x);
}


torch::Tensor inf_forward_rnn_0(void* module, torch::Tensor* x)
{
    torch::nn::RNNImpl* rnn = reinterpret_cast<torch::nn::RNNImpl*>(module);

    return std::get<0>(rnn->forward(*x));
}


torch::Tensor inf_forward_rnn_1(void* module, torch::Tensor* x)
{
    std::cout << "Yohoho \n";
    torch::nn::RNNImpl* rnn = reinterpret_cast<torch::nn::RNNImpl*>(module);
    torch::Tensor out = std::get<0>(rnn->forward(*x));
    return out.select(1, out.size(1) - 1);
}

torch::Tensor inf_forward_gru_0(void* module, torch::Tensor* x)
{
    torch::nn::GRUImpl* gru = reinterpret_cast<torch::nn::GRUImpl*>(module);
    return std::get<0>(gru->forward(*x));
}

torch::Tensor inf_forward_gru_1(void* module, torch::Tensor* x)
{
    torch::nn::GRUImpl* gru = reinterpret_cast<torch::nn::GRUImpl*>(module);
    torch::Tensor out = std::get<0>(gru->forward(*x));
    return out.select(1, out.size(1) - 1);
}

torch::Tensor inf_forward_lstm_0(void* module, torch::Tensor* x)
{
    torch::nn::LSTMImpl* lstm = reinterpret_cast<torch::nn::LSTMImpl*>(module);
    return std::get<0>(lstm->forward(*x));
}

torch::Tensor inf_forward_lstm_1(void* module, torch::Tensor* x)
{
    torch::nn::LSTMImpl* lstm = reinterpret_cast<torch::nn::LSTMImpl*>(module);
    torch::Tensor out = std::get<0>(lstm->forward(*x));
    return out.select(1, out.size(1) - 1);
}


torch::Tensor use_modele_forward_pass (struct SoftModele* SM, torch::Tensor* input)
{       

    torch::Tensor out = SM->forward_mod[0](SM->modules[0], input);

    
    out = SM->activation[0](&out);
    
    

    for (uint16_t i = 1; i < SM->nb_layers; ++i) 
    { 
    	out = SM->forward_mod[i](SM->modules[i], &out);
        out = SM->activation[i](&out); 
    }
    
    return out;
}




torch::Tensor apply_model_use(struct SoftModele* SM, struct Use_Data* UD)
{
    softmodele_to(SM, torch::kCPU);
    torch::NoGradGuard no_grad;
    std::vector<torch::Tensor> predictions;
    predictions.reserve(UD->nb_rows_use);

    for (int i = 0; i < UD->nb_rows_use; ++i)
        {
            torch::Tensor input = UD->D_use[i];
            if (input.dim() == 1)
                input = input.unsqueeze(0);  

            torch::Tensor output = use_modele_forward_pass(SM, &input);
            predictions.push_back(output.detach());
        }


    torch::Tensor all_preds = torch::cat(predictions, 0);

    return all_preds;
}


torch::Tensor get_tensor_target_use(struct Use_Data* UD)
{
    std::vector<torch::Tensor> all_targets;
    all_targets.reserve(UD->nb_rows_use);

    for (int i = 0; i < UD->nb_rows_use; ++i)
    {
        torch::Tensor t = UD->T_use[i];

        if (t.dim() == 1 && t.size(0) == 1)
            t = t.squeeze();  // passe de [1] → []

        all_targets.push_back(t);
    }

    torch::Tensor result = torch::stack(all_targets);  // shape : [N]

    return result;
}

void modele_use (struct Genuse* GU)
{

    torch::Tensor predictions = apply_model_use(GU->SM, GU->UD);

    if (GU->UD->type_target == 0) // target char*
        {
            torch::Tensor y_true = get_tensor_target_use(GU->UD);

            GU->Q = (struct Qualitys*)malloc(sizeof(struct Qualitys));
            GU->Q->QL =  get_qual_qualitys(predictions, y_true, 1);
            print_qual_qualitys(&(GU->Q->QL));

        }


    if (GU->UD->type_target == 1) // target quantitative
        {
            torch::Tensor y_true = get_tensor_target_use(GU->UD);
            GU->Q = (struct Qualitys*)malloc(sizeof(struct Qualitys));
            GU->Q->QT =  get_quant_qualitys(predictions, y_true);
            print_quant_qualitys(&(GU->Q->QT));

        }


    if (GU->UD->type_target == 2) // target booleenne
        {
            torch::Tensor y_true = get_tensor_target_use(GU->UD);
            GU->Q = (struct Qualitys*)malloc(sizeof(struct Qualitys));
            GU->Q->QB =  get_bool_qualitys(predictions, y_true);
            print_bool_qualitys(&(GU->Q->QB));
        }

}

