#include "hyperloglog.h"

const int AD_SPACE_PK_INDEX = 1;
const int USER_PK_INDEX = 2;
const int DIGEST_BIT_LENGTH = 64;
const uint BITSET_EXPONENT = 18;
uint BITSET_SIZE;
uint BITSET_LIMIT;

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
uint rho(uint64_t digest, uint bitlength, uint bitfrom) {
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

void updateM(Hyperloglog *hll, uint64_t digest) {
    uint j, first1;
    j = bucket_index(digest, hll->b);
    first1 = rho(digest, DIGEST_BIT_LENGTH, hll->b);
    hll->M[j] = max(hll->M[j], first1);
}

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

Hyperloglog *create_hll(uint b) {
    Hyperloglog *hll = (Hyperloglog*) malloc(sizeof(Hyperloglog));
    init_hll(hll, b);
    return hll;
}

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
    uint64_t digest_value;

    init_parser(&parser, try_fopen(path), 1000, 29, '\t');
    while (next_line(&parser)) {
        parse_line(parser.fields, &stats);
        // tohle zatim nebudeme pocitat -- dohodnout se s Vlkem
        if (strcmp("0", stats.uuid) == 0) {
            continue;
        }
        
        temp_item = find_hll(stats.id_server, hlls_table);
        if (temp_item == NULL) {
            hll = create_hll(b);
            add_hll(stats.id_server, hll, hlls_table);
            
            set = create_set(BITSET_SIZE); // 2^18
            add_set_to_dict(stats.id_server, set, sets_table);
        } else {
            hll = temp_item->hll;
            set = find_set_in_dict(stats.id_server, sets_table)->set;
        }
        //str2md5(stats.uuid, digest);
        digest_value = MurmurHash64A(stats.uuid, (int)strlen(stats.uuid), 42);
        updateM(hll, digest_value);
        set_element(set, bucket_index(digest_value, BITSET_EXPONENT));
    }
    
    free_parser(&parser);
}

void hyperloglog(uint b, const char *path) {
    BITSET_SIZE = 1 << BITSET_EXPONENT;
    BITSET_LIMIT = 1 << (BITSET_EXPONENT - 2);
    
    HllDictionary *hlls_table = create_empty_hll_dict();
    SetDictionary *sets_table = create_empty_set_dict();
    byte *digest = (byte *)malloc(sizeof(unsigned char) * 16);
    tinydir_dir dir;
    
    if (tinydir_open(&dir, path) == -1) {
        perror("Error opening file");
        return;
    }
    
    while (dir.has_next) {
        tinydir_file file;
        if (tinydir_readfile(&dir, &file) == -1) {
            perror("Error getting file");
            return;
        }
        
        if (file.name[0] != '.') {
            printf("Zpracovavam soubor: %s\n", file.path);
            process_file(file.path, &hlls_table, &sets_table, b, digest);
        }

		tinydir_next(&dir);
	}
    
    process_file(path, &hlls_table, &sets_table, b, digest);
    print_results(hlls_table, sets_table);
}
