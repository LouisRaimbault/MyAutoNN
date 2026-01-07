#include "../include/Get_Quality_Modele.h"
#include "../include/Usefull_Functions.h"



struct Quant_Quality get_quant_qualitys(torch::Tensor y_pred, torch::Tensor y_true)
{
    // ------------------------------------------------------------
    // 0️⃣ Préparation des tenseurs
    // ------------------------------------------------------------
    y_pred = y_pred.detach().cpu();
    y_true = y_true.detach().cpu();

    // Flatten au cas où les tenseurs seraient multidimensionnels
    y_pred = y_pred.view(-1);
    y_true = y_true.view(-1);

    // ------------------------------------------------------------
    // 1️⃣ Erreurs de base
    // ------------------------------------------------------------
    torch::Tensor diff = y_pred - y_true;
    torch::Tensor diff_abs = torch::abs(diff);
    torch::Tensor diff_sq  = diff * diff;

    double mse  = diff_sq.mean().item<double>();
    double rmse = std::sqrt(mse);
    double mae  = diff_abs.mean().item<double>();

    // ------------------------------------------------------------
    // 2️⃣ R2 score
    // ------------------------------------------------------------
    double mean_true = y_true.mean().item<double>();
    double ss_tot = ((y_true - mean_true).pow(2)).sum().item<double>();
    double ss_res = diff_sq.sum().item<double>();
    double r2 = 1.0 - ss_res / (ss_tot + 1e-12);

    // ------------------------------------------------------------
    // 3️⃣ MAPE
    // ------------------------------------------------------------
    torch::Tensor epsilon = torch::full_like(y_true, 1e-12); // éviter division par 0
    torch::Tensor denom = torch::max(torch::abs(y_true), epsilon);
    double mape = (diff_abs / denom).mean().item<double>() * 100.0;

    // ------------------------------------------------------------
    // 4️⃣ Pearson correlation
    // ------------------------------------------------------------
    double mean_pred = y_pred.mean().item<double>();
    torch::Tensor cov = ((y_pred - mean_pred) * (y_true - mean_true));
    double cov_val = cov.mean().item<double>();
    double std_pred = std::sqrt(((y_pred - mean_pred).pow(2)).mean().item<double>());
    double std_true = std::sqrt(((y_true - mean_true).pow(2)).mean().item<double>());
    double pearson = cov_val / (std_pred * std_true + 1e-12);

    // ------------------------------------------------------------
    // 5️⃣ Remplir la structure résultat
    // ------------------------------------------------------------
    struct Quant_Quality result;
    result.mse     = mse;
    result.rmse    = rmse;
    result.mae     = mae;
    result.r2      = r2;
    result.mape    = mape;
    result.pearson = pearson;

    return result;
}



struct Qual_Quality get_qual_qualitys(torch::Tensor y_pred, torch::Tensor y_true, int top_k)
{
    // ------------------------------------------------------------
    // 0️⃣ Préparation des tenseurs
    // ------------------------------------------------------------

    if (y_pred.dim() == 3) 
        { // séquence
            int B = y_pred.size(0);
            int T = y_pred.size(1);
            int C = y_pred.size(2);

            y_pred = y_pred.contiguous().view({B * T, C});
            y_true = y_true.contiguous().view({B * T});
        }

    y_pred = y_pred.detach().cpu();
    y_true = y_true.detach().cpu();

    // Convertit y_true one-hot → indices
    if (y_true.dim() > 1 && y_true.size(1) > 1) 
        y_true = y_true.argmax(1);

    // Convertit logits → probabilités
    torch::Tensor probs = torch::softmax(y_pred, 1);
    torch::Tensor pred_labels = probs.argmax(1);

    int nb_classes = probs.size(1);

    // ------------------------------------------------------------
    // 1️⃣ Accuracy & top-k accuracy
    // ------------------------------------------------------------
    double accuracy = (pred_labels.eq(y_true)).to(torch::kFloat32).mean().item<double>();

