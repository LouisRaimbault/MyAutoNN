#ifndef EXPORT_LATEX
#define EXPORT_LATEX

#include "Modele.h"

void write_dataset_info(FILE* f, struct Genstruct* GS);
void write_model_architecture_tex(FILE* f, struct Modele* M);
void write_histogram(FILE* f, const char* title, const char* metric_name, double* values, int nb_modele);
void write_confusion_matrix(FILE* f, struct Qual_Quality* Q, char** names_tgt);
void export_tex_ql(const char* filename, struct Genstruct* GS, struct Qual_Quality** tab_ql);
void export_tex_qt(const char* filename, struct Genstruct* GS, struct Quant_Quality** tab_qt);
void export_tex_qb(const char* filename, struct Genstruct* GS, struct Bool_Quality** tab_qb);
void export_tex_report(struct Genstruct* GS, struct Qualitys**tab_q);

#endif