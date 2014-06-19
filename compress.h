//
//  compress.h
//  cardinality
//
//  Created by Lukáš Havrlant on 19.6.14.
//  Copyright (c) 2014 Lukáš Havrlant. All rights reserved.
//

#ifndef cardinality_compress_h
#define cardinality_compress_h

#include <stdio.h>
#include "hll_decl.h"
#include "zlib/zlib.h"
#include <stdint.h>

uint64_t compress_hll(Hyperloglog *hll);

#endif