    torch::Tensor topk_idx = std::get<1>(torch::topk(probs, top_k, 1, true, true));
    torch::Tensor correct_topk = (topk_idx.eq(y_true.unsqueeze(1))).any(1);
    double top_k_accuracy = correct_topk.to(torch::kFloat32).mean().item<double>();

    // ------------------------------------------------------------
    // 2️⃣ Log Loss (Cross-Entropy)
    // ------------------------------------------------------------
    torch::Tensor log_probs = torch::log_softmax(y_pred, 1);
    torch::Tensor nll = torch::nll_loss(log_probs, y_true);
    double log_loss = nll.item<double>();

    // ------------------------------------------------------------
    // 3️⃣ Précision, rappel, F1 et balanced accuracy (macro moyenne)
    // ------------------------------------------------------------
    double precision = 0.0, recall = 0.0, f1 = 0.0, balanced_acc = 0.0;

    // Créer matrice de confusion
    uint16_t** confusion_mat = (uint16_t**)malloc(nb_classes*sizeof(uint16_t*));

    for (int i = 0; i < nb_classes; ++i)
    {
        confusion_mat[i] = (uint16_t*)malloc(nb_classes*sizeof(uint16_t));
        std::fill(confusion_mat[i], confusion_mat[i] + nb_classes, 0);
    }

    // Remplissage de la matrice de confusion
    for (int64_t i = 0; i < y_true.size(0); ++i)
    {
        int true_label = y_true[i].item<int>();
        int pred_label = pred_labels[i].item<int>();
        if (true_label >= 0 && true_label < nb_classes &&
            pred_label >= 0 && pred_label < nb_classes)
        {
            confusion_mat[true_label][pred_label]++;
        }
    }

    // Calcul des métriques par classe
    for (int c = 0; c < nb_classes; ++c)
    {
        double tp = confusion_mat[c][c];
        double fn = 0.0, fp = 0.0;

        for (int i = 0; i < nb_classes; ++i)
        {
            if (i != c)
            {
                fn += confusion_mat[c][i];  // vrais = c mais prédits autre
                fp += confusion_mat[i][c];  // prédits = c mais vrais autre
            }
        }

        // Total des échantillons
        double total = 0.0;
        for (int i = 0; i < nb_classes; ++i)
            {
                for (int j = 0; j < nb_classes; ++j) {total += confusion_mat[i][j];}
            }

        double tn = total - tp - fp - fn;

        double prec_c = tp / (tp + fp + 1e-12);
        double rec_c  = tp / (tp + fn + 1e-12);
        double f1_c   = 2.0 * prec_c * rec_c / (prec_c + rec_c + 1e-12);

        precision += prec_c;
        recall += rec_c;
        f1 += f1_c;
        balanced_acc += rec_c; // balanced accuracy = moyenne des recalls
    }

    precision /= nb_classes;
    recall /= nb_classes;
    f1 /= nb_classes;
    balanced_acc /= nb_classes;

    // ------------------------------------------------------------
    // 4️⃣ Remplir la structure résultat
    // ------------------------------------------------------------
    struct Qual_Quality result;
    result.precision = precision;
    result.accuracy = accuracy;
    result.recall = recall;
    result.f1 = f1;
    result.log_loss = log_loss;
    result.balanced_accuracy = balanced_acc;
    result.top_k_accuracy = top_k_accuracy;
    result.confusion_mat = confusion_mat;
    result.nb_classes = nb_classes;

    return result;
}





struct Bool_Quality get_bool_qualitys(torch::Tensor y_pred, torch::Tensor y_true)
{
    // ------------------------------------------------------------
    // 0️⃣ Préparation des tenseurs
    // ------------------------------------------------------------
    y_pred = y_pred.detach().cpu();
    y_true = y_true.detach().cpu();

    // Convertit logits → probabilités
    torch::Tensor probs = torch::sigmoid(y_pred);  // binaire
    torch::Tensor pred_labels = (probs >= 0.5).to(torch::kInt64);

