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

void compute_intersection(const char *path1, const char *path2) {
    Hyperloglog *hll1 = load_from_file(path1, 14);
    Hyperloglog *hll2 = load_from_file(path2, 14);
    /*uint cardinality = */estimate_intersection_cardinality(hll1, hll2);
    // printf("%u\n", cardinality);
}

int main(int argc, const char * argv[])
{
    if (argc > 1) {
        if (strcmp(argv[1], "--intersection") == 0) {
            if (argc < 4) {
                printf("Malo parametru.");
                return 0;
            } else {
                compute_intersection(argv[2], argv[3]);
            }
            return 0;
        }
        uint b = 14;
        if (argc > 2) {
            b = (uint) atoi(argv[2]);
        }
        hyperloglog(b, argv[1]);
    } else {
        // hyperloglog(14, "/Users/lukashavrlant/Desktop/test");
        
        /*uint index = 1;
        uint bitsize = 6;
        NBitVector *vector = create_buckets(100, bitsize);
        for (uint i = 0; i < 50; i++) {
            set_value_to_nbitvector(i, bitsize, vector, i);
        }
        
        for (uint i = 0; i < 50; i++) {
            if (get_value_from_bucket(i, bitsize, vector) != i) {
                printf("Nesedi to pro %u.\n", i);
            }
        }*/
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