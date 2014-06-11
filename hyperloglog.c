#include "hyperloglog.h"

const int AD_SPACE_PK_INDEX = 1;
const int USER_PK_INDEX = 2;
const int DIGEST_BIT_LENGTH = 64;
const uint BITSET_EXPONENT = 18;
uint BITSET_SIZE;
uint BITSET_LIMIT;

uint max(uint a, uint b) {
    return a > b ? a : b;
}

double linear_counting(uint m, uint V) {
    return m * log((double)m / (double)V);
}

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

void updateM(Hyperloglog *hll, uint64_t digest) {
    uint j, first1;
    j = bucket_index(digest, hll->b);
    first1 = rho(digest, hll->b);
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
    SetDictionary *s, *tmp;
    HllDictionary *h;
    uint card;
    HASH_ITER(hh, sets_table, s, tmp) {
        card = elements_count(s->set, BITSET_SIZE);
        card = linear_counting(BITSET_SIZE, BITSET_SIZE - card);
        if (card > BITSET_LIMIT) {
            h = find_hll(s->hash_id, &hlls_table);
            card = compute_cardinality(h->hll, compute_alpha(h->hll->m));
        }
        printf("'%s' : %u\n", s->hash_id, card);
    }
}

size_t compute_hash_length(View view, char** fields) {
    size_t length = 0;
    uint index;
    for (uint i = 0; i < view.fields_count; i++) {
        index = view.fields_indices[i];
        length += strlen(fields[index]);
        length += strlen(view_names[index]);
        length++;
    }
    length += view.fields_count - 1;
    return length;
}

char *create_hash_id(View view, char** fields) {
    size_t length = compute_hash_length(view, fields) + 1;
    char *newstring = (char*) malloc(length * sizeof(char));
    size_t j = 0;
    uint index;
    for (uint i = 0; i < view.fields_count; i++) {
        index = view.fields_indices[i];
        memcpy(&newstring[j], view_names[index], strlen(view_names[index]) * sizeof(char));
        j += strlen(view_names[index]);
        newstring[j++] = ':';
        memcpy(&newstring[j], fields[index], strlen(fields[index]) * sizeof(char));
        j += strlen(fields[index]);
        newstring[j++] = ',';
    }
    newstring[j - 1] = '\0';
    return newstring;
}

void process_file(const char *path, HllDictionary **hlls_table, SetDictionary **sets_table, uint b) {
    SimpleCSVParser parser;
    Dstats stats;
    HllDictionary *hll_for_the_id;
    Hyperloglog *hll;
    Set set;
    uint64_t digest_value;
    char *hash_id;

    init_parser(&parser, try_fopen(path), 1000, 29, '\t');
    while (next_line(&parser)) {
        parse_line(parser.fields, &stats);
        // tohle zatim nebudeme pocitat
        if (strcmp("0", stats.uuid) == 0) {
            continue;
        }
        for (uint i = 0; i < VIEWS_COUNT; i++) {
            hash_id = create_hash_id(views[i], parser.fields);
            hll_for_the_id = find_hll(hash_id, hlls_table);
            
            if (hll_for_the_id == NULL) {
                hll = create_hll(b);
                add_hll_to_dict(hash_id, hll, hlls_table);
                set = create_set(BITSET_SIZE);
                add_set_to_dict(hash_id, set, sets_table);
            } else {
                hll = hll_for_the_id->hll;
                set = find_set_in_dict(hash_id, sets_table)->set;
                free(hash_id);
            }
            digest_value = MurmurHash64A(stats.uuid, (int)strlen(stats.uuid), 42);
            updateM(hll, digest_value);
            set_element(set, bucket_index(digest_value, BITSET_EXPONENT));
        }
        
    }
    
    free_parser(&parser);
}

void init_constants() {
    BITSET_SIZE = 1 << BITSET_EXPONENT;
    BITSET_LIMIT = 1 << (BITSET_EXPONENT - 2);
}

void process_all_files(tinydir_dir dir, HllDictionary **hlls_table, SetDictionary **sets_table, uint b) {
    while (dir.has_next) {
        tinydir_file file;
        if (tinydir_readfile(&dir, &file) == -1) {
            perror("Error getting file");
            return;
        }
        
        if (file.name[0] != '.') {
            printf("Zpracovavam soubor: %s\n", file.path);
            process_file(file.path, hlls_table, sets_table, b);
        }
        
		tinydir_next(&dir);
	}
}

int try_open_dir(tinydir_dir *dir, const char* path) {
    if (tinydir_open(dir, path) == -1) {
        perror("Error opening file");
        return 0;
    }
    return 1;
}

void hyperloglog(uint b, const char *path) {
    init_constants();
    HllDictionary *hlls_table = create_empty_hll_dict();
    SetDictionary *sets_table = create_empty_set_dict();
    tinydir_dir dir;
    
    if (try_open_dir(&dir, path)) {
        process_all_files(dir, &hlls_table, &sets_table, b);
        print_results(hlls_table, sets_table);
    }
}
