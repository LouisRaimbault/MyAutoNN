#include "../include/Usefull_Functions.h"
#include "../include/Get_Data.h"
#include "../include/Modele.h"

void clean_torch_data (struct Torch_Data* TD, struct Genstruct* GS)
{
	delete [] TD->D_train;
	delete [] TD->D_test;
	if (GS->TD->nb_col_target != 0) {delete [] TD->T_train;}
	if (GS->TD->nb_col_target != 0 && GS->PD->type_targetmodel != 0 ) {delete [] TD->T_test;}

	free (TD);
}

void affiche_info_datas (struct Genstruct* GS)
{
	printf("          ---- Rappel sur la base %s ---- \n", GS->PD->path);
	
	for (int j = 0; j < GS->PD->nb_variables-1; ++j)
	{
		if (GS->PD->type_variables[j] == 0) {printf("          %s (str[%u])", GS->PD->name_variables[j], GS->PD->tab_nb_mod[j]);}
	    if (GS->PD->type_variables[j] == 1) {printf("          %s (num)", GS->PD->name_variables[j]);}
	}
	printf("\n Nombre total de modalites explciatives : %d\n", GS->TD->nb_modalites_D);
	printf("Information sur la variable cible : \n");
	if (GS->PD->type_variables[GS->PD->nb_variables-1] == 0) {printf("          %s (str[%u])\n", GS->PD->name_variables[GS->PD->nb_variables-1], GS->TD->nb_modalites_T);}
	if (GS->PD->type_variables[GS->PD->nb_variables-1] == 1) {printf("          %s (num)\n", GS->PD->name_variables[GS->PD->nb_variables-1]);}
	printf("\n Nombre de lignes pour la base d'entrainement  : %d\n", GS->TD->nb_rows_train);
	printf("\n Nombre de lignes pour la base de test  : %d\n", GS->TD->nb_rows_test);

	printf("          --------------------------------\n");

}



uint16_t is_float(const char* s) 
{
    if (s == NULL || *s == '\0') 
    {
        return 0;
    }

    char* endptr;
    strtof(s, &endptr);

    if (*endptr == '\0') 
    {
        return 1; 
    } 

    return 0;
}

uint16_t is_booleen(char** fval, int size)
{

    float epsilon = 0.00001;
    for (int i = 0; i < size; i++)
    {
        // On ne garde que 0 et 1
        if (fabsf(atof(fval[i]) - 0.0f) > epsilon && fabsf(atof(fval[i]) - 1.0f) > epsilon )
        	{
        		return 1;
        	}
    }

    return 2; // sinon float normal
}

void get_info_file(struct Pure_Data* PD, const char* filename) 
{
    FILE *fichier = fopen(filename, "r");
    if (fichier == NULL) 
    {
        perror("Erreur lors de l'ouverture du fichier");
        return ;  // Code d'erreur
    }

    int nb_rows = 1;
    int nb_col = 1;
    int caractere;

    while ((caractere = fgetc(fichier)) != '\n')
    	{
    		if (caractere == ',') {nb_col++;}
    	}
    

    while ((caractere = fgetc(fichier)) != EOF) 
    {
        if (caractere == '\n') 
        {
            nb_rows++;
        }
    }

    fclose(fichier);

    PD->nb_rows = nb_rows-2;
    PD->nb_variables = nb_col;
}



