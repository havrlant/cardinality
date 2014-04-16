#ifndef cardinality_md5_h
#define cardinality_md5_h

#if defined(__APPLE__)
#  define COMMON_DIGEST_FOR_OPENSSL
#  include <CommonCrypto/CommonDigest.h>
#else
#  include <openssl/md5.h>
#endif

#include <stdlib.h>
#include "declarations.h"

byte *str2md5(const char *str, int length);

#endif
