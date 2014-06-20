//
//  hlldictionary.h
//  cardinality
//
//  Created by Lukáš Havrlant on 10.6.14.
//  Copyright (c) 2014 Lukáš Havrlant. All rights reserved.
//

#ifndef cardinality_hlldictionary_h
#define cardinality_hlldictionary_h

#include "libs/uthash.h"
#include "declarations.h"
#include "hll_decl.h"

typedef struct {
    char *hash_id;
    Hyperloglog *hll;
    UT_hash_handle hh;
} HllDictionary;

HllDictionary *create_empty_hll_dict();
void add_hll_to_dict(char *id, Hyperloglog *hll, HllDictionary **dict);
HllDictionary *find_hll(char *id, HllDictionary **dict);

#endif
