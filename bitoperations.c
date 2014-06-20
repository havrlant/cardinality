#include "bitoperations.h"

/*
 Vrati pozici nejlevejsiho bitu, ktery je roven 1.
 Zacina hledat od index bitfrom. Indexuje se od 1.
 rho(1001000010, 10, 4) = 6
 ^
 */
uint rho(uint64_t digest, uint bitfrom) {
    uint64_t base_mask = ((uint64_t) 1) << 63;
    uint64_t mask;
    for (uint64_t i = bitfrom; i < 64; i++) {
        mask = base_mask >> i;
        if ((mask & digest) == mask) {
            return (uint)i - bitfrom + (uint)1;
        }
    }
    return 0;
}


/*
 Prevede prvnich b bitu na cislo
 bucketIndex(1001000010, 4) = 1001 = 9
 */
uint bucket_index(uint64_t digest, uint b) {
    return (uint)(digest >> (uint64_t)(64 - b));
}