    // ------------------------------------------------------------
    // 1️⃣ TP, TN, FP, FN
    // ------------------------------------------------------------
    torch::Tensor tp_t = ((pred_labels == 1) & (y_true == 1)).to(torch::kFloat32);
    torch::Tensor tn_t = ((pred_labels == 0) & (y_true == 0)).to(torch::kFloat32);
    torch::Tensor fp_t = ((pred_labels == 1) & (y_true == 0)).to(torch::kFloat32);
    torch::Tensor fn_t = ((pred_labels == 0) & (y_true == 1)).to(torch::kFloat32);

    double TP = tp_t.sum().item<double>();
    double TN = tn_t.sum().item<double>();
    double FP = fp_t.sum().item<double>();
    double FN = fn_t.sum().item<double>();

    // ------------------------------------------------------------
    // 2️⃣ Calcul des métriques
    // ------------------------------------------------------------
    double accuracy    = (TP + TN) / (TP + TN + FP + FN + 1e-12);
    double precision   = TP / (TP + FP + 1e-12);
    double recall      = TP / (TP + FN + 1e-12);
    double specificity = TN / (TN + FP + 1e-12);
    double f1          = 2.0 * precision * recall / (precision + recall + 1e-12);

    // Log-loss binaire
    torch::Tensor log_loss_t = -(y_true.to(torch::kFloat32) * torch::log(probs + 1e-12)
                                 + (1 - y_true.to(torch::kFloat32)) * torch::log(1 - probs + 1e-12));
    double log_loss = log_loss_t.mean().item<double>();

    // ------------------------------------------------------------
    // 3️⃣ Remplir la structure résultat
    // ------------------------------------------------------------
    struct Bool_Quality result;
    result.TP = (uint32_t)TP;
    result.TN = (uint32_t)TN;
    result.FP = (uint32_t)FP;
    result.FN = (uint32_t)FN;
    result.accuracy = accuracy;
    result.precision = precision;
    result.recall = recall;
    result.specificity = specificity;
    result.f1 = f1;
    result.log_loss = log_loss;

    return result;
}


void print_qual_qualitys(struct Qual_Quality* Ql)
{
    if (Ql->nb_classes < 11)
        {
            std::cout << "\n============================================================\n";
            std::cout << "                  MATRICE DE CONFUSION                    \n";
            std::cout << "============================================================\n\n";

            // En-tête colonnes
            std::cout << std::setw(10) << " ";
            for (int j = 0; j < Ql->nb_classes; ++j)
                std::cout << std::setw(10) << ("Pred " + std::to_string(j));
            std::cout << "\n";

            // Lignes de la matrice
            for (int i = 0; i < Ql->nb_classes; ++i)
            {
                std::cout << std::setw(10) << ("True " + std::to_string(i));
                for (int j = 0; j < Ql->nb_classes; ++j)
                {
                    std::cout << std::setw(10) << Ql->confusion_mat[i][j];
                }
                std::cout << "\n";
            }

        }

    if (Ql->nb_classes > 10)
        {
            long long total_correct = 0;
            long long total_errors  = 0;

            for (int i = 0; i < Ql->nb_classes; ++i)
            {
                total_correct += Ql->confusion_mat[i][i];
                for (int j = 0; j < Ql->nb_classes; ++j)                    
                    {
                        if (i != j) {total_errors += Ql->confusion_mat[i][j];}
                    }    
            }

            // ---------- Tableau global ----------
            std::cout << "\n              GLOBAL RESUME\n";
            std::cout << "------------------------------------------------------------\n";
            std::cout << std::left << std::setw(25) << "Correct predictions"
                      << " : " << total_correct << "\n";
            std::cout << std::left << std::setw(25) << "Incorrect predictions"
                      << " : " << total_errors << "\n";

            // ---------- Tableau par classe ----------
            std::cout << "\n              DETAILS\n";
            std::cout << "------------------------------------------------------------\n\n";

            std::cout << std::setw(10) << "Class"
                      << std::setw(15) << "Well predicted"
                      << std::setw(15) << "Wrongly pred"
                      << std::setw(15) << "Missed"
                      << "\n";

            for (int i = 0; i < Ql->nb_classes; ++i)
            {
                long long TP = Ql->confusion_mat[i][i];
                long long FP = 0;
                long long FN = 0;

                for (int j = 0; j < Ql->nb_classes; ++j)
                {
                    if (j != i)
                    {
                        FP += Ql->confusion_mat[j][i];
                        FN += Ql->confusion_mat[i][j];
                    }
                }

                std::cout << std::setw(10) << i
                          << std::setw(15) << TP
                          << std::setw(15) << FP
                          << std::setw(15) << FN
                          << "\n";
            }
        }

    std::cout << "\n============================================================\n\n";
    std::cout << "\n============================================================\n";
    std::cout << "                    QUALITÉ DU MODÈLE                     \n";
    std::cout << "============================================================\n\n";

    auto print_metric = [&](const char* name, double value)
    {
        std::cout << std::left << std::setw(25) << name
                  << " : " << std::fixed << std::setprecision(6) << value << "\n";
    };

    print_metric("Accuracy",            Ql->accuracy);
    print_metric("Balanced accuracy",   Ql->balanced_accuracy);
    print_metric("Precision",           Ql->precision);
    print_metric("Recall",              Ql->recall);
    print_metric("F1-score",            Ql->f1);
    print_metric("Top-K Accuracy",      Ql->top_k_accuracy);
    print_metric("Log Loss",            Ql->log_loss);

    std::cout << "\n============================================================\n\n";
}


