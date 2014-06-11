#include "io.h"

FILE* try_fopen(const char *path) {
    FILE* fd = fopen(path, "r");
    if (fd == NULL) {
        printf("Nepodarilo se otevrit soubor %s", path);
        exit(EXIT_FAILURE);
    }
    return fd;
}