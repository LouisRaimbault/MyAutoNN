#include "../include/Modele.h"
#include "../include/Usefull_Functions.h"

void clean_qualitys (struct Qualitys* Q)
{
    free(Q);
}

void clean_choices (struct choices* _choices, int nb_layers)
{   
    
    for (int i = 0; i < nb_layers; ++i) {free(_choices->name_activ[i]);}
    
    free(_choices->name_activ);
    
    free(_choices->name_optim);
    
    free(_choices->name_cost);
    
    free(_choices);
    
}

void clean_layer (struct Layer* L)
{
    delete L->module;
}

void clean_modele (struct Modele* M)
{
    
    for (int i = 0; i < M->nb_layers; ++i)
        {
            clean_layer(&(M->tab_layers[i]));
        }
    
    delete M->tab_layers;
       
    delete M->OC.optimizer;
    
    clean_qualitys(M->Q);
    
    clean_choices(M->_choices, M->nb_layers); 
    
}

std::vector<torch::Tensor> get_parameters(struct Modele* M)
{
    std::vector<torch::Tensor> params;

    // --- Paramètres des couches cachées ---
    for (int i = 0; i < M->nb_layers; ++i)
    {
        switch(M->tab_layers[i].type_module)
        {
            case 0: 
                for (auto p : reinterpret_cast<torch::nn::LinearImpl*>(M->tab_layers[i].module)->parameters()) {params.push_back(p);}
                break;
            case 1: 
                for (auto p : reinterpret_cast<TimeDistributedLinear*>(M->tab_layers[i].module)->parameters()) {params.push_back(p);}
                break;
            case 2: 
                for (auto p : reinterpret_cast<torch::nn::RNNImpl*>(M->tab_layers[i].module)->parameters()) {params.push_back(p);}
                break;
            case 3: 
                for (auto p : reinterpret_cast<torch::nn::GRUImpl*>(M->tab_layers[i].module)->parameters()) {params.push_back(p);}
                break;
            case 4: 
                for (auto p : reinterpret_cast<torch::nn::LSTMImpl*>(M->tab_layers[i].module)->parameters()) {params.push_back(p);}
                break;
        }
    }

    return params;
}


void init_layer_module(struct Layer* L, struct Modele* M)
{
    
    const char* type_layer[] =
    {
        "LIN",
        "RNN",
        "GRU",
        "LSTM"
    };

    torch::Tensor (*forwards_mod[10])(struct Layer* L, torch::Tensor*); 
    forwards_mod[0] = forward_linear;
    forwards_mod[1] = forward_rnn;
    forwards_mod[2] = forward_gru;
    forwards_mod[3] = forward_lstm;
   
    L->forward_mod = forwards_mod[L->type_module];

    bool b = (bool)L->bias;
    if (L->type_module == 0)
        {
            L->module = new torch::nn::LinearImpl(torch::nn::LinearOptions(L->dim_in, L->dim_out).bias(b));
            return;
        }

    // RNN simple
    if (L->type_module == 1)
        {
            
            L->module = new torch::nn::RNNImpl(torch::nn::RNNOptions(L->dim_in, L->dim_out).bias(b).batch_first(true));
            return;
        }

    // GRU
    if (L->type_module == 2)
        {
            
            L->module = new torch::nn::GRUImpl(torch::nn::GRUOptions(L->dim_in, L->dim_out).bias(b).batch_first(true));
            return;
        }

    // LSTM
    if (L->type_module == 3)
        {
            
            L->module = new torch::nn::LSTMImpl(torch::nn::LSTMOptions(L->dim_in, L->dim_out).bias(b).batch_first(true));
            return;
        }

}


void set_optimizer(Modele* M)
{
    std::vector<torch::Tensor> params = get_parameters(M); 

    if (M->num_optimizer == 0) // SGD
        {
            M->OC.optimizer = new torch::optim::SGD(params, torch::optim::SGDOptions(M->OC.learning_rate));
            M->_choices->name_optim = (char*)malloc(4*sizeof(char));
            strcpy(M->_choices->name_optim, "SGD");
        }

    else if (M->num_optimizer == 1) // Adam
        {
            M->OC.optimizer =  new torch::optim::Adam(params, torch::optim::AdamOptions(M->OC.learning_rate));
            M->_choices->name_optim = (char*)malloc(5*sizeof(char));
            strcpy(M->_choices->name_optim, "Adam");
        }

    else if (M->num_optimizer == 2) // AdamW
        {
            M->OC.optimizer =  new torch::optim::AdamW(params, torch::optim::AdamWOptions(M->OC.learning_rate));
            M->_choices->name_optim = (char*)malloc(6*sizeof(char));
            strcpy(M->_choices->name_optim, "AdamW");
        }

    else if (M->num_optimizer == 3) // RMSprop
        {
            M->OC.optimizer =  new torch::optim::RMSprop(params, torch::optim::RMSpropOptions(M->OC.learning_rate));
            M->_choices->name_optim = (char*)malloc(8*sizeof(char));
            strcpy(M->_choices->name_optim, "RMSprop");
        }

    else if (M->num_optimizer == 4) // Adagrad
        {
            M->OC.optimizer =  new torch::optim::Adagrad(params, torch::optim::AdagradOptions(M->OC.learning_rate));
            M->_choices->name_optim = (char*)malloc(8*sizeof(char));
            strcpy(M->_choices->name_optim, "Adagrad");      
        }

    else if (M->num_optimizer == 5) // LBFGS
        {
            M->OC.optimizer = new torch::optim::LBFGS(params, torch::optim::LBFGSOptions(M->OC.learning_rate));
            M->_choices->name_optim = (char*)malloc(6*sizeof(char));
            strcpy(M->_choices->name_optim, "LBFGS");
        }
    else
        {
            std::cerr << "[Erreur] Optimizer inconnu : " << M->num_optimizer << "\n";
            M->OC.optimizer = nullptr;
        }
}


void modele_init(struct Modele* M)
{
    for (int i = 0; i < M->nb_layers; ++i)
        {
            init_layer_module(&(M->tab_layers[i]), M);
        }
    set_optimizer(M);
}




