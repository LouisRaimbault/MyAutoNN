#include "../include/Export_Latex.h"
#include "../include/Usefull_Functions.h"

char* escape_latex_path(const char* path)
{
    int len = strlen(path);
    char* out = (char*)malloc(len * 2 + 1); // buffer assez grand
    int j = 0;

    for (int i = 0; i < len; i++)
    {
        char c = path[i];
        switch (c)
        {
            case '_': out[j++] = '\\'; out[j++] = '_'; break;
            case '#': out[j++] = '\\'; out[j++] = '#'; break;
            case '%': out[j++] = '\\'; out[j++] = '%'; break;
            case '&': out[j++] = '\\'; out[j++] = '&'; break;
            case '{': out[j++] = '\\'; out[j++] = '{'; break;
            case '}': out[j++] = '\\'; out[j++] = '}'; break;
            case '\\':
                j += sprintf(out + j, "$\\backslash$");
                break;
            default:
                out[j++] = c;
        }
    }

    out[j] = '\0';
    return out;
}

void write_dataset_info(FILE* f, struct Genstruct* GS)
{
    fprintf(f, "\\section*{Dataset et Informations sur l'Entraînement}\n\n");

    fprintf(f, "\\subsection*{Dimensions du Dataset}\n");
    fprintf(f, "\\begin{itemize}\n");
    fprintf(f, "  \\item Nombre total de lignes : %d\\\\\n", GS->PD->nb_rows);
    fprintf(f, "  \\item Nombre de variables explicatives : %d\\\\\n", GS->TD->nb_modalites_D);
    fprintf(f, "  \\item Nombre de variables de la cible : %d\\\\\n", GS->TD->nb_modalites_T);
    fprintf(f, "\\end{itemize}\n\n");

    fprintf(f, "\\subsection*{Découpage Train / Test}\n");
    fprintf(f, "\\begin{itemize}\n");
    fprintf(f, "  \\item Ratio train : %.2f\\\\\n", GS->TD->ratio_train);
    fprintf(f, "  \\item Nombre de lignes train : %d\\\\\n", GS->TD->nb_rows_train);
    fprintf(f, "  \\item Nombre de lignes test : %d\\\\\n", GS->TD->nb_rows_test);
    fprintf(f, "\\end{itemize}\n\n");

    fprintf(f, "\\subsection*{Batching}\n");
    fprintf(f, "\\begin{itemize}\n");
    fprintf(f, "  \\item Taille d'un batch : %u\\\\\n", GS->TB->size_batch);
    fprintf(f, "  \\item Nombre total de batchs : %u\\\\\n", GS->TB->nb_batch);
    fprintf(f, "\\end{itemize}\n\n");

    fprintf(f, "\\subsection*{Informations supplémentaires}\n");
    fprintf(f, "\\begin{itemize}\n");
    const char* std_text;

    if (GS->TD->do_std_x)
        std_text = "Oui";
    else
        std_text = "Non";

    fprintf(f, "  \\item Standardisation appliquée sur X : %s\\\\\n", std_text);

    if (GS->TD->do_std_x)
        std_text = "Oui";
    else
        std_text = "Non";

    fprintf(f, "  \\item Standardisation appliquée sur Y : %s\\\\\n", std_text);

    fprintf(f, "  \\item Type de la cible : %u\\\\\n", GS->TD->type_tgt);
    char* safe_path = escape_latex_path(GS->PD->path);

	fprintf(f, "  \\item Chemin du fichier source : %s\\\\\n", safe_path);
    free(safe_path);
    fprintf(f, "\\end{itemize}\n\n");
}


