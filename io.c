#include "io.h"

FILE* try_fopen(const char *path) {
    FILE* fd = fopen(path, "r");
    if (fd == NULL) {
        printf("Nepodarilo se otevrit soubor %s", path);
        exit(EXIT_FAILURE);
    }
    return fd;
}

int try_open_dir(tinydir_dir *dir, const char* path) {
    if (tinydir_open(dir, path) == -1) {
        perror("Error opening file");
        return 0;
    }
    return 1;
}
