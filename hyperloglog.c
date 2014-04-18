#include "hyperloglog.h"

const int BITS_IN_BYTE = 8;
const int AD_SPACE_PK_INDEX = 1;
const int USER_PK_INDEX = 2;
const int DIGEST_BIT_LENGTH = 64;

uint max(uint a, uint b) {
    return a > b ? a : b;
}

/*
 Vrati pozici nejlevejsiho bitu, ktery je roven 1.
 Zacina hledat od index bitfrom. Indexuje se od 1.
 rho(1001000010, 10, 4) = 6
 ^
 */
uint rho(const byte *digest, uint bitlength, uint bitfrom) {
    uint b = 0;
    uint byteIdx = 0;
    uint bitIdx = 0;
    for (b = bitfrom; b < bitlength; b++) {
        byteIdx = b / BITS_IN_BYTE;
        bitIdx  = (BITS_IN_BYTE - 1) - b % BITS_IN_BYTE;
        if ((digest[byteIdx] & (1 << bitIdx)) != 0) {
            return (b - bitfrom + 1);
        }
    }
    
    return (bitlength-bitfrom) + 1;
}


/*
 Prevede prvnich bucketBitLength bitu na cislo
 bucketIndex(1001000010, 4) = 1001 = 9
 */
uint bucket_index(byte *digest, Hyperloglog *hll) {
    uint64_t index;
    uint bytesHashLength = DIGEST_BIT_LENGTH / BITS_IN_BYTE;
    byte temparray[bytesHashLength];
    for (uint i = 0; i < bytesHashLength; i++) {
        temparray[bytesHashLength - i - 1]  = digest[i];
    }
    memcpy(&index, temparray, sizeof(uint64_t));
    index = index >> (DIGEST_BIT_LENGTH - hll->b);
    return (uint)index;
}

void updateM(Hyperloglog *hll, byte *digest) {
    uint j, first1;
    j = bucket_index(digest, hll);
    first1 = rho(digest, DIGEST_BIT_LENGTH, hll->b);
    hll->M[j] = max(hll->M[j], first1);
}

void fillM(SiteLoglog *siteloglog, Structure *structure, SimpleCSVParser *parser) {
    StructureRow *srow;
    int index;
    char *word;
    byte *digest = (byte *)malloc(sizeof(unsigned char) * 16);
    while (next_line(parser)) {
        word = parser->fields[USER_PK_INDEX];
        str2md5(word, digest);
        
        // cely web
        updateM(siteloglog->website, digest);
        
        // jednotlive sekce
        int ad_space_pk = atoi(parser->fields[AD_SPACE_PK_INDEX]);
        srow = find_row_by_ad_space_pk(structure, ad_space_pk);
        index = srow->section_id - 1;
        updateM(siteloglog->sections[index], digest);
        
        // jednotlive pozice
        index = find_index_by_ad_space_pk(structure, ad_space_pk);
        updateM(siteloglog->positions[index], digest);
    }
    free(digest);
}

double apply_corrections(double E, Hyperloglog *hll) {
    uint V = 0;
    double Estar = E;
    
    if (E <= (2.5 * hll->m)) {
        for (uint i = 0; i < hll->m; i++) {
            if (hll->M[i] == 0) {
                V++;
            }
        }
        if (V != 0) {
            Estar = hll->m * log2(hll->m / (double)V);
        }
    }
    
    double bound = pow(2, 32);
    if (E > bound / 30) {
        Estar = -bound * log2(1 - E / bound);
    }
    return Estar;
}

uint compute_cardinality(Hyperloglog *hll, double alpham) {
    uint j;
    double E = 0;
    double sum = 0, harmonicMean;
    for (j = 0; j < hll->m; j++) {
        sum += pow(2, -(hll->M[j]));
    }
    harmonicMean = hll->m / sum;
    E = alpham * hll->m * harmonicMean;
    E = apply_corrections(E, hll);
    return (uint)E;
}

double compute_alpha(unsigned int m) {
    return 0.7213 / (1 + 1.079 / m);
}

void init_hll(Hyperloglog *hll, uint b) {
    hll->b = b;
    hll->m = 1 << b; // 2^b
    hll->M = (byte*) calloc(hll->m, sizeof(byte));
}

void print_cardinalities(SiteLoglog *siteloglog, Structure structure) {
    double alpham = compute_alpha(siteloglog->website->m);
    uint cardinality = compute_cardinality(siteloglog->website, alpham);
    printf("Cely web: %u\n", cardinality);
    
    for (int i = 0; i < 2; i++) {
        alpham = compute_alpha(siteloglog->sections[i]->m);
        cardinality = compute_cardinality(siteloglog->sections[i], alpham);
        printf("Sekce c. %i: %u\n", i + 1, cardinality);
    }
    
    for (int i = 0; i < structure.length; i++) {
        alpham = compute_alpha(siteloglog->positions[i]->m);
        cardinality = compute_cardinality(siteloglog->positions[i], alpham);
        printf("Pozice c. %i (ad_space_pk: %i): %u\n", i + 1, structure.rows[i].ad_space_pk, cardinality);
    }
}

void hyperloglog(uint b, SimpleCSVParser *parser, Structure structure) {
    // cely web
    Hyperloglog website;
    init_hll(&website, b);
    
    // dve podsekce
    int sections_count = 0;
    for (int i = 0; i < structure.length; i++) {
        sections_count = max(sections_count, structure.rows[i].section_id);
    }
    Hyperloglog **sections = (Hyperloglog**) malloc(sections_count * sizeof(Hyperloglog*));
    for (int i = 0; i < sections_count; i++) {
        sections[i] = (Hyperloglog*) malloc(sizeof(Hyperloglog));
        init_hll(sections[i], b - 1);
    }
    
    // jednotlive pozice
    Hyperloglog **positions = (Hyperloglog**) malloc(structure.length * sizeof(Hyperloglog*));
    for (int i = 0; i < structure.length; i++) {
        positions[i] = (Hyperloglog*) malloc(sizeof(Hyperloglog));
        init_hll(positions[i], b - 2);
    }
    
    SiteLoglog siteloglog = {&website, sections, positions};
    
    // vypocet vsech kardinalit
    fillM(&siteloglog, &structure, parser);
    
    // vypis vsech kardinalit
    print_cardinalities(&siteloglog, structure);
}
