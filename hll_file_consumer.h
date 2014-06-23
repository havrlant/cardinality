#ifndef cardinality_hll_file_consumer_h
#define cardinality_hll_file_consumer_h

#include "declarations.h"
#include "hlldictionary.h"
#include "libs/tinydir.h"
#include "libs/simpleCSV.h"
#include "declarations.h"
#include "libs/simpleCSV.h"
#include "assert.h"
#include "hlldictionary.h"
#include "dstats.h"
#include "libs/tinydir.h"
#include "libs/murmurhash.h"
#include "io.h"
#include "hll_decl.h"
#include "compress.h"
#include "libs/utils.h"
#include "linear_counting.h"
#include "hyperloglog.h"

void process_all_files(tinydir_dir *dir, HllDictionary **hlls_table, uint b, uint hour);
void hyperloglog(uint b, const char *path);
size_t compute_hash_length(View view, char** fields);
char *build_hash_id(View view, char** fields);

#endif