void get_pure_data (struct Genstruct* GS)
{
	struct Pure_Data* PD = GS->PD;
	char* filename = GS->PD->path;
	int j = 0;
	get_info_file(PD, filename);
	PD->name_variables = (char**)calloc(PD->nb_variables, sizeof(char*));
	PD->type_variables = (uint16_t*)malloc(PD->nb_variables*sizeof(uint16_t));
	char buf [1000];
	int y = 0;
	int c;
	
    FILE *fichier = fopen(filename, "r");
    if (fichier == NULL) 
    {
        perror("Erreur lors de l'ouverture du fichier");
        return;
    }
   
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Récupération des noms de colonnes -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    for (j = 0; j < PD->nb_variables-1; ++j) 
    	{
    		y = 0;
    		c = fgetc(fichier);
    		while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
    		buf[y] = '\0'; 
    		PD->name_variables[j] = (char*)malloc(strlen(buf) + 1);

			strcpy(PD->name_variables[j], buf);
    	}



    y = 0;
    c = fgetc(fichier);
    while (c != '\n')  {buf[y++] = c; c = fgetc(fichier);}
    buf[y] = '\0'; 
    PD->name_variables[PD->nb_variables-1] = (char*)malloc(strlen(buf) + 1);
	strcpy(PD->name_variables[PD->nb_variables-1], buf);

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

	PD->D = (char***)malloc(PD->nb_variables*sizeof(char**));
	
	for (j = 0; j < PD->nb_variables; ++j)
		{
			PD->D[j] = (char**)calloc(PD->nb_rows, sizeof(char*));
		}

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Récupération types de donnees -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

    for (j = 0; j < PD->nb_variables-1; ++j) 
    	{
    		y = 0;
    		c = fgetc(fichier);
    		while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
    		buf[y] = '\0'; 
    		PD->D[j][0] = (char*)malloc(strlen(buf) + 1);
			strcpy(PD->D[j][0], buf);
			PD->type_variables[j] = is_float(PD->D[j][0]);
    	}

    y = 0;
    c = fgetc(fichier);
    while (c != '\n')  {buf[y++] = c; c = fgetc(fichier);}
    buf[y] = '\0';
	PD->D[PD->nb_variables-1][0] = (char*)malloc(strlen(buf) + 1);
	strcpy(PD->D[PD->nb_variables-1][0], buf);
	PD->type_variables[PD->nb_variables-1] = is_float(PD->D[PD->nb_variables-1][0]);


// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-

// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Récupération Reste de la donnée -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
    
    for (int i = 1; i < PD->nb_rows; ++i)
    	{   
		    for (j = 0; j < PD->nb_variables-1; ++j) 
		    	{
		    		y = 0;
		    		c = fgetc(fichier);
		    		while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
		    		buf[y] = '\0'; 
		    		PD->D[j][i] = (char*)malloc(strlen(buf) + 1);
					strcpy(PD->D[j][i], buf);
		    	}

		    y = 0;
		    c = fgetc(fichier);
		    while (c != '\n')  {buf[y++] = c; c = fgetc(fichier);}
		    buf[y] = '\0'; 
			PD->D[PD->nb_variables-1][i] = (char*)malloc(strlen(buf) + 1);
			strcpy(PD->D[j][i], buf);
    	}

    for (j = 0; j < PD->nb_variables-1; ++j) 
    	{
    		if (PD->type_variables[j] != 1) {continue;}
    		PD->type_variables[j] = is_booleen(PD->D[j], PD->nb_rows);
    	}


    if (PD->type_variables[PD->nb_variables-1] == 1)
    	{
    		PD->type_variables[PD->nb_variables-1] = is_booleen(PD->D[PD->nb_variables-1], PD->nb_rows);
    	}

    fclose(fichier);
}




int get_nb_modalites (char** col, int nb_rows)
{
	std::unordered_map<std::string,int> map;
	int cur = 0;
	for (int i = 0; i < nb_rows; ++i)
		{
			if (map[std::string(col[i])] == 0)
				{
					map[std::string(col[i])] = ++cur;
				}
		}

	return cur;
}

float* standardize_char_array(char** data, int nb_rows, int t, struct Genstruct* GS)
{
    float* result = (float*)malloc(nb_rows * sizeof(float));

    
    float sum = 0.0f;
    for (int i = 0; i < nb_rows; ++i)
    {
        result[i] = atof(data[i]);
        sum += result[i];
    }

    float mean = sum / nb_rows;

    // Étape 2 : calcul écart-type
    float sum_sq = 0.0f;
    for (int i = 0; i < nb_rows; ++i)
    {
        float diff = result[i] - mean;
        sum_sq += diff * diff;
    }

    float std = sqrtf(sum_sq / nb_rows);

    // Étape 3 : standardisation
    for (int i = 0; i < nb_rows; ++i)
    {
        result[i] = (result[i] - mean) / std;
    }

    if (t == 0)
    	{
		    GS->DIE->mean_features[GS->DIE->nb_quant] = mean;
		    GS->DIE->sd_features[GS->DIE->nb_quant] = std;
    	}

    if (t == 1)
    	{
    		GS->DIE->mean_target = mean;
    		GS->DIE->sd_target = std;
    	}


    return result;
}

