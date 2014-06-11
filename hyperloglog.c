#include "hyperloglog.h"

const int AD_SPACE_PK_INDEX = 1;
const int USER_PK_INDEX = 2;
const int DIGEST_BIT_LENGTH = 64;
const uint BITSET_EXPONENT = 18;
uint BITSET_SIZE = 1 << BITSET_EXPONENT;
uint BITSET_LIMIT = 1 << (BITSET_EXPONENT - 4);

FILE* try_fopen(const char *path) {
    FILE* fd = fopen(path, "r");
    if (fd == NULL) {
        printf("Nepodarilo se otevrit soubor %s", path);
        exit(EXIT_FAILURE);
    }
    return fd;
}

uint max(uint a, uint b) {
    return a > b ? a : b;
}


uint get_threshold(uint b) {
    return tresholds[b - 4];
}

double estimate_bias(double E, uint b) {
    for (int i = 1; i < 200; i++) {
        if (raw_estimate_data[b - 4][i] > E) {
            return ((raw_estimate_data[b - 4][i] - bias_data[b - 4][i]) + (raw_estimate_data[b - 4][i - 1] - bias_data[b - 4][i - 1])) / 2.0;
        }
    }
    return 0;
}

double linear_counting(uint m, uint V) {
    // return m * log2((double)m / (double)V);
    // printf("m: %u, V: %u, %g\n", m, V, log2((double)m / (double)V + 1));
    return m * log2((double)m / (double)V);
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
 Prevede prvnich b bitu na cislo
 bucketIndex(1001000010, 4) = 1001 = 9
 */
uint bucket_index(byte *digest, uint b) {
    uint64_t index;
    uint bytesHashLength = DIGEST_BIT_LENGTH / BITS_IN_BYTE;
    byte temparray[bytesHashLength];
    for (uint i = 0; i < bytesHashLength; i++) {
        temparray[bytesHashLength - i - 1]  = digest[i];
    }
    memcpy(&index, temparray, sizeof(uint64_t));
    index = index >> (DIGEST_BIT_LENGTH - b);
    return (uint)index;
}

void updateM(Hyperloglog *hll, byte *digest) {
    uint j, first1;
    j = bucket_index(digest, hll->b);
    first1 = rho(digest, DIGEST_BIT_LENGTH, hll->b);
    hll->M[j] = max(hll->M[j], first1);
}

/*void fillM(SiteLoglog *siteloglog, Structure *structure, SimpleCSVParser *parser) {
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
}*/

uint count_zero_buckets(Hyperloglog *hll) {
    uint count = 0;
    for (uint i = 0; i < hll->m; i++) {
        if (hll->M[i] == 0) {
            count++;
        }
    }
    return count;
}

double apply_corrections(double E, Hyperloglog *hll) {
    uint V = 0;
    double Estar = E;
    
    if (E <= (2.5 * hll->m)) {
        V = count_zero_buckets(hll);
        if (V != 0) {
            Estar = linear_counting(hll->m, V);
            // printf("E: %g Estar: %g, V: %u\n", E, Estar, V);
        }
    }
    
    // printf("%g, %g, %i \n", E, Estar, V);
    
    /*double bound = pow(2, 32);
    if (E > bound / 30) {
        Estar = -bound * log2(1 - E / bound);
    }*/
    return Estar;
}

/*double hllplus(double E, Hyperloglog *hll) {
    double E_, H;
    if (E <= (5 * hll->m)) {
        E_ = E - estimate_bias(E, hll->b);
    } else {
        E_ = E;
    }
    
    uint V = count_zero_buckets(hll);
    if (V != 0) {
        double nonzero = (double)(hll->m - V);
        H = linear_counting(hll->m, V);
        printf("E: %g, E_: %g, LC: %g, V: %i, rozdil: %i, odhad: %g\n", E, E_, H, V, hll->m - V, (hll->m - V) * log2(nonzero / hll->m + 2));
    } else {
        H = E_;
    }
    
    if (H <= get_threshold(hll->b)) {
        return H;
    } else {
        return E_;
    }
}*/

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
    // E = hllplus(E, hll);
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

void print_cardinalities(SiteLoglog *siteloglog, Structure *structure) {
    double alpham = compute_alpha(siteloglog->website->m);
    uint cardinality = compute_cardinality(siteloglog->website, alpham);
    printf("Cely web: %u\n", cardinality);
    
    for (int i = 0; i < structure->section_count; i++) {
        alpham = compute_alpha(siteloglog->sections[i]->m);
        cardinality = compute_cardinality(siteloglog->sections[i], alpham);
        printf("Sekce c. %i: %u\n", i + 1, cardinality);
    }
    
    for (int i = 0; i < structure->length; i++) {
        alpham = compute_alpha(siteloglog->positions[i]->m);
        cardinality = compute_cardinality(siteloglog->positions[i], alpham);
        printf("Pozice c. %i (ad_space_pk: %i): %u\n", i + 1, structure->rows[i].ad_space_pk, cardinality);
    }
}

void free_siteloglog(SiteLoglog *siteloglog, Structure *structure) {
    free(siteloglog->website->M);
    
    for (int i = 0; i < structure->section_count; i++) {
        free(siteloglog->sections[i]->M);
        free(siteloglog->sections[i]);
    }
    free(siteloglog->sections);
    
    for (int i = 0; i < structure->length; i++) {
        free(siteloglog->positions[i]->M);
        free(siteloglog->positions[i]);
    }
    free(siteloglog->positions);
}

Hyperloglog *create_hll(uint b) {
    Hyperloglog *hll = (Hyperloglog*) malloc(sizeof(Hyperloglog));
    init_hll(hll, b);
    return hll;
}

// hash tabulky

void print_results(HllDictionary *hlls_table, SetDictionary *sets_table) {
    SetDictionary *s;
    HllDictionary *h;
    uint card;
    for (s = sets_table; s != NULL; s = s->hh.next) {
        card = elements_count(s->set, BITSET_SIZE); // pridat Linear Counting
        if (card > BITSET_LIMIT) {
            h = find_hll(s->hash_id, &hlls_table);
            card = compute_cardinality(h->hll, compute_alpha(h->hll->m));
        }
        printf("SERVER_ID: %u, cardinality: %u\n", s->hash_id, card);
    }
}

void process_file(const char *path, HllDictionary **hlls_table, SetDictionary **sets_table, uint b, byte *digest) {
    SimpleCSVParser parser;
    Dstats stats;
    HllDictionary *temp_item;
    Hyperloglog *hll;
    Set set;

    init_parser(&parser, try_fopen(path), 1000, 29, '\t');
    while (next_line(&parser)) {
        parse_line(parser.fields, &stats);
        // tohle zatim nebudeme pocitat -- dohodnout se s Vlkem
        if (strcmp("0", stats.uuid) == 0) {
            continue;
        }
        
        temp_item = find_hll(stats.id_server, hlls_table);
        if (temp_item == NULL) {
            hll = (Hyperloglog*) malloc(sizeof(Hyperloglog));
            init_hll(hll, b);
            add_hll(stats.id_server, hll, hlls_table);
            
            set = create_set(BITSET_SIZE); // 2^18
            add_set_to_dict(stats.id_server, set, sets_table);
        } else {
            hll = temp_item->hll;
            set = find_set_in_dict(stats.id_server, sets_table)->set;
        }
        str2md5(stats.uuid, digest);
        updateM(hll, digest);
        set_element(set, bucket_index(digest, BITSET_EXPONENT));
    }
    
    free_parser(&parser);
}

void hyperloglog(uint b, const char *path) {
    HllDictionary *hlls_table = create_empty_hll_dict();
    SetDictionary *sets_table = create_empty_set_dict();
    byte *digest = (byte *)malloc(sizeof(unsigned char) * 16);
    tinydir_dir dir;
    
	if (tinydir_open(&dir, path) == -1) {
		perror("Error opening file");
        return;
	}
    
    while (dir.has_next)
	{
		tinydir_file file;
		if (tinydir_readfile(&dir, &file) == -1)
		{
			perror("Error getting file");
            return;
		}
        
        if (file.name[0] != '.') {
            printf("%s\n", file.path);
        }

		tinydir_next(&dir);
	}
    
    // process_file(path, &hlls_table, &sets_table, b, digest);
    // print_results(hlls_table, sets_table);
}
