#include "hyperloglog.h"

const int BITS_IN_BYTE = 8;

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
//uint bucketIndex(byte *digest, uint bucketBitLength, uint digestBitLength) {
uint bucketIndex(byte *digest, Hyperloglog *hll) {
    int i;
    uint32_t index;
    uint bytesHashLength = 4;
    byte temparray[bytesHashLength];
    for (i = 0; i < bytesHashLength; i++) {
        temparray[bytesHashLength - i - 1]  = digest[i];
    }
    memcpy(&index, temparray, sizeof(uint));
    index = index >> (hll->digestBitLength - hll->b);
    return (uint)index;
}


// Spocita rho hodnoty pro vsechny vstupni retezce a ulozi je do spravnych buckets
void update_M(Hyperloglog *hll, byte *digest) {
    uint j, first1;
    j = bucketIndex(digest, hll);
    first1 = rho(digest, hll->digestBitLength, hll->b);
    hll->M[j] = max(hll->M[j], first1);
}

void computeMaxes(Hyperloglog *hll, Hyperloglog *sections[], Structure structure, SimpleCSVParser *parser) {
    StructureRow srow;
    int index;
    byte *digest;
    char *word;
    while (next_line(parser) == 0) {
        // cely web
        word = parser->fields[2];
        digest = str2md5(word, (int)strlen(word));
        update_M(hll, digest);
        
        // jednotlive sekce
        srow = find_row_by_ad_space_pk(structure, atoi(parser->fields[1]));
        index = srow.section_id[0] - '1';
        update_M(sections[index], digest);
    }
}

double computeHyperCardinality(Hyperloglog *hll, double alpham) {
    int j;
    double E = 0;
    double sum = 0, harmonicMean;
    for (j = 0; j < hll->m; j++) {
        sum += pow(2, -(hll->M[j]));
    }
    harmonicMean = hll->m / sum;
    E = alpham * hll->m * harmonicMean;
    return E;
}

double computeHyperAlpha(unsigned int m) {
    return 0.7213 / (1 + 1.079 / m);
}

void init_hll(Hyperloglog *hll, uint b) {
    hll->b = b;
    hll->m = 1 << b; // 2^b
    hll->digestBitLength = 32;
    hll->M = (byte*) calloc(hll->m, sizeof(byte));
}

double hyperloglog(uint b, SimpleCSVParser *parser, Structure structure) {
    // cely web
    Hyperloglog website;
    init_hll(&website, b);
    
    // dve podsekce
    Hyperloglog **sections = (Hyperloglog**) malloc(2 * sizeof(Hyperloglog*));
    for (int i = 0; i < 2; i++) {
        sections[i] = (Hyperloglog*) malloc(sizeof(Hyperloglog));
        init_hll(sections[i], b);
    }
    
    computeMaxes(&website, sections, structure, parser);
    double cardinality = computeHyperCardinality(&website, computeHyperAlpha(website.m));
    printf("Cely web: %g\n", cardinality);
    for (int i = 0; i < 2; i++) {
        cardinality = computeHyperCardinality(sections[i], computeHyperAlpha(sections[i]->m));
        printf("Sekce c. %i: %g\n", i + 1, cardinality);
    }
    // ToDo: applyCorrections
    return cardinality;
}

