void set_die (struct Genstruct* GS)
{
	GS->DIE = (struct Data_Info_Export*)malloc(sizeof(struct Data_Info_Export));
	GS->DIE->nb_features = GS->TD->nb_modalites_D;
	GS->DIE->name_features = (char**)malloc(GS->DIE->nb_features*sizeof(char*));
	GS->DIE->indice_features = (int*)malloc(GS->DIE->nb_features*sizeof(int));
	GS->DIE->nb_quant = 0;
	for (int i = 0; i < GS->PD->nb_variables; ++i)
		{
			if (GS->PD->type_variables[i] == 1) {GS->DIE->nb_quant = GS->DIE->nb_quant+1;}
		}
	GS->DIE->mean_features = (float*)calloc(GS->DIE->nb_quant, sizeof(float));
	GS->DIE->sd_features = (float*)calloc(GS->DIE->nb_quant, sizeof(float));
	GS->DIE->mean_target = 0.0f;
	GS->DIE->sd_target = 0.0f;

}


torch::Tensor* get_tensor_target (struct Genstruct* GS)
{	
	int target_index = GS->PD->nb_variables - 1;
	torch::Tensor* T;

    if (GS->PD->type_variables[target_index] == 1) // numérique
	    {
	    	char** name_modalite_T = (char**)malloc(sizeof(char*));
	    	name_modalite_T[0] = GS->PD->name_variables[GS->PD->nb_variables-1];
	    	GS->TD->name_modalite_T = name_modalite_T;
	        T = new torch::Tensor[GS->PD->nb_rows];

	        for (int i = 0; i < GS->PD->nb_rows; ++i)
	        {
	        	if(GS->TD->do_std_y == 0)
	        		{
			            char* val = GS->PD->D[target_index][i];
			            float fval = std::atof(val); // conversion char* -> float

			            T[i] = torch::tensor({fval}, torch::kFloat32); // tenseur scalaire   
	        		}

	        	if(GS->TD->do_std_y == 1)
	        		{
	        			float* std_array = standardize_char_array(GS->PD->D[target_index], GS->PD->nb_rows, 1, GS);
							for (int i = 0; i < GS->PD->nb_rows; ++i)
								{
									T[i] = torch::tensor({std_array[i]}, torch::kFloat32); 
								}
						free(std_array);
	        		}
         
	        }
	        GS->TD->nb_modalites_T = 1;
	        GS->TD->type_tgt = 1;
	        return T;
	    }

	if (GS->PD->type_variables[target_index] == 2) // Booléen
	    {	
	    	char** name_modalite_T = (char**)malloc(sizeof(char*));
	    	name_modalite_T[0] = GS->PD->name_variables[GS->PD->nb_variables-1];
	    	GS->TD->name_modalite_T = name_modalite_T;

		    T = new torch::Tensor[GS->PD->nb_rows];

		    for (int i = 0; i < GS->PD->nb_rows; ++i)
		    {
		        char* val = GS->PD->D[target_index][i];

		        float bool_val = 0.0f;
		        if (strcmp(val, "1") == 0 || strcmp(val, "TRUE") == 0 || strcmp(val, "true") == 0)
		            bool_val = 1.0f;
		        else
		            bool_val = 0.0f;

		        // On stocke directement un scalaire float
		        T[i] = torch::tensor({bool_val}, torch::kFloat32);
		    }
	        GS->TD->nb_modalites_T = 1;
	        GS->TD->type_tgt = 2;
	        return T;
	    }

    GS->TD->type_tgt = 0;
	std::unordered_map<std::string,int> map;
	int nb_mod = 0;
	
	for (int i = 0; i < GS->PD->nb_rows; ++i)
		{
			if (map[std::string(GS->PD->D[target_index][i])] == 0)
				{
					map[std::string(GS->PD->D[target_index][i])] = ++nb_mod;
				}
		}

	T = new torch::Tensor[GS->PD->nb_rows];

	for (int i = 0; i < GS->PD->nb_rows; ++i)
	{
	    std::string key(GS->PD->D[target_index][i]);

	    int index = map[key] - 1;   // 0-based index

	    T[i] = torch::tensor(index, torch::kLong);
	}


	GS->TD->nb_modalites_T = nb_mod;
	char** name_modalite_T = (char**)malloc(GS->TD->nb_modalites_T*sizeof(char*));
	for (int i = 0; i < GS->TD->nb_modalites_T; ++i)
	{
	    for (std::unordered_map<std::string,int>::iterator it = map.begin(); it != map.end(); ++it)
	    {
	        if (it->second == i+1)
	        {
	        	std::string st = std::string(GS->PD->name_variables[GS->PD->nb_variables-1]) + ".";
	        	st = st + std::string(it->first);
	            name_modalite_T[i] = strdup(st.c_str());
	            break; 
	        }
	    }
	}
	GS->TD->name_modalite_T = name_modalite_T;
	return T;
}



