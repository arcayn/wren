#pragma once

#include <inttypes.h>
#include <vector>
#include <string>
#include <math.h>

using namespace std;

typedef vector<uint32_t> ivec;

uint32_t combineMod(uint32_t a, uint32_t b, uint32_t N);
uint32_t substitute(uint32_t n, vector<uint32_t> *key);
vector<uint32_t> *inverse_sub(vector<uint32_t> *key);

string to_string(ivec *v);

void msg_pos(string msg);
void msg_neg(string msg);
void msg_inf(string msg);