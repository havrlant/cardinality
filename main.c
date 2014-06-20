#include <stdio.h>
#include "declarations.h"
#include "hll_file_consumer.h"
#include "tests/alltests.h"

int main(int argc, const char * argv[])
{
    if (argc > 1) {
        if (strcmp(argv[1], "--test") == 0) {
            run_all_tests();
            return 0;
        }
        uint b = 14;
        if (argc > 2) {
            b = (uint) atoi(argv[2]);
        }
        hyperloglog(b, argv[1]);
    } else {
        // hyperloglog(14, "/Users/lukashavrlant/Desktop/test");
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