void get_tensor_data (struct Genstruct* GS)
{	
	
	// Raccourcis vers les structures de données principales
	struct Pure_Data* PD = GS->PD;
	struct Torch_Data* TD = GS->TD;

	// Pas de nom de modalité pour la cible ici (sera géré ailleurs)
	TD->name_modalite_T = NULL;
	// Un tenseur = 1 ligne de données (échantillon)
	// Étape 1 : la cible est traitée séparément (voir get_tensor_target)
	
	int nb_modalites = 0;

	// Tableau contenant le nombre de modalités par variable explicative
	PD->tab_nb_mod = (uint32_t*)malloc((PD->nb_variables-1)*sizeof(uint32_t));

	// Étape 2 : calcul du nombre total de features après encodage
	// (toutes les variables sauf la cible)
	for (int j = 0; j < PD->nb_variables-1; j++)
		{
			// Variable quantitative ou continue → 1 seule feature
			if (PD->type_variables[j] != 0)
				{
					PD->tab_nb_mod[j] = 1;
				}
			// Variable catégorielle → one-hot + modalité UNK
			else
				{
					PD->tab_nb_mod[j] = get_nb_modalites(PD->D[j], PD->nb_rows)+1; // (+1 pour UNK)
				}
			nb_modalites = nb_modalites + PD->tab_nb_mod[j];
		}

	// Étape 3 : allocation finale des dimensions du tenseur d'entrée
	TD->nb_modalites_D = nb_modalites;
	
	// Initialisation de la structure DIE (descripteur des features)
	set_die (GS);

	// Récupération du tenseur cible
	torch::Tensor* T = get_tensor_target(GS);

	// Allocation des tenseurs d'entrée (un tenseur par ligne)
	torch::Tensor* D = new torch::Tensor[PD->nb_rows];
	
	for (int i = 0; i < PD->nb_rows; ++i) 
		{
    		D[i] = torch::zeros({TD->nb_modalites_D}, torch::dtype(torch::kFloat32));
		}

	// Map associant "nom_feature" → indice global dans le tenseur
	std::unordered_map<std::string, int> map_indices;
	int curindice = 0;

	// Compteur de variables quantitatives
	GS->DIE->nb_quant = 0;	
	
	// Parcours de toutes les variables explicatives
	for (int j = 0; j < PD->nb_variables-1; ++j)
		{
			
			// Variable catégorielle → one-hot encoding
			if (PD->type_variables[j] == 0)
				{
					std::string cur_name_var = std::string (PD->name_variables[j]) + ".";
					for (int i = 0; i < PD->nb_rows; ++i)
						{
							std::string cur_name_mod = cur_name_var + std::string(PD->D[j][i]);

							// Attribution d’un indice unique si nouvelle modalité
							if (map_indices[cur_name_mod] == 0)
								{
									map_indices[cur_name_mod] = ++curindice;
								}

							// Activation du one-hot correspondant
							D[i].index_put_({map_indices[cur_name_mod]-1}, 1.0f);		
						}

					// Ajout explicite de la modalité UNK
					std::string cur_name_mod = cur_name_var + "UNK";
					map_indices[cur_name_mod] = ++curindice;
					continue;
				}

			// Variable quantitative normalisable
			if (PD->type_variables[j] == 1)
				{
					
					// Standardisation activée
					if (TD->do_std_x == 1)
						{
							float* std_array = standardize_char_array(PD->D[j], PD->nb_rows, 0, GS);
							for (int i = 0; i < PD->nb_rows; ++i)
								{
									D[i].index_put_({curindice}, std_array[i]);	
								}
							free(std_array);
						}
					// Sans standardisation
					else if (TD->do_std_x == 0)
						{
							for (int i = 0; i < PD->nb_rows; ++i)
								{
									D[i].index_put_({curindice}, std::atof(PD->D[j][i]));	
								}							
						}

					GS->DIE->nb_quant = GS->DIE->nb_quant+1;

					// Enregistrement du nom de feature
					map_indices[std::string(PD->name_variables[j])] = ++curindice;	
					continue;
				}

			// Variable numérique brute (sans normalisation)
			if (PD->type_variables[j] == 2)
				{
					
					for (int i = 0; i < PD->nb_rows; ++i)
						{
							D[i].index_put_({curindice}, std::atof(PD->D[j][i]));
						}

					map_indices[std::string(PD->name_variables[j])] = ++curindice;
				}
		}

	// Construction du mapping final indice → nom de feature
	for (std::unordered_map<std::string,int>::iterator it = map_indices.begin(); it != map_indices.end(); it++)
		{
			int indice = it->second;
			std::string st = it->first;
			char* tc = (char*)malloc((st.size()+1)*sizeof(char));   // +1 pour '\0'
			std::strcpy(tc, st.c_str());
			GS->DIE->name_features[indice -1] = tc;
			GS->DIE->indice_features[indice -1] = indice;
		}	

	// Tableau d’indices pour le shuffle train/test
	int indices_ratio [PD->nb_rows];
	for (int i = 0; i < PD->nb_rows; ++i) {indices_ratio[i] = i;}
	
	// Shuffle uniquement si ce n’est pas un modèle séquentiel
	if (PD->type_targetmodel != 1 ) {shuffle_array_int(&indices_ratio[0], PD->nb_rows);}
	
	// Split train / test
	TD->nb_rows_train = (int)((double)PD->nb_rows*TD->ratio_train);
	
	TD->D_train = NULL; TD->D_test = NULL;
	TD->D_train = new torch::Tensor[TD->nb_rows_train];
	TD->T_train = new torch::Tensor[TD->nb_rows_train];
	
	for (int i = 0; i < TD->nb_rows_train; ++i)
		{
			TD->D_train[i] = D[indices_ratio[i]];
			TD->T_train[i] = T[indices_ratio[i]];
		}

	int l = 0;
	TD->nb_rows_test = PD->nb_rows - TD->nb_rows_train;
	TD->D_test = new torch::Tensor[TD->nb_rows_test];
	TD->T_test = new torch::Tensor[TD->nb_rows_test];
	
	for (int i = TD->nb_rows_train; i < PD->nb_rows; ++i)
		{
			TD->D_test[l] = D[indices_ratio[i]];
			TD->T_test[l++] = T[indices_ratio[i]];
		}

	// Libération mémoire temporaire
	delete [] D;
	delete [] T;

}


