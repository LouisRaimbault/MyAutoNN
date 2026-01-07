#include "../include/Modele.h"
#include "../include/Use_Modele.h"
#include "../include/Usefull_Functions.h"
#include "../include/Activation_Functions.h"


void affiche_mod_lin (void* mod, int i)
{
    torch::nn::LinearImpl* lin = reinterpret_cast<torch::nn::LinearImpl*>(mod);
     auto W = lin->weight.to(torch::kCPU).contiguous();
        int out = W.size(0);
        int in  = W.size(1);

        std::cout << "=== Linear layer " << i << " ===\n";
        std::cout << "Poids (" << out << " x " << in << "):\n";

        for (int r = 0; r < out; ++r)
        {
            std::cout << "[ ";
            for (int c = 0; c < in; ++c)
            {
                std::cout << W[r][c].item<float>(); // ici exactement comme tu veux
                if (c != in - 1) std::cout << ", ";
            }
            std::cout << " ]\n";
        }

        if (lin->bias.defined())
        {
            auto b = lin->bias.to(torch::kCPU);
            std::cout << "Biais : [ ";
            for (int j = 0; j < b.size(0); ++j)
            {
                std::cout << b[j].item<float>();
                if (j != b.size(0) - 1) std::cout << ", ";
            }
            std::cout << " ]\n";
        }

    std::cout << "\n";
}

void affiche_mod_tdl (void* mod, int i)
{
    TimeDistributedLinear* tdl = reinterpret_cast<TimeDistributedLinear*>(mod);
     auto W = tdl->linear->weight.to(torch::kCPU).contiguous();
        int out = W.size(0);
        int in  = W.size(1);

        std::cout << "=== TimeDistributedLinear layer " << i << " ===\n";
        std::cout << "Poids (" << out << " x " << in << "):\n";

        for (int r = 0; r < out; ++r)
        {
            std::cout << "[ ";
            for (int c = 0; c < in; ++c)
            {
                std::cout << W[r][c].item<float>(); // ici exactement comme tu veux
                if (c != in - 1) std::cout << ", ";
            }
            std::cout << " ]\n";
        }

        if (tdl->linear->bias.defined())
        {
            auto b = tdl->linear->bias.to(torch::kCPU);
            std::cout << "Biais : [ ";
            for (int j = 0; j < b.size(0); ++j)
            {
                std::cout << b[j].item<float>();
                if (j != b.size(0) - 1) std::cout << ", ";
            }
            std::cout << " ]\n";
        }

    std::cout << "\n";
}

