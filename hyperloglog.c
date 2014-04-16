#include "hyperloglog.h"

const int BITS_IN_BYTE = 8;
const int AD_SPACE_PK_INDEX = 1;
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
uint bucketIndex(byte *digest, Hyperloglog *hll) {
    uint64_t index;
    uint bytesHashLength = DIGEST_BIT_LENGTH / BITS_IN_BYTE;
    byte temparray[bytesHashLength];
    for (int i = 0; i < bytesHashLength; i++) {
        temparray[bytesHashLength - i - 1]  = digest[i];
    }
    memcpy(&index, temparray, sizeof(uint64_t));
    index = index >> (DIGEST_BIT_LENGTH - hll->b);
    return (uint)index;
}

void update_M(Hyperloglog *hll, byte *digest) {
    uint j, first1;
    j = bucketIndex(digest, hll);
    first1 = rho(digest, DIGEST_BIT_LENGTH, hll->b);
    hll->M[j] = max(hll->M[j], first1);
}

void computeMaxes(Hyperloglog *website, Hyperloglog **sections, Hyperloglog **positions, Structure structure, SimpleCSVParser *parser) {
    StructureRow *srow;
    int index;
    char *word;
    byte *digest = (byte *)malloc(sizeof(unsigned char) * 16);
    while (next_line(parser) == 0) {
        word = parser->fields[2];
        str2md5(word, digest);
        
        // cely web
        update_M(website, digest);
        
        // jednotlive sekce
        int ad_space_pk = atoi(parser->fields[AD_SPACE_PK_INDEX]);
        srow = find_row_by_ad_space_pk(structure, ad_space_pk);
        index = srow->section_id - 1;
        update_M(sections[index], digest);
        
        // jednotlive pozice
        // index = ((ad_space_pk == 89229) ? 15 : ad_space_pk - 89202);
        index = find_index_by_ad_space_pk(structure, ad_space_pk);
        update_M(positions[index], digest);
    }
    free(digest);
}

double applyCorrections(double E, Hyperloglog *hll) {
    uint V = 0;
    double Estar = E;
    
    if (E <= ((5 / 2) * hll->m)) {
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

uint computeHyperCardinality(Hyperloglog *hll, double alpham) {
    int j;
    double E = 0;
    double sum = 0, harmonicMean;
    for (j = 0; j < hll->m; j++) {
        sum += pow(2, -(hll->M[j]));
    }
    harmonicMean = hll->m / sum;
    E = alpham * hll->m * harmonicMean;
    E = applyCorrections(E, hll);
    return (uint)E;
}

double computeHyperAlpha(unsigned int m) {
    return 0.7213 / (1 + 1.079 / m);
}

void init_hll(Hyperloglog *hll, uint b) {
    hll->b = b;
    hll->m = 1 << b; // 2^b
    hll->M = (byte*) calloc(hll->m, sizeof(byte));
}

double hyperloglog(uint b, SimpleCSVParser *parser, Structure structure) {
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
    
    // vypocet vsech kardinalit
    computeMaxes(&website, sections, positions, structure, parser);
    
    
    // vypis vsech kardinalit
    uint cardinality = computeHyperCardinality(&website, computeHyperAlpha(website.m));
    printf("Cely web: %u\n", cardinality);
    
    for (int i = 0; i < 2; i++) {
        cardinality = computeHyperCardinality(sections[i], computeHyperAlpha(sections[i]->m));
        printf("Sekce c. %i: %u\n", i + 1, cardinality);
    }
    
    for (int i = 0; i < structure.length; i++) {
        cardinality = computeHyperCardinality(positions[i], computeHyperAlpha(positions[i]->m));
        printf("Pozice c. %i (ad_space_pk: %i): %u\n", i + 1, structure.rows[i].ad_space_pk, cardinality);
    }
    
    return cardinality;
}

































