#include <stdio.h>
#include "simpleCSV.h"
#include "structure.h"
#include "hyperloglog.h"
#include "declarations.h"
#include "tests/unittests.h"
#include "uthash.h"
#include "dstats.h"
#include "hlldictionary.h"

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
        /*View view = {(uint[]){ 9 }, 2};
        char *fields[] = {"a", "c", "n", "h", "s", "a", "c", "n", "h", "s", "a", "c", "n", "h", "s", "a", "c", "n", "h", "s"};
        printf("computed: %u\n", (uint)compute_hash_length(view, fields));
        char *result = create_hash_id(view, fields);
        printf("str: '%s'\n", result);
        printf("strlen: %u\n", (uint)strlen(result));*/
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