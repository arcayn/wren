#pragma once

#include "util.hpp"
#include <time.h>  

template <class T>
class Keyspace {
    protected: 
        T internal = NULL;
        T baseState = NULL;
        bool hasSteppedThrough = false;
        uint64_t stateSize;
        bool isIterable = true;

    public:
        Keyspace() {
            srand(time(NULL));
        }
        virtual bool steppedThrough() {
            return hasSteppedThrough;
        }
        virtual bool iterable() {
            return isIterable;
        }
        virtual T getState() {
            return internal;
        }
        virtual T nextState() { return internal; }
        virtual void resetState() {
            internal = baseState;
            hasSteppedThrough = false;
        }
        virtual T peturbState(int magnitude) { msg_neg("called wrong peturb"); return internal; }
        virtual string toString() { return "Keyspace object"; }
};


class ZNVectorKeyspace : public Keyspace<ivec*> {
    protected:
        uint32_t modulus;
        uint32_t size;
    
    public:
        ZNVectorKeyspace(uint32_t n, uint32_t k);
        virtual ivec* nextState();
        virtual ivec* peturbState(int magnitude) override;
        virtual ivec* peturbSpecificComponent(uint32_t magnitude, int i);
        virtual ivec* peturbSingleComponent(uint32_t magnitude);
        virtual ivec* peturbEqually(uint32_t magnitude);
        virtual string toString();
};

class PermutationKeyspace : public Keyspace<ivec*> {
    protected:
        uint32_t size;
        bool isIterable = false;
    
    public:
        PermutationKeyspace(uint32_t k);
        virtual ivec* nextState();
        virtual ivec* peturbState(int magnitude);
        virtual string toString();
};

template <class T, class U>
class PostProcessKeyspace : public Keyspace<U> {
    protected:
        Keyspace<T> *wrappedKeyspace;
        U (*map_fun)(T);

    public:
        PostProcessKeyspace(Keyspace<T> *k, U (*mf)(T)) {
            wrappedKeyspace = k;
            map_fun = mf;
        }

        virtual bool steppedThrough() {
            return wrappedKeyspace->steppedThrough();
        }

        virtual U getState() {
            return map_fun(wrappedKeyspace->getState());
        }

        virtual U nextState() {
            return map_fun(wrappedKeyspace->nextState());
        }

        virtual void resetState() {
            wrappedKeyspace->resetState();
        }

        virtual U peturbState(int magnitude) {
            return map_fun(wrappedKeyspace->peturbState(magnitude));
        }

        virtual string toString() {
            return "Wrapped instance of: " + wrappedKeyspace->toString();
        }
};