void write_model_architecture_tex(FILE* f, struct Modele* M)
{
    fprintf(f, "\\subsubsection*{Architecture du Modèle}\n\n");

    fprintf(f, "\\begin{table}[H]\n");
    fprintf(f, "\\centering\n");
    // Ajouter une colonne pour Bias
    fprintf(f, "\\begin{tabular}{lcccc}\n");
    fprintf(f, "\\toprule\n");
    fprintf(f, "Layer & Dim In & Dim Out & Activation & Bias \\\\\n");
    fprintf(f, "\\midrule\n");

    // ---- 2) HIDDEN LAYERS ----
    for (int i = 0; i < M->nb_layers; i++)
    {
        fprintf(f,
                "Layer~%d & %d & %d & %s & %d \\\\\n",
                i + 1,
                M->tab_layers[i].dim_in,
                M->tab_layers[i].dim_out,
                M->_choices->name_activ[i],  // activation suivante
                M->tab_layers[i].bias      // affichage du bias (0/1)
        );
    }

    fprintf(f, "\\bottomrule\n");
    fprintf(f, "\\end{tabular}\n");
    fprintf(f, "\\caption{Architecture du réseau de neurones}\n");
    fprintf(f, "\\end{table}\n\n");

    // ------------------------------------------------------------
    // HYPERPARAMETRES DU MODELE
    // ------------------------------------------------------------
    fprintf(f, "\\subsubsection*{Hyperparamètres du Modèle}\n\n");

    fprintf(f, "\\begin{itemize}\n");

    fprintf(f, "    \\item \\textbf{Fonction de coût} : %s\n",
            M->_choices->name_cost);

    fprintf(f, "    \\item \\textbf{Optimiseur} : %s\n",
            M->_choices->name_optim);

    fprintf(f, "    \\item \\textbf{Learning Rate} : %.6f\n",
            M->OC.learning_rate);

    fprintf(f, "\\end{itemize}\n\n");
}

void write_histogram(FILE* f, const char* title, const char* metric_name, double* values, int nb_modele)
{
    fprintf(f, "\\begin{figure}[H]\n");
    fprintf(f, "\\centering\n");
    fprintf(f, "\\begin{tikzpicture}\n");
    fprintf(f, "\\begin{axis}[\n");
    fprintf(f, "    ybar,\n");
    fprintf(f, "    xlabel={Model},\n");
    fprintf(f, "    ylabel={%s},\n", metric_name);
    fprintf(f, "    ymin=0,\n");
    fprintf(f, "    ymax=1,\n");
    fprintf(f, "    width=12cm,\n");
    fprintf(f, "    height=6cm,\n");
    fprintf(f, "    symbolic x coords={");

	for (int i = 0; i < nb_modele; i++)
	{
	    fprintf(f, "M%d", i);
	    if (i + 1 < nb_modele) {fprintf(f, ",");}
	}

    fprintf(f, "},\n    xtick=data]\n");
    fprintf(f, "]\n");

    // Les données
    fprintf(f, "\\addplot coordinates {");
    for (int i = 0; i < nb_modele; i++)
        fprintf(f, "(M%d,%.6f)", i, values[i]);
    fprintf(f, "};\n");

    fprintf(f, "\\end{axis}\n");
    fprintf(f, "\\end{tikzpicture}\n");
    fprintf(f, "\\caption{%s}\n", title);
    fprintf(f, "\\end{figure}\n\n");
}

void write_confusion_matrix(FILE* f, struct Qual_Quality* Q, char** names_tgt)
{
    
    if (Q->nb_classes > 10)
        {
            unsigned long correct = 0;
            unsigned long errors  = 0;

            for (int i = 0; i < Q->nb_classes; i++)
            {
                for (int j = 0; j < Q->nb_classes; j++)
                {
                    if (i == j)
                        correct += Q->confusion_mat[i][j];
                    else
                        errors  += Q->confusion_mat[i][j];
                }
            }

            fprintf(f, "\\begin{table}[H]\n");
            fprintf(f, "\\centering\n");
            fprintf(f, "\\begin{tabular}{lc}\n");
            fprintf(f, "\\toprule\n");
            fprintf(f, "Mesure & Valeur \\\\\n");
            fprintf(f, "\\midrule\n");
            fprintf(f, "Prédictions correctes & %lu \\\\\n", correct);
            fprintf(f, "Erreurs & %lu \\\\\n", errors);
            fprintf(f, "\\bottomrule\n");
            fprintf(f, "\\end{tabular}\n");
            fprintf(f, "\\caption{Résumé des performances (trop de classes pour afficher la matrice)}\n");
            fprintf(f, "\\end{table}\n\n");

            return;
        }

    fprintf(f, "\\begin{table}[H]\n");
    fprintf(f, "\\centering\n");
    fprintf(f, "\\begin{tabular}{c");

    // Colonnes supplémentaires
    for (int j = 0; j < Q->nb_classes; j++)
        fprintf(f, "c");
    fprintf(f, "}\n\\toprule\n");

    // En-tête colonnes (prédictions)
    fprintf(f, " & ");
    for (int j = 0; j < Q->nb_classes; j++)
    {
        fprintf(f, "Pred~%s", names_tgt[j]);
        if (j + 1 < Q->nb_classes)
            fprintf(f, " & ");
        else
            fprintf(f, " \\\\\n");
    }

    fprintf(f, "\\midrule\n");

    // Lignes de la matrice
    for (int i = 0; i < Q->nb_classes; i++)
    {
        fprintf(f, "True~%s & ", names_tgt[i]);

        for (int j = 0; j < Q->nb_classes; j++)
        {
            fprintf(f, "%u", Q->confusion_mat[i][j]);
            if (j + 1 < Q->nb_classes)
                fprintf(f, " & ");
            else
                fprintf(f, " \\\\\n");
        }
    }

    fprintf(f, "\\bottomrule\n");
    fprintf(f, "\\end{tabular}\n");
    fprintf(f, "\\caption{Matrice de confusion}\n");
    fprintf(f, "\\end{table}\n\n");
}



