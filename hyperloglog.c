#include "hyperloglog.h"

const int AD_SPACE_PK_INDEX = 1;
const int USER_PK_INDEX = 2;
const int DIGEST_BIT_LENGTH = 64;
const int MAXIMUM_CSV_LINE_LENGTH = 5000;
int badcounter = 0;

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

double linear_counting(uint m, uint V) {
    return m * log((double)m / (double)V);
}

uint hyperloglog_cardinality(Hyperloglog *hll, double alpham) {
    uint j;
    double E = 0;
    double sum = 0, harmonicMean;
    for (j = 0; j < hll->m; j++) {
        sum += pow(2, -(hll->M[j]));
    }
    harmonicMean = hll->m / sum;
    E = alpham * hll->m * harmonicMean;
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

uint apply_corrections(Hyperloglog *hll, uint cardinality) {
    if (cardinality <= 5 * hll->m) {
        cardinality -= estimate_bias(cardinality, hll->b);
    }
    uint V = count_zero_buckets(hll);
    uint H = (V == 0) ? cardinality : linear_counting(hll->m, V);
    if (H < get_threshold(hll->b)) {
        return H;
    }
    return cardinality;
}

uint estimate_cardinality(Hyperloglog *hll) {
    uint cardinality = hyperloglog_cardinality(hll, compute_alpha(hll->m)); // cachovat
    cardinality = apply_corrections(hll, cardinality);
    return cardinality;
}

void print_results(HllDictionary *hlls_table) {
    HllDictionary *h, *tmp;
    uint card;
    HASH_ITER(hh, hlls_table, h, tmp) {
        card = estimate_cardinality(h->hll);
        // printf("'%s' : %u\n", h->hash_id, card);
    }
    printf("Counter: %i\n", badcounter);
}

size_t compute_hash_length(View view, char** fields) {
    size_t length = 0;
    uint index;
    for (uint i = 0; i < view.fields_count; i++) {
        index = view.fields_indices[i];
        length += strlen(fields[index]);
        length += (index + 1) > 9 ? 2 : 1;
        length++;
    }
    length += view.fields_count - 1;
    return length;
}

char *create_hash_id(View view, char** fields) {
    size_t length = compute_hash_length(view, fields) + 1;
    char *newstring = (char*) calloc(length, sizeof(char));
    size_t j = 0;
    uint index;
    uint csvindex;
    size_t field_length;
    for (uint i = 0; i < view.fields_count; i++) {
        index = view.fields_indices[i];
        csvindex = index + 1;
        if (csvindex > 9) {
            newstring[j++] = '0' + (csvindex / 10);
        }
        newstring[j++] = '0' + (csvindex % 10);
        newstring[j++] = ':';
        field_length = strlen(fields[index]);
        memcpy(&newstring[j], fields[index], field_length * sizeof(char));
        j += field_length;
        newstring[j++] = ',';
    }
    newstring[j - 1] = '\0';
    return newstring;
}

void process_file(const char *path, HllDictionary **hlls_table, uint b) {
    SimpleCSVParser parser;
    Dstats stats;
    HllDictionary *hll_for_the_id;
    Hyperloglog *hll;
    uint64_t digest_value;
    char *hash_id;
    
    
    init_parser(&parser, try_fopen(path), MAXIMUM_CSV_LINE_LENGTH, 29, '\t');
    while (next_line(&parser)) {
        parse_line(parser.fields, &stats);
        // tohle zatim nebudeme pocitat
        if (strcmp("0", stats.uuid) == 0) {
            continue;
        }
        for (uint i = 0; i < VIEWS_COUNT; i++) {
            hash_id = create_hash_id(views[i], parser.fields);
            if (atoi(parser.fields[0]) == 18913 &&
                atoi(parser.fields[1]) == 5 &&
                atoi(parser.fields[2]) == 26 &&
                atoi(parser.fields[3]) == 2 &&
                atoi(parser.fields[5]) == 125525 &&
                atoi(parser.fields[6]) == 463261 &&
                // atoi(parser.fields[9]) == 496989 &&
                atoi(parser.fields[4]) == 250425
                ) {
                printf("hash_id: %s, '%s', '%s', '%s'\n", hash_id, parser.fields[5], parser.fields[6], parser.fields[9]);
                badcounter++;
                // printf("line: %s")
            }
            hll_for_the_id = find_hll(hash_id, hlls_table);
            
            if (hll_for_the_id == NULL) {
                hll = create_hll(b);
                add_hll_to_dict(hash_id, hll, hlls_table);
            } else {
                hll = hll_for_the_id->hll;
                free(hash_id);
            }
            digest_value = MurmurHash64A(stats.uuid, (int)strlen(stats.uuid), 42);
            updateM(hll, digest_value);
        }
        
    }
    
    free_parser(&parser);
}

void process_all_files(tinydir_dir dir, HllDictionary **hlls_table, uint b) {
    while (dir.has_next) {
        tinydir_file file;
        if (tinydir_readfile(&dir, &file) == -1) {
            perror("Error getting file");
            return;
        }
        
        if (file.name[0] != '.') {
            process_file(file.path, hlls_table, b);
        }
        
		tinydir_next(&dir);
	}
}

void hyperloglog(uint b, const char *path) {
    HllDictionary *hlls_table = create_empty_hll_dict();
    tinydir_dir dir;
    
    if (try_open_dir(&dir, path)) {
        process_all_files(dir, &hlls_table, b);
        print_results(hlls_table);
    }
}