void get_tensor_data_no_tgt (struct Genstruct* GS)
{	
	
	// Accès rapide aux structures principales
	struct Pure_Data* PD = GS->PD;
	struct Torch_Data* TD = GS->TD;

	// Pas de cible explicite dans ce mode
	TD->name_modalite_T = NULL;
	TD->type_tgt = 0;

	int nb_modalites = 0;

	// Tableau contenant le nombre de modalités par variable
	PD->tab_nb_mod = (uint32_t*)malloc((PD->nb_variables)*sizeof(uint32_t));

	// Cas particulier : une seule variable → son type devient le type cible implicite
	if (PD->nb_variables == 1) {TD->type_tgt = PD->type_variables[0];}

	// Calcul du nombre total de features après encodage
	for (int j = 0; j < PD->nb_variables; j++)
		{
			// Variable quantitative ou continue → 1 seule dimension
			if (PD->type_variables[j] != 0)
				{
					PD->tab_nb_mod[j] = 1;
				}
			// Variable catégorielle → one-hot + UNK
			else
				{
					PD->tab_nb_mod[j] = get_nb_modalites(PD->D[j], PD->nb_rows)+1; // (+1 pour UNK)
				}
			nb_modalites = nb_modalites + PD->tab_nb_mod[j];
		}

	// Même espace de représentation pour D et T (pas de vraie cible)
	TD->nb_modalites_D = nb_modalites;
	TD->nb_modalites_T = nb_modalites;

	// Initialisation de la structure DIE (descripteur des features)
	set_die (GS);

	// Allocation des tenseurs d'entrée (un par ligne)
	torch::Tensor* D = new torch::Tensor[PD->nb_rows];

	for (int i = 0; i < PD->nb_rows; ++i) 
		{
    		D[i] = torch::zeros({TD->nb_modalites_D}, torch::dtype(torch::kFloat32));
		}

	// Mapping nom_feature → indice global dans le tenseur
	std::unordered_map<std::string, int> map_indices;
	int curindice = 0;
		
	// Compteur de variables quantitatives
	GS->DIE->nb_quant = 0;	

	// Parcours de toutes les variables
	for (int j = 0; j < PD->nb_variables; ++j)
		{
			// Variable catégorielle → one-hot encoding
			if (PD->type_variables[j] == 0)
				{
					std::string cur_name_var = std::string (PD->name_variables[j]) + ".";
					for (int i = 0; i < PD->nb_rows; ++i)
						{
							std::string cur_name_mod = cur_name_var + std::string(PD->D[j][i]);

							// Attribution d’un indice unique à chaque modalité
							if (map_indices[cur_name_mod] == 0)
								{
									map_indices[cur_name_mod] = ++curindice;
								}

							// Activation de la feature correspondante
							D[i].index_put_({map_indices[cur_name_mod]-1}, 1.0f);		
						}

					// Ajout explicite de la modalité inconnue
					std::string cur_name_mod = cur_name_var + "UNK";
					map_indices[cur_name_mod] = ++curindice;
					continue;
				}

			// Variable quantitative (normalisable)
			if (PD->type_variables[j] == 1)
				{
					// Standardisation activée
					if (TD->do_std_x == 1)
						{
							float* std_array = standardize_char_array(PD->D[j], PD->nb_rows, 0, GS);
							for (int i = 0; i < PD->nb_rows; ++i)
								{
									D[i].index_put_({curindice}, std_array[i]);	
								}
							free(std_array);
						}
					// Valeurs brutes
					else if (TD->do_std_x == 0)
						{
							for (int i = 0; i < PD->nb_rows; ++i)
								{
									D[i].index_put_({curindice}, std::atof(PD->D[j][i]));	
								}							
						}

					GS->DIE->nb_quant = GS->DIE->nb_quant+1;

					// Enregistrement du nom de feature
					map_indices[std::string(PD->name_variables[j])] = ++curindice;
					continue;
				}

			// Variable numérique brute (sans normalisation)
			if (PD->type_variables[j] == 2)
				{	

					for (int i = 0; i < PD->nb_rows; ++i)
						{
							D[i].index_put_({curindice}, std::atof(PD->D[j][i]));
						}

					map_indices[std::string(PD->name_variables[j])] = ++curindice;
				}
		}

	// Allocation et remplissage des noms de modalités (features)
	GS->TD->name_modalite_T = (char**)malloc(GS->TD->nb_modalites_T*sizeof(char*));
	for (std::unordered_map<std::string,int>::iterator it = map_indices.begin(); it != map_indices.end(); it++)
		{
			int indice = it->second;
			std::string st = it->first;

			// Duplication des noms pour DIE et TD
			char* tc = (char*)malloc((st.size()+1)*sizeof(char));
			char* tc2 = (char*)malloc((st.size()+1)*sizeof(char));
			std::strcpy(tc, st.c_str());
			std::strcpy(tc2, st.c_str());

			GS->DIE->name_features[indice -1] = tc;
			GS->DIE->indice_features[indice -1] = indice;
			GS->TD->name_modalite_T[indice - 1] = tc2;
		}	

	// Indices pour le shuffle train/test
	int indices_ratio [PD->nb_rows];
	for (int i = 0; i < PD->nb_rows; ++i) {indices_ratio[i] = i;}
	
	// Pas de shuffle pour les modèles séquentiels
	if (PD->type_targetmodel != 1) {shuffle_array_int(&indices_ratio[0], PD->nb_rows);}

	// Split train / test
	TD->nb_rows_train = (int)((double)PD->nb_rows*TD->ratio_train);
	TD->D_train = NULL; TD->T_train = NULL;
	TD->D_test = NULL; TD->T_test = NULL;

	TD->D_train = new torch::Tensor[TD->nb_rows_train];

	// Ici, pas de vraie cible : T_train pointe sur D_train
	TD->T_train = TD->D_train;	
	for (int i = 0; i < TD->nb_rows_train; ++i)
		{
			TD->D_train[i] = D[indices_ratio[i]];
		}
	

	// Cas modèle non supervisé / auto-encodeur sans cible distincte
	if (PD->type_targetmodel == 0)
		{
			int l = 0;
			TD->nb_rows_test = PD->nb_rows - TD->nb_rows_train;
			TD->D_test = new torch::Tensor[TD->nb_rows_test];
			TD->T_test = TD->D_test;

			for (int i = TD->nb_rows_train; i < PD->nb_rows; ++i)
				{
					TD->D_test[l] = D[indices_ratio[i]];
					l++;
				}
			return;
		}

	// Cas modèle avec logique cible différée (ex: séquentiel)
	if (PD->type_targetmodel == 1)
		{
			int l = 0;
			TD->nb_rows_test = PD->nb_rows - TD->nb_rows_train;
			TD->D_test = new torch::Tensor[TD->nb_rows_test];
			TD->T_test = new torch::Tensor[TD->nb_rows_test];

			for (int i = TD->nb_rows_train; i < PD->nb_rows; ++i)
				{
					TD->D_test[l] = D[indices_ratio[i]];
					l++;
					
				}
			return;
		}
    
}

