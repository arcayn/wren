#pragma once

#include "util.hpp"
#include "scorers.hpp"
#include "ciphers.hpp"
#include "keyspace.hpp"

template <class T>
struct conditions {
    T target;
    uint64_t iter_count = 0;
    double score = -__DBL_MAX__;
};

template <class T, class S>
class Solver {
    protected:
        Cipher *cipherInstance;
        Keyspace<T> *keyspace;
        Scorer *scorer;

    public:
        virtual T solve(S ct) {
            return solve(ct, NULL);
        }
        virtual T solve(S ct, struct conditions<S>* cond) = 0;
};

class BruteForceSolver : public Solver<ivec*, ivec*> {
    public:
        using Solver::solve;
        BruteForceSolver(Cipher *ci, Keyspace<ivec*> *ks, Scorer *s);
        virtual ivec* solve(ivec* ct, struct conditions<ivec*>* cond) override;
};

class HillClimbSolver : public Solver<ivec*, ivec*> {
    protected:
        string name = "Hill climb";
        virtual bool selectionFunction(double best_score, double score, int itr);

    public:
        using Solver::solve;
        HillClimbSolver(Cipher *ci, Keyspace<ivec*> *ks, Scorer *s);
        virtual ivec* solve(ivec* ct, struct conditions<ivec*>* cond) override;
};

class SimulatedAnnealingSolver : public HillClimbSolver {
    protected:
        double start_temp = 20;
        double cooling_rate = 0.0001;
        virtual bool selectionFunction(double best_score, double score, int itr) override;

    public:
        using HillClimbSolver::solve;
        SimulatedAnnealingSolver(Cipher *ci, Keyspace<ivec*> *ks, Scorer *s);
        virtual ivec* solve(ivec* ct, struct conditions<ivec*>* cond, double t, double c);
};