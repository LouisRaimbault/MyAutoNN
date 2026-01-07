#ifndef MODELE
#define MODELE


#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <unistd.h>
#include <sstream>
#include <vector>
#include <random>
#include <limits>
#include <torch/torch.h>
#include <torch/optim/sgd.h>
#include <torch/optim/adam.h>
#include <torch/optim/adamw.h>
#include <torch/optim/rmsprop.h>
#include <torch/optim/adagrad.h>
#include <torch/optim/lbfgs.h>
#include <torch/optim/optimizer.h>
#include <torch/optim/serialize.h>
#include <iomanip>


struct TimeDistributedLinear : torch::nn::Module 
{
    torch::nn::Linear linear{nullptr};
    TimeDistributedLinear(int in_features, int out_features, bool bias = true);
    torch::Tensor forward(const torch::Tensor& x);
};



struct Data_Info_Export 
{
	int nb_features;
	char** name_features;
	int* indice_features;
	int nb_quant;
	float* mean_features;
	float* sd_features;
	float mean_target;
	float sd_target;
};

struct Pure_Data
{
	char* path;
	char*** D;
	char** name_variables;
	int nb_variables;
	int nb_rows;
	uint16_t* type_variables; // 0 char*, 1 float, 2 bool
	uint16_t type_targetmodel; 
	uint32_t* tab_nb_mod;
};

struct Torch_Data
{	
	double ratio_train;
	torch::Tensor* D_train;
	torch::Tensor* D_test;
	int nb_modalites_D;
	torch::Tensor* T_train;
	torch::Tensor* T_test;
	int nb_modalites_T;
	char** name_modalite_T;
	uint16_t type_tgt; // 0 char**, 1 quantitative, 2 booléeene
	int nb_rows_train;
	int nb_rows_test;
	uint16_t do_std_x;
	uint16_t do_std_y;
	uint16_t nb_col_target; 
	uint16_t is_autoencodeur;	
};

struct Torch_Batch
{
	torch::Tensor* array_batch_D;
	torch::Tensor* array_batch_T;
	uint32_t size_batch;
	uint32_t nb_batch;
	uint32_t size_seq;
};


struct Layer 
{
    //char* name_module;
	void* module;   // pointeur vers AnyModule
    int dim_in;
    int dim_out;
    int type_module;  // 0 Linear, 1 RNN, 2 GRU, 3 LSTM
    uint16_t num_activation;
    uint8_t bias;
    uint8_t special;
    torch::Tensor (*activation)(torch::Tensor*);
    torch::Tensor (*forward_mod)(struct Layer* L, torch::Tensor*);
};




struct Quant_Quality
{
	double mse;
	double rmse;
	double mae;
	double r2;
	double mape;
	double pearson;
};


struct Qual_Quality
{
	int nb_classes;
	double precision;
	double accuracy;
	double balanced_accuracy;
	double recall;
	double f1;
	double log_loss;
	double top_k_accuracy;
	uint16_t ** confusion_mat;
};

struct Bool_Quality
{
    double accuracy;        // proportion de prédictions correctes
    double precision;       // TP / (TP + FP)
    double recall;          // TP / (TP + FN)
    double specificity;     // TN / (TN + FP)
    double f1;              // 2 * (precision * recall) / (precision + recall)
    double log_loss;        // log-loss pour classification binaire
    uint32_t TP;            // vrais positifs
    uint32_t TN;            // vrais négatifs
    uint32_t FP;            // faux positifs
    uint32_t FN;            // faux négatifs
};


struct Qualitys
{
	struct Quant_Quality QT;
	struct Qual_Quality QL;
	struct Bool_Quality QB;

};


struct choices
{
	char** name_activ;
	char* name_optim;
	char* name_cost;

};

struct OptimConfig
{
    torch::optim::Optimizer* optimizer;
    float learning_rate;
    float weight_decay;
    float betas;
};


struct Modele
{
	torch::Tensor (*costfunc)(torch::Tensor*, torch::Tensor*);
	struct Qualitys* Q;
	struct choices* _choices;
	struct Layer* tab_layers;
	uint16_t nb_layers;
	uint16_t num_optimizer;
	struct OptimConfig OC;
	uint16_t num_weigth_init;
	uint32_t nb_epoch;
	uint8_t training;

};




struct Genstruct
{
	struct Data_Info_Export* DIE;
	struct Pure_Data* PD;
	struct Torch_Data* TD;
	struct Torch_Batch* TB;
	struct Modele* tab_modeles;
	uint16_t nb_modele;
};


// Modele Module 

// Modele Init 
void clean_qualitys (struct Qualitys* Q);
void clean_choices (struct choices* _choices, int nb_layers);
void clean_layer (struct Layer* L);
void clean_modele (struct Modele* M);
void set_optimizer(Modele* M);
void modele_init(struct Modele* M);
std::vector<torch::Tensor> get_parameters(Modele* M);
void init_layer_module(struct Layer* L, struct Modele* M);

// Modele Train

torch::Tensor forward_linear (struct Layer* L, torch::Tensor *x);
torch::Tensor forward_tdl(Layer* L, torch::Tensor* x);
torch::Tensor forward_rnn (struct Layer* L, torch::Tensor *x);
torch::Tensor forward_gru (struct Layer* L, torch::Tensor *x);
torch::Tensor forward_lstm (struct Layer* L, torch::Tensor *x);
void modele_to(struct Modele* M, torch::Device device);
torch::Tensor forward_pass_batches (struct Modele* M, torch::Tensor* input);
void modele_train(struct Modele* M, struct Torch_Batch* TB, torch::Device device );

// Modele Test
torch::Tensor forward_pass (struct Modele* M, torch::Tensor* input);
torch::Tensor apply_model(struct Modele* M, Torch_Data* TD);
torch::Tensor get_tensor_target(struct Torch_Data* TD);
void add_batch_dim_to_test_sequences(Torch_Data* TD);
void modele_test (struct Modele* M, Torch_Data* TD, uint16_t type_targetmodel);

#endif
