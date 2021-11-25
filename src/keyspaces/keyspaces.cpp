#include "../../include/keyspace.hpp"
#include <iostream>

ZNVectorKeyspace::ZNVectorKeyspace(uint32_t n, uint32_t k) {
    modulus = n;
    size = k;
    internal = new ivec;
    baseState = new ivec(size, 0); 
    resetState();
}

ivec* ZNVectorKeyspace::nextState() {
    int cidx = 0;
    while (cidx < size) {
        (*internal)[cidx]++;
        if ((*internal)[cidx] >= modulus)
            (*internal)[cidx] = 0;
        else
            break;
        cidx++;
    }
    
    if (cidx == size)
        hasSteppedThrough = true;
    return internal;
}

/* "magnitude" here is not the actual expected magnitude since we do not want
    to waste time on lots of multiplication operations */
ivec* ZNVectorKeyspace::peturbState(int magnitude) {
    for (int i = 0; i < size; i++) {
        uint32_t change = rand() % ((magnitude/size) * 2);
        if (rand() % 2)
            change = modulus - change;
        (*internal)[i] = combineMod((*internal)[i], change, modulus);
    }
    return internal;
}

ivec* ZNVectorKeyspace::peturbSpecificComponent(uint32_t magnitude, int i) {
    (*internal)[i] = combineMod((*internal)[i], rand() % (magnitude * 2), modulus);
    return internal;
}

ivec* ZNVectorKeyspace::peturbSingleComponent(uint32_t magnitude) {
    return peturbSpecificComponent(magnitude, rand() % size);
}

ivec* ZNVectorKeyspace::peturbEqually(uint32_t magnitude) {
    uint32_t change = rand() % ((magnitude/size) * 2);
    if (rand() % 2)
        change = modulus - change;
    for (int i = 0; i < size; i++)
        (*internal)[i] = combineMod((*internal)[i], change, modulus);
        
    return internal;
}

string ZNVectorKeyspace::toString() {
    return to_string(internal);
}