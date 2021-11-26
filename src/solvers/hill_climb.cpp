#include "../../include/solvers.hpp"
#include <iostream>

HillClimbSolver::HillClimbSolver(Cipher *ci, Keyspace<ivec*> *ks, Scorer *s)
    { 
        cipherInstance = ci;
        keyspace = ks;
        scorer = s;
    }

int HillClimbSolver::selectionFunction(double overall_best_score, double best_score, double score, int itr) {
    return score > best_score ? 2 : 0;
}

/* NOTE: if cond is set to NULL, this function will never return */
ivec *HillClimbSolver::solve(ivec* ct, struct conditions<ivec*>* cond) {
    bool exit_on_itr_count = cond != NULL && cond->iter_count > 0;
    bool exit_on_score = cond != NULL && cond->score > -__DBL_MAX__;

    msg_inf("Beginning " + name + " solve...");
    
    keyspace->resetState();
    keyspace->nextState();
    ivec best_key; ivec overall_best_key;
    copy(keyspace->getState()->begin(), keyspace->getState()->end(), back_inserter(best_key));

    ivec best_plaintext; ivec overall_best_plaintext;
    cipherInstance->setKey(&best_key);

    cipherInstance->decrypt(ct, &best_plaintext);
    double best_score = scorer->scoreCTO(&best_plaintext);

    double overall_best_score = best_score;
    copy(best_plaintext.begin(), best_plaintext.end(), back_inserter(overall_best_plaintext));
    copy(keyspace->getState()->begin(), keyspace->getState()->end(), back_inserter(overall_best_key));

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
        int SF = selectionFunction(overall_best_score, best_score, temp_score, itr_count);
        if (SF > 0) {
            best_plaintext = ivec(temp_plaintext);
            best_score = temp_score;
            
            best_key.clear();
            copy(keyspace->getState()->begin(), keyspace->getState()->end(), back_inserter(best_key));

            if (SF > 1) {
                overall_best_plaintext = ivec(temp_plaintext);
                overall_best_score = temp_score;
                
                overall_best_key.clear();
                copy(keyspace->getState()->begin(), keyspace->getState()->end(), back_inserter(overall_best_key));
                msg_pos("Key update @ iteration " + to_string(itr_count) + ":");
                msg_pos("Best fitness: " + to_string(best_score));
                msg_pos("Best plaintext: " + convertToEnglish(&best_plaintext));
            }
            if (exit_on_score && best_score > cond->score)
                break;
        } else {
            keyspace->getState()->clear();
            copy(best_key.begin(), best_key.end(), back_inserter(*keyspace->getState()));

        }
        itr_count++;
    }
    msg_pos(name + " solve finished!");
    msg_pos("Best fitness: " + to_string(overall_best_score));
    msg_pos("Best plaintext: " + convertToEnglish(&overall_best_plaintext));
    return new ivec(best_key);
}