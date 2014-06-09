//
//  set.h
//  cardinality
//
//  Created by Lukáš Havrlant on 9.6.14.
//  Copyright (c) 2014 Lukáš Havrlant. All rights reserved.
//

#ifndef cardinality_set_h
#define cardinality_set_h

#include "declarations.h"
#include <stdlib.h>
#include "math.h"

typedef byte* Set;

uint is_element(Set set, uint index);
void set_element(Set set, uint index);
Set create_set(uint maximum_elements_count);
uint elements_count(Set set, uint maximum_elements_count);

#endif
