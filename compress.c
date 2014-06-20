#include "compress.h"

uint64_t compress_hll(Hyperloglog *hll) {
    byte *compressed = (byte*) malloc(hll->m);
    byte *decompressed = (byte*) malloc(hll->m);
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

    // z_stream infstream;
    // infstream.zalloc = Z_NULL;
    // infstream.zfree = Z_NULL;
    // infstream.opaque = Z_NULL;
    // // setup "b" as the input and "c" as the compressed output
    // infstream.avail_in = defstream.total_out; // size of input
    // infstream.next_in = (Bytef *)compressed; // input char array
    // infstream.avail_out = (uInt)hll->m; // size of output
    // infstream.next_out = (Bytef *)decompressed; // output char array
     
    // // the actual DE-compression work.
    // inflateInit(&infstream);
    // inflate(&infstream, Z_NO_FLUSH);
    // inflateEnd(&infstream);

    // for (int i = 0; i < hll->m; ++i)
    // {
    //     if (hll->M[i] != decompressed[i])
    //     {
    //         printf("Chyba!!!! %i, %u, %u\n", i, hll->M[i], decompressed[i]);
    //     }
    // }
    
    // printf("Compressed size is: %lu\n", defstream.total_out);
    return (uint64_t)defstream.total_out;
}
