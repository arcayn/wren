#include "../../include/solvers.hpp"
#include <iostream>

SimulatedAnnealingSolver::SimulatedAnnealingSolver(Cipher *ci, Keyspace<ivec*> *ks, Scorer *s)
    : HillClimbSolver(ci, ks, s)
    { name = "Simulated annealing"; }

bool SimulatedAnnealingSolver::selectionFunction(double best_score, double score, int itr) {
    auto T = [this] (uint32_t time) {
        return start_temp - (cooling_rate * (double)time);
    };

    auto P = [] (double e0, double e1, double t) {
        if (e0 - e1 > 0) return 1.0 * RAND_MAX;
        if (t <= 0) return 0.0;
        return RAND_MAX * exp((e0 - e1) / t);
    };

    return P(score, best_score, T(itr)) > rand();
}

/* NOTE: if cond is set to NULL, this function will never return */
ivec *SimulatedAnnealingSolver::solve(ivec* ct, struct conditions<ivec*>* cond, double t, double c) {
    start_temp = t;
    cooling_rate = c;
    return solve(ct, cond);
}