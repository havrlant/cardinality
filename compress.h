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
#include <stdint.h>
#include "hll_decl.h"
#include "zlib/zlib.h"
#include "linear_counting.h"
#include "libs/utils.h"

ulong compress_bytes(byte *array, byte *compressed, uint size_input, uint size_output);
ulong compress_hll(Hyperloglog *hll, byte *compressed);
ulong compress_sparse(Hyperloglog *hll, byte *compressed, SparsePair *temp);

#endif
