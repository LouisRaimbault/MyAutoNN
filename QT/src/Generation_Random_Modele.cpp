#include "../include/Generation_Random_Modele.h"
#include "../include/QT_General.h"

void fprintf_float_dot2(FILE* f, const char* fmt, double x)
{
    char buf[64];

    snprintf(buf, sizeof(buf), fmt, x);

    for (char* p = buf; *p; ++p)
    {
        if (*p == ',') {*p = '.'; break;}
    }

    fprintf(f, "%s", buf);
}

int rand_int(int min, int max)
{
    if (max < min)
    {
        int tmp = min;
        min = max;
        max = tmp;
    }

    return min + rand() % (max - min + 1);
}
 
float rand_float(float min, float max)
{
    if (max < min)
    {
        float tmp = min;
        min = max;
        max = tmp;
    }

    return min + ((float)rand() / (float)RAND_MAX) * (max - min);
}



void rdm_standard_mod (struct Rdm_Modele* RM, struct Rdm_Modele_Info* RMI)
{
    RM->learning_rate = rand_float(RMI->range_learning_rate[0], RMI->range_learning_rate[1]);
    RM->nb_epoch = rand_int(RMI->range_nb_epoch[0], RMI->range_nb_epoch[1]);
    RM->optimizer = RMI->choices_optimizer[rand_int(0, RMI->nb_choices_cost_optimizer - 1)];
    RM->cost_function = RMI->choices_cost_function[rand_int(0, RMI->nb_choices_cost_function - 1)];
    RM->nb_layers = rand_int(RMI->range_nb_layers[0], RMI->range_nb_layers[1]);
    RM->dim_in = (int*)malloc(RM->nb_layers * sizeof(int));
    RM->dim_out = (int*)malloc(RM->nb_layers * sizeof(int));
    RM->bias = (int*)malloc(RM->nb_layers * sizeof(int));
    RM->module = (int*)malloc(RM->nb_layers * sizeof(int));  
    RM->activation_function = (int*)malloc(RM->nb_layers * sizeof(int));  
    RM->special = (int*)calloc(RM->nb_layers, sizeof(int));  
        
    for (int i = 0; i < RM->nb_layers; ++i)
        {   
            RM->module[i] = RMI->choices_module[rand_int(0, RMI->nb_choices_module - 1)];
            if (i == 0)
                {
                    RM->dim_out[i] = rand_int(RMI->range_dim[0], RMI->range_dim[1]);
                    RM->bias[i] = 0;
                    int p = rand_int(0, 100);
                    if (p < RMI->add_bias_proba) {RM->bias[i] = 1;}
                    RM->activation_function[i] = RMI->choices_activation_function[rand_int(0, RMI->nb_choices_activation_function - 1)];

                    if (RM->nb_layers == 1)
                        {
                            
                            if (RMI->activ_last_layers == 0)
                                {
                                    RM->activation_function[i] = 0;
                                }

                        }
                    continue;
                }

            RM->dim_in[i] = RM->dim_out[i - 1];
            RM->dim_out[i] = rand_int(RMI->range_dim[0], RMI->range_dim[1]);
            RM->bias[i] = 0;
            int p = rand_int(0, 100);
            if (p < RMI->add_bias_proba) {RM->bias[i] = 1;}
            RM->activation_function[i] = RMI->choices_activation_function[rand_int(0, RMI->nb_choices_activation_function - 1)];

            if (i == RM->nb_layers - 1 && RMI->activ_last_layers == 0)
                {
                    RM->activation_function[i] = 0;
                }
        }               
        
}

