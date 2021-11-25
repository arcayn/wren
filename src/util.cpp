#include "../include/util.hpp"
#include <iostream>

uint32_t combineMod(uint32_t a, uint32_t b, uint32_t N) {
    return (a + b) % N;
}

uint32_t substitute(uint32_t n, vector<uint32_t> *key) {
    return (*key)[n];
}

vector<uint32_t> *inverse_sub(vector<uint32_t> *key) {
    vector<uint32_t> *r_key = new vector<uint32_t>(key->size(), 0);
    for (int i = 0; i < key->size(); i++) {
        (*r_key)[(*key)[i]] = i;
    }
    return r_key;
}

string to_string(ivec *v) {
    string r = "(";
    for (int i = 0; i < v->size(); i++)
        r += to_string((*v)[i]) + ", ";
    return r + ')';
} 

void msg_pos(string msg) {
    cout << "[+] " << msg << endl;
}
void msg_neg(string msg) {
    cout << "[-] " << msg << endl;
}
void msg_inf(string msg) {
    cout << "[*] " << msg << endl;
}