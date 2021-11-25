#include "../../include/solvers.hpp"
#include <iostream>

SimulatedAnnealingSolver::SimulatedAnnealingSolver(Cipher *ci, Keyspace<ivec*> *ks, Scorer *s)
    { 
        cipherInstance = ci;
        keyspace = ks;
        scorer = s;
    }

/* NOTE: if cond is set to NULL, this function will never return */
ivec *SimulatedAnnealingSolver::solve(ivec* ct, struct conditions<ivec*>* cond) {
    return solve(ct, cond, default_start_temp, default_cooling_rate);
}

/* NOTE: if cond is set to NULL, this function will never return */
ivec *SimulatedAnnealingSolver::solve(ivec* ct, struct conditions<ivec*>* cond, double start_temp, double cooling_rate) {
    bool exit_on_itr_count = cond != NULL && cond->iter_count > 0;
    bool exit_on_score = cond != NULL && cond->score > -__DBL_MAX__;

    msg_inf("Beginning simulated annealing solve...");

    auto T = [start_temp, cooling_rate] (uint32_t time) {
        return start_temp - (cooling_rate * (double)time);
    };

    auto P = [] (double e0, double e1, double t) {
        if (e0 - e1 > 0) return 1.0 * RAND_MAX;
        if (t <= 0) return 0.0;
        return RAND_MAX * exp((e0 - e1) / t);
    };
    
    keyspace->resetState();
    keyspace->nextState();
    ivec best_key;
    copy(keyspace->getState()->begin(), keyspace->getState()->end(), back_inserter(best_key));

    ivec best_plaintext;
    cipherInstance->setKey(&best_key);

    cipherInstance->decrypt(ct, &best_plaintext);
    double best_score = scorer->scoreCTO(&best_plaintext);

    ivec temp_plaintext; double temp_score;
    uint64_t itr_count = 0;
    while (true) {
        if (exit_on_itr_count && itr_count > cond->iter_count)
            break;
        ivec* knns = keyspace->peturbState(1);
        cipherInstance->setKey(knns);

        temp_plaintext.clear();
        cipherInstance->decrypt(ct, &temp_plaintext);
        temp_score = scorer->scoreCTO(&temp_plaintext);
        if (P(temp_score, best_score, T(itr_count)) > rand()) {
            best_plaintext = ivec(temp_plaintext);
            best_score = temp_score;
            
            best_key.clear();
            copy(keyspace->getState()->begin(), keyspace->getState()->end(), back_inserter(best_key));

            msg_pos("Key update @ iteration " + to_string(itr_count) + ":");
            msg_pos("Best fitness: " + to_string(best_score));
            msg_pos("Best plaintext: " + convertToEnglish(&best_plaintext));
            if (exit_on_score && best_score > cond->score)
                break;
        } else {
            keyspace->getState()->clear();
            copy(best_key.begin(), best_key.end(), back_inserter(*keyspace->getState()));

        }
        itr_count++;
    }
    msg_pos("Hill Climb solve finished!");
    msg_pos("Best fitness: " + to_string(best_score));
    msg_pos("Best plaintext: " + convertToEnglish(&best_plaintext));
    return new ivec(best_key);
}