void rdm_sequence_mod (struct Rdm_Modele* RM, struct Rdm_Modele_Info* RMI)
{
    RM->learning_rate = rand_float(RMI->range_learning_rate[0], RMI->range_learning_rate[1]);
    RM->nb_epoch = rand_int(RMI->range_nb_epoch[0], RMI->range_nb_epoch[1]);
    RM->optimizer = RMI->choices_optimizer[rand_int(0, RMI->nb_choices_cost_optimizer - 1)];
    RM->cost_function = RMI->choices_cost_function[rand_int(0, RMI->nb_choices_cost_function - 1)];
    RM->nb_layers = rand_int(RMI->range_nb_layers[0], RMI->range_nb_layers[1]);
    RM->dim_in = (int*)malloc(RM->nb_layers * sizeof(int));
    RM->dim_out = (int*)malloc(RM->nb_layers * sizeof(int));
    RM->bias = (int*)malloc(RM->nb_layers * sizeof(int));
    RM->module = (int*)malloc(RM->nb_layers * sizeof(int));  
    RM->activation_function = (int*)malloc(RM->nb_layers * sizeof(int));  
    RM->special = (int*)calloc(RM->nb_layers, sizeof(int));  

    if (RMI->is_modele_autoencodeur == 1)
        {
            for (int i = 0; i < RM->nb_layers; ++i)
                {   
                    RM->module[i] = RMI->choices_module[rand_int(0, RMI->nb_choices_module - 1)];
                    if (i == 0)
                        {
                            RM->dim_out[i] = rand_int(RMI->range_dim[0], RMI->range_dim[1]);
                            RM->bias[i] = 0;
                            int p = rand_int(0, 100);
                            if (p < RMI->add_bias_proba) {RM->bias[i] = 1;}
                            RM->activation_function[i] = RMI->choices_activation_function[rand_int(0, RMI->nb_choices_activation_function - 1)];

                            if (RM->nb_layers == 1)
                                {
                                    
                                    if (RMI->activ_last_layers == 0)
                                        {
                                            RM->activation_function[i] = 0;
                                        }

                                }

                            continue;
                        }

                    RM->dim_in[i] = RM->dim_out[i - 1];
                    RM->dim_out[i] = rand_int(RMI->range_dim[0], RMI->range_dim[1]);
                    RM->bias[i] = 0;
                    int p = rand_int(0, 100);
                    if (p < RMI->add_bias_proba) {RM->bias[i] = 1;}
                    RM->activation_function[i] = RMI->choices_activation_function[rand_int(0, RMI->nb_choices_activation_function - 1)];

                    if (i == RM->nb_layers - 1 && RMI->activ_last_layers == 0)
                        {
                            RM->activation_function[i] = 0;
                        }
                }  

            return;              
        }


    uint16_t broken_seq =0;
    int pct_break_sequence = RMI->pct_break_sequence;
        
    if (RMI->is_modele_autoencodeur == 0)
        {
            for (int i = 0; i < RM->nb_layers; ++i)
                {   

                    RM->module[i] = RMI->choices_module[rand_int(0, RMI->nb_choices_module - 1)];
                    if (broken_seq == 1)
                        {
                            RM->module[i] = 0;
                        }

                    if (broken_seq == 0)
                        {
                            int p1 = rand_int(0, 100);
                            if (p1 < pct_break_sequence) 
                                {
                                 while (RM->module[i] == 0)
                                    {
                                        RM->module[i] = RMI->choices_module[rand_int(0, RMI->nb_choices_module - 1)];
                                    }                                   
                                    broken_seq = 1;
                                    RM->special[i] = 1;
                                }
                            pct_break_sequence = pct_break_sequence + RMI->evo_break_sequence;                                                        
                        }

                    if (i == RM->nb_layers - 1 && broken_seq == 0) 
                        {
                            while (RM->module[i] == 0)
                                {
                                    RM->module[i] = RMI->choices_module[rand_int(0, RMI->nb_choices_module - 1)];
                                }
                            RM->special[i] = 1;
                        }


                    if (i == 0)
                        {
                            RM->dim_out[i] = rand_int(RMI->range_dim[0], RMI->range_dim[1]);
                            RM->bias[i] = 0;
                            int p2 = rand_int(0, 100);
                            if (p2 < RMI->add_bias_proba) {RM->bias[i] = 1;}
                            RM->activation_function[i] = RMI->choices_activation_function[rand_int(0, RMI->nb_choices_activation_function - 1)];

                            if (RM->nb_layers == 1)
                                {
                                    
                                    if (RMI->activ_last_layers == 0)
                                        {
                                            RM->activation_function[i] = 0;
                                        }

                                }
                            continue;
                        }

                    RM->dim_in[i] = RM->dim_out[i - 1];
                    RM->dim_out[i] = rand_int(RMI->range_dim[0], RMI->range_dim[1]);
                    RM->bias[i] = 0;
                    int p = rand_int(0, 100);
                    if (p < RMI->add_bias_proba) {RM->bias[i] = 1;}
                    RM->activation_function[i] = RMI->choices_activation_function[rand_int(0, RMI->nb_choices_activation_function - 1)];

                    if (i == RM->nb_layers - 1 && RMI->activ_last_layers == 0)
                        {
                            RM->activation_function[i] = 0;
                        }
                }  

            return;              
        }

}


