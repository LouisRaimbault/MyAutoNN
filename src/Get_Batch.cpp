#include "../include/Get_Batch.h"
#include "../include/Modele.h"
#include "../include/Usefull_Functions.h"

void clean_torch_batch (struct Torch_Batch* TB)
{
	delete [] TB->array_batch_D;
	delete [] TB->array_batch_T;
	free(TB);
}


void get_batch(struct Genstruct* GS)
{	
	
	// Accès rapide aux structures Torch
	struct Torch_Data* TD = GS->TD;
	struct Torch_Batch* TB = GS->TB;

	// Allocation des tableaux de batches (inputs et targets)
	TB->array_batch_D = new torch::Tensor [TB->nb_batch];
	TB->array_batch_T = new torch::Tensor [TB->nb_batch];
	
	// ------------------------------------------------------------------
	// Cas non séquentiel (données tabulaires / classification classique)
	// ------------------------------------------------------------------
	if (GS->PD->type_targetmodel == 0)
	{
	    
	    torch::Tensor indices;

	    // Shuffle aléatoire des indices (hors modèles séquentiels)
	    if (GS->PD->type_targetmodel != 1)
	        {
	        	indices = torch::randperm(TD->nb_rows_train, torch::TensorOptions().dtype(torch::kInt64));
	        }
	    // Ordre conservé (cas particulier)
	    else
	    	{
	        	indices = torch::arange(0, TD->nb_rows_train, torch::TensorOptions().dtype(torch::kInt64));
	    	}

		// Construction de chaque batch
		for (uint32_t i = 0; i < TB->nb_batch; ++i)
		{
		    // Point de départ aléatoire pour le batch
		    int start = random_ab(0, TD->nb_rows_train - TB->size_batch); 

		    std::vector<torch::Tensor> batch_vec_D;
		    std::vector<torch::Tensor> batch_vec_T;

		    // Remplissage du batch
		    for (uint32_t j = 0; j < TB->size_batch; ++j)
		    {
		        int idx = indices[start + j].item<int64_t>();
		        batch_vec_D.push_back(TD->D_train[idx]);
		        batch_vec_T.push_back(TD->T_train[idx]);
		    }

		    // Stack final : [batch_size, nb_features]
		    TB->array_batch_D[i] = torch::stack(batch_vec_D);
		    TB->array_batch_T[i] = torch::stack(batch_vec_T);
		}
	    
	}
}

