#include "../include/Usefull_Functions.h"
#include "../include/Modele.h"
#include "../include/Get_Config.h"
#include "../include/Get_Data.h"
#include "../include/Get_Batch.h"
#include "../include/Activation_Functions.h"
#include "../include/Cost_Functions.h"
#include "../include/Export_Latex.h"
#include "../include/Export_Modele.h"
#include "../include/Use_Modele.h"
#include "../include/one_shot.h"



void afficher_tableau_type_layer()
{
    const char* type_layer[] =
    {
        "LIN ",
        "RNN",
        "GRU",
        "LSTM"
    };

    const char* descriptions[] =
    {
        "Linear ",
        "RNN simple avec activation tanh",
        "GRU - Gated Recurrent Unit",
        "LSTM - Long Short-Term Memory"
    };

    const int n = 4;

    std::cout << "\n+------+--------+-------------------------------------------+\n";
    std::cout << "|  #   | Type   | Description                               |\n";
    std::cout << "+------+--------+-------------------------------------------+\n";

    for (int i = 0; i < n; i++)
    {
        std::cout << "| "
                  << std::setw(3) << i << "  | "
                  << std::left << std::setw(6) << type_layer[i]
                  << " | "
                  << std::left << std::setw(41) << descriptions[i]
                  << "|\n";
    }

    std::cout << "+------+--------+-------------------------------------------+\n";
}



void afficher_tableau_weight_init()
{
    const char* noms[] =
    {
    	"torch choice"
        "rdm_ab (matrice)",
        "xavier_uniform",
        "xavier_normal",
        "kaiming_uniform",
        "kaiming_normal"
    };

    const char* descriptions[] =
    {
    	"Choix automatique de torch"
        "Remplit avec valeurs aleatoires uniformes entre [inf, sup] (2D)",
        "Initialisation Xavier uniforme (Glorot) - favorise variance constante",
        "Initialisation Xavier normale (Glorot) - gaussienne centrée",
        "Initialisation Kaiming uniforme - adaptée a ReLU",
        "Initialisation Kaiming normale - adaptée a ReLU"
    };

    const int n = 6;

    std::cout << "\n+------+-------------------+-------------------------------------------------------------+\n";
    std::cout << "|  #   | Nom               | Description                                                 |\n";
    std::cout << "+------+-------------------+-------------------------------------------------------------+\n";

    for (int i = 0; i < n; i++)
    {
        std::cout << "| "
                  << std::setw(3) << i << "  | "
                  << std::left << std::setw(17) << noms[i]
                  << " | "
                  << std::left << std::setw(57) << descriptions[i]
                  << "|\n";
    }

    std::cout << "+------+-------------------+-------------------------------------------------------------+\n";
}


void afficher_tableau_costs()
{
    const char* noms[] =
    {
        "Mean Squared Error",
        "Mean Absolute Error (L1)",
        "Smooth L1 / Huber",
        "Cross Entropy",
        "Binary Cross-Entropy With Logits",
        "nll"
    };

    const char* types_target[] =
    {
        "continuous tensor",
        "continuous tensor",
        "continuous tensor",
        "class indices (long tensor)",
        "binary tensor (0/1)",
        "xxx"
    };

    const int n = 6;

    std::cout << "\n+------+--------------------------------------+--------------------------+\n";
    std::cout << "|  #   | Loss Function                        | Type of target           |\n";
    std::cout << "+------+--------------------------------------+--------------------------+\n";

    for (int i = 0; i < n; i++)
    {
        std::cout << "| "
                  << std::setw(3) << i << "  | "
                  << std::left << std::setw(36) << noms[i]
                  << " | "
                  << std::left << std::setw(24) << types_target[i]
                  << "|\n";
    }

    std::cout << "+------+--------------------------------------+--------------------------+\n";
}


void afficher_tableau_activations()
{
    // Tableau local des noms correspondant aux fonctions
    const char* noms[] = 
    {
        "none",
        "relu",
        "tanh",
        "sigmoid",
        "leakyrelu"
    };

    // Vérifie que la taille demandée ne dépasse pas le tableau de noms

    // Affichage du tableau ASCII
    std::cout << "\n+------+-------------------+\n";
    std::cout << "|  #   | Fonction          |\n";
    std::cout << "+------+-------------------+\n";

    for (int i = 0; i < 5; i++)
    {
        std::cout << "| "
                  << std::setw(3) << i << "  | "
                  << std::left << std::setw(17) << noms[i]
                  << "|\n";
    }

    std::cout << "+------+-------------------+\n";
}

