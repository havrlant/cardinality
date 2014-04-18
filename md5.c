#include "md5.h"

void str2md5(const char *str, byte *checksum) {
    int length = (int)strlen(str);
    MD5_CTX c;
    const int blocklength = 512;
    MD5_Init(&c);
    
    while (length > 0) {
        if (length > blocklength) {
            MD5_Update(&c, str, blocklength);
        } else {
            MD5_Update(&c, str, length);
        }
        length -= blocklength;
        str += blocklength;
    }
    
    MD5_Final(checksum, &c);
}
