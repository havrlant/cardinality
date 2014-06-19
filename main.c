#include <stdio.h>
#include "simpleCSV.h"
#include "structure.h"
#include "hyperloglog.h"
#include "declarations.h"
#include "tests/unittests.h"
#include "uthash.h"
#include "dstats.h"
#include "hlldictionary.h"
#include "nbitvector.h"

/*void compute_intersection(const char *path1, const char *path2) {
    Hyperloglog *hll1 = load_from_file(path1, 14);
    Hyperloglog *hll2 = load_from_file(path2, 14);
    uint cardinality = estimate_intersection_cardinality(hll1, hll2);
    // printf("%u\n", cardinality);
}*/

int main(int argc, const char * argv[])
{
    if (argc > 1) {
        uint b = 14;
        if (argc > 2) {
            b = (uint) atoi(argv[2]);
        }
        hyperloglog(b, argv[1]);
    } else {
        hyperloglog(14, "/Users/lukashavrlant/Desktop/test");
    }
    
    return 0;
}

/*
 1:17898,2:1,3:1,4:2,6:0,
 1:17898,2:1,3:1,4:2,6:0,7:0,
 1:17898,2:1,3:1,4:2,6:0,7:0,10:0,
 1:17898,2:1,3:1,4:2,6:0,7:0,10:0,5:0,
 '1:17898,2:1,3:1,4:2,6:0,7:0,10:0,5:0', 36, 35
*/