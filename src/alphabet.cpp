#include "../include/alphabet.hpp"

void convertFromEnglish(string input, vector<uint32_t> *output) {
    for (int i = 0; i < input.length(); i++) {
        output->push_back(input[i] < 'a' ? input[i] - 'A' : input[i] - 'a');
    }
}

string convertToEnglish(vector<uint32_t> *input) {
    string r = "";
    for (int i = 0; i < input->size(); i++)
    {
        r += (char) (*input)[i] + 'a';
    }
    return r;
}