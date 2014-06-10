//
//  hlldictionary.h
//  cardinality
//
//  Created by Lukáš Havrlant on 10.6.14.
//  Copyright (c) 2014 Lukáš Havrlant. All rights reserved.
//

#ifndef cardinality_hlldictionary_h
#define cardinality_hlldictionary_h

#include "uthash.h"
#include "declarations.h"

typedef struct {
    int hash_id;                    /* key */
    Hyperloglog *hll;
    UT_hash_handle hh;         /* makes this structure hashable */
} HllDictionary;

HllDictionary *create_empty_hll_dict();
void add_hll(uint id, Hyperloglog *hll, HllDictionary **dict);
HllDictionary *find_hll(uint id, HllDictionary **dict);

#endif
