#include "../../include/ciphers.hpp"
#include <iostream>

static int gcd(int a, int b) {
    while(b) {
        auto t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static vector<uint32_t> *createCaesarKey(uint32_t k, uint32_t N) {
    vector<uint32_t> *r = new vector<uint32_t>;
    r->push_back(k);
    for (int i = 0; i < N - 1; i++)
        r->push_back(((*r)[i] + 1) % N);
    return r;
}

vector<uint32_t> *createAffineKey(uint32_t a, uint32_t b, uint32_t N) {
    if (gcd(a,N) != 1 || a == 0)
        a = 1;
    vector<uint32_t> *r = new vector<uint32_t>;
    r->push_back(b);
    for (int i = 0; i < N - 1; i++) {
        r->push_back(((*r)[i] + a) % N);
    }
    return r;
}

/* Generic Simple Sub */
SimpleSubCipher::SimpleSubCipher(vector<uint32_t> *k, uint32_t N) {
    modulus = N;
    setKey(k);
}

void SimpleSubCipher::generateDecKey() {
    dec_key = inverse_sub(enc_key);
}

void SimpleSubCipher::encrypt(vector<uint32_t> *plaintext, vector<uint32_t> *ciphertext) {
    //cout << "PSS: " << plaintext->size() << endl;
    for (int i = 0; i < plaintext->size(); i++) {
        //cout << substitute((*plaintext)[i], enc_key) << endl;
        ciphertext->push_back(substitute((*plaintext)[i], enc_key));
    }
}

void SimpleSubCipher::decrypt(vector<uint32_t> *ciphertext, vector<uint32_t> *plaintext) {
    for (int i = 0; i < ciphertext->size(); i++)
        plaintext->push_back(substitute((*ciphertext)[i], dec_key));
}

/* Caesar */
CaesarCipher::CaesarCipher(uint32_t k, uint32_t N) :
    SimpleSubCipher(createCaesarKey(k, N), N)
    { }

/* Affine */
AffineSubCipher::AffineSubCipher(uint32_t a, uint32_t b, uint32_t N) :
    SimpleSubCipher(createAffineKey(a, b, N), N)
    { }