#include "md5.h"

byte *str2md5(const char *str, int length) {
    MD5_CTX c;
    unsigned char *digest = (byte *)malloc(sizeof(unsigned char) * 16);
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
    
    MD5_Final(digest, &c);
    return digest;
}