void set_Dtest_to_sequence(Torch_Data* TD, int size_seq)
{	
    // Nombre maximal de points de départ possibles pour créer des séquences complètes
    int max_start = TD->nb_rows_test - size_seq - 1;

    // Nouveau nombre d’échantillons après transformation en séquences
    TD->nb_rows_test = max_start + 1;

    // Allocation du nouveau tableau de tenseurs séquentiels
    torch::Tensor* D_test_seq = new torch::Tensor[TD->nb_rows_test];

    // Construction des séquences glissantes
    for (int i = 0; i <= max_start; ++i)
	    {
	        std::vector<torch::Tensor> seq_vec;

	        // Construire une séquence de longueur size_seq
	        for (int j = 0; j < size_seq; ++j)
	            {
	            	seq_vec.push_back(TD->D_test[i + j]);
	            }

	        // Cas prédiction classique : la cible est l’élément suivant la séquence
	        if (TD->is_autoencodeur == 0)
	        	{
	        		TD->T_test[i] = TD->D_test[i + size_seq].clone();
	        	}    
	        
	        // Empilement des pas de temps → [seq_len, feature_dim]
	        torch::Tensor seq = torch::stack(seq_vec);
	        
	        // Cas autoencodeur : la cible est la séquence elle-même
	        if (TD->is_autoencodeur == 1)
	        	{
	        		TD->T_test[i] = seq.clone();
	        	}

	        // Ajout d’une dimension batch → [1, seq_len, feature_dim]
	        seq = seq.unsqueeze(0);

	        // Stockage de la séquence d’entrée
	        D_test_seq[i] = seq.clone();
	    }

    // Libération de l’ancien tableau et remplacement par les séquences
  	delete [] TD->D_test;
  	TD->D_test = D_test_seq;
}


