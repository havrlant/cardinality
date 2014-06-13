//
//  hll_decl.h
//  cardinality
//
//  Created by Lukáš Havrlant on 13.6.14.
//  Copyright (c) 2014 Lukáš Havrlant. All rights reserved.
//

#ifndef cardinality_hll_decl_h
#define cardinality_hll_decl_h

#include "nbitvector.h"

typedef struct {
    uint b;
    uint m;
    byte *M;
} Hyperloglog;

#endif
