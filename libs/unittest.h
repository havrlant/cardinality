#ifndef parkc_unittest_h
#define parkc_unittest_h

#include <string.h>
#include <stdio.h>

extern int _total_tests;
extern int _total_errors;

#define sassert(test) do {\
    _total_tests++;\
    if (!(test)) {\
        _total_errors++;\
        printf("Test failed: %s\n", #test);\
    }\
} while (0)

#define assert_eq(expected, actual) do {\
    _total_tests++;\
    if ((expected) != (actual)) {\
        _total_errors++;\
        printf("Test failed. %s != %s\n", #expected, #actual);\
    }\
} while(0)

#define assert_array_eq(expected, actual, length, type) do {\
    type *_actual = (actual);\
    type *_expected = (expected);\
    _total_tests++;\
    for(size_t _i = 0; _i < length; _i++) {\
        if (_expected[_i] != _actual[_i]) {\
            _total_errors++;\
            printf("Test failed. Array %s at index %zu: %g, expected: %g\n", #actual, _i, (double)_actual[_i], (double)_expected[_i]);\
            break;\
        }\
    }\
} while(0);

#define assert_str(expected, actual) do {\
    const char *_actual = (actual);\
    const char *_expected = (expected);\
    _total_tests++;\
    if(strcmp(_expected, _actual) != 0) {\
        _total_errors++;\
        printf("Test failed. Code %s returns %s, expected: %s\n", #actual, _actual, _expected);\
    }\
} while(0)

#endif