void set_onehot_Ttest_to_long(Torch_Data* TD)
{

    // Aucun traitement si aucun échantillon de test
    if (TD->nb_rows_test == 0) return;

    // Dimension du tenseur cible (permet de distinguer vecteur vs séquence)
    int dim0 = TD->T_test[0].dim();

    // Cas classification classique : one-hot [nb_classes]
    if (dim0 == 1)
	    {
	        for (int i = 0; i < TD->nb_rows_test; ++i)
	        {
	            // Conversion one-hot → index de classe
	            long index = TD->T_test[i].argmax().item<long>();
	            TD->T_test[i] = torch::tensor(index, torch::kLong);
	        }

	        return;
	    }
    
    // Cas classification séquentielle : [seq_len, nb_classes]
    if (dim0 == 2)
    {
        for (int i = 0; i < TD->nb_rows_test; ++i)
		    {
		        // Argmax sur l’axe des classes pour chaque pas de temps
		        torch::Tensor indices = TD->T_test[i].argmax(1);
		        TD->T_test[i] = indices.to(torch::kLong);
		    }

        return;
    }

}

void export_data_info_file(struct Genstruct* GS)
{
    char filename[256];
    sprintf(filename, "output/info_data/idata.txt");

    FILE* f = fopen(filename, "w");
    if (!f) 
    {
        printf("Erreur : impossible d'ouvrir le fichier %s\n", filename);
        return;
    }

    fprintf(f, "------ Base: Informations Donnees ------\n\n");

    //
    fprintf(f,
        "do_std_x-do_std_y-ncol_tgt-is_autoencodeur-type_targetmodel,cat_target,size_seq#%u,%u,%u,%u,%u,%u,%u,\n\n",
        GS->TD->do_std_x,
        GS->TD->do_std_y,
        GS->TD->nb_col_target,
        GS->TD->is_autoencodeur,
        GS->PD->type_targetmodel,
        GS->TD->type_tgt,
        GS->TB->size_seq
    );

    //
    // 2) Variables
    //
    fprintf(f, "variables#%d[", GS->PD->nb_variables);
    for (int i=0; i < GS->PD->nb_variables; ++i) 
    {
        fprintf(f, "%s(%u) ", GS->PD->name_variables[i], GS->PD->type_variables[i]);
    }
    fprintf(f, "]\n\n");

    //
    // 3) Features
    //
    fprintf(f, "features#%d[", GS->DIE->nb_features);
    for (int i=0; i < GS->DIE->nb_features; ++i) 
    {
        fprintf(f, "%s(%d) ", GS->DIE->name_features[i], GS->DIE->indice_features[i]);
    }
    fprintf(f, "]\n\n");

    //
    // 4) Stats features
    //
    fprintf(f, "stats features#%d[", GS->DIE->nb_quant);
    for (int i=0; i < GS->DIE->nb_quant; ++i) 
    {
        fprintf(f, "%.6f(%.6f) ", GS->DIE->mean_features[i], GS->DIE->sd_features[i]);
    }
    fprintf(f, "]\n\n");

    //
    // 5) Targets
    //
    fprintf(f, "targets#%d[", GS->TD->nb_modalites_T);
    for (int i=0; i < GS->TD->nb_modalites_T; ++i) 
    {
        fprintf(f, "%s(%d) ", GS->TD->name_modalite_T[i], i+1);
    }
    fprintf(f, "]\n\n");

    //
    // 6) Stats targets
    //
    fprintf(f, "stats targets#%.6f(%.6f)\n\n",
            GS->DIE->mean_target,
            GS->DIE->sd_target
            );

    fclose(f);
}

void clean_die (struct Genstruct* GS)
{
	for (int i = 0; i < GS->DIE->nb_features; ++i) {free(GS->DIE->name_features[i]);}
	free(GS->DIE->name_features);
	free(GS->DIE->indice_features);
	free(GS->DIE->mean_features);
	free(GS->DIE->sd_features);

	free(GS->DIE);
}

void get_data (struct Genstruct* GS)
{
	get_pure_data (GS);
	
	if (GS->TD->nb_col_target == 1)
		{
			
			get_tensor_data (GS);
			export_data_info_file(GS);
			clean_die(GS);

			return;
		}

	if (GS->TD->nb_col_target == 0)
		{
			get_tensor_data_no_tgt (GS);
			if (GS->PD->type_targetmodel == 1)
				{	
					set_Dtest_to_sequence(GS->TD, GS->TB->size_seq);
					if (GS->TD->type_tgt == 0) {set_onehot_Ttest_to_long(GS->TD);}
				}
			export_data_info_file(GS);
			clean_die(GS);
			return;
		}

}