void afficher_tableau_optimizers()
{
    const char* noms[] = 
    {
        "SGD",
        "Adam",
        "AdamW",
        "RMSprop",
        "Adagrad",
        "LBFGS"
    };

    const char* descriptions[] = 
    {
        "Stochastic Gradient Descent",
        "Adaptive Moment Estimation",
        "Adam with decoupled weight decay",
        "Root Mean Square Propagation",
        "Adaptive Gradient Algorithm",
        "Limited-memory BFGS"
    };

    const int n = 6;

    // Affichage du tableau ASCII
    std::cout << "\n+------+----------------+-------------------------------------------+\n";
    std::cout << "|  #   | Nom Optimizer  | Description                               |\n";
    std::cout << "+------+----------------+-------------------------------------------+\n";

    for (int i = 0; i < n; i++)
    {
        std::cout << "| "
                  << std::setw(3) << i << "  | "
                  << std::left << std::setw(14) << noms[i]
                  << " | "
                  << std::left << std::setw(41) << descriptions[i]
                  << "|\n";
    }

    std::cout << "+------+----------------+-------------------------------------------+\n";
}

void get_genuse_paths (struct Genuse* GU, char* path_paths)
{   
    
    FILE *fichier = fopen(path_paths, "r");
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
    GU->path_base  = (char*)malloc(strlen(buf)+1);
    strcpy(GU->path_base, buf);

    while (car != '#') {car = fgetc(fichier);} 
    while ((car = fgetc(fichier)) != ',') {buf[y++] = car;} buf[y] = '\0'; y = 0;
    GU->path_info_base  = (char*)malloc(strlen(buf)+1);
    strcpy(GU->path_info_base, buf);

    while (car != '#') {car = fgetc(fichier);} 
    while ((car = fgetc(fichier)) != ',') {buf[y++] = car;} buf[y] = '\0'; y = 0;
    GU->path_info_model  = (char*)malloc(strlen(buf)+1);
    strcpy(GU->path_info_model, buf);

    fclose(fichier);
}



int main (int argc, char** argv)
{
	print_debog(0,"*.*.*.*.*.*.*.*.*.*.*.*.*.*.*.* Welcome to AutoTorch Prog  : *.*.*.*.*.*.*.*.*.*.*.*.*.*.*.*\n\n", "white");

        std::cout << TORCH_VERSION_MAJOR << "."
              << TORCH_VERSION_MINOR << "."
              << TORCH_VERSION_PATCH << std::endl;


	
	if (atoi(argv[1]) == 1)
		{
            srandom(time(NULL));
            struct Genstruct GS;
            GS.PD = (struct Pure_Data*)malloc(sizeof(struct Pure_Data));
            GS.TD = (struct Torch_Data*)malloc(sizeof(struct Torch_Data));
            GS.TB = (struct Torch_Batch*)malloc(sizeof(struct Torch_Batch));
			
			get_config_from_configfile (argv[2], &GS);
			
			get_data (&GS);
			construct_batch(&GS);
			get_config_from_data (&GS);

            torch::Device device (torch::kCPU);


            struct Qualitys** tab_q = (struct Qualitys**)malloc(GS.nb_modele*sizeof(struct Qualitys*));

            for (int i = 0; i < GS.nb_modele; ++i)
                {
                    print_modele(&(GS.tab_modeles[i]));
                    modele_init(&(GS.tab_modeles[i]));
                    modele_train (&(GS.tab_modeles[i]), GS.TB ,device);
                    modele_test (&(GS.tab_modeles[i]), GS.TD, GS.PD->type_targetmodel);
                    tab_q[i] = GS.tab_modeles[i].Q;
                    export_modele_file(&(GS.tab_modeles[i]), i);
                }


            export_tex_report(&GS, tab_q);

            clean_torch_batch (GS.TB);

            clean_torch_data (GS.TD, &GS);

            for (int i = 0; i < GS.nb_modele; ++i) {clean_modele(&(GS.tab_modeles[i]));}
            
            delete [] GS.tab_modeles;

            return 0;
		}

    if (atoi(argv[1]) == 2)
        {

            struct Genuse GU;
            get_genuse_paths(&GU, argv[2]);
            get_use_data(&GU);
            import_modele_file(&GU);
            modele_use(&GU);
            clean_softmodele (GU.SM);
            clean_use_data(GU.UD);

        }



 	return 0;	
}
