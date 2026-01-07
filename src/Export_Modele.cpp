#include "../include/Export_Modele.h"
#include "../include/Usefull_Functions.h"



void write_weight_linear(FILE* f, struct Layer* L)
{
    if (!L->module) 
        {
            fprintf(f, "poids = [NULL]\n");
            if (L->bias) 
                {
                    fprintf(f, "biais = [NULL]\n");
                }
            return;
        }

    torch::nn::LinearImpl* lin = reinterpret_cast<torch::nn::LinearImpl*>(L->module);

    fprintf(f, "poids = [");
    for (int r = 0; r < lin->weight.size(0); ++r) 
        {
            for (int c = 0; c < lin->weight.size(1); ++c) 
                {
                    fprintf(f, "%.6f ", lin->weight[r][c].item<float>());
                }
        }
    fprintf(f, "]\n");

    if (L->bias) 
        {
            fprintf(f, "biais = [");
            for (int j = 0; j < lin->bias.size(0); ++j) 
                {
                    fprintf(f, "%.6f ", lin->bias[j].item<float>());
                }
            fprintf(f, "]\n");
        }
}

void write_weight_tdl(FILE* f, struct Layer* L)
{
    if (!L->module) 
        {
            fprintf(f, "poids = [NULL]\n");
            if (L->bias) 
                {
                    fprintf(f, "biais = [NULL]\n");
                }
            return;
        }

    TimeDistributedLinear* tdl = reinterpret_cast<TimeDistributedLinear*>(L->module);

    if (!tdl) 
        {
            fprintf(f, "poids = [NULL]\n");
            if (L->bias) {fprintf(f, "biais = [NULL]\n");}
            return;
        }

    // Accès via le linear interne
    torch::Tensor w = tdl->linear->weight;
    fprintf(f, "poids = [");
    for (int r = 0; r < w.size(0); ++r) 
        {
            for (int c = 0; c < w.size(1); ++c) 
                {
                    fprintf(f, "%.6f ", w[r][c].item<float>());
                }
        }
    fprintf(f, "]\n");

    if (L->bias) 
        {
            torch::Tensor b = tdl->linear->bias;
            fprintf(f, "biais = [");
            for (int j = 0; j < b.size(0); ++j) 
                {
                    fprintf(f, "%.6f ", b[j].item<float>());
                }
            fprintf(f, "]\n");
        }
}



void write_weight_rnn(FILE* f, struct Layer* L)
{
    if (!L->module) 
    {
        fprintf(f, "poids = [NULL]\n");
        if (L->bias) fprintf(f, "biais = [NULL]\n");
        return;
    }

    torch::nn::RNNImpl* rnn = reinterpret_cast<torch::nn::RNNImpl*>(L->module);

    auto params = rnn->named_parameters();

    torch::Tensor w_ih = params["weight_ih_l0"];

    torch::Tensor w_hh = params["weight_hh_l0"];

    fprintf(f, "poids_ih = [");
    for (int r = 0; r < w_ih.size(0); ++r) 
        {
            for (int c = 0; c < w_ih.size(1); ++c) 
                {
                    fprintf(f, "%.6f ", w_ih[r][c].item<float>());
                }
        }
    fprintf(f, "]\n");
    fprintf(f, "poids_hh = [");
    for (int r = 0; r < w_hh.size(0); ++r) 
        {
            for (int c = 0; c < w_hh.size(1); ++c) 
                {
                    fprintf(f, "%.6f ", w_hh[r][c].item<float>());
                }
        }
    fprintf(f, "]\n");
    if (L->bias)
    {
        torch::Tensor b_ih = params["bias_ih_l0"];
        torch::Tensor b_hh = params["bias_hh_l0"];

        fprintf(f, "biais_ih = [");
        for (int j = 0; j < b_ih.size(0); ++j) {fprintf(f, "%.6f ", b_ih[j].item<float>());}
        fprintf(f, "]\n");

        fprintf(f, "biais_hh = [");
        for (int j = 0; j < b_hh.size(0); ++j) {fprintf(f, "%.6f ", b_hh[j].item<float>());}
        fprintf(f, "]\n");
    }
}