void export_tex_ql(const char* filename, struct Genstruct* GS, struct Qual_Quality** tab_ql)
{
	
    FILE* f = fopen(filename, "w");
    if (!f) return;
    
    // Préambule
    fprintf(f,
        "\\documentclass{article}\n"
        "\\usepackage{geometry}\n"
        "\\usepackage{pgfplots}\n"
        "\\usepackage{booktabs}\n"
        "\\usepackage{float}\n"
        "\\pgfplotsset{compat=1.17}\n"
        "\\geometry{margin=2cm}\n"
        "\\begin{document}\n"
        "\\title{Comparaison des Modèles AutoTorch- Cible qualitative}\n"
        "\\maketitle\n"
    );
    write_dataset_info(f, GS);
    
    fprintf(f, "\\section*{Architecture et matrices de confusion}\n");
    for (int i = 0; i < GS->nb_modele; ++i)
    	{
    		fprintf(f, "\\subsection*{Modèle %d}\n", i);
            //printf("Avant architecture \n");
    		write_model_architecture_tex(f, &(GS->tab_modeles[i]));
            //printf("Avant confusion \n");
        	write_confusion_matrix(f, tab_ql[i], GS->TD->name_modalite_T);
    		//printf("Apres confusion \n");
    	}
    
    fprintf(f, "\\section*{Histogrammes de comparaison}\n");
    //printf("Avant allocation tableau \n");
    // Préparer les tableaux valeurs
	double* accuracy = (double*) malloc(GS->nb_modele * sizeof(double));
	double* f1       = (double*) malloc(GS->nb_modele * sizeof(double));
	double* precision= (double*) malloc(GS->nb_modele * sizeof(double));
	double* recall   = (double*) malloc(GS->nb_modele * sizeof(double));
	double* balacc   = (double*) malloc(GS->nb_modele * sizeof(double));
	double* topk     = (double*) malloc(GS->nb_modele * sizeof(double));
	double* logloss  = (double*) malloc(GS->nb_modele * sizeof(double));
	
    for (int i = 0; i < GS->nb_modele; ++i)
    {
        accuracy[i]  = tab_ql[i]->accuracy;
        f1[i]        = tab_ql[i]->f1;
        precision[i] = tab_ql[i]->precision;
        recall[i]    = tab_ql[i]->recall;
        balacc[i]    = tab_ql[i]->balanced_accuracy;
        topk[i]      = tab_ql[i]->top_k_accuracy;
        logloss[i]   = tab_ql[i]->log_loss;
    }
    
    // Histogrammes
    write_histogram(f, "Accuracy", "Accuracy", accuracy, GS->nb_modele);
    write_histogram(f, "F1-score", "F1-score", f1, GS->nb_modele);
    write_histogram(f, "Precision", "Precision", precision, GS->nb_modele);
    write_histogram(f, "Recall", "Recall", recall, GS->nb_modele);
    write_histogram(f, "Balanced Accuracy", "Balanced Accuracy", balacc, GS->nb_modele);
    write_histogram(f, "Top-k Accuracy", "Top-k Accuracy", topk, GS->nb_modele);
    write_histogram(f, "Log Loss", "Log Loss", logloss, GS->nb_modele);

    // Libération
    free(accuracy);
    free(f1);
    free(precision);
    free(recall);
    free(balacc);
    free(topk);
    free(logloss);
	
    fprintf(f, "\\end{document}\n");
    fclose(f);
    
}



