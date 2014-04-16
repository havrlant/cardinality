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
void update_M(Hyperloglog *hll, char **csv_row, StructureRow srow) {
    byte *digest;
    uint j, first1;
    digest = str2md5(csv_row[2], (int)strlen(csv_row[2]));
    j = bucketIndex(digest, hll);
    first1 = rho(digest, hll->digestBitLength, hll->b);
    hll->M[j] = max(hll->M[j], first1);
    free(digest);
}

void computeMaxes(Hyperloglog *hll, SimpleCSVParser *parser) {
    StructureRow row;
    hll->M = (byte*) calloc(hll->m, sizeof(byte));
    while (next_line(parser) == 0) {
        update_M(hll, parser->fields, row);
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

double hyperloglog(uint b, SimpleCSVParser *parser, Structure structure) {
    Hyperloglog hll;
    hll.b = b;
    hll.m = (uint) pow(2, b);
    hll.digestBitLength = 32;
    computeMaxes(&hll, parser);
    double cardinality = computeHyperCardinality(&hll, computeHyperAlpha(hll.m));
    // ToDo: applyCorrections
    return cardinality;
}