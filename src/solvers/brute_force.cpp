#include "../../include/solvers.hpp"
#include <iostream>

BruteForceSolver::BruteForceSolver(Cipher *ci, Keyspace<ivec*> *ks, Scorer *s)
    { 
        cipherInstance = ci;
        keyspace = ks;
        scorer = s;
    }


ivec *BruteForceSolver::solve(ivec* ct, struct conditions<ivec*>* cond) {
    if (!keyspace->iterable()) {
        msg_neg("Keyspace is not iterable. Aborting...");
        return NULL;
    }

    msg_inf("Beginning brute-force solve...");
    
    keyspace->resetState();
    keyspace->nextState();
    ivec best_key = ivec(*keyspace->getState());

    ivec best_plaintext;
    cipherInstance->setKey(&best_key);
    cipherInstance->decrypt(ct, &best_plaintext);
    double best_score = scorer->scoreCTO(&best_plaintext);

    ivec temp_plaintext; double temp_score;
    while (!keyspace->steppedThrough()) {
        ivec* knns = keyspace->nextState();
        
        cipherInstance->setKey(knns);

        temp_plaintext.clear();
        cipherInstance->decrypt(ct, &temp_plaintext);
        temp_score = scorer->scoreCTO(&temp_plaintext);
        if (temp_score > best_score) {
            best_plaintext = ivec(temp_plaintext);
            best_score = temp_score;
            best_key = ivec(*(keyspace->getState()));
        }
    }
    msg_pos("Brute force solve finished!");
    msg_pos("Best fitness: " + to_string(best_score));
    msg_pos("Best plaintext: " + convertToEnglish(&best_plaintext));
    return new ivec(best_key);
}