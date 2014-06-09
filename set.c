#include "set.h"

uint array_size(uint size) {
    return ceil(size / (double)BITS_IN_BYTE);
}

uint elements_count_in_single_cell(Set set, uint index) {
    uint count = 0;
    uint cell_index;
    for (int i = 0; i < BITS_IN_BYTE; i++) {
        cell_index = 128 >> i;
        if ((cell_index & set[index]) == cell_index) {
            count++;
        }
    }
    return count;
}


Set create_set(uint maximum_elements_count) {
    uint size_in_bytes = array_size(maximum_elements_count);
    Set arr = (Set) calloc(size_in_bytes, sizeof(byte));
    return arr;
}

void set_element(Set set, uint index) {
    uint arr_index = index / BITS_IN_BYTE;
    byte cell_index = 128 >> (index % BITS_IN_BYTE);
    set[arr_index] |= cell_index;
}

uint is_element(Set set, uint index) {
    uint arr_index = index / BITS_IN_BYTE;
    byte cell_index = 128 >> (index % BITS_IN_BYTE);
    return (set[arr_index] & cell_index) == cell_index;
}

uint elements_count(Set set, uint maximum_elements_count) {
    uint count = 0;
    uint arr_size = array_size(maximum_elements_count);
    for (int i = 0; i < arr_size; i++) {
        count += elements_count_in_single_cell(set, i);
    }
    return count;
}
