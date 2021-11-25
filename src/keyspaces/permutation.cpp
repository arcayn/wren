#include "../../include/keyspace.hpp"

static void swapPositions(ivec *v, int a, int b) {
    iter_swap(v->begin() + a, v->begin() + b);
}

PermutationKeyspace::PermutationKeyspace(uint32_t k) {
    size = k;
    baseState = new ivec;
    internal = new ivec;
    for (int i = 0; i < size; i++)
        baseState->push_back(i);
    resetState();
}

ivec* PermutationKeyspace::nextState() {
    return peturbState(1);
}

ivec* PermutationKeyspace::peturbState(int magnitude) {
    for (int i = 0; i < magnitude; i++)
        swapPositions(internal, rand() % size, rand() % size);
    return internal;
}

string PermutationKeyspace::toString() {
    return to_string(internal);
}