void write_weight_gru(FILE* f, struct Layer* L)
{
    if (!L->module) 
        {
            fprintf(f, "poids_ih = [NULL]\n");
            fprintf(f, "poids_hh = [NULL]\n");
            if (L->bias) 
                {
                    fprintf(f, "biais_ih = [NULL]\n");
                    fprintf(f, "biais_hh = [NULL]\n");
                }
            return;
        }

    torch::nn::GRUImpl* gru = reinterpret_cast<torch::nn::GRUImpl*>(L->module);

    auto params = gru->named_parameters();

    torch::Tensor w_ih = params["weight_ih_l0"];
    torch::Tensor w_hh = params["weight_hh_l0"];

    fprintf(f, "poids_ih = [");
    for (int r = 0; r < w_ih.size(0); ++r)
        {
            for (int c = 0; c < w_ih.size(1); ++c) 
                {
                    fprintf(f, "%.6f ", w_ih[r][c].item<float>());
                }
        }
    fprintf(f, "]\n");

    fprintf(f, "poids_hh = [");
    for (int r = 0; r < w_hh.size(0); ++r)
        {
            for (int c = 0; c < w_hh.size(1); ++c) 
                {
                    fprintf(f, "%.6f ", w_hh[r][c].item<float>());
                }
        }
    fprintf(f, "]\n");

    if (L->bias) 
    {
        torch::Tensor b_ih = params["bias_ih_l0"];
        torch::Tensor b_hh = params["bias_hh_l0"];

        fprintf(f, "biais_ih = [");
        for (int j = 0; j < b_ih.size(0); ++j) {fprintf(f, "%.6f ", b_ih[j].item<float>());}
        fprintf(f, "]\n");

        fprintf(f, "biais_hh = [");
        for (int j = 0; j < b_hh.size(0); ++j) {fprintf(f, "%.6f ", b_hh[j].item<float>());}
        fprintf(f, "]\n");
    }
}

void write_weight_lstm(FILE* f, struct Layer* L)
{
    if (!L->module) 
        {
            fprintf(f, "poids_ih = [NULL]\n");
            fprintf(f, "poids_hh = [NULL]\n");
            if (L->bias) 
                {
                    fprintf(f, "biais_ih = [NULL]\n");
                    fprintf(f, "biais_hh = [NULL]\n");
                }
            return;
        }

    torch::nn::LSTMImpl* lstm = reinterpret_cast<torch::nn::LSTMImpl*>(L->module);

    auto params = lstm->named_parameters();

    torch::Tensor w_ih = params["weight_ih_l0"];
    torch::Tensor w_hh = params["weight_hh_l0"];

    fprintf(f, "poids_ih = [");
    for (int r = 0; r < w_ih.size(0); ++r)
        {
            for (int c = 0; c < w_ih.size(1); ++c)
                {
                    fprintf(f, "%.6f ", w_ih[r][c].item<float>());
                }
        }
    fprintf(f, "]\n");

    fprintf(f, "poids_hh = [");
    for (int r = 0; r < w_hh.size(0); ++r)
        {   
            for (int c = 0; c < w_hh.size(1); ++c)
                {
                    fprintf(f, "%.6f ", w_hh[r][c].item<float>());
                }
        }   
    fprintf(f, "]\n");

    if (L->bias) {
        torch::Tensor b_ih = params["bias_ih_l0"];
        torch::Tensor b_hh = params["bias_hh_l0"];

        fprintf(f, "biais_ih = [");
        for (int j = 0; j < b_ih.size(0); ++j) {fprintf(f, "%.6f ", b_ih[j].item<float>());}
        fprintf(f, "]\n");

        fprintf(f, "biais_hh = [");
        for (int j = 0; j < b_hh.size(0); ++j) {fprintf(f, "%.6f ", b_hh[j].item<float>());}
        fprintf(f, "]\n");
    }
}

void export_modele_file(struct Modele* M, int i)
{	

	char filename[256];  

    sprintf(filename, "output/modeles/modele_%d.txt", i);

	void (*tab_write_weight[4])(FILE*, struct Layer*) =
		{
    		write_weight_linear,  // type_module 0
    		write_weight_rnn,     // type_module 1
    		write_weight_gru,     // type_module 2
    		write_weight_lstm     // type_module 3
		};

    std::cout << "export modele file 0 \n";
    FILE* f = fopen(filename, "w");
    
    if (!f) 
    {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    fprintf(f, "------ Modele : Architecture et Poids ------\n\n");

    // Nombre total de layers = input + hidden
    fprintf(f, "Nb Layers #%d,\n\n", M->nb_layers);

    for (int i = 0; i < M->nb_layers; ++i)
    {	
        Layer* L = &M->tab_layers[i];

        // Affichage simplifié
        fprintf(f, "layer l%d #%d,%d,%d,%d,%d,%d,\n",
                i,
                L->dim_in,
                L->dim_out,
                L->type_module,
                L->num_activation,
                L->bias,
                L->special
                );
        tab_write_weight[L->type_module](f, L);
        fprintf(f, "\n");
    }

    fclose(f);
}