void gen_random_modele (struct Rdmgen_Modele_Window* RMW)
{
    RMW->RW->tab_modele = (struct Rdm_Modele*)malloc(RMW->RM.nb_modele * sizeof(struct Rdm_Modele));
    void (*rdm_modele_funcs[3]) (struct Rdm_Modele*, struct Rdm_Modele_Info*);
    RMW->RW->RMI.is_modele_autoencodeur = RMW->RM.is_modele_autoencodeur;
    rdm_modele_funcs[0] = rdm_standard_mod;
    rdm_modele_funcs[1] = rdm_sequence_mod;

    for (int i = 0; i < RMW->RM.nb_modele; ++i)
        {
            rdm_modele_funcs[RMW->RM.target_model](&(RMW->RW->tab_modele[i]), &(RMW->RW->RMI));
        }


}





void write_configfile_rdm_modele (struct Rdmgen_Modele_Window* RMW)
{
    std::cout << "Debut d'écriture du fichier de configuration \n";
    size_t len = strlen(RMW->RM.config_folder) + 1 + strlen(RMW->RM.config_filename) + 1; // "/" + '\0'

    char* filename = (char*)malloc(len * sizeof(char));
    snprintf(filename, len, "%s/%s", RMW->RM.config_folder, RMW->RM.config_filename);

    FILE* f = fopen(filename, "w");

    /* ================= DATA CONFIG ================= */
    fprintf(f, "+------+ Data Config +------+\n\n");

    fprintf(f, "path to file #%s,\n", RMW->RM.path_database);
    fprintf(f, "Target model #%d,\n", RMW->RM.target_model);
    fprintf(f, "Nombre de colonnes cibles #%d,\n", RMW->RM.nb_col_target);
    fprintf(f, "is_modele_autoencodeur #%d,\n", RMW->RM.is_modele_autoencodeur);
    fprintf(f, "Normalize X #%d,\n", RMW->RM.normalize_X);
    fprintf(f, "Normalize Y #%d,\n", RMW->RM.normalize_Y);
    fprintf(f, "ratio_train #");
    fprintf_float_dot2(f, "%.3f", RMW->RM.ratio_train);
    fprintf(f, ",\n\n");

    /* ================= BATCH CONFIG ================= */
    fprintf(f, "+------+ Batchs Config +------+\n\n");

    fprintf(f, "Nb_Batch #%d,\n", RMW->RM.nb_batch);
    fprintf(f, "Size_Batch #%d,\n", RMW->RM.size_batch);
    fprintf(f, "Size_Seq #%d,\n\n", RMW->RM.size_seq);

    /* ================= ARCHITECTURE CONFIG ================= */
    fprintf(f, "+------+ Architecture Config +------+\n\n");
    fprintf(f, "Nb Modeles #%d,\n\n", RMW->RM.nb_modele);

    /* ================= MODELS ================= */
    for (int m = 0; m < RMW->RM.nb_modele; ++m)
    {
            struct Rdm_Modele* model = &(RMW->RW->tab_modele[m]);

            fprintf(f, "+-+ Modele %d +-+\n\n", m + 1);
            fprintf(f, "nb_Layer #%d,\n\n", model->nb_layers);

            for (int l = 0; l < model->nb_layers; ++l)
                {
                    if (l == 0)
                        {
                            fprintf(f, "+ Input Layer +\n");
                        }
                    else if (l == model->nb_layers - 1)
                            {
                                fprintf(f, "+ Output Layer +\n");
                            }

                    else {fprintf(f, "+ Hidden Layer %d +\n", l);}

                    fprintf(f, "type_module #%d,\n", model->module[l]);
                    fprintf(f, "special #%d,\n", model->special[l]);

                    if (l != model->nb_layers -1) {fprintf(f, "dim output #%d,\n", model->dim_out[l]);}

                    fprintf(f, "activ function #%d,\n", model->activation_function[l]);
                    fprintf(f, "bias #%d,\n\n", model->bias[l]);
                }

            fprintf(f, "learning rate #");
            fprintf_float_dot2(f, "%.6f", model->learning_rate);
            fprintf(f, ",\n");
            fprintf(f, "optimizer #%d,\n", model->optimizer);
            fprintf(f, "cost function #%d,\n", model->cost_function);
            fprintf(f, "initialize weight #0,\n");
            fprintf(f, "nb_epch #%d,\n\n", model->nb_epoch);

            fprintf(f, "------------------------------------------------\n");
    }

    

    fclose(f);
    free(filename);

    std::cout << "Fin de l'écriture de fichier \n";
}