void export_tex_qt(const char* filename, struct Genstruct* GS, struct Quant_Quality** tab_qt)
{
    
    FILE* f = fopen(filename, "w");
    if (!f) return;
    

    // Préambule LaTeX
    fprintf(f,
        "\\documentclass{article}\n"
        "\\usepackage{geometry}\n"
        "\\usepackage{pgfplots}\n"
        "\\usepackage{booktabs}\n"
        "\\usepackage{float}\n"
        "\\pgfplotsset{compat=1.17}\n"
        "\\geometry{margin=2cm}\n"
        "\\begin{document}\n"
        "\\title{Comparaison des Modèles AutoTorch - Cible Quantitative}\n"
        "\\maketitle\n"
    );
    write_dataset_info(f, GS); // même fonction que pour QL si tu veux les infos dataset
    

    fprintf(f, "\\section*{Architecture et métriques quantitatives}\n");

    for (int i = 0; i < GS->nb_modele; ++i)
    {
        fprintf(f, "\\subsection*{Modèle %d}\n", i);
        write_model_architecture_tex(f, &(GS->tab_modeles[i])); // réutilisation de ta fonction architecture

        // Tableau des métriques quantitatives
        fprintf(f,
            "\\begin{tabular}{lrrrrrr}\n"
            "\\toprule\n"
            "Métrique & MSE & RMSE & MAE & R² & MAPE & Pearson \\\\\n"
            "\\midrule\n"
            "Valeur & %.6f & %.6f & %.6f & %.6f & %.6f & %.6f \\\\\n"
            "\\bottomrule\n"
            "\\end{tabular}\n\n",
            tab_qt[i]->mse,
            tab_qt[i]->rmse,
            tab_qt[i]->mae,
            tab_qt[i]->r2,
            tab_qt[i]->mape,
            tab_qt[i]->pearson
        );
    }

    fprintf(f, "\\section*{Histogrammes de comparaison}\n");

    // Préparer les tableaux de valeurs pour histogrammes
    double* mse     = (double*) malloc(GS->nb_modele * sizeof(double));
    double* rmse    = (double*) malloc(GS->nb_modele * sizeof(double));
    double* mae     = (double*) malloc(GS->nb_modele * sizeof(double));
    double* r2      = (double*) malloc(GS->nb_modele * sizeof(double));
    double* mape    = (double*) malloc(GS->nb_modele * sizeof(double));
    double* pearson = (double*) malloc(GS->nb_modele * sizeof(double));

    
    for (int i = 0; i < GS->nb_modele; ++i)
    {
        mse[i]     = tab_qt[i]->mse;
        rmse[i]    = tab_qt[i]->rmse;
        mae[i]     = tab_qt[i]->mae;
        r2[i]      = tab_qt[i]->r2;
        mape[i]    = tab_qt[i]->mape;
        pearson[i] = tab_qt[i]->pearson;
    }

    
    // Histogrammes
    write_histogram(f, "MSE", "MSE", mse, GS->nb_modele);
    write_histogram(f, "RMSE", "RMSE", rmse, GS->nb_modele);
    write_histogram(f, "MAE", "MAE", mae, GS->nb_modele);
    write_histogram(f, "R2", "R²", r2, GS->nb_modele);
    write_histogram(f, "MAPE", "MAPE", mape, GS->nb_modele);
    write_histogram(f, "Pearson", "Pearson", pearson, GS->nb_modele);

    // Libération
    free(mse);
    free(rmse);
    free(mae);
    free(r2);
    free(mape);
    free(pearson);

    
    fprintf(f, "\\end{document}\n");
    fclose(f);
    
}

