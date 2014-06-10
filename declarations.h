#ifndef cardinality_declarations_h
#define cardinality_declarations_h

typedef unsigned char byte;
typedef unsigned int uint;

const int BITS_IN_BYTE;

typedef struct {
    uint b;
    uint m;
    byte *M;
} Hyperloglog;

#endif
