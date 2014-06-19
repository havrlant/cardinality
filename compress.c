#include "compress.h"

uint64_t compress_hll(Hyperloglog *hll) {
    byte *compressed = (byte*) malloc(hll->m);
    // zlib struct
    z_stream defstream;
    defstream.zalloc = Z_NULL;
    defstream.zfree = Z_NULL;
    defstream.opaque = Z_NULL;
    // setup "a" as the input and "b" as the compressed output
    defstream.avail_in = (uInt)hll->m; // size of input, string + terminator
    defstream.next_in = (Bytef *)hll->M; // input char array
    defstream.avail_out = (uInt)hll->m; // size of output
    defstream.next_out = (Bytef *)compressed; // output char array
    
    deflateInit(&defstream, Z_BEST_COMPRESSION);
    deflate(&defstream, Z_FINISH);
    deflateEnd(&defstream);
    
    // printf("Compressed size is: %lu\n", defstream.total_out);
    return (uint64_t)defstream.total_out;
}
