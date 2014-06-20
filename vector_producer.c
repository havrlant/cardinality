#include "vector_producer.h"

void save_vector(Hyperloglog *hll, char *filename) {
    char path[256];
    strcpy(path, "../compress/vectors/");
    strcat(path, filename);
    FILE *fp = fopen(path, "wb");
    
    if (fp == NULL) {
        perror("Neotevrel se soubor ");
        return;
    }
    
    if (!fwrite(hll->M, sizeof(byte), hll->m, fp)) {
        perror("Chyba pri zapise ");
    }
    
    fclose(fp);
}

void save_sparse(Hyperloglog *hll, char *filename) {
    char path[256];
    strcpy(path, "../compress/sparse/");
    strcat(path, filename);
    
    double V = (double)count_zero_buckets(hll);
    uint j = 0;
    uint16_t index;
    if ((V / (double)hll->m) >= 2.0 / 3.0) {
        FILE *fp = fopen(path, "wr");
        
        for (uint i = 0; i < hll->m; i++) {
            if (hll->M[i] > 0) {
                index = (uint16_t)i;
                fwrite(&index, 2, 1, fp);
                fwrite(&(hll->M[i]), 1, 1, fp);
                j++;
            }
        }
        
        fclose(fp);
    }
    
    save_vector(hll, filename);
}

