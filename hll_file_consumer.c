#include "hll_file_consumer.h"

const int MAXIMUM_CSV_LINE_LENGTH = 5000;
const uint HOURS_IN_DAY = 24;

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

char *build_hash_id(View view, char** fields) {
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
        newstring[j++] = '_';
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
    Hyperloglog *hll = NULL;
    uint64_t digest_value;
    char *hash_id;
    
    init_parser(&parser, try_fopen(path), MAXIMUM_CSV_LINE_LENGTH, 29, '\t');
    while (next_line(&parser)) {
        parse_line(parser.fields, &stats);
        for (uint i = 0; i < VIEWS_COUNT; i++) {
            hash_id = build_hash_id(views[i], parser.fields);
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

uint64_t print_results(HllDictionary *hlls_table, uint b) {
    HllDictionary *h, *tmp;
    uint card;
    uint64_t bytes_sum = 0;
    uint i = 0;
    uint m = 1 << b;
    byte *compressed = (byte*) malloc(m);
    SparsePair *pairs = (SparsePair*) malloc(sizeof(SparsePair) * m); // ToDo dve tretiny m
    uint32_t hll_compressed_size;
    uint32_t sparse_size;
    HASH_ITER(hh, hlls_table, h, tmp) {
        i++;
        card = estimate_cardinality(h->hll);
        // printf("%s:%u\n", h->hash_id, card);
        // save_sparse(h->hll, h->hash_id);
        hll_compressed_size = compress_hll(h->hll, compressed);
        sparse_size = compress_sparse(h->hll, compressed, pairs);
        bytes_sum += min(hll_compressed_size, sparse_size);
        free(h->hll->M);
        free(h->hll);
        free(h->hash_id);
    }
    HASH_CLEAR(hh, hlls_table);
    free(compressed);
    printf("Celkova velikost vektoru:  %g MB\n", bytes_sum / (1024*1024.0));
    printf("Prumerna velikost vektoru: %g B\n", (bytes_sum / (double)i));
    return bytes_sum;
}

void process_all_files(tinydir_dir *dir, HllDictionary **hlls_table, uint b, uint hour) {
    uint counter = 0;
    uint filehour;
    tinydir_file file;
    
    while (dir->has_next) {
        if (tinydir_readfile(dir, &file) == -1) {
            perror("Error getting file");
            return;
        }
        
        if (file.name[0] != '.') {
            filehour = get_hour_from_dstats(file.path);
            if (filehour == hour) {
                counter++;
                process_file(file.path, hlls_table, b);
            }
        }
        
		tinydir_next(dir);
	}
}

void hyperloglog(uint b, const char *path) {
    HllDictionary *table;
    uint64_t bytes_sum = 0;
    tinydir_dir dir;
    for (uint hour = 0; hour < HOURS_IN_DAY; hour++) {
        table = NULL;
        tinydir_open(&dir, path); // ToDo: error handling
        process_all_files(&dir, &table, b, hour);
        if (table != NULL) {
            printf("%u. hodina\n", hour);
            bytes_sum += print_results(table, b);
        }
        tinydir_close(&dir);
    }
    printf("\n\nVelikost vsech vektoru z daneho dne: %u MB\n", (uint) (bytes_sum / (1024*1024)));
}