void print_quant_qualitys(struct Quant_Quality* Qt)
{
    std::cout << "\n============================================================\n";
    std::cout << "                    QUALITÉ DU MODÈLE                     \n";
    std::cout << "============================================================\n\n";

    auto print_metric = [&](const char* name, double value)
    {
        std::cout << std::left << std::setw(25) << name
                  << " : " << std::fixed << std::setprecision(6) << value << "\n";
    };

    print_metric("Mean Squared Error (MSE)",          Qt->mse);
    print_metric("Root Mean Squared Error (RMSE)",   Qt->rmse);
    print_metric("Mean Absolute Error (MAE)",        Qt->mae);
    print_metric("R² Score",                         Qt->r2);
    print_metric("Mean Absolute Percentage Error",   Qt->mape);
    print_metric("Pearson Correlation",             Qt->pearson);

    std::cout << "\n============================================================\n\n";
}



void print_bool_qualitys(struct Bool_Quality* Qb)
{
    std::cout << "\n============================================================\n";
    std::cout << "                 MATRICE DE CONFUSION (binaire)            \n";
    std::cout << "============================================================\n\n";

    //         Pred 0   Pred 1
    std::cout << std::setw(12) << " "
              << std::setw(12) << "Pred 0"
              << std::setw(12) << "Pred 1" << "\n";

    // True 0: TN, FP
    std::cout << std::setw(12) << "True 0"
              << std::setw(12) << Qb->TN
              << std::setw(12) << Qb->FP << "\n";

    // True 1: FN, TP
    std::cout << std::setw(12) << "True 1"
              << std::setw(12) << Qb->FN
              << std::setw(12) << Qb->TP << "\n";

    std::cout << "\n============================================================\n";
    std::cout << "                     QUALITÉ DU MODÈLE                     \n";
    std::cout << "============================================================\n\n";

    auto print_metric = [&](const char* name, double value)
    {
        std::cout << std::left << std::setw(25) << name
                  << " : " << std::fixed << std::setprecision(6) << value << "\n";
    };

    print_metric("Accuracy",       Qb->accuracy);
    print_metric("Precision",      Qb->precision);
    print_metric("Recall (TPR)",   Qb->recall);
    print_metric("Specificity",    Qb->specificity);
    print_metric("F1-score",       Qb->f1);
    print_metric("Log Loss",       Qb->log_loss);

    std::cout << "\n============================================================\n\n";
}