void export_tex_qb(const char* filename, struct Genstruct* GS, struct Bool_Quality** tab_qb)
{
    
    FILE* f = fopen(filename, "w");
    if (!f) return;
   

    // Préambule LaTeX
    fprintf(f,
        "\\documentclass{article}\n"
        "\\usepackage{geometry}\n"
        "\\usepackage{pgfplots}\n"
        "\\usepackage{booktabs}\n"
        "\\usepackage{float}\n"
        "\\pgfplotsset{compat=1.17}\n"
        "\\geometry{margin=2cm}\n"
        "\\begin{document}\n"
        "\\title{Comparaison des Modèles AutoTorch - Cible booléenne}\n"
        "\\maketitle\n"
    );

    write_dataset_info(f, GS);
    

    fprintf(f, "\\section*{Architecture et matrices de confusion}\n");

    for (int i = 0; i < GS->nb_modele; ++i)
    {
        fprintf(f, "\\subsection*{Modèle %d}\n", i);
        write_model_architecture_tex(f, &(GS->tab_modeles[i]));

        // ---- Matrice de confusion binaire (2x2) ----
        fprintf(f,
            "\\subsubsection*{Matrice de confusion}\n"
            "\\begin{tabular}{lcc}\n"
            "\\toprule\n"
            " & Pred~0 & Pred~1 \\\\\n"
            "\\midrule\n"
            "True~0 & %u & %u \\\\\n"
            "True~1 & %u & %u \\\\\n"
            "\\bottomrule\n"
            "\\end{tabular}\n\n",
            tab_qb[i]->TN, tab_qb[i]->FP,
            tab_qb[i]->FN, tab_qb[i]->TP
        );

        // ---- Tableau des métriques ----
        fprintf(f,
            "\\subsubsection*{Métriques}\n"
            "\\begin{tabular}{lr}\n"
            "\\toprule\n"
            "Métrique & Valeur \\\\\n"
            "\\midrule\n"
            "Accuracy & %.6f \\\\\n"
            "Precision & %.6f \\\\\n"
            "Recall & %.6f \\\\\n"
            "Specificity & %.6f \\\\\n"
            "F1-score & %.6f \\\\\n"
            "Log Loss & %.6f \\\\\n"
            "\\bottomrule\n"
            "\\end{tabular}\n\n",
            tab_qb[i]->accuracy,
            tab_qb[i]->precision,
            tab_qb[i]->recall,
            tab_qb[i]->specificity,
            tab_qb[i]->f1,
            tab_qb[i]->log_loss
        );
    }

    fprintf(f, "\\section*{Histogrammes de comparaison}\n");

    // ---- Préparation tableaux valeurs ----
    double* accuracy    = (double*) malloc(GS->nb_modele * sizeof(double));
    double* precision   = (double*) malloc(GS->nb_modele * sizeof(double));
    double* recall      = (double*) malloc(GS->nb_modele * sizeof(double));
    double* specificity = (double*) malloc(GS->nb_modele * sizeof(double));
    double* f1          = (double*) malloc(GS->nb_modele * sizeof(double));
    double* logloss     = (double*) malloc(GS->nb_modele * sizeof(double));


    for (int i = 0; i < GS->nb_modele; ++i)
    {
        accuracy[i]    = tab_qb[i]->accuracy;
        precision[i]   = tab_qb[i]->precision;
        recall[i]      = tab_qb[i]->recall;
        specificity[i] = tab_qb[i]->specificity;
        f1[i]          = tab_qb[i]->f1;
        logloss[i]     = tab_qb[i]->log_loss;
    }


    // ---- Histogrammes ----
    write_histogram(f, "Accuracy",    "Accuracy",    accuracy, GS->nb_modele);
    write_histogram(f, "Precision",   "Precision",   precision, GS->nb_modele);
    write_histogram(f, "Recall",      "Recall",      recall, GS->nb_modele);
    write_histogram(f, "Specificity", "Specificity", specificity, GS->nb_modele);
    write_histogram(f, "F1-score",    "F1-score",    f1, GS->nb_modele);
    write_histogram(f, "Log Loss",    "Log Loss",    logloss, GS->nb_modele);

    // Libération mémoire
    free(accuracy);
    free(precision);
    free(recall);
    free(specificity);
    free(f1);
    free(logloss);


    fprintf(f, "\\end{document}\n");
    fclose(f);
}


void export_tex_report(struct Genstruct* GS, struct Qualitys**tab_q)
{
    char filename[256];
    sprintf(filename, "output/latex_quality/quality_modeles.txt");

	if (GS->TD->type_tgt == 0)
		{

			struct Qual_Quality** tab_ql = (struct Qual_Quality**)malloc(GS->nb_modele*sizeof(struct Qual_Quality*));
			for (int i = 0; i < GS->nb_modele; ++i)
				{
					tab_ql[i] = &(tab_q[i]->QL);
				}

			export_tex_ql(filename, GS, tab_ql);


			return;
		}

	if (GS->TD->type_tgt == 1)
		{

			struct Quant_Quality** tab_qt = (struct Quant_Quality**)malloc(GS->nb_modele*sizeof(struct Quant_Quality*));
			for (int i = 0; i < GS->nb_modele; ++i)
				{
					tab_qt[i] = &(tab_q[i]->QT);
				}

			export_tex_qt(filename, GS, tab_qt);


			return;

		}

	if (GS->TD->type_tgt == 2)
		{

			struct Bool_Quality** tab_qb = (struct Bool_Quality**)malloc(GS->nb_modele*sizeof(struct Bool_Quality*));
			for (int i = 0; i < GS->nb_modele; ++i)
				{
					tab_qb[i] = &(tab_q[i]->QB);
				}

			export_tex_qb(filename, GS, tab_qb);


			return;

		}

}