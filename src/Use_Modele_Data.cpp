#include "../include/Modele.h"
#include "../include/Use_Modele.h"


void clean_use_data (struct Use_Data* UD)
{
	
	delete [] UD->D_use;
	delete [] UD->T_use;

	
	if (UD->type_variables != NULL) {free(UD->type_variables);}

	if (UD->mean_features != NULL) {free(UD->mean_features);}
	if (UD->sd_features != NULL) {free(UD->sd_features);}

	
	if (UD->name_variables != NULL)
		{
			for (int i = 0; i < UD->nb_variables; ++i)
				{
					if (UD->name_variables[i] != NULL)
						{
							free(UD->name_variables[i]);
						}
				}

			free(UD->name_variables);
		}
	
	
	if (UD->name_features != NULL)
		{
			for (int i = 0; i < UD->nb_features; ++i)
				{
					if (UD->name_features[i] != NULL)
						{
							free(UD->name_features[i]);
						}
				}
			free(UD->name_features);
		}
	

	if (UD->name_mod_target != NULL)
		{
			for (int i = 0; i < UD->nb_mod_target; ++i)
				{
					if (UD->name_mod_target[i] != NULL)
						{
							free(UD->name_mod_target[i]);
						}
				}
			free(UD->name_mod_target);
		}
		
	
	if (UD->indice_mod_target != NULL) {free(UD->indice_mod_target);}

	free(UD);

}

void printUseData(struct Use_Data* UD)
{

    printf("=========== Use_Data ===========\n");

    printf("nb_rows_use        : %d\n", UD->nb_rows_use);
    printf("do_std_x          : %u\n", UD->do_std_x);
    printf("do_std_y          : %u\n", UD->do_std_y);
    printf("nb_col_targets    : %u\n", UD->nb_col_targets);
    printf("is_autoencodeur   : %u\n", UD->is_autoencodeur);
    printf("type_targetmodel  : %u\n", UD->type_targetmodel);
    printf("type_target  : %u\n", UD->type_target);
    printf("size_seq          : %u\n", UD->size_seq);
    printf("nb_variables      : %d\n", UD->nb_variables);
    printf("nb_features       : %d\n", UD->nb_features);

    printf("\n--- Variables (%d) ---\n", UD->nb_variables);

    for (int i = 0; i < UD->nb_variables; i++)
	    {
	        printf("  [%d] name = ", i);

	        if (UD->name_variables != NULL && UD->name_variables[i] != NULL)
		        {
		            printf("%s", UD->name_variables[i]);
		        }
	        else
		        {
		            printf("(NULL)");
		        }

	        printf(" | type = ");

	        if (UD->type_variables != NULL)
		        {
		            printf("%u", UD->type_variables[i]);
		        }
	        else
		        {
		            printf("?");
		        }

	        printf("\n");
	    }

    printf("\n--- Features (%d) ---\n", UD->nb_features);

    for (int i = 0; i < UD->nb_features; i++)
	    {
	        printf("  [%d] ", i);

	        if (UD->name_features != NULL && UD->name_features[i] != NULL)
		        {
		            printf("%s", UD->name_features[i]);
		        }
	        else
		        {
		            printf("(NULL)");
		        }

	        printf(" (index=");

	        if (UD->indice_features != NULL)
		        {
		            printf("%d", UD->indice_features[i]);
		        }
	        else
		        {
		            printf("?");
		        }

	        printf(")\n");
	    }

    printf("\n--- Quantitative features (%d) ---\n", UD->nb_quant);

    for (int i = 0; i < UD->nb_quant; i++)
	    {
	        printf("  [%d] mean = ", i);

	        if (UD->mean_features != NULL)
		        {
		            printf("%.6f", UD->mean_features[i]);
		        }
	        else
		        {
		            printf("?");
		        }

	        printf(" | sd = ");

	        if (UD->sd_features != NULL)
		        {
		            printf("%.6f", UD->sd_features[i]);
		        }
	        else
		        {
		            printf("?");
		        }

	        printf("\n");
	    }

    printf("\n--- Target Modules (%d) ---\n", UD->nb_mod_target);

    for (int i = 0; i < UD->nb_mod_target; i++)
	    {
	        printf("  [%d] ", i);

	        if (UD->name_mod_target != NULL && UD->name_mod_target[i] != NULL)
		        {
		            printf("%s", UD->name_mod_target[i]);
		        }
	        else
		        {
		            printf("(NULL)");
		        }

	        printf(" (index=");

	        if (UD->indice_mod_target != NULL)
		        {
		            printf("%d", UD->indice_mod_target[i]);
		        }
	        else
		        {
		            printf("?");
		        }

	        printf(")\n");
	    }

    printf("\n--- Target stats ---\n");
    printf("mean_target       : %.6f\n", UD->mean_target);
    printf("sd_target         : %.6f\n", UD->sd_target);

    printf("=================================\n");
}