void affiche_mod_rnn(void* mod, int i)
{
    torch::nn::RNNImpl* rnn = reinterpret_cast<torch::nn::RNNImpl*>(mod);

    auto params = rnn->named_parameters();
    torch::Tensor w_ih = params["weight_ih_l0"].to(torch::kCPU).contiguous();
    torch::Tensor w_hh = params["weight_hh_l0"].to(torch::kCPU).contiguous();

    std::cout << "=== RNN layer " << i << " ===\n";

    // ---- W_ih ----
    int out_ih = w_ih.size(0);
    int in_ih  = w_ih.size(1);
    std::cout << "Poids_ih (" << out_ih << " x " << in_ih << "):\n";

    for (int r = 0; r < out_ih; ++r)
    {
        std::cout << "[ ";
        for (int c = 0; c < in_ih; ++c)
        {
            std::cout << w_ih[r][c].item<float>();
            if (c != in_ih - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    // ---- W_hh ----
    int out_hh = w_hh.size(0);
    int in_hh  = w_hh.size(1);
    std::cout << "Poids_hh (" << out_hh << " x " << in_hh << "):\n";

    for (int r = 0; r < out_hh; ++r)
    {
        std::cout << "[ ";
        for (int c = 0; c < in_hh; ++c)
        {
            std::cout << w_hh[r][c].item<float>();
            if (c != in_hh - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    // ---- Bias ----
    if (rnn->options.bias())
    {
        auto b_ih = params["bias_ih_l0"].to(torch::kCPU);
        auto b_hh = params["bias_hh_l0"].to(torch::kCPU);

        std::cout << "Biais_ih : [ ";
        for (int j = 0; j < b_ih.size(0); ++j)
        {
            std::cout << b_ih[j].item<float>();
            if (j != b_ih.size(0) - 1) std::cout << ", ";
        }
        std::cout << " ]\n";

        std::cout << "Biais_hh : [ ";
        for (int j = 0; j < b_hh.size(0); ++j)
        {
            std::cout << b_hh[j].item<float>();
            if (j != b_hh.size(0) - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    std::cout << "\n";
}

void affiche_mod_gru(void* mod, int i)
{
    torch::nn::GRUImpl* gru = reinterpret_cast<torch::nn::GRUImpl*>(mod);

    auto params = gru->named_parameters();
    torch::Tensor w_ih = params["weight_ih_l0"].to(torch::kCPU).contiguous();
    torch::Tensor w_hh = params["weight_hh_l0"].to(torch::kCPU).contiguous();

    std::cout << "=== GRU layer " << i << " ===\n";

    // ---- W_ih ----
    int out_ih = w_ih.size(0);
    int in_ih  = w_ih.size(1);
    std::cout << "Poids_ih (" << out_ih << " x " << in_ih << "):\n";

    for (int r = 0; r < out_ih; ++r)
    {
        std::cout << "[ ";
        for (int c = 0; c < in_ih; ++c)
        {
            std::cout << w_ih[r][c].item<float>();
            if (c != in_ih - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    // ---- W_hh ----
    int out_hh = w_hh.size(0);
    int in_hh  = w_hh.size(1);
    std::cout << "Poids_hh (" << out_hh << " x " << in_hh << "):\n";

    for (int r = 0; r < out_hh; ++r)
    {
        std::cout << "[ ";
        for (int c = 0; c < in_hh; ++c)
        {
            std::cout << w_hh[r][c].item<float>();
            if (c != in_hh - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    // ---- Bias ----
    if (gru->options.bias())
    {
        auto b_ih = params["bias_ih_l0"].to(torch::kCPU);
        auto b_hh = params["bias_hh_l0"].to(torch::kCPU);

        std::cout << "Biais_ih : [ ";
        for (int j = 0; j < b_ih.size(0); ++j)
        {
            std::cout << b_ih[j].item<float>();
            if (j != b_ih.size(0) - 1) std::cout << ", ";
        }
        std::cout << " ]\n";

        std::cout << "Biais_hh : [ ";
        for (int j = 0; j < b_hh.size(0); ++j)
        {
            std::cout << b_hh[j].item<float>();
            if (j != b_hh.size(0) - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    std::cout << "\n";
}


void affiche_mod_lstm(void* mod, int i)
{
    torch::nn::LSTMImpl* lstm = reinterpret_cast<torch::nn::LSTMImpl*>(mod);

    auto params = lstm->named_parameters();
    torch::Tensor w_ih = params["weight_ih_l0"].to(torch::kCPU).contiguous();
    torch::Tensor w_hh = params["weight_hh_l0"].to(torch::kCPU).contiguous();

    std::cout << "=== LSTM layer " << i << " ===\n";

    // ---- W_ih ----
    int out_ih = w_ih.size(0);
    int in_ih  = w_ih.size(1);
    std::cout << "Poids_ih (" << out_ih << " x " << in_ih << "):\n";

    for (int r = 0; r < out_ih; ++r)
    {
        std::cout << "[ ";
        for (int c = 0; c < in_ih; ++c)
        {
            std::cout << w_ih[r][c].item<float>();
            if (c != in_ih - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    // ---- W_hh ----
    int out_hh = w_hh.size(0);
    int in_hh  = w_hh.size(1);
    std::cout << "Poids_hh (" << out_hh << " x " << in_hh << "):\n";

    for (int r = 0; r < out_hh; ++r)
    {
        std::cout << "[ ";
        for (int c = 0; c < in_hh; ++c)
        {
            std::cout << w_hh[r][c].item<float>();
            if (c != in_hh - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    // ---- Bias ----
    if (lstm->options.bias())
    {
        auto b_ih = params["bias_ih_l0"].to(torch::kCPU);
        auto b_hh = params["bias_hh_l0"].to(torch::kCPU);

        std::cout << "Biais_ih : [ ";
        for (int j = 0; j < b_ih.size(0); ++j)
        {
            std::cout << b_ih[j].item<float>();
            if (j != b_ih.size(0) - 1) std::cout << ", ";
        }
        std::cout << " ]\n";

        std::cout << "Biais_hh : [ ";
        for (int j = 0; j < b_hh.size(0); ++j)
        {
            std::cout << b_hh[j].item<float>();
            if (j != b_hh.size(0) - 1) std::cout << ", ";
        }
        std::cout << " ]\n";
    }

    std::cout << "\n";
}


void affiche_modele(struct SoftModele* SM)
{
    for (int i = 0; i < SM->nb_layers; ++i)
        {
            if (SM->type_modules[i] == 0) {affiche_mod_lin(SM->modules[i], i); continue;}
            if (SM->type_modules[i] == 1) {affiche_mod_tdl(SM->modules[i], i); continue;}
            if (SM->type_modules[i] == 2) {affiche_mod_rnn(SM->modules[i], i); continue;}
            if (SM->type_modules[i] == 3) {affiche_mod_gru(SM->modules[i], i); continue;}
            if (SM->type_modules[i] == 4) {affiche_mod_lstm(SM->modules[i], i); continue;}
        }
      
}
void set_module_lin (struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*))
{   
    std::cout << "Module linear dim in = " << AI->dim_in << " et dim out = " << AI->dim_out << "\n"; 
    bool b = bool(AI->bias);
    SM->modules[i] = new torch::nn::LinearImpl(torch::nn::LinearOptions(AI->dim_in, AI->dim_out).bias(b));
    SM->forward_mod[i] = inf_forward_linear;
    SM->activation[i] = a[AI->num_activation];
}

void set_weights_lin (struct SoftModele* SM, struct Archi_Info* AI, int i, FILE* fichier)
{
    std::cout << "welcome to set weights lin \n";
    torch::NoGradGuard no_grad;
    torch::nn::LinearImpl* lin = reinterpret_cast<torch::nn::LinearImpl*>(SM->modules[i]);
    std::cout << "lin->weight.size(0) = " << lin->weight.size(0) << "\n";
    std::cout << "lin->weight.size(1) = " << lin->weight.size(1) << "\n";
    char buf [100];
    int y = 0;
    int caractere = fgetc(fichier);

    while (caractere != '[') {caractere = fgetc(fichier);} 

    for (int r = 0; r < lin->weight.size(0); ++r)
        {
            for (int c = 0; c < lin->weight.size(1); ++c)
                {   
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    lin->weight.index_put_({r, c}, val);  // assignation
                }
        }


    if (AI->bias == 1)
        {
            while (caractere != '[') {caractere = fgetc(fichier);} 
            for (int j = 0; j < lin->bias.size(0); ++j)
                {
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    lin->bias.index_put_({j}, val);  // assignation                    
                }
        }
}



void set_module_tdl (struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*))
{   
    bool b = bool(AI->bias);
    SM->modules[i] = new TimeDistributedLinear(AI->dim_in, AI->dim_out, b);
    SM->forward_mod[i] = inf_forward_tdl;
    SM->activation[i] = a[AI->num_activation];
}

void set_weights_tdl (struct SoftModele* SM, struct Archi_Info* AI, int i, FILE* fichier)
{   
    torch::NoGradGuard no_grad;
    TimeDistributedLinear* tdl = reinterpret_cast<TimeDistributedLinear*>(SM->modules[i]);

    char buf [100];
    int y = 0;
    int caractere = fgetc(fichier);

    while (caractere != '[') {caractere = fgetc(fichier);} 

    for (int r = 0; r < tdl->linear->weight.size(0); ++r)
        {
            for (int c = 0; c < tdl->linear->weight.size(1); ++c)
                {   
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    tdl->linear->weight.index_put_({r, c}, val);  // assignation
                }
        }


    if (AI->bias == 1)
        {
            while (caractere != '[') {caractere = fgetc(fichier);} 
            for (int j = 0; j < tdl->linear->bias.size(0); ++j)
                {
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    tdl->linear->bias.index_put_({j}, val);  // assignation                    
                }
        }
}



void set_module_rnn (struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*))
{   
    bool b = bool(AI->bias);
    SM->modules[i] =  new torch::nn::RNNImpl(torch::nn::RNNOptions(AI->dim_in, AI->dim_out).bias(b).batch_first(true));
    if (AI->special == 0)
        {
            SM->forward_mod[i] = inf_forward_rnn_0;
        }

    if (AI->special == 1)
        {
            SM->forward_mod[i] = inf_forward_rnn_1;
        }    
    SM->activation[i] = a[AI->num_activation];
}


void set_weights_rnn (struct SoftModele* SM, struct Archi_Info* AI, int i, FILE* fichier)
{
    torch::NoGradGuard no_grad;
    torch::nn::RNNImpl* rnn = reinterpret_cast<torch::nn::RNNImpl*>(SM->modules[i]);

    char buf [100];
    int y = 0;
    int caractere = fgetc(fichier);

    auto params = rnn->named_parameters();
    torch::Tensor* w_ih = &params["weight_ih_l0"];

    while (caractere != '[') {caractere = fgetc(fichier);} 

    for (int r = 0; r < w_ih->size(0); ++r)
        {
            for (int c = 0; c < w_ih->size(1); ++c)
                {   
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    w_ih->index_put_({r, c}, val);  // assignation
                }
        }

    torch::Tensor* w_hh = &params["weight_hh_l0"];

    while (caractere != '[') {caractere = fgetc(fichier);} 

    for (int r = 0; r < w_hh->size(0); ++r)
        {
            for (int c = 0; c < w_hh->size(1); ++c)
                {   
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    w_hh->index_put_({r, c}, val);  // assignation
                }
        }

    if (AI->bias == 1)
        {
            torch::Tensor* b_ih = &params["bias_ih_l0"];
        
            while (caractere != '[') {caractere = fgetc(fichier);} 
            for (int j = 0; j < b_ih->size(0); ++j)
                {
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    b_ih->index_put_({j}, val);  // assignation                    
                }

            torch::Tensor* b_hh = &params["bias_hh_l0"];


            while (caractere != '[') {caractere = fgetc(fichier);} 
            for (int j = 0; j < b_ih->size(0); ++j)
                {
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    b_hh->index_put_({j}, val);  // assignation                    
                }
        }
}




void set_module_gru (struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*))
{   
    bool b = bool(AI->bias);
    SM->modules[i] =  new torch::nn::GRUImpl(torch::nn::GRUOptions(AI->dim_in, AI->dim_out).bias(b).batch_first(true));
    if (AI->special == 0)
        {
            SM->forward_mod[i] = inf_forward_gru_0;
        }

    if (AI->special == 1)
        {
            SM->forward_mod[i] = inf_forward_gru_1;
        }    
    SM->activation[i] = a[AI->num_activation];
}


void set_weights_gru (struct SoftModele* SM, struct Archi_Info* AI, int i, FILE* fichier)
{
    torch::NoGradGuard no_grad;
    torch::nn::GRUImpl* gru = reinterpret_cast<torch::nn::GRUImpl*>(SM->modules[i]);

    char buf [100];
    int y = 0;
    int caractere = fgetc(fichier);

    auto params = gru->named_parameters();
    torch::Tensor* w_ih = &params["weight_ih_l0"];

    while (caractere != '[') {caractere = fgetc(fichier);} 

    for (int r = 0; r < w_ih->size(0); ++r)
        {
            for (int c = 0; c < w_ih->size(1); ++c)
                {   
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    w_ih->index_put_({r, c}, val);  // assignation
                }
        }

    torch::Tensor* w_hh = &params["weight_hh_l0"];

    while (caractere != '[') {caractere = fgetc(fichier);} 

    for (int r = 0; r < w_hh->size(0); ++r)
        {
            for (int c = 0; c < w_hh->size(1); ++c)
                {   
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    w_hh->index_put_({r, c}, val);  // assignation
                }
        }

    if (AI->bias == 1)
        {
            torch::Tensor* b_ih = &params["bias_ih_l0"];
        
            while (caractere != '[') {caractere = fgetc(fichier);} 
            for (int j = 0; j < b_ih->size(0); ++j)
                {
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    b_ih->index_put_({j}, val);  // assignation                    
                }

            torch::Tensor* b_hh = &params["bias_hh_l0"];


            while (caractere != '[') {caractere = fgetc(fichier);} 
            for (int j = 0; j < b_ih->size(0); ++j)
                {
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    b_hh->index_put_({j}, val);  // assignation                    
                }
        }
}




void set_module_lstm (struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*))
{   
    bool b = bool(AI->bias);
    SM->modules[i] =  new torch::nn::GRUImpl(torch::nn::GRUOptions(AI->dim_in, AI->dim_out).bias(b).batch_first(true));
    if (AI->special == 0)
        {
            SM->forward_mod[i] = inf_forward_gru_0;
        }

    if (AI->special == 1)
        {
            SM->forward_mod[i] = inf_forward_gru_1;
        }    
    SM->activation[i] = a[AI->num_activation];
}

void set_weights_lstm (struct SoftModele* SM, struct Archi_Info* AI, int i, FILE* fichier)
{
    torch::NoGradGuard no_grad;
    torch::nn::LSTMImpl* lstm = reinterpret_cast<torch::nn::LSTMImpl*>(SM->modules[i]);

    char buf [100];
    int y = 0;
    int caractere = fgetc(fichier);

    auto params = lstm->named_parameters();
    torch::Tensor* w_ih = &params["weight_ih_l0"];

    while (caractere != '[') {caractere = fgetc(fichier);} 

    for (int r = 0; r < w_ih->size(0); ++r)
        {
            for (int c = 0; c < w_ih->size(1); ++c)
                {   
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    w_ih->index_put_({r, c}, val);  // assignation
                }
        }

    torch::Tensor* w_hh = &params["weight_hh_l0"];

    while (caractere != '[') {caractere = fgetc(fichier);} 

    for (int r = 0; r < w_hh->size(0); ++r)
        {
            for (int c = 0; c < w_hh->size(1); ++c)
                {   
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    w_hh->index_put_({r, c}, val);  // assignation
                }
        }

    if (AI->bias == 1)
        {
            torch::Tensor* b_ih = &params["bias_ih_l0"];
        
            while (caractere != '[') {caractere = fgetc(fichier);} 
            for (int j = 0; j < b_ih->size(0); ++j)
                {
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    b_ih->index_put_({j}, val);  // assignation                    
                }

            torch::Tensor* b_hh = &params["bias_hh_l0"];


            while (caractere != '[') {caractere = fgetc(fichier);} 
            for (int j = 0; j < b_ih->size(0); ++j)
                {
                    while ((caractere = fgetc(fichier)) != ' ') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
                    float val = std::stof(buf);  // conversion string -> float
                    b_hh->index_put_({j}, val);  // assignation                    
                }
        }
}



void import_modele_file(struct Genuse* GU)
{

    torch::Tensor (*activations[10])(torch::Tensor*); 
    activations[0] = simple_none;
    activations[1] = simple_relu;
    activations[2] = simple_tanh;
    activations[3] = simple_sigmoid;
    activations[4] = simple_leakyrelu;

    void (*set_modules[10])(struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*));
    set_modules[0] = set_module_lin;
    set_modules[1] = set_module_tdl;
    set_modules[2] = set_module_rnn;
    set_modules[3] = set_module_gru;
    set_modules[4] = set_module_lstm;

    void (*set_weigths[10])(struct SoftModele*, struct Archi_Info*, int , FILE*);
    set_weigths[0] = set_weights_lin;
    set_weigths[1] = set_weights_tdl;
    set_weigths[2] = set_weights_rnn;
    set_weigths[3] = set_weights_gru;
    set_weigths[4] = set_weights_lstm;

    FILE *fichier = fopen(GU->path_info_model, "r");

    char buf [1000];
    int y = 0;
    int caractere = fgetc(fichier);
    GU->SM = (struct SoftModele*)malloc(sizeof(struct SoftModele));
    
    struct Archi_Info AI;

    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
    GU->SM->nb_layers = (uint16_t)atoi(buf);
    GU->SM->type_modules = (uint16_t*)malloc(GU->SM->nb_layers * sizeof(uint16_t));
    GU->SM->modules = (void**)malloc(GU->SM->nb_layers * sizeof(void*));
    GU->SM->activation = (torch::Tensor (**)(torch::Tensor*)) malloc(GU->SM->nb_layers * sizeof(torch::Tensor (*)(torch::Tensor*)));
    GU->SM->forward_mod = (torch::Tensor (**)(void*, torch::Tensor*)) malloc(GU->SM->nb_layers * sizeof(torch::Tensor (*)(void*, torch::Tensor*)));

    for (int i = 0; i < GU->SM->nb_layers; ++i)
        {
            while (caractere != '#') {caractere = fgetc(fichier);} 
            while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
            AI.dim_in = atoi(buf);
            std::cout << "AI.dim_in = " << AI.dim_in << "\n";
            while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
            std::cout << "buf = " << std::string(buf) << "\n";
            AI.dim_out = atoi(buf);
            std::cout << "AI.dim_out = " << AI.dim_out << "\n";
            while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
            AI.type_module = atoi(buf);
            GU->SM->type_modules[i] = AI.type_module;
            std::cout << "AI.type_module = " << AI.type_module << "\n";
            while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
            AI.num_activation = atoi(buf);
            std::cout << "AI.num_activation = " << AI.num_activation << "\n";
            while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
            AI.bias = atoi(buf);
            std::cout << "AI.bias = " << AI.bias << "\n";
            while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
            AI.special = atoi(buf);
            std::cout << "AI.special = " << AI.special << "\n";
            set_modules[AI.type_module](GU->SM, &AI, i, activations);
            std::cout << "Apres set modules \n";
            set_weigths[AI.type_module](GU->SM, &AI, i, fichier);
            std::cout << "Apres set weighs \n";
        }

    fclose(fichier);
    fichier = NULL;
    affiche_modele(GU->SM);

}