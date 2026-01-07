#ifndef GET_BATCH
#define GET_BATCH

#include "Modele.h"

void clean_torch_batch (struct Torch_Batch* TB);
void get_batch(struct Genstruct* GS);
void get_batch_no_tgt(struct Genstruct* GS);
void construct_batch (struct Genstruct* GS);
#endif