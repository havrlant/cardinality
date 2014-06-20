//
//  io.h
//  cardinality
//
//  Created by Lukáš Havrlant on 11.6.14.
//  Copyright (c) 2014 Lukáš Havrlant. All rights reserved.
//

#ifndef cardinality_io_h
#define cardinality_io_h

#include <stdio.h>
#include <stdlib.h>
#include "libs/tinydir.h"

FILE* try_fopen(const char *path);
int try_open_dir(tinydir_dir *dir, const char* path);

#endif