void get_ud_infos (struct Genuse* GU)
{
	FILE *fichier = fopen(GU->path_info_base, "r");

	if (fichier == NULL) 
		{
		    perror("Erreur lors de l'ouverture du fichier");
		    return;
		}

	char buf [1000];
    int y = 0;
    int car = fgetc(fichier);

    while (car != '#') {car = fgetc(fichier);} 
    while ((car = fgetc(fichier)) != ',') {buf[y++] = car;} buf[y] = '\0'; y = 0;
    GU->UD->do_std_x = (uint16_t)atoi(buf);

    while ((car = fgetc(fichier)) != ',') {buf[y++] = car;} buf[y] = '\0'; y = 0;
    GU->UD->do_std_y = (uint16_t)atoi(buf);

    while ((car = fgetc(fichier)) != ',') {buf[y++] = car;} buf[y] = '\0'; y = 0;
    GU->UD->nb_col_targets = (uint16_t)atoi(buf);

    while ((car = fgetc(fichier)) != ',') {buf[y++] = car;} buf[y] = '\0'; y = 0;
    GU->UD->is_autoencodeur = (uint16_t)atoi(buf);

    while ((car = fgetc(fichier)) != ',') {buf[y++] = car;} buf[y] = '\0'; y = 0;
    GU->UD->type_targetmodel = (uint16_t)atoi(buf);

    while ((car = fgetc(fichier)) != ',') {buf[y++] = car;} buf[y] = '\0'; y = 0;
    GU->UD->type_target = (uint16_t)atoi(buf);

    while ((car = fgetc(fichier)) != ',') {buf[y++] = car;} buf[y] = '\0'; y = 0;
	GU->UD->size_seq = (uint16_t)atoi(buf);


	// Recuperation nb et types de variables totales
	while (car != '#') {car = fgetc(fichier);} 
	while ((car = fgetc(fichier)) != '[') {buf[y++] = car;} buf[y] = '\0'; y = 0;
	GU->UD->nb_variables = atoi(buf);


	GU->UD->name_variables = (char**)malloc(GU->UD->nb_variables*sizeof(char*));
	GU->UD->type_variables = (uint16_t*)malloc(GU->UD->nb_variables*sizeof(uint16_t));
	for (int i = 0; i < GU->UD->nb_variables; ++i)
		{
			while ((car = fgetc(fichier)) != '(') {buf[y++] = car;} buf[y] = '\0'; y = 0;
		    GU->UD->name_variables[i] = (char*)malloc(strlen(buf) + 1);
		    strcpy(GU->UD->name_variables[i], buf);

		    while ((car = fgetc(fichier)) != ')') {buf[y++] = car;} buf[y] = '\0'; y = 0;
		    GU->UD->type_variables[i] = atoi(buf);

		    while ((car = fgetc(fichier)) != ' ') {;}
		}

	// Recuperation nb et indice features pour map
	while (car != '#') {car = fgetc(fichier);} 
	while ((car = fgetc(fichier)) != '[') {buf[y++] = car;} buf[y] = '\0'; y = 0;
	GU->UD->nb_features = atoi(buf);

	GU->UD->name_features = (char**)malloc(GU->UD->nb_features*sizeof(char*));
	GU->UD->indice_features = (int*)malloc(GU->UD->nb_features*sizeof(int));
	for (int i = 0; i < GU->UD->nb_features; ++i)
		{
			while ((car = fgetc(fichier)) != '(') {buf[y++] = car;} buf[y] = '\0'; y = 0;
		    GU->UD->name_features[i] = (char*)malloc(strlen(buf) + 1);
		    strcpy(GU->UD->name_features[i], buf);

		    while ((car = fgetc(fichier)) != ')') {buf[y++] = car;} buf[y] = '\0'; y = 0;
		    GU->UD->indice_features[i] = atoi(buf);

		    while ((car = fgetc(fichier)) != ' ') {;}
		}
	
	// Recuperation nb et valeurs des moyennes et écart types des features	
	while (car != '#') {car = fgetc(fichier);} 
	while ((car = fgetc(fichier)) != '[') {buf[y++] = car;} buf[y] = '\0'; y = 0;
	GU->UD->nb_quant = atoi(buf);



	if (GU->UD->nb_quant == 0)
		{
			GU->UD->mean_features = NULL;
			GU->UD->sd_features = NULL;
		}
	if (GU->UD->nb_quant != 0)
		{
			GU->UD->mean_features = (float*)malloc(GU->UD->nb_quant*sizeof(float));
			GU->UD->sd_features = (float*)malloc(GU->UD->nb_quant*sizeof(float));
			for (int i = 0; i < GU->UD->nb_quant; ++i)
				{
					while ((car = fgetc(fichier)) != '(') {buf[y++] = car;} buf[y] = '\0'; y = 0;
				    GU->UD->mean_features[i] = atof(buf);
				    while ((car = fgetc(fichier)) != ')') {buf[y++] = car;} buf[y] = '\0'; y = 0;
				    GU->UD->sd_features[i] = atof(buf);
				    while ((car = fgetc(fichier)) != ' ') {;}				
				}						
		}


	// Recuperation nb et indices des modalites de la cible
	while (car != '#') {car = fgetc(fichier);} 
	while ((car = fgetc(fichier)) != '[') {buf[y++] = car;} buf[y] = '\0'; y = 0;
	GU->UD->nb_mod_target = atoi(buf);
	GU->UD->name_mod_target = (char**)malloc(GU->UD->nb_mod_target*sizeof(char*));
	GU->UD->indice_mod_target = (int*)malloc(GU->UD->nb_mod_target*sizeof(int));

	for (int i = 0; i < GU->UD->nb_mod_target; ++i)
		{
			while ((car = fgetc(fichier)) != '(') {buf[y++] = car;} buf[y] = '\0'; y = 0;
		    GU->UD->name_mod_target[i] = (char*)malloc(strlen(buf) + 1);
		    strcpy(GU->UD->name_mod_target[i], buf);
		    while ((car = fgetc(fichier)) != ')') {buf[y++] = car;} buf[y] = '\0'; y = 0;
		    GU->UD->indice_mod_target[i] = atoi(buf);
		    while ((car = fgetc(fichier)) != ' ') {;}
		}

	while (car != '#') {car = fgetc(fichier);}
	while ((car = fgetc(fichier)) != '(') {buf[y++] = car;} buf[y] = '\0'; y = 0;
	GU->UD->mean_target = atof(buf);
	while ((car = fgetc(fichier)) != ')') {buf[y++] = car;} buf[y] = '\0'; y = 0;
	GU->UD->sd_target = atof(buf);
	fclose(fichier);
	fichier = NULL;
}

