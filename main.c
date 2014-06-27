#include <stdio.h>
#include <string.h>
#include "declarations.h"
#include "hll_file_consumer.h"
#include "tests/alltests.h"
#include "tests/integration.h"

int main(int argc, const char * argv[])
{
    if (argc > 1) {
        if (strcmp(argv[1], "--test") == 0) {
            run_all_tests();
            if (argc > 2) {
                run_integration_test(argv[2]);
            }
            return 0;
            // hyperloglog(14, "/Users/lukashavrlant/Desktop/test");
        } else {
            uint b = 14;
            if (argc > 2) {
                b = (uint) atoi(argv[2]);
            }
            hyperloglog(b, argv[1]);
        }
    } else {
        run_all_tests();
        run_integration_test("/home/lukas/Data/test500");
    }
//         run_integration_test("/home/lukas/Data/test500");
        //hyperloglog(14, "/Users/lukashavrlant/Desktop/test");
        // printf("It's working!\n");

    return 0;
}