void get_batch_no_tgt(struct Genstruct* GS)
{	
	
	// Accès rapide aux structures Torch
	struct Torch_Data* TD = GS->TD;
	struct Torch_Batch* TB = GS->TB;

	// Allocation des tableaux de batches
	TB->array_batch_D = new torch::Tensor [TB->nb_batch];
	TB->array_batch_T = new torch::Tensor [TB->nb_batch];
	
	
	// -------------------------------------------------------
	// Cas non séquentiel, sans cible explicite (ex: autoencodeur)
	// -------------------------------------------------------
	if (GS->PD->type_targetmodel == 0)  
		{
			// Dans ce cas, la cible est identique à l’entrée
			TB->array_batch_T = TB->array_batch_D;
		    
		    torch::Tensor indices;

		    // Shuffle par défaut
		    if (GS->PD->type_targetmodel != 2)
		        {
		        	indices = torch::randperm(TD->nb_rows_train, torch::TensorOptions().dtype(torch::kInt64));
		        }
		    // Ordre conservé
		    else
		    	{
		        	indices = torch::arange(0, TD->nb_rows_train, torch::TensorOptions().dtype(torch::kInt64));
		    	}

			// Construction des batches
			for (uint32_t i = 0; i < TB->nb_batch; ++i)
			{
			    int start = random_ab(0, TD->nb_rows_train - TB->size_batch); 

			    std::vector<torch::Tensor> batch_vec_D;

			    for (uint32_t j = 0; j < TB->size_batch; ++j)
			    {
			        int idx = indices[start + j].item<int64_t>();
			        batch_vec_D.push_back(TD->D_train[idx]);
			    }

			    // Stack final : [batch_size, nb_features]
			    TB->array_batch_D[i] = torch::stack(batch_vec_D);
			}
		    
		}

	// ------------------------------------------------------------------
	// Cas séquentiel supervisé (hors autoencodeur)
	// ------------------------------------------------------------------
	if (GS->PD->type_targetmodel == 1 && TD->is_autoencodeur == 0)
		{
		    int seq_len = TB->size_seq;

		    // Dernier point valide pour construire une séquence + cible
		    int max_start = TD->nb_rows_train - seq_len - 1;

		    int i = 0;
		   
		    // Cible catégorielle (one-hot → index)
		    if (GS->TD->type_tgt == 0)
		    	{
				    while (i < TB->nb_batch) 
					    {
					        std::vector<torch::Tensor> batch_vec_D;
					        std::vector<torch::Tensor> batch_vec_T;

					        for (uint32_t k = 0; k < GS->TB->size_batch; ++k)
						        {
						            int start = torch::randint(0, max_start + 1, {1}).item<int>();
						            int end   = start + seq_len;

						            // ---------------------------
						            // X : séquence d’entrée
						            // ---------------------------
						            std::vector<torch::Tensor> seq;
						            for (uint32_t j = start; j < end; ++j) {seq.push_back(TD->D_train[j]);}

						            batch_vec_D.push_back(torch::stack(seq));

						            // ---------------------------
						            // Y : classe suivante
						            // ---------------------------
						            torch::Tensor target_onehot = TD->D_train[end];
						            int64_t idx = target_onehot.argmax(0).item<int64_t>();
						            torch::Tensor target_index = torch::tensor(idx, torch::kLong);                         
						            batch_vec_T.push_back(target_index);
						        }

					        TB->array_batch_D[i] = torch::stack(batch_vec_D);
					        TB->array_batch_T[i] = torch::stack(batch_vec_T);

					        ++i;
					    }		    		
		    	}

		    // Cible vectorielle (régression / embedding)
		    if (GS->TD->type_tgt == 1)
		    	{
					while (i < TB->nb_batch) 
						{
						    std::vector<torch::Tensor> batch_vec_D;
						    std::vector<torch::Tensor> batch_vec_T;

						    for (uint32_t k = 0; k < GS->TB->size_batch; ++k)
						    {
						        int start = torch::randint(0, max_start + 1, {1}).item<int>();
						        int end   = start + seq_len;

						        // ---------------------------
						        // X : séquence
						        // ---------------------------
						        std::vector<torch::Tensor> seq;
						        for (uint32_t j = start; j < end; ++j) {seq.push_back(TD->D_train[j]);}
						        batch_vec_D.push_back(torch::stack(seq));

						        // ---------------------------
						        // Y : vecteur suivant
						        // ---------------------------
						        torch::Tensor target_vector = TD->D_train[end];
						        batch_vec_T.push_back(target_vector);
						    }

						    TB->array_batch_D[i] = torch::stack(batch_vec_D);
						    TB->array_batch_T[i] = torch::stack(batch_vec_T);

						    ++i;
						}		    		
		    	}
	   
		}

	// ------------------------------------------------------------------
	// Cas séquentiel autoencodeur
	// ------------------------------------------------------------------
	if (GS->PD->type_targetmodel == 1 && TD->is_autoencodeur == 1)
		{
			
		    int seq_len = TB->size_seq;
		    int max_start = TD->nb_rows_train - seq_len - 1;

		    uint32_t i = 0;

		    // Autoencodeur catégoriel
		    if (GS->TD->type_tgt == 0)
		    	{
				    while (i < TB->nb_batch) 
					    {
					        std::vector<torch::Tensor> batch_vec_D;
					        std::vector<torch::Tensor> batch_vec_T;

					        for (uint32_t k = 0; k < GS->TB->size_batch; ++k)
						        {
						            int start = torch::randint(0, max_start + 1, {1}).item<int>();
						            int end   = start + seq_len;

						            // ---------------------------
						            // X : one-hot sequence
						            // ---------------------------
						            std::vector<torch::Tensor> seq;
						            for (uint32_t j = start; j < end; ++j) {seq.push_back(TD->D_train[j]);}

						            torch::Tensor seq_tensor = torch::stack(seq);

						            // ---------------------------
						            // Y : indices par pas de temps
						            // ---------------------------
						            torch::Tensor target_tensor = seq_tensor.argmax(1).to(torch::kLong);

						            batch_vec_D.push_back(seq_tensor);
						            batch_vec_T.push_back(target_tensor);
						        }

					        TB->array_batch_D[i] = torch::stack(batch_vec_D);
					        TB->array_batch_T[i] = torch::stack(batch_vec_T);

					        ++i;
					    }		    		
		    	}

		    // Autoencodeur vectoriel
		    if (GS->TD->type_tgt == 1)
		    	{
					while (i < TB->nb_batch) 
						{
						    std::vector<torch::Tensor> batch_vec_D;
						    std::vector<torch::Tensor> batch_vec_T;

						    for (uint32_t k = 0; k < GS->TB->size_batch; ++k)
						    {
						        int start = torch::randint(0, max_start + 1, {1}).item<int>();
						        int end   = start + seq_len;

						        // ---------------------------
						        // X = Y : reconstruction complète
						        // ---------------------------
						        std::vector<torch::Tensor> seq;
						        for (uint32_t j = start; j < end; ++j) {seq.push_back(TD->D_train[j]);}
					            torch::Tensor seq_tensor = torch::stack(seq);

					            batch_vec_D.push_back(seq_tensor.clone());
					            batch_vec_T.push_back(seq_tensor.clone());
						    }

						    TB->array_batch_D[i] = torch::stack(batch_vec_D);
						    TB->array_batch_T[i] = torch::stack(batch_vec_T);

						    ++i;
						}		    		
		    	}
		   
		}

}

void construct_batch (struct Genstruct* GS)
{
	if (GS->TD->nb_col_target == 1)
		{	
			get_batch (GS);
		}

	if (GS->TD->nb_col_target == 0)
		{			
			get_batch_no_tgt (GS);
		}

}