void get_nb_rows_col (struct Genuse* GU)
{
	FILE *fichier = fopen(GU->path_base, "r");
		if (fichier == NULL) 
		{
		    perror("Erreur lors de l'ouverture du fichier");
		    return;
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

	GU->UD->nb_rows_use = nb_rows-2;
	GU->UD->nb_variables = nb_col;
	fclose(fichier);
	fichier = NULL;
}

std::unordered_map<std::string,int> rebuild_map_indices (struct Genuse* GU, int t)
{
	std::unordered_map<std::string,int> map_indices;

	if (t == 0)
		{
			for (int j = 0; j < GU->UD->nb_features; ++j) {map_indices[std::string(GU->UD->name_features[j])] = GU->UD->indice_features[j];}
		}
	

	if (t == 1)
		{
			for (int j = 0; j < GU->UD->nb_mod_target; ++j) {map_indices[std::string(GU->UD->name_mod_target[j])] = GU->UD->indice_mod_target[j];}
		}
	
	return map_indices;
}

void set_tenseur_1tgt_no_seq (struct Genuse* GU, FILE* fichier)
{

	std::unordered_map map_indices_features = rebuild_map_indices(GU, 0);
	std::unordered_map map_indices_target = rebuild_map_indices(GU, 1);
	torch::Tensor* D = new torch::Tensor[GU->UD->nb_rows_use];
	torch::Tensor* T = new torch::Tensor[GU->UD->nb_rows_use];

	int indice_unknows [GU->UD->nb_variables];
	for (int j  = 0; j < GU->UD->nb_variables-1; ++j)
		{
			if (GU->UD->type_variables[j] != 0) {indice_unknows[j] = 100; continue;}
			std::string st = std::string(GU->UD->name_variables[j]) + ".UNK";
			indice_unknows[j] = map_indices_features[st];
		}

	for (int i = 0; i < GU->UD->nb_rows_use; ++i)
		{
			D[i] = torch::zeros({GU->UD->nb_features}, torch::dtype(torch::kFloat32));	
		}


	char buf [1000];
    int y = 0;
    int c;


	for (int i = 0; i < GU->UD->nb_rows_use; i++)
    	{   
    		int cur_quant = 0;
		    for (int j = 0; j < GU->UD->nb_variables-1; ++j) 
		    	{	
		    		y = 0;
		    		c = fgetc(fichier);
		    		while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
		    		buf[y] = '\0'; 
		    		if (GU->UD->type_variables[j] == 0)
		    			{
		    				std::string st = std::string(GU->UD->name_variables[j]) + ".";
		    				st = st + std::string(buf);
		    				if (map_indices_features[st] == 0) 
		    					{
		    						D[i].index_put_({indice_unknows[j]-1}, 1.0f);
		    						continue;
		    					}

		    				D[i].index_put_({map_indices_features[st]-1}, 1.0f);
		    				continue;
		    			}

		    		if (GU->UD->type_variables[j] == 1)
		    			{
		    				if (GU->UD->do_std_x == 0)
		    					{
		    						D[i].index_put_({map_indices_features[std::string(GU->UD->name_variables[j])]-1}, atof(buf));
		    						cur_quant++;
		    						continue;
		    					}

		    				float val = (atof(buf) - GU->UD->mean_features[cur_quant]) / GU->UD->sd_features[cur_quant];
		    				D[i].index_put_({map_indices_features[std::string(GU->UD->name_variables[j])]-1}, val);
		    				cur_quant++;
		    				continue;
		    			}

					D[i].index_put_({map_indices_features[std::string(GU->UD->name_variables[j])]-1}, atof(buf));			    		
		    	}

    		y = 0;
    		c = fgetc(fichier);
    		while (c != '\n') {buf[y++] = c; c = fgetc(fichier);}
    		buf[y] = '\0'; 	

		    if (GU->UD->type_variables[GU->UD->nb_variables-1] == 0)
		    	{
    				std::string st = std::string(GU->UD->name_variables[GU->UD->nb_variables-1]) + ".";
    				st = st + std::string(buf);
    				int index = map_indices_target[st]-1;
    				T[i] = torch::tensor(index, torch::kLong);
    				continue;
		    	}

		    if (GU->UD->type_variables[GU->UD->nb_variables-1] == 1)
		    	{
		    		if (GU->UD->do_std_y == 0)
		            	{	
		            		float fval = std::atof(buf); // conversion char* -> float
		            		T[i] = torch::tensor({fval}, torch::kFloat32); // tenseur scalaire
		            		continue;
		            	}

		    		if (GU->UD->do_std_y == 1)
		            	{	
		            		float fval = std::atof(buf); // conversion char* -> float
		            		fval = (fval - GU->UD->mean_target) / GU->UD->sd_target;
		            		T[i] = torch::tensor({fval}, torch::kFloat32); // tenseur scalaire
		            		continue;
		            	}         		    		
		    	}

		    if (GU->UD->type_variables[GU->UD->nb_variables-1] == 2)
		    	{
		            float fval = (float)((int)(std::atof(buf))); // conversion char* -> float
		            T[i] = torch::tensor({fval}, torch::kFloat32); // tenseur scalaire
		            continue;     		    		
		    	}
    	}
    	
    GU->UD->D_use = D;
    GU->UD->T_use = T;
}

void set_tenseur_1tgt_seq (struct Genuse* GU, FILE* fichier)
{

	std::unordered_map map_indices_features = rebuild_map_indices(GU, 0);
	std::unordered_map map_indices_target = rebuild_map_indices(GU, 1);
	int max_seq = GU->UD->nb_rows_use - GU->UD->size_seq +1;
	if (GU->UD->is_autoencodeur == 0)
		{
			max_seq = max_seq -1;
		}

	torch::Tensor* D = new torch::Tensor[GU->UD->nb_rows_use];
	torch::Tensor* S = new torch::Tensor[max_seq];
	torch::Tensor* T = new torch::Tensor[max_seq];

	char buf [1000];
    int y = 0;
    int c;
	int indice_unknows [GU->UD->nb_variables];
	for (int j  = 0; j < GU->UD->nb_variables; ++j)
		{
			if (GU->UD->type_variables[j] != 0) {continue;}
			std::string st = std::string(GU->UD->name_variables[j]) + ".UNK";
			indice_unknows[j] = map_indices_features[st];
		}

	for (int i = 0; i < GU->UD->nb_rows_use; ++i)
		{
			D[i] = torch::zeros({GU->UD->nb_features}, torch::dtype(torch::kFloat32));	
		}
	for (int i = 0; i < GU->UD->nb_rows_use; i++)
    	{   
    		int cur_quant = 0;
		    for (int j = 0; j < GU->UD->nb_variables - 1; ++j) 
		    	{	
		    		y = 0;
		    		c = fgetc(fichier);
		    		while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
		    		buf[y] = '\0'; 
		    		if (GU->UD->type_variables[j] == 0)
		    			{
		    				std::string st = std::string(GU->UD->name_variables[j]) + ".";
		    				st = st + std::string(buf);
		    				if (map_indices_features[st] == 0) 
		    					{
		    						D[i].index_put_({indice_unknows[j]-1}, 1.0f);
		    						continue;
		    					}

		    				D[i].index_put_({map_indices_features[st]-1}, 1.0f);
		    				continue;
		    			}

		    		if (GU->UD->type_variables[j] == 1)
		    			{
		    				if (GU->UD->do_std_x == 0)
		    					{
		    						D[i].index_put_({map_indices_features[std::string(GU->UD->name_variables[j])]-1}, atof(buf));
		    						cur_quant++;
		    						continue;
		    					}

		    				float val = (atof(buf) - GU->UD->mean_features[cur_quant]) / GU->UD->sd_features[cur_quant];
		    				D[i].index_put_({map_indices_features[std::string(GU->UD->name_variables[j])]-1}, val);
		    				cur_quant++;
		    				continue;
		    			}

					D[i].index_put_({map_indices_features[std::string(GU->UD->name_variables[j])]-1}, atof(buf));			    		
		    	}

    		y = 0;
    		c = fgetc(fichier);
    		while (c != '\n') {buf[y++] = c; c = fgetc(fichier);}
    		buf[y] = '\0'; 
    		if (GU->UD->type_variables[GU->UD->nb_variables-1] == 0)
    			{
    				std::string st = std::string(GU->UD->name_variables[GU->UD->nb_variables-1]) + ".";
    				st = st + std::string(buf);
    				if (map_indices_features[st] == 0) 
    					{
    						D[i].index_put_({indice_unknows[GU->UD->nb_variables-1]-1}, 1.0f);
    						continue;
    					}

    				D[i].index_put_({map_indices_features[st]-1}, 1.0f);
    				continue;
    			}

    		if (GU->UD->type_variables[GU->UD->nb_variables-1] == 1)
    			{
    				if (GU->UD->do_std_x == 0)
    					{
    						D[i].index_put_({map_indices_features[std::string(GU->UD->name_variables[GU->UD->nb_variables-1])]-1}, atof(buf));
    						cur_quant++;
    						continue;
    					}

    				float val = (atof(buf) - GU->UD->mean_features[cur_quant]) / GU->UD->sd_features[cur_quant];
    				D[i].index_put_({map_indices_features[std::string(GU->UD->name_variables[GU->UD->nb_variables-1])]-1}, val);
    				cur_quant++;
    				continue;
    			}

    		D[i].index_put_({map_indices_features[std::string(GU->UD->name_variables[GU->UD->nb_variables-1])]-1}, atof(buf));
    	}

	int max_start = max_seq -1;

   	for (int i = 0; i <= max_start; ++i) 
   		{
	        std::vector<torch::Tensor> seq_vec;

	        for (int j = 0; j < GU->UD->size_seq; ++j)
	            {
	            	seq_vec.push_back(D[i + j]);
	            }

	        if (GU->UD->is_autoencodeur == 0)
	        	{
	        		T[i] = D[i + GU->UD->size_seq].clone();
	        	}    

	        torch::Tensor seq = torch::stack(seq_vec);

	        if (GU->UD->is_autoencodeur == 1)
	        	{
	        		T[i] = seq.clone();
	        	}
	        seq = seq.unsqueeze(0);
	        // Stockage
	        S[i] = seq.clone();   			
   		}
   
   	delete [] D; 		
    GU->UD->D_use = S;
    GU->UD->T_use = T;
    GU->UD->nb_rows_use = max_seq;
}

void get_use_data (struct Genuse* GU)
{

	GU->UD = (struct Use_Data*)malloc(sizeof(struct Use_Data));
	get_ud_infos(GU);


	get_nb_rows_col(GU);

	char buf [1000];
	int y = 0;
	int c;

    FILE *fichier = fopen(GU->path_base, "r");
    if (fichier == NULL) 
	    {
	        perror("Erreur lors de l'ouverture du fichier");
	        return;
	    }
   
// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+ Récupération des noms de colonnes -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
    for (int j = 0; j < GU->UD->nb_variables-1; ++j) 
    	{
    		y = 0;
    		c = fgetc(fichier);
    		while (c != ',') {buf[y++] = c; c = fgetc(fichier);}
    		buf[y] = '\0'; 
    		GU->UD->name_variables[j] = (char*)malloc(strlen(buf) + 1);

			strcpy(GU->UD->name_variables[j], buf);
    	}



    y = 0;
    c = fgetc(fichier);
    while (c != '\n')  {buf[y++] = c; c = fgetc(fichier);}
    buf[y] = '\0'; 
    GU->UD->name_variables[GU->UD->nb_variables-1] = (char*)malloc(strlen(buf) + 1);
	strcpy(GU->UD->name_variables[GU->UD->nb_variables-1], buf);


// -+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+--+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
	if (GU->UD->type_targetmodel == 0)
		{
			set_tenseur_1tgt_no_seq (GU, fichier);
		}

	if (GU->UD->type_targetmodel == 1)
		{
			set_tenseur_1tgt_seq (GU, fichier);
		}

	//printUseData(GU->UD);	
	fclose(fichier);
}
