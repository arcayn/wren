#pragma once

#include "util.hpp"
#include "alphabet.hpp"

#include <unordered_map>

#define SCORE_FREQ 1
#define SCORE_BIGRAM 2
#define SCORE_TRIGRAM 3
#define SCORE_QUADGRAM 4

class Scorer {
    public:
        virtual double scoreCTO(ivec *decryption) { return 0.0; };
        virtual double scoreKPA(ivec *decryption, ivec *plaintext) {
            return scoreCTO(decryption);
        }
};

class MarkovScorer : public Scorer {
    protected:
        uint8_t length;
        Language lang;
        uint32_t language_size;
        uint32_t language_size_2;
        uint32_t language_size_3;
        double *ngram_map = NULL;
        double baseline;

        void loadNgrams();
        uint32_t ngramID(ivec *ngram) {
            uint32_t id = (*ngram)[0];

            if (length > 1)
                id += (*ngram)[1] * language_size;
            if (length > 2)
                id += (*ngram)[2] * language_size_2;
            if (length > 3)
                id += (*ngram)[3] * language_size_3;
            return id;
        }
    
    public:
        MarkovScorer(uint8_t length, Language l);
        ~MarkovScorer() {
            if (ngram_map != NULL)
                free(ngram_map);
        }
        virtual double scoreSingle(uint32_t id);
        virtual double scoreCTO(ivec *decryption);
};