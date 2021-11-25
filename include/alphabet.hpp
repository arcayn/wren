#pragma once

#include "util.hpp"

enum Language { en };

void convertFromEnglish(string input, vector<uint32_t> *output);
string convertToEnglish(vector<uint32_t> *input);
