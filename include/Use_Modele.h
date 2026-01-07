#ifndef USE_MODELE
#define USE_MODELE

#include "Modele.h"


struct Use_Data
{
	torch::Tensor* D_use;
	torch::Tensor* T_use;
	int nb_rows_use;
	uint16_t do_std_x;
	uint16_t do_std_y;
	uint16_t nb_col_targets; 
	uint16_t is_autoencodeur;
	uint16_t type_targetmodel;
	uint16_t* type_variables;
	uint16_t type_target;
	uint16_t size_seq;	
	int nb_variables;
	int nb_features;
	char** name_variables;
	char** name_features;
	int* indice_features;
	int nb_quant;
	float* mean_features;
	float* sd_features;
	int nb_mod_target;
	char** name_mod_target;
	int* indice_mod_target;
	float mean_target;
	float sd_target;	
};


struct Archi_Info
{
    int dim_in;
    int dim_out;
    int type_module;
    int num_activation;
    int bias;
    int special;  
};


struct SoftModele 
{
    uint16_t nb_layers;
    void** modules; // pointeur générique
    uint16_t* type_modules;
    torch::Tensor (**activation)(torch::Tensor*);
    torch::Tensor (**forward_mod)(void* module, torch::Tensor* input);
    
};

struct Genuse
{
	char* path_base;
	char* path_info_base;
	char* path_info_model;
	struct SoftModele* SM;
	struct Use_Data* UD;
	struct Qualitys* Q;
};



// Section Use Modele Data
void clean_use_data (struct Use_Data* UD);
void printUseData(struct Use_Data* UD);
void get_ud_infos (struct Genuse* GU);
void get_nb_rows_col (struct Genuse* GU);
void set_tenseur_1tgt_no_seq (struct Genuse* GU, FILE* fichier);
void set_tenseur_1tgt_seq (struct Genuse* GU, FILE* fichier);
void get_use_data (struct Genuse* GU);


// Section Use Modele Import
void clean_softmodele (struct SoftModele* SM);
torch::Tensor forward_pass_batches (struct SoftModele* SM, torch::Tensor* input);
void set_module_lin (struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*));
void set_weights_lin (struct SoftModele* SM, struct Archi_Info* AI, int i, FILE* fichier);
void set_module_tdl (struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*));
void set_weights_tdl (struct SoftModele* SM, struct Archi_Info* AI, int i, FILE* fichier);
void set_module_rnn (struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*));
void set_weights_rnn (struct SoftModele* SM, struct Archi_Info* AI, int i, FILE* fichier);
void set_module_gru (struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*));
void set_weights_gru (struct SoftModele* SM, struct Archi_Info* AI, int i, FILE* fichier);
void set_module_lstm (struct SoftModele* SM, struct Archi_Info* AI, int i, torch::Tensor (**a)(torch::Tensor*));
void set_weights_lstm (struct SoftModele* SM, struct Archi_Info* AI, int i, FILE* fichier);


void import_modele_file(struct Genuse* GU);
// Section Use Modele inference
torch::Tensor inf_forward_linear(void* module, torch::Tensor* x);
torch::Tensor inf_forward_tdl(void* module, torch::Tensor* x);
torch::Tensor inf_forward_rnn_0(void* module, torch::Tensor* x);
torch::Tensor inf_forward_rnn_1(void* module, torch::Tensor* x);
torch::Tensor inf_forward_gru_0(void* module, torch::Tensor* x);
torch::Tensor inf_forward_gru_1(void* module, torch::Tensor* x);
torch::Tensor inf_forward_lstm_0(void* module, torch::Tensor* x);
torch::Tensor inf_forward_lstm_1(void* module, torch::Tensor* x);
torch::Tensor use_modele_forward_pass (struct SoftModele* SM, torch::Tensor* input);
torch::Tensor apply_model_use(struct SoftModele* SM, struct Use_Data* UD);
void modele_use (struct Genuse* GU);

#endif
