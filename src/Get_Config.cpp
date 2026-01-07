#include "../include/Get_Config.h"
#include "../include/Usefull_Functions.h"
#include "../include/Modele.h"
#include "../include/Get_Data.h"
#include "../include/Get_Batch.h"
#include "../include/Activation_Functions.h"
#include "../include/Cost_Functions.h"
#include "../include/Export_Latex.h"


void print_modele(Modele* M)
{
	const char* types_module[] =
	    {
	        "LIN ",
	        "RNN",
	        "GRU",
	        "LSTM"
	    };


    const char* noms_optims[] = 
	    {
	        "SGD",
	        "Adam",
	        "AdamW",
	        "RMSprop",
	        "Adagrad",
	        "LBFGS"
	    };

    const char* types_special[] = 
	    {
	        "none",
	        "Seq to lin"
	    };

	printf("\n\n -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
	printf("\n\n -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
	printf("\n╔════════════════════════════════════════════════════════════════════════╗\n");
	printf("║                      Informations du modèle                             ║\n");
	printf("╚════════════════════════════════════════════════════════════════════════╝\n\n");

	// --- Table des couches ---
	printf("┌────────────┬────────┬────────┬──────────────┬───────────────┬──────┬──────────────┐\n");
	printf("│ Layer Num  │ Dim In │ Dim Out│ Type Layer   │ Activation    │ Bias │ Special      │\n");
	printf("┌────────────┬────────┬────────┬──────────────┬───────────────┬──────┬──────────────┐\n");


	for (int i = 0; i < M->nb_layers; ++i)
	{	

	    struct Layer* L = &M->tab_layers[i];
	    printf("│ %-10d │ %-6d │ %-6d │ %-12s │ %-13s │ %-4d │ %-12s │\n",
	           i + 1, L->dim_in, L->dim_out,
	           types_module[L->type_module],
	           M->_choices->name_activ[i],
	           L->bias,
	           types_special[L->special]);
	}

	printf("├────────────┼────────┼────────┼──────────────┼───────────────┼──────┼──────────────┤\n");

    // --- Infos globales ---
    printf("Learning rate : %.6f\n", M->OC.learning_rate);
    printf("Optimizer     : %s\n", noms_optims[M->num_optimizer]);
    printf("Cost function : %s\n", M->_choices->name_cost);
    printf("Number of epochs : %d\n", M->nb_epoch);
    printf("Number of layers : %d\n\n", M->nb_layers);
}




void get_config_from_configfile (const char* filename, struct Genstruct* GS)
{

	const char* activation_name[10] = 
	{
	    "none",
	    "relu",
	    "tanh",
	    "sigmoid",
	    "leakyrelu",
	};


	torch::Tensor (*activations[10])(torch::Tensor*);	
	activations[0] = simple_none;
	activations[1] = simple_relu;
	activations[2] = simple_tanh;
	activations[3] = simple_sigmoid;
	activations[4] = simple_leakyrelu;

	torch::Tensor (*costs[10])(torch::Tensor*, torch::Tensor*);	
	costs[0] = simple_mse_cost;
	costs[1] = simple_l1_cost;
	costs[2] = simple_smoothl1_cost;
	costs[3] = simple_crossentropy_cost;
	costs[4] = simple_BCEWithLogits_cost;
	costs[5] = simple_nll_cost;
	costs[6] = sequence_crossentropy_cost;

	const char* cost_name[10] = 
	{
	    "mse",
	    "l1",
	    "smoothl1",
	    "cross entropy",
	    "BCE with logits",
	    "nll",
	    "sequence cross entropy"
	};

	int choice;

    FILE *fichier = fopen(filename, "r");
    if (fichier == NULL) 
    {
        perror("Erreur lors de l'ouverture du fichier");
        return ;  // Code d'erreur
    }

    char buf [1000];
	int y = 0;
    int caractere = fgetc(fichier);

    // Section Recuperation base de donnees 
    
    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
    GS->PD->path = strdup(buf);

   
    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
    GS->PD->type_targetmodel = (uint16_t)atoi(buf);

    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;	    
    GS->TD->nb_col_target = (uint16_t)atoi(buf);

    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;	    
    GS->TD->is_autoencodeur = (uint16_t)atoi(buf);

    
    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
    GS->TD->do_std_x = (uint16_t)atoi(buf);

    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
    GS->TD->do_std_y = (uint16_t)atoi(buf);
    
    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
    GS->TD->ratio_train = atof(buf);

    // Section batches
    
    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
    GS->TB->nb_batch = (uint32_t)atoi(buf);
    
    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
    GS->TB->size_batch = (uint32_t)atoi(buf);

    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;	    
    GS->TB->size_seq = (uint32_t)atoi(buf);



    // Section Modele
    while (caractere != '#') {caractere = fgetc(fichier);} 
    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
    GS->nb_modele = (uint16_t)atoi(buf);
    
    GS->tab_modeles = new Modele[GS->nb_modele];

    for (int i = 0; i < GS->nb_modele; ++i)
    	{
		    Modele* M = &(GS->tab_modeles[i]); 
		    M->_choices = (struct choices*)malloc(sizeof(struct choices));

		    while (caractere != '#') {caractere = fgetc(fichier);} 
		    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
		    M->nb_layers = (uint16_t)atoi(buf);
		    M->tab_layers = new Layer[M->nb_layers];
		    M->_choices->name_activ = (char**)malloc((M->nb_layers) * sizeof(char*));
			
			for (int j = 0; j < GS->tab_modeles[i].nb_layers; ++j)
				{
					
					struct Layer* L = &(M->tab_layers[j]);

					if (j == 0)
						{
				            if (M->nb_layers == 1)
					            {
									while (caractere != '#') {caractere = fgetc(fichier);} 
				    				while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
				    				L->type_module = atoi(buf);		

				    				while (caractere != '#') {caractere = fgetc(fichier);} 
								    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
								    L->special = (uint8_t)atoi(buf);

								    while (caractere != '#') {caractere = fgetc(fichier);} 
								    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
								    L->num_activation = (uint16_t)atoi(buf); choice = atoi(buf);
								    L->activation = activations[choice];
								    M->_choices->name_activ[j] = (char*)malloc(strlen(activation_name[choice]) + 1);
									strcpy(M->_choices->name_activ[j], activation_name[choice]);	

				    				while (caractere != '#') {caractere = fgetc(fichier);} 
								    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
								    L->bias = (uint8_t)atoi(buf);
					                continue;
					            }

							while (caractere != '#') {caractere = fgetc(fichier);} 
		    				while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
		    				L->type_module = atoi(buf);	

    						while (caractere != '#') {caractere = fgetc(fichier);} 
						    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
						    L->special = (uint8_t)atoi(buf);	

						    while (caractere != '#') {caractere = fgetc(fichier);} 
						    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
						    L->dim_out = atoi(buf);

						    while (caractere != '#') {caractere = fgetc(fichier);} 
						    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
						    L->num_activation = (uint16_t)atoi(buf); choice = atoi(buf);
						    L->activation = activations[choice];
						    M->_choices->name_activ[j] = (char*)malloc(strlen(activation_name[choice]) + 1);
							strcpy(M->_choices->name_activ[j], activation_name[choice]);

		    				while (caractere != '#') {caractere = fgetc(fichier);} 
						    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
						    L->bias = (uint8_t)atoi(buf);	

							continue;			
						}

			        if (j == M->nb_layers - 1)
				        {  
							L->dim_in = M->tab_layers[j - 1].dim_out;
							while (caractere != '#') {caractere = fgetc(fichier);} 
		    				while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
		    				L->type_module = atoi(buf);	
		    					

    						while (caractere != '#') {caractere = fgetc(fichier);} 
						    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
						    L->special = (uint8_t)atoi(buf);	
						    
						    while (caractere != '#') {caractere = fgetc(fichier);} 
						    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
						    L->num_activation = (uint16_t)atoi(buf); choice = atoi(buf);
						    
						    L->activation = activations[choice];
						    M->_choices->name_activ[j] = (char*)malloc(strlen(activation_name[choice]) + 1);
							strcpy(M->_choices->name_activ[j], activation_name[choice]);

				    		while (caractere != '#') {caractere = fgetc(fichier);} 
							while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
							L->bias = (uint8_t)atoi(buf);								
			                
			                continue;
				        }
				   
				    L->dim_in = M->tab_layers[j - 1].dim_out;
		    		while (caractere != '#') {caractere = fgetc(fichier);} 
    				while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
    				L->type_module = atoi(buf);	
    				
					while (caractere != '#') {caractere = fgetc(fichier);} 
				    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
				    L->special = (uint8_t)atoi(buf);		
				    
				    while (caractere != '#') {caractere = fgetc(fichier);} 
				    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
				    L->dim_out = atoi(buf);
				    
				    while (caractere != '#') {caractere = fgetc(fichier);} 
				    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
				    L->num_activation = (uint16_t)atoi(buf); choice = atoi(buf);
				    L->activation = activations[choice];
				    M->_choices->name_activ[j] = (char*)malloc(strlen(activation_name[choice]) + 1);
					strcpy(M->_choices->name_activ[j], activation_name[choice]);

    				while (caractere != '#') {caractere = fgetc(fichier);} 
				    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
				    L->bias = (uint8_t)atoi(buf);	

				}
				
		    while (caractere != '#') {caractere = fgetc(fichier);} 
		    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
		    M->OC.learning_rate = atof(buf);

		    	
		    while (caractere != '#') {caractere = fgetc(fichier);} 
		    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
		    M->num_optimizer = (uint16_t)atoi(buf);

		    	
		    while (caractere != '#') {caractere = fgetc(fichier);} 
		    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
		    choice = atoi(buf);
		    M->costfunc = costs[choice];
		    M->_choices->name_cost = (char*)malloc(strlen(cost_name[choice]) + 1);
			strcpy(M->_choices->name_cost, cost_name[choice]);

				
		    while (caractere != '#') {caractere = fgetc(fichier);} 
		    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
		    M->num_weigth_init = (uint16_t)atoi(buf);

		    while (caractere != '#') {caractere = fgetc(fichier);} 
		    while ((caractere = fgetc(fichier)) != ',') {buf[y++] = caractere;} buf[y] = '\0'; y = 0;
		    M->nb_epoch = (uint32_t)atoi(buf);

    	}

    fclose(fichier);

}



void get_config_from_data (struct Genstruct* GS)
{

	if (GS->TD->nb_col_target == 1)
		{
			for (int i = 0; i < GS->nb_modele; ++i)
				{
					GS->tab_modeles[i].tab_layers[0].dim_in = GS->TD->nb_modalites_D;

					GS->tab_modeles[i].tab_layers[GS->tab_modeles[i].nb_layers-1].dim_out = GS->TD->nb_modalites_T;
					
				}
		}

	if (GS->TD->nb_col_target == 0)
		{
			for (int i = 0; i < GS->nb_modele; ++i)
				{
					GS->tab_modeles[i].tab_layers[0].dim_in = GS->TD->nb_modalites_D;

					GS->tab_modeles[i].tab_layers[GS->tab_modeles[i].nb_layers-1].dim_out = GS->TD->nb_modalites_D;
					
				}
		}		

}