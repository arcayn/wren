#include "../include/scorers.hpp"
#include <fstream>
#include <stdexcept>
#include <math.h>
#include <iostream>

static int powF(int n, uint e) {
    int r = 1;
    for (int i = 0; i < e; i++)
        r *= n;
    return r;
}

/* TODO: Make more generic */
/* TODO: Path resolution! */
void MarkovScorer::loadNgrams() {
    ngram_map = (double *) calloc(pow(language_size, length), sizeof (double));
    msg_inf("Beginning Ngram loading...");
    ifstream datafile;
    string filename;
    switch (lang) {
        case en:
            switch (length) {
                case SCORE_FREQ:
                    filename = "en/frequencies.txt";
                    break;
                case SCORE_BIGRAM:
                    filename = "en/bigrams.txt";
                    break;
                case SCORE_TRIGRAM:
                    filename = "en/trigrams.txt";
                    break;
                case SCORE_QUADGRAM:
                    filename = "en/quadgrams.txt";
                    break;
                default:
                    throw invalid_argument("Invalid langauge/length ngram combination");
            }
        default:
            break;
    }
    datafile.open("data/" + filename);
    msg_pos("Opened Ngram file");
    uint64_t total;
    datafile >> total;

    baseline = log(0.01 / total);
    string gram;
    uint64_t cnt;
    for (int i = 0; i < pow(language_size, length); i++) {
        datafile >> gram >> cnt;
        ivec* gram_converted = new ivec;
        convertFromEnglish(gram, gram_converted);
        //cout << gram << " " << cnt << " " << ngramID(gram_converted) << endl;
        ngram_map[ngramID(gram_converted)] = log ((double) cnt / (double) total);
        free(gram_converted);
    }
    datafile.close();
    msg_pos("Loaded Ngrams");
}

MarkovScorer::MarkovScorer(uint8_t n, Language l) {
    length = n;
    lang = l;
    switch(l) {
        case en:
            language_size = 26;
            language_size_2 = 26 * 26;
            language_size_3 = 26 * 26 * 26;
            break;
        default:
            break;
    }
    loadNgrams();
}

double MarkovScorer::scoreSingle(uint32_t id) {
    double r = ngram_map[id];
    return (r == 0) ? baseline : r;
}

double MarkovScorer::scoreCTO(ivec *decryption) {
    double total = 0;
    uint32_t gram;
    for (int i = 0; i < decryption->size() - length + 1; i++) {
        gram = (*decryption)[i];

        if (length > 1)
            gram += (*decryption)[i + 1] * language_size;
        if (length > 2)
            gram += (*decryption)[i + 2] * language_size_2;
        if (length > 3)
            gram += (*decryption)[i + 3] * language_size_3;
        ivec xxe = ivec{(*decryption)[i],(*decryption)[i+1],(*decryption)[i+2],(*decryption)[i+3]};
        total += scoreSingle(gram);
    }
    return total;
}