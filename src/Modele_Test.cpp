#include "../include/Modele.h"
#include "../include/Usefull_Functions.h"
#include "../include/Activation_Functions.h"
#include "../include/Cost_Functions.h"
#include "../include/Get_Quality_Modele.h"



torch::Tensor forward_pass (struct Modele* M, torch::Tensor* input)
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



torch::Tensor apply_model(struct Modele* M, Torch_Data* TD)
{
    modele_to(M, torch::kCPU);

    torch::NoGradGuard no_grad;
    int n_samples = TD->nb_rows_test;
    std::vector<torch::Tensor> predictions;
    predictions.reserve(n_samples);
    
    for (int i = 0; i < n_samples; ++i)
    {
        torch::Tensor input = TD->D_test[i];

        
        if (input.dim() == 1) {input = input.unsqueeze(0);}

        torch::Tensor output = forward_pass(M, &input);
        predictions.push_back(output.detach());
    }

    
    torch::Tensor all_preds = torch::cat(predictions, 0);

    return all_preds;
}


torch::Tensor get_tensor_target(struct Torch_Data* TD)
{
    std::vector<torch::Tensor> all_targets;
    all_targets.reserve(TD->nb_rows_test);

    for (int i = 0; i < TD->nb_rows_test; ++i)
    {
        torch::Tensor t = TD->T_test[i];


        if (t.dim() == 1 && t.size(0) == 1) {t = t.squeeze();}  

        all_targets.push_back(t);
    }

    torch::Tensor result = torch::stack(all_targets);  

    return result;
}


void modele_test (struct Modele* M, Torch_Data* TD, uint16_t type_targetmodel)
{
    // Construction de tenseur unique 
    torch::Tensor predictions = apply_model(M, TD);

    torch::Tensor predictions2 = apply_model(M, TD);

    
   
    if (TD->type_tgt == 0) // target char*
        {
            
            torch::Tensor y_true = get_tensor_target(TD);
            
            M->Q = (struct Qualitys*)malloc(sizeof(struct Qualitys));
            
            M->Q->QL =  get_qual_qualitys(predictions, y_true, 1);

            print_qual_qualitys(&(M->Q->QL));

        }


    if (TD->type_tgt == 1) // target quantitative
        {
            torch::Tensor y_true = get_tensor_target(TD);
            M->Q = (struct Qualitys*)malloc(sizeof(struct Qualitys));

            M->Q->QT =  get_quant_qualitys(predictions, y_true);

            print_quant_qualitys(&(M->Q->QT));
            
        }


    if (TD->type_tgt == 2) // target booleenne
        {
            torch::Tensor y_true = get_tensor_target(TD);
            M->Q = (struct Qualitys*)malloc(sizeof(struct Qualitys));
           
            M->Q->QB =  get_bool_qualitys(predictions, y_true);

            print_bool_qualitys(&(M->Q->QB));
        }

}