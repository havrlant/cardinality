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
    }
    
    return 0;
}
